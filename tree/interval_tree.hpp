#ifndef _interval_tree_hpp_1337420
#define _interval_tree_hpp_1337420

#include <cstdlib>

#include "../ztl_base.hpp"
#include "../container/interval.hpp"
#include "../container/vector.hpp"

__ztl_namespace_start
	template <class T, typename K = __stl::size_t>
	class interval_tree {
	public:
		typedef vector<interval<T, K>>		iv;
		typedef interval_tree*		tree_ptr;

		iv intervals;
		tree_ptr left;
		tree_ptr right;
		K center;

		interval_tree(void) : left(NULL), right(NULL), center(K()) { }
		interval_tree(iv& ivals, K depth = 16, K minbucket = 64, K leftextent = 0, K rightextent = 0, K maxbucket = 512) {
			--depth;
			if (depth == 0 || (ivals.size() < minbucket && ivals.size() < maxbucket)) {
				__stl::sort(ivals.begin(), ivals.end(), interval<T, K>::start_comparer());
				intervals = ivals;
			}
			else {
				if (leftextent == 0 && rightextent == 0) {
					__stl::sort(ivals.begin(), ivals.end(), interval<T, K>::start_comparer());
				}

				K leftp = 0;
				K rightp = 0;
				K centerp = 0;

				if (leftextent || rightextent) {
					leftp = leftextent;
					rightp = rightextent;
				}
				else {
					leftp = ivals.front().start;
					vector<K> stops;
					stops.resize(ivals.size());
					for (unsigned int i = 0; i < ivals.size(); i++)
						stops[i] = ivals[i].stop;
					rightp = *max_element(stops.begin(), stops.end());
				}

				centerp = ivals[ivals.size() / 2].start;
				center = centerp;

				iv lefts, rights;

				for (auto i = ivals.cbegin(); i != ivals.cend(); ++i) {
					if ((*i).stop < center)		lefts.push_back((*i));
					else if ((*i).start > center)	rights.push_back((*i));
					else  intervals.push_back((*i));
				}

				if (!lefts.empty()) {
					left = tree_ptr(new interval_tree(lefts, depth, minbucket, leftp, centerp));
				}
				if (!rights.empty()) {
					right = tree_ptr(new interval_tree(rights, depth, minbucket, centerp, rightp));
				}
			}
		}

		interval_tree(const interval_tree&) {
			intervals = other.intervals;
			left = other.left ? copyTree(*other.left) : NULL;
			right = other.right ? copyTree(*other.right) : NULL;
			center = other.center;
		}

		interval_tree& operator=(const interval_tree&) {
			center = other.center;
			intervals = other.intervals;
			left = other.left ? copyTree(*other.left) : NULL;
			right = other.right ? copyTree(*other.right) : NULL;
			return *this;
		}
		~interval_tree(void) = default;

	private:
		inline tree_ptr copyTree(const interval_tree& orig) { return tree_ptr(new interval_tree(orig)); }

		void m_query_overlapping(K start, K stop, iv& overlapping) const {
			if (!intervals.empty() && !(stop < intervals.front().start))
				for (auto val : intervals)
					if (val.stop >= start && val.start <= stop)
						overlapping.push_back(val);

			if (left && start <= center) left->query_overlapping(start, stop, overlapping);
			if (right && stop >= center) right->query_overlapping(start, stop, overlapping);
		};


		void m_query_contained(K start, K stop, iv& contained) const{
			if (!intervals.empty() && !(stop < intervals.front().start))
				for (auto val : intervals)
					if (val.start >= start && val.stop <= stop)
						contained.push_back(val);

			if (left && start <= center) left->query_contained(start, stop, contained);
			if (right && stop >= center) right->query_contained(start, stop, contained);
		}

	public:
		inline iv query_overlapping(K start, K stop) const {
			iv vec;
			this->m_query_overlapping(start, stop, vec);
			return vec;
		}

		inline iv query_contained(K start, K stop) const {
			iv contained;
			this->m_query_contained(start, stop, contained);
			return contained;
		}
	};
__ztl_namespace_end

#endif /* _interval_tree_hpp_1337420 */
