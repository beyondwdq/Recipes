#include <gtest/gtest.h>

#include "config.h"
#include "plainconfigreader.h"

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
		 PlainConfigReader *reader = new PlainConfigReader();
		 DummyConfig config;
		 ASSERT_EQ( config.indicator_, true );
		 ASSERT_EQ( config.count_, 100 );
		 ASSERT_EQ( config.ratio_, 0.1 );

		 bool suc = reader->load("file_not_existed.txt", &config);
		 ASSERT_FALSE(suc);

		 suc = reader->load("bool_incorrect_cfg.txt", &config);
		 ASSERT_FALSE(suc);

		 suc = reader->load("int_incorrect_cfg.txt", &config);
		 ASSERT_FALSE(suc);

		 suc = reader->load("double_incorrect_cfg.txt", &config);
		 ASSERT_FALSE(suc);

		 suc = reader->load("cfg.txt", &config);
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
