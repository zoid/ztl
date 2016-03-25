/* Used in segment_tree.hpp */

#ifndef _segment_hpp_1337420
#define _segment_hpp_1337420

#include "../ztl_base.hpp"

__ztl_namespace_start

#ifndef __min
	#define __min(A, B) A < B ? A : B
#endif

#ifndef __max 
	#define __max(A, B) A < B ? B : A
#endif

template <class Key> class segment {
public:
	Key min, max, sum;
	friend const segment<Key> operator+(const segment<Key>& s1, const segment<Key>& s2) {
		return segment<Key>(__min(s1.min, s2.min), __max(s1.max, s2.max), s1.sum + s2.sum);
	}

	segment<Key>() : min(INT_MAX), max(INT_MIN), sum(NULL) {};
	segment<Key>(Key value) : min(value), max(value), sum(value) {};
	segment<Key>(Key _min, Key _max, Key _sum) : min(_min), max(_max), sum(_sum) {};
};

__ztl_namespace_end


#endif /* _segment_hpp_1337420 */