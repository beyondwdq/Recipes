#ifndef PLAINCONFIGREADER_H
#define PLAINCONFIGREADER_H

#include "configreader.h"

class PlainConfigReader : public ConfigReader {
	public:
		virtual bool load(const char *filename, Config *cfg);
};

#endif
