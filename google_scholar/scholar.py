#! /usr/bin/env python
"""
This module provides classes for querying Google Scholar and parsing
returned results.  
"""
# Version: 1.4 -- $Date: 2012-03-16 15:13:16 -0800 (Fri, 16 Mar 2012) $
# Version: 1.3 -- $Date: 2012-02-01 16:51:16 -0800 (Wed, 01 Feb 2012) $
#
# ChangeLog
# ---------
# 1.4  Modified by WANG DANQI to produce a output of publications for PhD 
#       thesis submission. Other changes include:
#       1. Support multi-page search.
#       2. Change the search url.
#       3. Search author enhanced.
#       4. Search string can be empty.
#   For instructions on produce the output, please refer to the README.md.
#   Date:2012-03-16
#
# 1.3:  Updates to reflect changes in Scholar's page rendering.
#
# 1.2:  Minor tweaks, mostly thanks to helpful feedback from Dan Bolser.
#       Thanks Dan!
#
# 1.1:  Made author field explicit, added --author option.
#
# pylint: disable-msg=C0111
#
# Copyright 2010--2012 Christian Kreibich. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    1. Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

import optparse
import sys
import urllib
import urllib2
from BeautifulSoup import BeautifulSoup
import BeautifulSoup as BS


class Article():
    """
    A class representing articles listed on Google Scholar.  The class
    provides basic dictionary-like behavior.
    """
    def __init__(self):
        self.attrs = {'title':         [None, 'Title',          0],
                      'url':           [None, 'URL',            1],
                      'publish':       [None, 'Publish',        2],
                      'num_citations': [0,    'Citations',      3],
                      'num_versions':  [0,    'Versions',       4],
                      'url_citations': [None, 'Citations list', 5],
                      'url_versions':  [None, 'Versions list',  6]}

    def __getitem__(self, key):
        if key in self.attrs:
            return self.attrs[key][0]
        return None

    def __setitem__(self, key, item):
        if key in self.attrs:
            self.attrs[key][0] = item
        else:
            self.attrs[key] = [item, key, len(self.attrs)]

    def __delitem__(self, key):
        if key in self.attrs:
            del self.attrs[key]

    def as_txt(self):
        # Get items sorted in specified order:
        items = sorted(self.attrs.values(), key=lambda item: item[2])
        # Find largest label length:
        max_label_len = max([len(str(item[1])) for item in items])
        fmt = '%%%ds %%s' % max_label_len
        return '\n'.join([fmt % (item[1], item[0]) for item in items])

    def as_csv(self, header=False, sep='|'):
        # Get keys sorted in specified order:
        keys = [pair[0] for pair in \
                    sorted([(key, val[2]) for key, val in self.attrs.items()],
                        key=lambda pair: pair[1])]
        res = []
        if header:
            res.append(sep.join(keys))
        res.append(sep.join([str(self.attrs[key][0]) for key in keys]))
        return '\n'.join(res)

    def as_ref(self):
        return '\n'.join(
                [self.attrs['title'][0],
                    self.attrs['publish'][0],
                    "Cited by:" + str(self.attrs['num_citations'][0])])


class ScholarParser():
    """
    ScholarParser can parse HTML document strings obtained from Google
    Scholar. It invokes the handle_article() callback on each article
    that was parsed successfully.
    """
    SCHOLAR_SITE = 'http://scholar.google.com'

    def __init__(self, site=None):
        self.soup = None
        self.article = None
        self.site = site or self.SCHOLAR_SITE

    def handle_article(self, art):
        """
        In this base class, the callback does nothing.
        """

    def parse(self, html, nav_links):
        """
        This method initiates parsing of HTML content.
        """
        self.soup = BeautifulSoup(html, convertEntities="html")
        for div in self.soup.findAll(ScholarParser._tag_checker):
            self._parse_article(div)
        if nav_links:
            div_n = self.soup.find('div', {"id": "gs_n"})
            return [self._path2url(a['href']) for a in div_n.findAll('a')]
        return None

    def _parse_article(self, div):
        self.article = Article()

        for tag in div:
            if not hasattr(tag, 'name'):
                continue

            if tag.name == 'div' and tag.get('class') == 'gs_rt' and \
                    tag.h3 and tag.h3.a:
                self.article['title'] = ''.join(tag.h3.a.findAll(text=True))
                self.article['url'] = self._path2url(tag.h3.a['href'])

            if tag.name == 'font':
                for tag2 in tag:
                    if not hasattr(tag2, 'name'):
                        continue
                    if tag2.name == 'span' and tag2.get('class') == 'gs_fl':
                        self._parse_links(tag2)

        if self.article['title']:
            self.handle_article(self.article)

    def _parse_links(self, span):
        for tag in span:
            if not hasattr(tag, 'name'):
                continue
            if tag.name != 'a' or tag.get('href') == None:
                continue

            if tag.get('href').startswith('/scholar?cites'):
                if hasattr(tag, 'string') and tag.string.startswith('Cited by'):
                    self.article['num_citations'] = \
                        self._as_int(tag.string.split()[-1])
                self.article['url_citations'] = self._path2url(tag.get('href'))

            if tag.get('href').startswith('/scholar?cluster'):
                if hasattr(tag, 'string') and tag.string.startswith('All '):
                    self.article['num_versions'] = \
                        self._as_int(tag.string.split()[1])
                self.article['url_versions'] = self._path2url(tag.get('href'))

    @staticmethod
    def _tag_checker(tag):
        if tag.name == 'div' and tag.get('class') == 'gs_r':
            return True
        return False

    def _as_int(self, obj):
        try:
            return int(obj)
        except ValueError:
            return None

    def _path2url(self, path):
        if path.startswith('http://'):
            return path
        if not path.startswith('/'):
            path = '/' + path
        return self.site + path


