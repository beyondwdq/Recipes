#include <gtest/gtest.h>

#include "fixedsizeorderedlist.h"


namespace{

	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) < 0;
		}
	};

	class FixedSizeOrderedListTest: public ::testing::Test{
		protected:
			FixedSizeOrderedListTest(){
			}

			virtual ~FixedSizeOrderedListTest(){
			}

			virtual void SetUp(){
			}

			virtual void TearDown(){
			}

	};

	 TEST_F(FixedSizeOrderedListTest, InsertTest){
		 const int N = 6;
		 const int max_size = 5;
		 const char* a[N] = {"isomer", "ephemeral", "prosaic", 
			 "nugatory", "artichoke", "serif"};
		 FixedSizeOrderedList<const char*, ltstr> A(max_size);
		 for (int i=0; i<N; i++){
			 A.insert(a[i]);
		 }
		 ASSERT_EQ( (int)A.size(), max_size );

		 const char *last_item = NULL;
		 FixedSizeOrderedList<const char*, ltstr>::iterator itr = A.begin(),
			 itre = A.end();
		 for (; itr!=itre; itr++){
			 if (last_item){
				 ASSERT_TRUE(ltstr()(last_item, *itr));
			 }
			 last_item = *itr;
		 }
	 }
}

 int main(int argc, char **argv){
	 ::testing::InitGoogleTest(&argc, argv);
	 return RUN_ALL_TESTS();
 }
