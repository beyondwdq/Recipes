#include <set>

template < class Key, class Compare = std::less<Key>,
		 class Allocator = std::allocator<Key> >
class FixedSizeOrderedList : public std::multiset<Key, Compare, Allocator>
{
	public:

	typedef typename std::multiset<Key, Compare, Allocator>::value_type value_type;
	typedef typename std::multiset<Key, Compare, Allocator>::iterator iterator;

	FixedSizeOrderedList(size_t max_size)
		: std::multiset<Key, Compare, Allocator>()
		, max_size_(max_size) {
		}


	iterator insert(const value_type& x) {
		if (!isAddable(x)) return std::multiset<Key, Compare, Allocator>::end();

		iterator itr = std::multiset<Key, Compare, Allocator>::insert(x);

		if (std::multiset<Key, Compare, Allocator>::size()>maxSize()){
			std::multiset<Key, Compare, Allocator>::erase(
					--(std::multiset<Key, Compare, Allocator>::end()));
		}

		return itr;
	}

	bool isAddable(const value_type&x) {
		return ( (std::multiset<Key, Compare, Allocator>::size()<maxSize()) ||
				std::multiset<Key, Compare, Allocator>::key_comp()(x, back()) );
	}


	size_t maxSize() const {
		return max_size_;
	}

	private:
	const value_type& back() const {
		iterator itr = std::multiset<Key, Compare, Allocator>::end();
		return *(--itr);
	}


	private:
	size_t max_size_;
};
