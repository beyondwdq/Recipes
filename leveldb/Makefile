
CXXFLAGS :=  -Wall 

createdb : createdb.o keygen.o
	g++ $(CXXFLAGS) -o createdb createdb.o keygen.o libleveldb.a

write_read : write_read.o keygen.o
	g++ $(CXXFLAGS) -o write_read write_read.o keygen.o libleveldb.a

comparator : comparator.o keygen.o
	g++ $(CXXFLAGS) -o comparator comparator.o keygen.o libleveldb.a

batch_write : batch_write.o keygen.o
	g++ $(CXXFLAGS) -o batch_write batch_write.o keygen.o libleveldb.a

keygen.o : keygen.cc keygen.h

createdb.o : createdb.cc keygen.h

write_read.o : write_read.cc keygen.h

comparator.o : comparator.cc keygen.h

batch_write.o : batch_write.cc keygen.h

clean:
	-rm -f *.o createdb write_read comparator batch_write
