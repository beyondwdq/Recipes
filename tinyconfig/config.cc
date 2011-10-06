#include <fstream>
#include <iostream>
#include <algorithm> //use transform

#include "config.h"

using namespace std;

bool Config::Item::set(const std::string& value)
{
	bool bret = true;

	if (p_bool_!=NULL){
		string s(value);
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		if (s=="true" || s=="1")
			*p_bool_ = true;
		else if (s=="false" || s=="0")
			*p_bool_ = false;
		else
			bret = false;

	} else if (p_int_!=NULL){
		if (value.find_first_not_of("-0123456789")!=string::npos)
			bret = false;
		else{
			*p_int_ = atoi(value.c_str());
		}
	} else if (p_double_!=NULL){
		if (value.find_first_not_of("-.0123456789")!=string::npos)
			bret = false;
		else{
			string::size_type idot = value.find('.');
			if (idot==string::npos || idot==value.rfind('.')){
				*p_double_ = atof(value.c_str());
			} else
				bret = false;
		}
	} else{
		bret = false;
	}

	return bret;
}

Config::Config()
{
}

bool Config::load(const char *filename)
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

		if (!set(textline.substr(ikeystart, ikeyend-ikeystart), 
					textline.substr(ivaluestart, ivalueend))){
			cerr<<"Failed to set value for line: "<<textline<<endl;
			return false;
		}
	}

	return true;
}

bool Config::set(const string& name, const string& value)
{
	TItemMap::iterator itr = item_map_.find(name);
	if (itr==item_map_.end()){
		cerr<<"name:"<<name<<" not found"<<endl;
		return false;
	}
	return itr->second.set(value);
}

