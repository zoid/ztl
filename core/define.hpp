#define __ztl_namespace_start  namespace ztl {
#define __ztl_namespace_end  }

#define __stl ::std
#define __ztl ztl

__ztl_namespace_start

#ifdef __stl_compatibility
	#include <cstdlib>
	typedef __stl::size_t size_t;
#else
	typedef unsigned long long size_t;
#endif


__ztl_namespace_end
