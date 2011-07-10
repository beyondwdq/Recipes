#include <gtest/gtest.h>

#include "config.h"

namespace{
	class DummyConfig : public Config{
		public:
			bool indicator_;
			int count_;
			double ratio_;

			DummyConfig()
				: Config()
			{
				add("indicator", &indicator_, true);
				add("count", &count_, 100);
				add("ratio", &ratio_, 0.1);
			}
	};

	class ConfigTest: public ::testing::Test{
		protected:
			ConfigTest(){
			}

			virtual ~ConfigTest(){
			}

			virtual void SetUp(){
			}

			virtual void TearDown(){
			}

	};

	 TEST_F(ConfigTest, LoadConfigTest){
		 DummyConfig config;
		 ASSERT_EQ( config.indicator_, true );
		 ASSERT_EQ( config.count_, 100 );
		 ASSERT_EQ( config.ratio_, 0.1 );

		 bool suc = config.load("file_not_existed.txt");
		 ASSERT_FALSE(suc);

		 suc = config.load("bool_incorrect_cfg.txt");
		 ASSERT_FALSE(suc);

		 suc = config.load("int_incorrect_cfg.txt");
		 ASSERT_FALSE(suc);

		 suc = config.load("double_incorrect_cfg.txt");
		 ASSERT_FALSE(suc);

		 suc = config.load("cfg.txt");
		 ASSERT_TRUE(suc);
		 ASSERT_EQ( config.indicator_, false );
		 ASSERT_EQ( config.count_, 200 );
		 ASSERT_EQ( config.ratio_, 100.1 );
	 }
}

 int main(int argc, char **argv){
	 ::testing::InitGoogleTest(&argc, argv);
	 return RUN_ALL_TESTS();
 }
