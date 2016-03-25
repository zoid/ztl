#ifndef _interval_hpp_1337420
#define _interval_hpp_1337420

#include "../ztl_base.hpp"
#include <cstdlib>

__ztl_namespace_start
template <class T, typename K = __stl::size_t>
class interval {
public:
	K start, stop;
	T value;
	interval(K _s, K _e) : value(T()), start(_s), stop(_e) { }
	interval(K _s, K _e, const T& _v) : start(_s), stop(_e), value(_v) {}

	struct start_comparer {
		bool operator() (const interval& a, const interval& b) { return a.start < b.start; }
	};

	struct end_comparer {
		bool operator() (const interval& a, const interval& b) { return a.end < b.end; }
	};

	inline bool operator=(const interval& _x) { return _x.start == start && _x.stop == stop && _x.value == value; }
	inline bool operator!=(const interval& _x) { return !(x == *this); }
};
__ztl_namespace_end
#endif /* _interval_hpp_1337420 */
