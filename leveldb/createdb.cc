#include <cassert>
#include <iostream>
#include "leveldb/db.h"

using namespace std;

int main(int argc, const char *argv[])
{
	
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "db/createdb", &db);
  if (!status.ok()) {
	  cerr << status.ToString() << endl;
	  exit(1);
  }
  delete db;
  return 0;
}
