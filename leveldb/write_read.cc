#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include "leveldb/db.h"
#include "leveldb/iterator.h"
#include "keygen.h"

using namespace std;

int main(int argc, const char *argv[])
{
	
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "db/write_read", &db);
  if (!status.ok()) {
	  cerr << status.ToString() << endl;
	  exit(1);
  }

  vector<string> keys;
  int total_keys = 10;
  keys.reserve(total_keys);

  for (int i=0; i<total_keys; i++) {
	  const char *k = nextKey();
	  keys.push_back(string(k));
	  leveldb::Status s = db->Put(leveldb::WriteOptions(), k, k);
	  if (!s.ok()) {
		  cerr<< s.ToString()<<endl;
		  break;
	  }
  }

  {
	  vector<string>::iterator itr = keys.begin(),
		  itre = keys.end();
	  for (; itr!=itre; itr++) {
		  string value;
		  leveldb::Status s = db->Get(leveldb::ReadOptions(), *itr, &value);
		  if (s.ok()) {
			  cout<<"key:"<<*itr<<" value:"<<value<<endl;
		  }
	  }
  }

  cout<<"using leveldb iterator"<<endl;
  {
	  leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	  for (it->SeekToFirst(); it->Valid(); it->Next()) {
		  cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
	  }
	  assert(it->status().ok());  // Check for any errors found during the scan
	  delete it;
  }

  delete db;
  return 0;
}
