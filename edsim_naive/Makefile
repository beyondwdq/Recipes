objects = edsim_naive.o mm1.o

CXXFLAGS :=  -g -Wall -O0 

mm1 : $(objects)
	g++ $(CXXFLAGS) -o mm1 $(objects)

edsim_naive.o : edsim_naive.cc edsim_naive.h

mm1.o : mm1.cc edsim_naive.h


clean:
	-rm -f *.o mm1

