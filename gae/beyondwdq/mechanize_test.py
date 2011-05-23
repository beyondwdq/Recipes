import mechanize
from google.appengine.ext import webapp

class MechanizeTest(webapp.RequestHandler):
    def get(self):
        # Browser
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
        response = br.response()
        self.response.out.write(str(response.info()))
        #content = response.read()
        #self.response.out.write(str(len(content)))
