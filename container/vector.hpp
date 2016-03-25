#ifndef _vector_hpp_1337420
#define _vector_hpp_1337420

#include <cstdlib>
#include <new>
#include <utility>
#include <memory>
#include <type_traits>
#include <iterator>

#include "../ztl_base.hpp"

__ztl_namespace_start
	template <class T>
	class vector {
	public:
		typedef T						value_type;
		typedef T&						reference;
		typedef T*						iterator;
		typedef const T*						const_iterator;
		typedef __stl::random_access_iterator_tag		iterator_category;
		typedef __stl::reverse_iterator<const_iterator>		const_reverse_iterator;
		typedef __stl::reverse_iterator<iterator>		reverse_iterator;
		typedef __stl::size_t					size_type;
		typedef __stl::ptrdiff_t					difference_type;

	private:
		iterator p_begin;
		iterator p_last;
		iterator p_end;

		inline void enlarge() {
			enlarge((p_end - p_begin + 1) * 2);
		}

		inline void enlarge(int size) {
			int n = p_last - p_begin;
			p_begin = (iterator)realloc((void*)p_begin, sizeof(value_type) * size);
			p_end = p_begin + size - 1;
			p_last = p_begin + n;
		}

	public:
		vector() {
			p_begin = p_end = (iterator)malloc(sizeof(value_type));
			p_last = p_begin - 1;
		}

		vector(size_type starting_size) {
			p_begin = (iterator)malloc(sizeof(value_type) * starting_size);
			p_end = p_begin + starting_size - 1;
			p_last = p_begin - 1;
		}

		~vector() {
			if (!__stl::is_trivially_destructible<value_type>::value) {
				++p_last;
				while (--p_last >= p_begin) {
					(p_last)->~T();
				}
			}

			free(p_begin);
		}

		template<class... Args>
		inline void emplace_back(Args&&... args) {
			if (p_last == p_end) enlarge();
			new (++p_last) T(__stl::forward<Args>(args)...);
		}

		inline void push_back(const T& other) {
			if (p_last == p_end) enlarge();
			new (++p_last) T(other);
		}

		/* Access elements */
		inline reference at(unsigned int id) {
			if (!(id < size()))
				throw __stl::out_of_range("Index out of range");
			return p_begin[id];
		}

		inline const reference at(unsigned int id) const {
			if (!(id < size()))
				throw __stl::out_of_range("Index out of range");
			return p_begin[id];
		}

		inline reference operator[](unsigned int id) { return p_begin[id]; }
		inline const reference operator[](unsigned int id) const { return p_begin[id]; }


		/* Iterators */
		inline iterator				begin() { return p_begin; }
		inline iterator				end() { return p_end; }
		inline const_iterator			cbegin() const { return p_begin; }
		inline const_iterator			cend() const { return p_end; }
		inline reverse_iterator			rbegin() { return reverse_iterator(p_end); }
		inline reverse_iterator			rend() { return reverse_iterator(p_begin); }
		inline const_reverse_iterator		crbegin() const { return const_reverse_iterator(p_end); }
		inline const_reverse_iterator		crend() const { return const_reverse_iterator(p_begin); }

		inline reference			first() { return *p_begin; }
		inline const reference			first() const { return *p_begin; }
		inline reference			last() { return *p_last; }
		inline const reference			last() const { return *p_last; }

		/* Size / Allocation */
		inline bool				empty() const { return size() == 0; }
		inline size_type			size() const { return p_last - p_begin + 1; }
		inline size_type			reserved() const { return p_end - p_begin + 1; }
		inline void				reserve(size_type num) { enlarge(num); }
		inline void				clear() { erase(p_begin, p_last); }
		inline void				erase(size_type a, size_type b) { erase(p_begin + a, p_begin + b); }
		inline void				erase(iterator pa, iterator pb) {
			if (!__stl::is_trivially_destructible<value_type>::value) {
				iterator a = pa - 1;
				while (a < pb) (++a)->~T();
			}

			memmove(pa, pb + 1, sizeof(value_type) * (p_last - pb));
			p_last = pa + (p_last - pb) - 1;
		}
	};

__ztl_namespace_end

#endif /* _vector_hpp_1337420 */
