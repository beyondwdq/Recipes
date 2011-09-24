#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h> //strtoul
#include <string.h> //memcmp
#include "leveldb/db.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"
#include "leveldb/comparator.h"
#include "keygen.h"

using namespace std;

class UintComparator : public leveldb::Comparator 
{
	public:
	int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
		string sa = a.ToString(),
			   sb = b.ToString();
		uint32_t ia = strtoul(sa.c_str(), NULL, 0),
				 ib = strtoul(sb.c_str(), NULL, 0);
		if (ia>ib) return 1;
		if (ia<ib) return -1;
		return 0;
	}
	
	// Ignore the following methods for now:
    const char* Name() const { return "UintComparator"; }
    void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
    void FindShortSuccessor(std::string*) const { }
};

class StrlenComparator : public leveldb::Comparator 
{
	public:
	int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
		size_t sa = a.size(),
			   sb = b.size();
		if (sa==sb) return memcmp(a.data(), b.data(), sa);
		return (sa>sb ? 1 : -1);
	}
	
	// Ignore the following methods for now:
    const char* Name() const { return "StrlenComparator"; }
    void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
    void FindShortSuccessor(std::string*) const { }
};

int main(int argc, const char *argv[])
{

	StrlenComparator cmp;
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	options.comparator = &cmp;
	leveldb::Status status = leveldb::DB::Open(options, "db/comparator", &db);
	if (!status.ok()) {
		cerr << status.ToString() << endl;
		exit(1);
	}

	vector<string> keys;
	int total_keys = 101;
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
