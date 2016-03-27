#ifndef _hash_hpp_1337420
#define _hash_hpp_1337420

#include "../ztl_base.hpp"

__ztl_namespace_start

template <class _Key> struct hash { };
inline size_t hash_string(const char* _s) {
	unsigned long h = 0;
	for (; *_s; ++_s) h = 5 * h + *_s;
	return size_t(h);
}

template<> struct hash<char*> { size_t operator()(const char* _s) const { return hash_string(_s); } };
template<> struct hash<const char*> { size_t operator()(const char* _s) const { return hash_string(_s); } };
template<> struct hash<char> { size_t operator()(char _x) const { return _x; } };
template<> struct hash<unsigned char> { size_t operator()(unsigned char _x) const { return _x; } };
template<> struct hash<signed char> { size_t operator()(unsigned char _x) const { return _x; } };
template<> struct hash<short> { size_t operator()(short _x) const { return _x; } };
template<> struct hash<unsigned short> { size_t operator()(unsigned short _x) const { return _x; } };
template<> struct hash<int> { size_t operator()(int __x) const { return __x; } };
template<> struct hash<unsigned int> { size_t operator()(unsigned int _x) const { return _x; } };
template<> struct hash<long> { size_t operator()(long _x) const { return _x; } };
template<> struct hash<unsigned long> { size_t operator()(unsigned long _x) const { return _x; } };


__ztl_namespace_end

#endif /* _hash_hpp_1337420 */
