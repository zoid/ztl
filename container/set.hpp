#ifndef _set_hpp_1337420
#define _set_hpp_1337420

#include "../ztl_base.hpp"
#include "../tree/rb_tree.hpp"

__ztl_namespace_start
template <class Key, class Compare = __stl::less<Key> >
class set {
public:
	typedef Key						key_type;
	typedef Key						value_type;
	typedef Compare						key_compare;
	typedef Compare						value_compare;

private:
	typedef rb_tree<Key, Key, key_compare>			Tree;
	Tree m_tree;

public:
	typedef typename Tree::const_pointer			pointer;
	typedef typename Tree::const_pointer			const_pointer;
	typedef typename Tree::const_reference			reference;
	typedef typename Tree::const_reference			const_reference;
	typedef typename Tree::const_iterator			iterator;
	typedef typename Tree::const_iterator			const_iterator;
	typedef typename Tree::const_reverse_iterator		reverse_iterator;
	typedef typename Tree::const_reverse_iterator		const_reverse_iterator;
	typedef typename Tree::size_type			size_type;
	typedef typename Tree::difference_type			difference_type;


	set() : m_tree(Compare()) { }
	set(const set<Key, Compare>& _x) : m_tree(_x.m_tree) {}
	explicit set(const Compare& _comp) : m_tree(_comp) {}


	// ACCESS:
	inline key_compare			key_comp() const { return m_tree.key_comp(); }
	inline value_compare			value_comp() const { return m_tree.key_comp(); }

	inline iterator				begin() const { return m_tree.cbegin(); }
	inline iterator				end() const { return m_tree.cend(); }
	inline reverse_iterator			rbegin() const { return m_tree.rbegin(); }
	inline reverse_iterator			rend() const { return m_tree.rend(); }

	inline bool				empty() const { return m_tree.empty(); }
	inline size_type			size() const { return m_tree.size(); }
	inline size_type			max_size() const { return m_tree.max_size(); }
	inline void				swap(set<Key, Compare>& _x) { m_tree.swap(_x.m_tree); }

	// MODIFY
	inline pair<iterator, bool> insert(const value_type& _x) {
		pair<typename Tree::iterator, bool> p = m_tree.insert_unique(_x, _x);
		return make_pair(p.first, p.second);
	}

	iterator insert(iterator _position, const value_type& _x) {
		return m_tree.insert_unique((typename Tree::iterator&)_position, _x);
	}


	inline void				clear() { m_tree.clear(); }
	inline void				erase(iterator _position) { m_tree.erase((typename Tree::iterator&)_position); }
	inline size_type			erase(const key_type& _x) { return m_tree.erase(_x); }
	inline void 				erase(iterator _first, iterator _last) {
		m_tree.erase((typename Tree::iterator&)_first, (typename Tree::iterator&)_last);
	}

	// QUERY
	iterator				find(const key_type& _x) const { return m_tree.find(_x); }
	size_type				count(const key_type& _x) const { return m_tree.find(_x) == m_tree.end() ? 0 : 1; }
	iterator				lower_bound(const key_type& _x) const { return m_tree.lower_bound(_x); }
	iterator				upper_bound(const key_type& _x) const { return m_tree.upper_bound(_x); }
	pair<iterator, iterator> equal_range(const key_type& _x) const { return m_tree.equal_range(_x); }

	// OPERATORS
	inline set&				operator=(const set& _x) { m_tree = _x.m_tree; return *this; }
	inline bool				operator==(const set& _x) { return this->m_tree == _x.m_tree; }
	inline bool				operator<(const set& _x) { return this->m_tree < _x.m_tree; }
	inline bool				operator!=(const set& _x) { return !this->operator=(_x); }
	inline bool				operator>(const set& _x) { return _x < *this; }
	inline bool				operator<=(const set& _x) { return !(x_ < *this); }
	inline bool				operator>=(const set& _x) { return !(*this < _x); }
};


template <class Key, class Compare>
void swap(set<Key, Compare>& _x, set<Key, Compare>& _y) { _x.swap(_y); }

__ztl_namespace_end

#endif /* _set_hpp_1337420 */
