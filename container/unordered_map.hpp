
#ifndef __SGI_STL_INTERNAL_HASH_MAP_H
#define __SGI_STL_INTERNAL_HASH_MAP_H

#include "../ztl_base.hpp"
#include "../core/hash.hpp"
#include "hashtable.hpp"


__ztl_namespace_start

template <class Key, class Value, class HashAlg = hash<Key>, class KeyComp = equal_to<Key>>
class unordered_map {
private:
	typedef hashtable<pair<Key, Value>, Key, HashAlg, typename pair<Key, Value>::select1st, KeyComp> HashTable;
	HashTable m_table;

public:
	typedef typename HashTable::key_type				key_type;
	typedef Value							data_type;
	typedef Value							mapped_type;
	typedef typename HashTable::value_type				value_type;
	typedef typename HashTable::hasher				hasher;
	typedef typename HashTable::key_equal				key_equal;

	typedef typename HashTable::size_type				size_type;
	typedef typename HashTable::difference_type			difference_type;
	typedef typename HashTable::pointer				pointer;
	typedef typename HashTable::const_pointer			const_pointer;
	typedef typename HashTable::reference				reference;
	typedef typename HashTable::const_reference			const_reference;

	typedef typename HashTable::iterator				iterator;
	typedef typename HashTable::const_iterator			const_iterator;

	hasher		m_hash() const { return m_table.m_hash(); }
	key_equal	key_eq() const { return m_table.key_eq(); }

public:
	unordered_map() : m_table(100, hasher(), key_equal()) {}
	explicit unordered_map(size_type _n) : m_table(_n, hasher(), key_equal()) {}
	unordered_map(size_type _n, const hasher& _h) : m_table(_n, _h, key_equal()) {}
	unordered_map(size_type _n, const hasher& _h, const key_equal& _e) : m_table(_n, _h, _e) {}

	inline size_type				size() const { return m_table.size(); }
	inline size_type				max_size() const { return m_table.max_size(); }
	inline bool					empty() const { return m_table.empty(); }
	inline void					swap(unordered_map& _hs) { m_table.swap(_hs.m_table); }


	iterator					begin() { return m_table.begin(); }
	iterator					end() { return m_table.end(); }
	const_iterator					cbegin() const { return m_table.begin(); }
	const_iterator					cend() const { return m_table.end(); }


	inline pair<iterator, bool>		  	insert(const value_type& _val) { return m_table.insert_unique(_val); }
	inline iterator					find(const key_type& _key) { return m_table.find(_key); }
	inline const_iterator			      	find(const key_type& _key) const { return m_table.find(_key); }
	inline Value&					operator[](const key_type& _key) { return m_table.find_or_insert(make_pair(_key, Value())).second; }
	inline size_type				count(const key_type& _key) const { return m_table.count(_key); }
	inline size_type				erase(const key_type& _key) { return m_table.erase(_key); }
	inline void					erase(iterator _it) { m_table.erase(_it); }
	inline void					erase(iterator _f, iterator _l) { m_table.erase(_f, _l); }
	inline void					clear() { m_table.clear(); }
	inline void					resize(size_type _hint) { m_table.resize(_hint); }
	inline size_type				bucket_count() const { return m_table.bucket_count(); }
	inline size_type				max_bucket_count() const { return m_table.max_bucket_count(); }
	inline size_type				elems_in_bucket(size_type _n) const { return m_table.elems_in_bucket(_n); }

	inline pair<iterator, iterator> equal_range(const key_type& _key) { return m_table.equal_range(_key); }
	inline pair<const_iterator, const_iterator> equal_range(const key_type& _key) const { return m_table.equal_range(__key); }

	// OPERATORS
	inline bool operator==(const unordered_map& _x) { return _x.m_table == this->m_table; }
	inline bool operator!=(const unordered_map& _x) { return _x.m_table != this->m_table; }
};

template <class V, class K, class H, class E>
inline void swap(unordered_map<V, K, H, E>& _x, unordered_map<V, K, H, E>& _y) { _x.swap(_y); }

__ztl_namespace_end

#endif
