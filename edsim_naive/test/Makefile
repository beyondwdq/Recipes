all_tests = edsim_naive_test

CXXFLAGS :=  -g -Wall -O0 
INCDIR := ../
CPPFLAGS += $(addprefix -I, $(INCDIR))

edsim_naive : edsim_naive_test.o
	g++ $(CXXFLAGS) -o edsim_naive_test edsim_naive_test.o ../edsim_naive.o -lgtest

edsim_naive_test.o : edsim_naive_test.cc

clean:
	-rm -f *.o $(all_tests)

