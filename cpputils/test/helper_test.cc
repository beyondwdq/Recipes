#include <stdlib.h>
#include <vector>
#include <iostream>
#include <gtest/gtest.h>

#include "helper.h"

using namespace std;

//for CountTest
struct A{
	A(int i) : s_(i) {}

	int s_;
};

struct GreaterThanPred{
	GreaterThanPred(int threshold)
		: threshold_(threshold) 
	{}

	bool operator ()(int x){
		return x>threshold_;
	}

	int threshold_;

};
//end of CountTest

namespace{
	class HelperTest: public ::testing::Test{
		protected:
			HelperTest(){
			}

			virtual ~HelperTest(){
			}

			virtual void SetUp(){
			}

			virtual void TearDown(){
			}

	};

	 TEST_F(HelperTest, CountTest){
		 const int array_size = 20;
		 int a[array_size];
		 for (int i=0; i<array_size; i++){
			 a[i] = i;
		 }
		 int cnt = mtl::count(a, a+array_size, GreaterThanPred(10));
		 ASSERT_EQ( cnt, 9 );
	 }

	 TEST_F(HelperTest, toStringTest){
		 const char *delim = ":";
		 vector<int> v0, v1, vn;
		 string stest;
		 vector<int>::iterator itr = vn.begin(), 
			 itre = vn.end();

		 for (int i=0; i<5; i++){
			 vn.push_back(i);
			 if (!stest.empty()){
				 stest.append(delim);
			 }
			 char buf[10];
			 sprintf(buf, "%d", i);
			 stest.append(buf);
		 }
		 string sout = mtl::toString(vn.begin(), vn.end(), delim);
		 ASSERT_EQ(stest, sout);
		 string sout0 = mtl::toString(v0.begin(), v0.end(), delim);
		 ASSERT_EQ(string(""), sout0);
		 v1.push_back(1);
		 string sout1 = mtl::toString(v1.begin(), v1.end(), delim);
		 ASSERT_EQ(string("1"), sout1);
	 }
}

 int main(int argc, char **argv){
	 ::testing::InitGoogleTest(&argc, argv);
	 return RUN_ALL_TESTS();
 }
