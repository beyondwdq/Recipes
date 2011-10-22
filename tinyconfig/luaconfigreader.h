#ifndef LUACONFIGREADER_H
#define LUACONFIGREADER_H

#include "configreader.h"

class LuaConfigReader : public ConfigReader {
	public:
		virtual bool load(const char *filename, Config *cfg);
};

#endif 
