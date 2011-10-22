#ifndef CONFIGREADER_H 
#define CONFIGREADER_H

class Config;

class ConfigReader{
	public:
		virtual ~ConfigReader(){}

		virtual bool load(const char *filename, Config *cfg) = 0;
};

#endif 
