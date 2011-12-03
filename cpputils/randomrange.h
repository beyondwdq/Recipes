
#ifndef p2pvod_randomrange_h
#define p2pvod_randomrange_h

#include <vector>
#include <algorithm>
#include <stdint.h>

//random sequence in range [start, end)
//use RandomRangePro if the difference of start and end is large
//but the required number of sequences is small
class RandomRange{
	public:
		RandomRange( uint32_t start, uint32_t end )
			: start_(start), end_(end), index_(0) {

			sequence_.resize( end-start );
			std::vector<uint32_t>::iterator itr = sequence_.begin(),
				itre = sequence_.end();
			for( uint32_t i=start; itr!=itre; itr++,i++ ) *itr = i;

			std::random_shuffle( sequence_.begin(), sequence_.end() );
		}

		bool hasNext() const{
			return index_<sequence_.size();
		}

		uint32_t next(){
			if ( index_<sequence_.size() ){
				return sequence_[index_++];
			}
			else
				return invalid_sequence;
		}

		void reshuffle(){
			std::random_shuffle( sequence_.begin(), sequence_.end() );
			index_ = 0;
		}
	private:
		std::vector<uint32_t> sequence_;
		uint32_t start_;
		uint32_t end_;
		uint32_t index_;
		static const uint32_t invalid_sequence;
};


#endif /* p2pvod_randomrange_h */

