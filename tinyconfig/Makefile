objects = config.o plainconfigreader.o luaconfigreader.o

CXXFLAGS :=  -I/opt/local/include -g -Wall -O0

libconfig.a : $(objects)
	ar -rv $@ $(objects)

config.o : config.cc config.h

plainconfigreader.o : plainconfigreader.cc plainconfigreader.h configreader.h

luaconfigreader.o : luaconfigreader.cc luaconfigreader.h configreader.h

clean:
	-rm -f *.o libconfig.a

