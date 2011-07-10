all_tests = config_test

CXXFLAGS :=  -g -Wall -O0 
INCDIR := ../
CPPFLAGS += $(addprefix -I, $(INCDIR) ${GTEST_DIR}/include)


config : config_test.o libgtest.a
	g++ $(CXXFLAGS) -o config_test config_test.o ../config.o libgtest.a

config_test.o : config_test.cc

libgtest.a : ${GTEST_DIR}/src/gtest-all.cc
	g++ -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

clean:
	-rm -f *.o $(all_tests)

