cpputils
========

This directory containes some utilities I have written during my developement.

* fixedsizeorderedlist.h implements an ordered list with fixed size. 
The class `FixedSizeOrderedList` is derived from `std::set`. It works, 
but it is risky to derive a class from `std::set` since it does have a virtual
destructor (Effective C++ 3rd edition, item 7). Currently private inheritance is
used to solve this problem. It would be better to use aggregation instead of
inheritance.

* helper.h includes a set of utility functions for various purposes. All functions 
are in the namespace `mtl`.

* randomrange.h and randomrange.cc implement a random number generator in a range,
namely `RandomRange`.  Now the type of the range start and end is `uint32_t`. 
I plan to change it to a template in the future.

* randomiteration.h implements a generator which can produce iterators that randomly
iterate over a range of a stl container. It depends on `RandomRange`.
