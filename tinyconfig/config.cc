#include <iostream>
#include <algorithm> //use transform

#include "configreader.h"
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

Config::Config(ConfigReader *reader)
	: reader_(reader)
{
}

Config::~Config()
{
	
}

bool Config::load(const char *filename)
{
	return reader_->load(filename, this);
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

