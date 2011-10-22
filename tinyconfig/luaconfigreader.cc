#include <string>
#include <iostream>

extern "C"{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "luaconfigreader.h"
#include "config.h"

using namespace std;

bool LuaConfigReader::load(const char *filename, Config *cfg)
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)){
		cerr<<"cannot run configuration file: "<<lua_tostring(L, -1)<<endl;
		return false;
	}
	
	// push the first key (nil = beginning of table)
	lua_pushnil(L);

	// lua_next will:
	// 1 - pop the key
	// 2 - push the next key
	// 3 - push the value at that key
	// ... so the key will be at index -2 and the value at index -1
	while (lua_next(L, LUA_GLOBALSINDEX) != 0) {
		// get type of key and value
		int key_type = lua_type(L, -2);
		int value_type = lua_type(L, -1);

		// support only string keys
		// globals aren't likely to have a non-string key, but just to be certain ...
		if (key_type != LUA_TSTRING) {
			lua_pop(L, 1); // pop the value so that the top contains the key for the next iteration
			continue;
		}

		// support only number, boolean and string values
		if (value_type != LUA_TNUMBER &&
				value_type != LUA_TBOOLEAN &&
				value_type != LUA_TSTRING) {
			lua_pop(L, 1); // again, pop the value before going to the next loop iteration
			continue;
		}

		// get the key as a string
		string key_string = lua_tostring(L, -2); // no copy required - we already know this is a string

		// do not support variables that start with '_'
		// lua has some predefined values like _VERSION. They all start with underscore

		if (!key_string.size()) { // this again is highly unlikely, but still ...
			lua_pop(L, 1);
			continue;
		}
		if (key_string[0] == '_') {
			lua_pop(L, 1);
			continue;
		}

		string value_string;

		// convert the value to a string. This depends on its type
		switch (value_type) {
			case LUA_TSTRING:
			case LUA_TNUMBER:
				// numbers can be converted to strings

				// get the value as a string (this requires a copy because traversing tables
				// uses the top of the stack as an index. If conversion from a number to string
				// happens, the top of the stack will be altered and the table index will become invalid)
				lua_pushvalue(L, -1);
				value_string = lua_tostring(L, -1);
				lua_pop(L, 1);
				break;
			case LUA_TBOOLEAN:
				value_string = lua_toboolean(L, -1) == 0 ? "false" : "true";
				break;
		}

		if (!cfg->set(key_string, value_string)){
			cerr<<"Failed to set value for key:"<<key_string
				<<" value:"<<value_string<<endl;
			return false;
		}
		lua_pop(L, 1);
	}
	return true;
}
