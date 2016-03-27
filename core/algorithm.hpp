#ifndef _algorithm_hpp_1337420
#define _algorithm_hpp_1337420

#include "../ztl_base.hpp"

__ztl_namespace_start

template<typename T> void swap(T& t1, T& t2) {
	T tmp(t1);
	t1 = t2;     
	t2 = tmp; 			 
}

template<typename T>
void memset32(T *dest, T val, uint32_t count) {
	/*__asm {
		mov ecx, val
		mov eax, count
		mov edi, dest
		rep stosd
	};*/
	while (count--)
		*dest++ = val;
}

template<typename T>
void memset_custom(T *dest, T val, uint32_t count) {
	while (count--)
		*dest++ = val;
}


__ztl_namespace_end

#endif /*_algorithm_hpp_1337420 */
