#include <stdint.h>
#include <stdio.h>

uint32_t nextID()
{
	static uint32_t current_id = 1;
	return current_id++;
}

const char* nextKey()
{
	//The maximum value uint32_t is 4,294,967,295, which has 10 digits.
	static char buffer[11];
	sprintf(buffer, "%u", nextID());
	return buffer;
}

