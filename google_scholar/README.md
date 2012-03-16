Produce the Output added by version 1.4
=======================================

Assume search by author "DE Knuth", simply run the following command:

`python scholar.py --ref --author 'DE Knuth'`

The script will extract all publications returned by Google Scholar that are
cited at least once and print them on the standard output.

If you want to limit the number of publications, use the `-c` option.

The output can be redirect to a file. One thing need to know is that, the output
may contain some non-ascii characters and is encoded in utf-8. If you have
problem displaying the output in an editor (this is likely to happen when under
Windows), simply open the file with a browser should solve the problem.
