all_tests = edsim_naive_test

CXXFLAGS :=  -g -Wall -O0 
INCDIR := ../
CPPFLAGS += $(addprefix -I, $(INCDIR), ${GTEST_DIR}/include)


edsim_naive : edsim_naive_test.o libgtest.a
	g++ $(CXXFLAGS) -o edsim_naive_test edsim_naive_test.o ../edsim_naive.o libgtest.a

edsim_naive_test.o : edsim_naive_test.cc

libgtest.a : ${GTEST_DIR}/src/gtest-all.cc
	g++ -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

clean:
	-rm -f *.o $(all_tests)

