#include <fstream>
#include <iostream>

#include "plainconfigreader.h"
#include "config.h"

using namespace std;

bool PlainConfigReader::load(const char *filename, Config *cfg)
{
	ifstream in;
	in.open(filename);
	if (in.fail()){
		cerr<<"Failed to open file "<<filename<<endl;
		return false;
	}

	string textline;
	while(getline(in, textline)){
		cout<<textline<<endl;
		if (textline.empty() || textline[0]=='#')
			continue;

		string::size_type isplit = textline.find("=");

		if (isplit==string::npos){
			cerr<<"Incorrect line: "<<textline<<endl;
			return false;
		}

		string::size_type ikeystart = textline.find_first_not_of(" ");
		string::size_type ikeyend = textline.find(" ", ikeystart);
		if (ikeyend==string::npos || ikeyend>isplit)
			ikeyend = isplit;
		string::size_type ivaluestart = 
			textline.find_first_not_of(" ", isplit+1);
		if (ivaluestart==string::npos){
			cerr<<"Incorrect line: "<<textline<<endl;
			return false;
		}
		string::size_type ivalueend = textline.find(" ", ivaluestart);
		if (ivalueend==string::npos)
			ivalueend = textline.size();

		if (!cfg->set(textline.substr(ikeystart, ikeyend-ikeystart), 
					textline.substr(ivaluestart, ivalueend))){
			cerr<<"Failed to set value for line: "<<textline<<endl;
			return false;
		}
	}

	return true;
}


