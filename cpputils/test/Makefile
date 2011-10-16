all_tests = helper_test

CXXFLAGS :=  -g -Wall -O0 
INCDIR := ../
CPPFLAGS += $(addprefix -I, $(INCDIR) ${GTEST_DIR}/include)

all : helper fixedsizeorderedlist


helper : helper_test.o libgtest.a
	g++ $(CXXFLAGS) -o helper_test helper_test.o libgtest.a

fixedsizeorderedlist : fixedsizeorderedlist_test.o libgtest.a
	g++ $(CXXFLAGS) -o fixedsizeorderedlist_test fixedsizeorderedlist_test.o libgtest.a

helper_test.o : helper_test.cc

fixedsizeorderedlist_test.o : fixedsizeorderedlist_test.cc

libgtest.a : ${GTEST_DIR}/src/gtest-all.cc
	g++ -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

clean:
	-rm -f *.o $(all_tests)

