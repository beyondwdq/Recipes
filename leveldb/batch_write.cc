#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <string.h> //memcmp

#include "leveldb/db.h"
#include "leveldb/slice.h"
#include "leveldb/iterator.h"
#include "leveldb/comparator.h"
#include "leveldb/write_batch.h"
#include "keygen.h"

using namespace std;

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

template <typename T>
struct PtrGuard
{
	PtrGuard(T *p)
		: p_(p) {}

	~PtrGuard() {
		delete p_;
	}

	T *p_;
};

int main(int argc, const char *argv[])
{

	StrlenComparator cmp;
	leveldb::DB* db = NULL;

	leveldb::Options options;
	options.create_if_missing = true;
	options.comparator = &cmp;
	leveldb::Status status = leveldb::DB::Open(options, "db/batch_write", &db);
	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return 1;
	}

	PtrGuard<leveldb::DB> guard(db);
	vector<string> keys;
	int total_keys = 101;

	leveldb::WriteBatch batch;
	for (int i=0; i<total_keys; i++) {
		const char *k = nextKey();
		batch.Put(k, k);
	}

	leveldb::Status s = db->Write(leveldb::WriteOptions(), &batch);
	if (!s.ok()) {
		cerr<< s.ToString()<<endl;
	}

	{
		leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
		for (it->SeekToFirst(); it->Valid(); it->Next()) {
			cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
		}
		assert(it->status().ok());  // Check for any errors found during the scan
		delete it;
	}

	return 0;
}
