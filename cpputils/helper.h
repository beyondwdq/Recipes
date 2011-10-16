#include <algorithm>
#include <iterator>
#include <cassert>

/* My template library ^-^ */
namespace mtl{

/* sum */
//Adder
template <typename T>
struct Adder : public std::unary_function<T,void>{
	Adder() : sum_() {}
	void operator() (T x) { sum_ += x; }
	T sum_;
		
};

template <typename T, class InputIterator>
T sum(InputIterator first, InputIterator last){
	return std::for_each(first, last, Adder<T>()).sum_;
}

/* sum with extractor. It's a generalization of sum() */
//Specialize this class template for your type
template <typename T, typename Treturn>
struct Extractor{
	typedef Treturn return_type;
	static Treturn extract(const T& t){
		assert(false);
		return Treturn();
	}
};

//Two extractors for std::pair
template <typename Tpair>
struct PairFirstExtractor{
	typedef typename Tpair::first_type return_type;
	static return_type extract(const Tpair& t){
		return t.first;
	}
};
template <typename Tpair>
struct PairSecondExtractor{
	typedef typename Tpair::second_type return_type;
	static return_type extract(const Tpair& t){
		return t.second;
	}
};

template <typename T, typename Tsum, typename Textract>
struct AdderWithExtractor : public std::unary_function<T,void>{
	AdderWithExtractor() : sum_() {}
	void operator() (const T& x) { sum_ += Textract::extract(x); }
	Tsum sum_;
		
};


/* Though usually Tsum could be Textract::return_type, we leave Tsum
 * as a sperate template parameter since sometimes a different type 
 * (i.e. a double type to sum float values) may be needed to avoid
 * overflow. */
template <typename Tsum, typename Textract, typename InputIterator>
Tsum sum(InputIterator first, InputIterator last){
	typedef typename InputIterator::value_type Tvalue;
	return std::for_each(first, last, 
			AdderWithExtractor<Tvalue, Tsum, Textract>()
			).sum_;
}
/* END of sum with extractor. It's a generalization of sum() */


//variance
template <typename T>
struct DiffSquareAdder : public std::unary_function<T,void>{
	DiffSquareAdder(T m) : sum_(), mean_(m), count_() {}
	void operator() (T x) { 
		T diff = x - mean_;
		sum_ += (diff*diff);
		++count_;
	}
	T sum_;
	T mean_;
	size_t count_;
};
template <typename InputIterator>
double variance(InputIterator first, InputIterator last, double mean){
	DiffSquareAdder<double> result = std::for_each(first, last, DiffSquareAdder<double>(mean));
	double var = result.sum_ / (double)(result.count_-1);
	return var;
}
template <typename InputIterator>
double variance(InputIterator first, InputIterator last){
	double mean = mtl::sum(first, last) / (double)(std::distance(first,last)-1);
	return variance<InputIterator>(first, last, mean);
}
//end of variance

//counter
template <typename T, typename Predicate>
struct Counter{
	Counter(const Predicate& pred) 
	: pred_(pred)
	, count_(0) {}

	void operator ()(T x){
		if (pred_(x)) ++count_;
	}

	Predicate pred_;
	size_t count_;
};

template <typename InputIterator, typename Predicate>
size_t count(InputIterator first, InputIterator last, Predicate pred){
	typedef typename std::iterator_traits<InputIterator>::value_type value_type; 
	return std::for_each(
			first, last, 
			Counter<value_type, Predicate>(pred)
			).count_;
}
//end of counter

//shrink a container, for std::set or std::multiset which lack this method
template <typename Container>
void shrinkToSize(Container& c, size_t s){
	size_t cur_size = c.size();
	if ( cur_size>s ){
		typename Container::iterator itr = c.end();
		int diff = (int)s - (int)cur_size;
		std::advance(itr, diff);
		c.erase(itr, c.end());
	}
}

/* Update an elemennt in std::map, or add it if not exists.
 * copied from <<Effective STL>> */
template<typename MapType, typename KeyArgType, typename ValueArgtype>
typename MapType::iterator efficientAddOrUpdate(MapType& m,
		const KeyArgType& k,
		const ValueArgtype& v)
{
	typename MapType::iterator Ib =
		m.lower_bound(k);
	if(Ib != m.end() && !(m.key_comp()(k, Ib->first))) {
		Ib->second = v;
		return Ib;
	}
	else{
		typedef typename MapType::value_type MVT;
		return m.insert(Ib, MVT(k, v)); 
	}
}

}

