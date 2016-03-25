#ifndef _map_hpp_1337420
#define _map_hpp_1337420

#include "../ztl_base.hpp"
#include "../container/pair.hpp"
#include "../tree/rb_tree.hpp"

__ztl_namespace_start

template <class Key, class Value, class Compare = __stl::less<Key>> 
class map {
public:
	typedef Key						key_type;
	typedef Value                   data_type;
	typedef Value                   mapped_type;
	typedef pair<const Key, Value>	value_type;
	typedef Compare					key_compare;

	class value_compare : public __stl::binary_function<value_type, value_type, bool> {
		friend class map<Key, Value, Compare>;
	protected:
		Compare comp;
		value_compare(Compare _c) : comp(_c) {}
	public:
		bool operator()(const value_type& _x, const value_type& _y) const {
			return comp(_x.first, _y.first);
		}
	};

private:
	typedef rb_tree<key_type, value_type, key_compare> Tree;
	Tree m_tree; 
public:
	typedef typename Tree::pointer					pointer;
	typedef typename Tree::const_pointer			const_pointer;
	typedef typename Tree::reference				reference;
	typedef typename Tree::const_reference			const_reference;
	typedef typename Tree::iterator					iterator;
	typedef typename Tree::const_iterator			const_iterator;
	typedef typename Tree::reverse_iterator			reverse_iterator;
	typedef typename Tree::const_reverse_iterator	const_reverse_iterator;
	typedef typename Tree::size_type				size_type;
	typedef typename Tree::difference_type			difference_type;

	map() : m_tree(Compare()) {}
	explicit map(const Compare& _comp) : m_tree(_comp) {}
	map(const map& _x) : m_tree(_x.m_tree) {}

	// ACCESS
	inline key_compare				key_comp() const	{ return m_tree.key_comp(); }
	inline value_compare			value_comp() const	{ return value_compare(m_tree.key_comp()); }
	
	inline iterator					begin() { return m_tree.begin(); }
	inline const_iterator			cbegin() const { return m_tree.begin(); }
	inline iterator					end() { return m_tree.end(); }
	inline const_iterator			cend() const { return m_tree.end(); }
	inline reverse_iterator			rbegin() { return m_tree.rbegin(); }
	inline const_reverse_iterator	crbegin() const { return m_tree.crbegin(); }
	inline reverse_iterator			rend() { return m_tree.rend(); }
	inline const_reverse_iterator	crend() const { return m_tree.crend(); }
	
	inline bool						empty() const { return m_tree.empty(); }
	inline size_type				size() const { return m_tree.size(); }
	inline size_type				max_size() const { return m_tree.max_size(); }
	
	inline Value&					operator[](const key_type& _k) {
		iterator i = this->lower_bound(_k);

		if (i == end() || key_comp()(_k, (*i).first))
			i = insert(i, value_type(_k, Value()));
		
		return (*i).second;
	}

	inline void swap(map& _x) { m_tree.swap(_x.m_tree); }


	// MODIFY
	inline pair<iterator, bool>		insert(const value_type& _x) { return m_tree.insert_unique(_x.first, _x.second); }
	inline iterator					insert(iterator position, const value_type& _x){ return m_tree.insert_unique(position, _x.first, _x.second); }

	inline void						erase(iterator _position) { m_tree.erase(_position); }
	inline size_type				erase(const key_type& _x) { return m_tree.erase(_x); }
	inline void						erase(iterator _first, iterator _last) { m_tree.erase(_first, _last); }
	inline void						clear() { m_tree.clear(); }


	// QUERY
	inline iterator			find(const key_type& _x) { return m_tree.find(_x); }
	inline const_iterator	find(const key_type& _x) const { return m_tree.find(_x); }
	inline size_type		count(const key_type& _x) const { return m_tree.find(_x) == m_tree.end() ? 0 : 1; }
	inline iterator			lower_bound(const key_type& _x) { return m_tree.lower_bound(_x); }
	inline const_iterator	lower_bound(const key_type& _x) const {	return m_tree.lower_bound(_x); }
	inline iterator			upper_bound(const key_type& _x) { return m_tree.upper_bound(_x); }
	inline  const_iterator	upper_bound(const key_type& _x) const {	return m_tree.upper_bound(_x); }

	inline pair<iterator, iterator>	equal_range(const key_type& _x) { return m_tree.equal_range(_x); }
	inline pair<const_iterator, const_iterator> equal_range(const key_type& _x) const { return m_tree.equal_range(_x); }


	// OPERATORS
	inline map&				operator=(const map& _x) { m_tree = _x.m_tree; return *this; }
	inline bool				operator==(const map& _x) { return this->m_tree == _x.m_tree; }
	inline bool				operator<(const map& _x) { return this->m_tree < _x.m_tree; }
	inline bool				operator!=(const map& _x) { return !this->operator=(_x); }
	inline bool				operator>(const map& _x) { return _x < *this; }
	inline bool				operator<=(const map& _x) { return !(x_ < *this); }
	inline bool				operator>=(const map& _x) { return !(*this < _x); }
};

template <class Key, class Value, class Compare>
void swap(map<Key, Value, Compare>& _x, map<Key, Value, Compare>& _y) { _x.swap(_y); }


__ztl_namespace_end

#endif /* _map_hpp_1337420 */