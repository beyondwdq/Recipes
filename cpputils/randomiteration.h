#ifndef RANDOM_ITERATION_H
#define RANDOM_ITERATION_H

#include <algorithm>
#include <iterator>
#include "randomrange.h"

namespace mtl {
template <typename InputIterator>
class RandomIteration
{
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;

    public: 
    RandomIteration(InputIterator first, InputIterator last)
        : first_(first)
        , last_(last) 
        , len_ (std::distance(first, last))
        , generator_(0, len_)
    {
    }

    InputIterator next()
    {
        if (generator_.hasNext()){
            InputIterator itr = first_;
            std::advance(itr, generator_.next());
            return itr;
        }
        return last_;
    }

    private:
    InputIterator first_;
    InputIterator last_;
    difference_type len_;
    RandomRange generator_;
};

} //namespace mtl

#endif //RANDOM_ITERATION_H
