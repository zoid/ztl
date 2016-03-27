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

		template<class U1, class U2>
		inline pair& operator=(const pair<U1, U2>& _x) { if (*this != _x) { first = _x.first; second = _x.second; } return *this; }
		inline pair& operator=(const pair& _x) { if (*this != _x) { first = _x.first; second = _x.second; } return *this; }
		inline bool operator==(const pair& _x) { return _x.first == this->first && _x.second == this->second; }
		inline bool operator<(const pair& _x) { return _x.first < this->first || (!(this->first < _x.first) && _x.second < this->second); }
		inline bool operator!=(const pair& _x) { return _x.first != this->first && _x.second != this->second;}
		inline bool operator>(const pair& _x) { return &this <_x; }
		inline bool operator<=(const pair& _x) { return !(&this < _x); }
		inline bool operator>=(const pair& _x) { return !(_x < &this); }

		struct select1st {
			const T1 operator()(const pair<T1, T2> _val) { return _val.first; }
		};

		struct select2nd {
			const T2 operator()(const pair<T1, T2> _val) { return _val.second; }
		};
	};

	template <class T1, class T2>
	pair<T1, T2> make_pair(const T1& _x, const T2& _y) { return pair<T1, T2>(_x, _y); }

__ztl_namespace_end

#endif /*_pair_hpp_1337420 */
