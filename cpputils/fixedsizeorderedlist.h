#ifndef FIXEDSIZEORDEREDLIST_H
#define FIXEDSIZEORDEREDLIST_H

#include <set>

/* NOTE: It is risky to derive a class from `std::set` since it does have a virtual
 * destructor (Effective C++ 3rd edition, item 7). Currently private inheritance
 * is used to solve this problem.It would be better to use
 * aggregation instead of inheritence. This is a part of the future work.
 * */

template < class Key, class Compare = std::less<Key>,
		 class Allocator = std::allocator<Key> >
class FixedSizeOrderedList : private std::multiset<Key, Compare, Allocator>
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

#endif //FIXEDSIZEORDEREDLIST_H
