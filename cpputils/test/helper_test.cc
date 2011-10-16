#include <gtest/gtest.h>

#include "helper.h"

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
}

 int main(int argc, char **argv){
	 ::testing::InitGoogleTest(&argc, argv);
	 return RUN_ALL_TESTS();
 }
