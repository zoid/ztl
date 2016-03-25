#ifndef _segment_tree_hpp_1337420
#define _segment_tree_hpp_1337420

#include <cstdlib>
#include <memory>
#include <iostream>
#include <cmath>

#include "../ztl_base.hpp"
#include "../container/segment.hpp"

__ztl_namespace_start

	template <class T> class segment_tree {
	private:
		__stl::size_t data_size, tree_size;
		T* tree;

		inline T m_query(int l, int r, int i) {
			int range_from = pow(2, log2(data_size) - floor(log2(i))) * i,
				range_to = range_from + pow(2, log2(data_size) - floor(log2(i))) - 1;

			if (r <= range_from || range_to <= l) return T();
			else if (l <= range_from && range_to <= r) { return tree[i]; }
			return query(l, r, i * 2) + query(l, r, i * 2 + 1);
		}

		inline void m_update(int i) { tree[i] = tree[2 * i] + tree[2 * i + 1]; }

	public:
		segment_tree<T>() : tree(NULL), data_size(0), tree_size(0) {}
		segment_tree<T>(const T* values, int size) {
			data_size = size;
			tree_size = pow(2, ceil(log2(2 * size)));
			tree = new T[tree_size];
			memcpy(tree + size, values, sizeof(T) * size);

			for (int i = data_size - 1; i > 0; i--) update(i);
		}

		inline T query(int l, int r) { return this->m_query(l + data_size, r + data_size, 1); }
		inline void update(int index, int value)
		{
			if (index > data_size) return;
			index += data_size;
			tree[index] = value;
			do {
				index /= 2;
				this->m_update(index);
			} while (index != 1);
		}
	};
}

#endif /* _segment_tree_hpp_1337420 */