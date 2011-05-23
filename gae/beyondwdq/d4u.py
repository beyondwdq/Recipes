#!/usr/bin/env python

import os
import zipfile 
from StringIO import StringIO
from urlparse import urlparse
from posixpath import basename
import urllib2
import mechanize

from google.appengine.ext import webapp
from google.appengine.ext.webapp import template
from google.appengine.api import urlfetch

SUPPORTED_PROTO = ['http', 'https']
FILE_SIZE_LIMIT = 1*1024*1024 #1MB

def protocol_not_supported(protocol):
    return """<html><body>The %s protocol is not supported.  <br>
              Supported protocol: %s</body></html>
           """ % (protocol, ' '.join(SUPPORTED_PROTO))
def file_size_exceeded():
    return """<html><body>The file size has exceeded GAE's limit.</body></html>
           """
def encode_error():
    return """<html><body>The file name contains non-ascii characters. Please specify the file name in the 'Save as' field and make sure it only contains ascii characters.</body></html>
           """
def file_name_missing():
    return """<html><body>File name missing. Specify it in the 'Save as' field</body></html>
           """

class D4U(webapp.RequestHandler):
    def get(self):
        path = os.path.join(os.path.dirname(__file__), 'd4u.html')
        self.response.out.write(template.render(path, {}))

    def post(self):
        url = self.request.get("url")
        mode = self.request.get("download_method")

        parse_obj = urlparse(url)
        if not parse_obj.scheme in SUPPORTED_PROTO:
            self.response.out.write(protocol_not_supported(parse_obj.scheme))
            return

        #The zipfile module with python 2.5 has problem adding file name of unicode class.
        #App Engine now only supports python 2.5
        #http://bugs.python.org/issue1734346
        #fn = basename(parse_obj.path)
        fn = self.request.get('saveas')
        if mode=='post' and not fn:
            self.response.out.write(file_name_missing())
            return
        if not fn:
            fn = basename(parse_obj.path)
        try:
            fn = fn.encode('ascii')
        except UnicodeEncodeError:
            self.response.out.write(encode_error())


        zipstream=StringIO() 
        f = zipfile.ZipFile(zipstream,"w")
        # repeat this for every URL that should be added to the zipfile
        f =self.addFile(f,url,fn, mode)

        if f is None:
            return

        # we have finished with the zip so package it up and write the directory
        f.close()
        zipstream.seek(0)

        if len(zipstream.getvalue())>FILE_SIZE_LIMIT:
            self.response.out.write(file_size_exceeded())
            return

        # create and return the output stream
        self.response.headers['Content-Type'] ='application/zip'
        self.response.headers['Content-Disposition'] = 'attachment; filename="%s"'%(fn+'.zip')
        while True:
            buf=zipstream.read(2048)
            if buf=="": break
            self.response.out.write(buf)

    def addFile(self,zipstream,url,fname, mode):
        # get the contents          
        result = None
        try:
            if mode=='post': 
                result = self.post_download(url)
            else:
                result = self.direct_download(url)
        except urllib2.URLError, e:
            self.response.out.write(str(result.headers))
            self.response.out.write("bad request %s"%(result.status_code))
            return None

        content = result.read()
        length = 2048

        # write the contents to the zip file
        zipstream.writestr(fname,content)
        return zipstream

    def direct_download(self, url):
        return urllib2.urlopen(url)

    def post_download(self, url):
        br = mechanize.Browser()
        br.set_handle_equiv(True)
        br.set_handle_gzip(True)
        br.set_handle_redirect(True)
        br.set_handle_referer(True)
        br.set_handle_robots(False)
        br.set_handle_refresh(mechanize._http.HTTPRefreshProcessor(), max_time=1)
        br.addheaders = [('User-agent', 'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008071615 Fedora/3.0.1-1.fc9 Firefox/3.0.1')]

        # we inspect the all form element in the http://stackoverflow.com
        br.open('http://www.downhh.com/link.php?ref=FGVTLeMuiH')
        #for form in br.forms():
            #self.response.out.write(str(form))
        br.select_form(nr=0)
        br.submit()
        return br.response()
