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

/* Compatibility modes */
#ifdef __stl_compatibility
	#define _stl_use_containers 1
	#define _stl_use_string 0
	#define _stl_override_streams 1
	#define _stl_throw_exceptions 1
	#define _stl_accept_stl_params 1
#else
	#define _stl_use_containers 0
	#define _stl_use_string 0
	#define _stl_override_streams 0
	#define _stl_throw_exceptions 0
	#define _stl_accept_stl_params 0
#endif



__ztl_namespace_end
