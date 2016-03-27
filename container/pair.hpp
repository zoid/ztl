#ifndef _pair_hpp_1337420
#define _pair_hpp_1337420

#include "../ztl_base.hpp"

__ztl_namespace_start
	template <class T1, class T2>
	class pair {
	public:
		typedef T1 first_type;
		typedef T2 second_type;

		T1 first;
		T2 second;

		pair() : first(T1()), second(T2()) { }
		pair(const T1& _a, const T2& _b) : first(_a), second(_b) { }

		template <class U1, class U2>
		pair(const pair<U1, U2>& _p) : first(_p.first), second(_p.second) { }

		inline pair& operator=(const pair& _x) { this->first = _x.first; this->second = _x.second; return *this; }
		inline bool operator==(const pair& _x) { return _x.first == this->first && _x.second == this->second; }
		inline bool operator<(const pair& _x) { return _x.first < this->first || (!(this->first < _x.first) && _x.second < this->second); }
		inline bool operator!=(const pair& _x) { return !(_x == &this); }
		inline bool operator>(const pair& _x) { return &this <_x; }
		inline bool operator<=(const pair& _x) { return !(&this < _x); }
		inline bool operator>=(const pair& _x) { return !(_x < &this); }
	};

	template <class T1, class T2>
	pair<T1, T2> make_pair(const T1& _x, const T2& _y) { return pair<T1, T2>(_x, _y); }


	template<class T>
	struct select1st {
		typedef typename T::first_type return_type;
		return_type operator()(const T _val) { return _val.first; }
	};
	
	template<class T>
	struct select2nd {
		typedef typename T::first_type return_type;
		return_type operator()(const T _val) { return _val.second; }
	};

__ztl_namespace_end

#endif /*_pair_hpp_1337420 */
