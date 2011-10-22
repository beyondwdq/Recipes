objects = config.o plainconfigreader.o

CXXFLAGS :=  -g -Wall -O0 

libconfig.a : $(objects)
	ar -rv $@ $(objects)

config.o : config.cc config.h

plainconfigreader.o : plainconfigreader.cc plainconfigreader.h configreader.h

clean:
	-rm -f *.o libconfig.a

