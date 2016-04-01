#ifndef _heap_hpp_1337420
#define _heap_hpp_1337420

#include "../ztl_base.hpp"
#include "../core/algorithm.hpp"

#if _stl_use_containers 
	#include <vector>
#else	
	#include "vector.hpp"
#endif

__ztl_namespace_start

template<class T, class Compare = less<T> >
class heap {
public:
	typedef size_t size_type;

	heap() : m_comp(Compare()) { }
	heap(T* arr, int size) : m_data(size), m_comp(Compare()) {
		for (int i = 0; i < size; i++) m_data[i] = arr[i];
		m_heapify();
	}

#if _stl_use_containers
	heap(const __stl::vector<T>& _vec) : m_data(_vec), m_comp(Compare()) { m_heapify(); }
#else
	heap(const vector<T>& _vec) : m_data(_vec), m_comp(Compare()) { m_heapify(); }
#endif
	~heap() = default;

	// ASIGN OPERATOR
	heap& operator= (const heap& _heap) {
		if (&_heap != this) {
			m_comp = _heap.m_comp;
			m_data = _heap.m_data;
		}

		return *this;
	}


	inline void			push(int element) {
		m_data.push_back(element);
		m_bubble_up(m_data.size() - 1);
	}

	inline void			pop() {
		if (empty()) return;

		m_data[0] = m_data.at(m_data.size() - 1);
		m_data.pop_back();
		m_bubble_down(0);
	}

	inline const T&			top()			{ return empty() ? T(0) : m_data[0]; }
	inline bool			empty() const		{ return m_data.empty(); }
	inline size_type		size()	const		{ return m_data.size(); }
	inline size_type		max_size() const	{ return m_data.max_size(); }
	inline void			clear()			{ m_data.clear(); }

private:

#if _stl_use_containers
	__stl::vector<T>		m_data;
#else
	vector<T>			m_data;
#endif

	Compare				m_comp;

	inline void			m_heapify() { for (int i = m_data.size() - 1; i >= 1; i--) m_bubble_up(i);  }
	inline int			m_left(int _i) { return ((_i << 1) + 1 < m_data.size()) ? (_i << 1) + 1 : -1; }
	inline int			m_right(int _i) { return ((_i << 1) + 2 < m_data.size()) ? (_i << 1) + 2 : -1; }
	inline int			m_parent(int _i) { return (_i == 0) ? -1 : (_i - 1) >> 1; }

	void				m_bubble_up(int _idx) {
		while ((_idx > 0) && (m_parent(_idx) >= 0) && m_comp(m_data[_idx], m_data[m_parent(_idx)])) {
			swap(m_data[_idx], m_data[m_parent(_idx)]);
			_idx = m_parent(_idx);
		}
	}

	void				m_bubble_down(int _idx) {
		int child = m_left(_idx);
		if ((child > 0) && (m_right(_idx) > 0) && m_comp(m_data[m_right(_idx)], m_data[child]))
			child = m_right(_idx);

		if (child > 0 && (m_comp(m_data[child], m_data[_idx]))) {
			swap(m_data[_idx], m_data[child]);
			m_bubble_down(child);
		}
	}
};


__ztl_namespace_end

#endif