class ScholarParser120201(ScholarParser):
    """
    This class reflects update to the Scholar results page layout that
    Google recently.
    """
    def _parse_article(self, div):
        self.article = Article()

        for tag in div:
            if not hasattr(tag, 'name'):
                continue

            if tag.name == 'h3' and tag.get('class') == 'gs_rt' and tag.a:
                self.article['title'] = ''.join(tag.a.findAll(text=True))
                self.article['url'] = self._path2url(tag.a['href'])

            if tag.name == 'div' and tag.get('class') == 'gs_a':
                self.article['publish'] = ' '.join(
                        [' '.join(c.findAll(text=True))
                            if isinstance(c, BS.Tag)
                            else unicode(c)
                            for c in tag.contents])
                #.replace(u'\u2026', '...')

            if tag.name == 'div' and tag.get('class') == 'gs_fl':
                self._parse_links(tag)

        if self.article['title']:
            self.handle_article(self.article)


class ScholarQuerier():
    """
    ScholarQuerier instances can conduct a search on Google Scholar
    with subsequent parsing of the resulting HTML content.  The
    articles found are collected in the articles member, a list of
    Article instances.
    """
    SCHOLAR_URL='http://scholar.google.com.sg/scholar?start=0&q=%s&hl=en&as_sdt=0,5'

    """
    Older URLs:
    http://scholar.google.com/scholar?q=%s&hl=en&btnG=Search&as_sdt=2001&as_sdtp=on
    """

    UA = 'Mozilla/5.0 (X11; U; FreeBSD i386; en-US; rv:1.9.2.9) Gecko/20100913 Firefox/3.6.9'

    class Parser(ScholarParser120201):
        def __init__(self, querier):
            ScholarParser.__init__(self)
            self.querier = querier

        def handle_article(self, art):
            self.querier.add_article(art)

    class CitationParser(Parser):
        def __init__(self, querier):
            ScholarQuerier.Parser.__init__(self, querier)

        def handle_article(self, art):
            if art['num_citations'] > 0:
                self.querier.add_article(art)

    def __init__(self, author='', scholar_url=None):
        self.articles = []
        self.author = author.strip()
        self.scholar_url = scholar_url or self.SCHOLAR_URL

    def build_query_string(self, search):
        sl = search.strip().split()
        if self.author:
            author_str = 'author:"%s"' % self.author.replace(' ', '+')
            s = '+'.join(sl + [author_str,])
        else:
            s = '+'.join(sl)
        return s

    def query(self, search, count):
        """
        This method initiates a query with subsequent parsing of the
        response.
        """
        link = self.scholar_url % self.build_query_string(search)
        print >>sys.stderr, "querying url:"
        print >>sys.stderr, link

        req = urllib2.Request(url=link,
                              headers={'User-Agent': self.UA})
        hdl = urllib2.urlopen(req)
        html = hdl.read()
        hdl.close()
        navs = self.parse(html, nav_links=True)

        #skip the last "next" link
        navs = navs[0:len(navs) - 1]
        for nav in navs:
            if count > 0 and len(self.articles) >= count:
                break
            req = urllib2.Request(url=nav,
                                  headers={'User-Agent': self.UA})
            hdl = urllib2.urlopen(req)
            html = hdl.read()
            hdl.close()
            self.parse(html)

    def parse(self, html, nav_links=False):
        """
        This method allows parsing of existing HTML content.
        """
        parser = self.CitationParser(self)
        return parser.parse(html, nav_links)

    def add_article(self, art):
        self.articles.append(art)


def ref(query, author, count):
    querier = ScholarQuerier(author=author)
    querier.query(query, count)
    articles = querier.articles
    if count > 0:
        articles = articles[:count]
    articles = sorted(articles, key=lambda a: a['num_citations'], reverse=True)
    total_citations = sum((a['num_citations'] for a in articles))
    print u"Google:%s\n" % total_citations
    for art in articles:
        print art.as_ref().encode('utf-8') + '\n'


def txt(query, author, count):
    querier = ScholarQuerier(author=author)
    querier.query(query, count)
    articles = querier.articles
    if count > 0:
        articles = articles[:count]
    for art in articles:
        print art.as_txt() + '\n'


def csv(query, author, count, header=False, sep='|'):
    querier = ScholarQuerier(author=author)
    querier.query(query, count)
    articles = querier.articles
    if count > 0:
        articles = articles[:count]
    for art in articles:
        result = art.as_csv(header=header, sep=sep)
        print result.encode('utf-8')
        header = False


def main():
    usage = """scholar.py [options] <query string>
A command-line interface to Google Scholar."""

    fmt = optparse.IndentedHelpFormatter(max_help_position=50,
                                         width=100)
    parser = optparse.OptionParser(usage=usage, formatter=fmt)
    parser.add_option('-a', '--author',
                      help='Author name')
    parser.add_option('--csv', action='store_true',
                      help='Print article data in CSV format (separator is "|")')
    parser.add_option('--csv-header', action='store_true',
                      help='Like --csv, but print header line with column names')
    parser.add_option('--txt', action='store_true',
                      help='Print article data in text format')
    parser.add_option('--ref', action='store_true',
                      help='Print article data in text format')
    parser.add_option('-c', '--count', type='int',
                      help='Maximum number of results')
    parser.set_defaults(count=0, author='')
    options, args = parser.parse_args()

    query = ""
    if args:
        query = ' '.join(args)

    if options.csv:
        csv(query, author=options.author, count=options.count)
    if options.csv_header:
        csv(query, author=options.author, count=options.count, header=True)
    if options.txt:
        txt(query, author=options.author, count=options.count)
    if options.ref:
        ref(query, author=options.author, count=options.count)

if __name__ == "__main__":
    main()
