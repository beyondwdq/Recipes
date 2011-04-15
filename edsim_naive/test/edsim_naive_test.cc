#include <gtest/gtest.h>

#include "edsim_naive.h"

namespace{
    void dummyEventHandler(void){
    }

	class EdsimNaiveTest: public ::testing::Test{
		protected:
			EdsimNaiveTest(){
			}

			virtual ~EdsimNaiveTest(){
			}

			virtual void SetUp(){
			}

			virtual void TearDown(){
			}

	};

	 TEST_F(EdsimNaiveTest, EventQueueTest){
         double time_seq[] = {100.0, 100.1, 0.0, 1000.5, 40.4};
         int array_size = sizeof(time_seq)/sizeof(time_seq[0]);
         double max_time = 0.0;
         for (int i=0; i<array_size; ++i){
             enqueue(Event(time_seq[i], &dummyEventHandler));
             if (time_seq[i]>max_time) max_time = time_seq[i];
         }
         run_sim(max_time);
		 ASSERT_EQ( now(), max_time );
	 }
}

 int main(int argc, char **argv){
	 ::testing::InitGoogleTest(&argc, argv);
	 return RUN_ALL_TESTS();
 }
