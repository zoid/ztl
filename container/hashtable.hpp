#ifndef _hashtable_hpp_1337420
#define _hashtable_hpp_1337420

#include "../ztl_base.hpp"
#include "../core/algorithm.hpp"

#include <iterator>

#include "vector.hpp"
#include "pair.hpp"

__ztl_namespace_start

template <class Value>
struct hashtable_node {
	hashtable_node* m_next;
	Value m_value;
};

template <class Value, class Key, class HashAlg, class KeyVal, class KeyComp> class hashtable;
template <class Value, class Key, class HashAlg, class KeyVal, class KeyComp> struct hashtable_iterator;
template <class Value, class Key, class HashAlg, class KeyVal, class KeyComp> struct hashtable_const_iterator;


template <class Value, class Key, class HashAlg, class KeyVal, class KeyComp>
struct hashtable_iterator {
	typedef hashtable_node<Value>							Node;
	typedef hashtable<Value, Key, HashAlg, KeyVal, KeyComp>				HTable;
	typedef hashtable_iterator<Value, Key, HashAlg, KeyVal, KeyComp>		Self;
	typedef hashtable_const_iterator<Value, Key, HashAlg, KeyVal, KeyComp>		const_iterator;

	typedef __stl::forward_iterator_tag						iterator_category;
	typedef Value									value_type;
	typedef int									difference_type;
	typedef size_t									size_type;
	typedef Value&									reference;
	typedef Value*									pointer;

	Node* ptr;
	HTable* m_table;

	hashtable_iterator(Node* _n, HTable* _t)	: ptr(_n), m_table(_t) { }
	hashtable_iterator() { }
	
	reference	operator*()	const { return ptr->m_value; }
	pointer		operator->() const { return &(operator*()); }

	bool operator==(const Self&_it) const { return ptr == _it.ptr; }
	bool operator!=(const Self& _it) const { return ptr != _it.ptr; }

	Self& operator++() {
		const Node* old = ptr;
		ptr = ptr->m_next;
		if (!ptr) {
			size_type bucket = m_table->m_buckets_count(old->m_value);
			while (!ptr && ++bucket < m_table->m_buckets.size())
				ptr = m_table->m_buckets[bucket];
		}
		return *this;
	}

	Self operator++(int) {
		Self tmp = *this;
		++(*this);
		return tmp;
	}
};


template <class Value, class Key, class HashAlg, class KeyVal, class KeyComp>
struct hashtable_const_iterator {
	typedef hashtable_node<Value>							Node;
	typedef hashtable<Value, Key, HashAlg, KeyVal, KeyComp>				HTable;
	typedef hashtable_iterator<Value, Key, HashAlg, KeyVal, KeyComp>		iterator;
	typedef hashtable_const_iterator<Value, Key, HashAlg, KeyVal, KeyComp>		Self;


	typedef __stl::forward_iterator_tag						iterator_category;
	typedef Value									value_type;
	typedef int									difference_type;
	typedef size_t									size_type;
	typedef const Value&								reference;
	typedef const Value*								pointer;

	const Node* ptr;
	const HTable* m_table;

	hashtable_const_iterator(const Node* _n, const HTable* _t) : ptr(_n), m_table(_t) { }
	hashtable_const_iterator() {}
	hashtable_const_iterator(const iterator& _it) : ptr(_it.ptr), m_table(_it.m_table) {}
	
	reference operator*() const { return ptr->m_value; }
	pointer operator->() const { return &(operator*()); }

	Self& operator++() {
		const Node* old = ptr;
		ptr = ptr->m_next;
		if (!ptr) {
			size_type bucket = m_table->m_buckets_count(old->m_value);
			while (!ptr && ++bucket < m_table->m_buckets.size())
				ptr = m_table->m_buckets[bucket];
		}
		return *this;
	}

	Self operator++(int) {
		Self tmp = *this;
		++(*this);
		return tmp;
	}
	bool operator==(const Self& _it) const { return ptr == _it.ptr; }
	bool operator!=(const Self& _it) const { return ptr != _it.ptr; }
};

static const int primes_size = 28;
static const unsigned long primes[primes_size] = {
	53ul,         97ul,         193ul,       389ul,       769ul,
	1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
	49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
	1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
	50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
	1610612741ul, 3221225473ul, 4294967291ul
};

unsigned long next_prime(unsigned long n) {
	int pos = 0;
	for (; pos < primes_size && primes[pos] < n; pos++) {}
	return pos == primes_size ? primes[pos - 1] : primes[pos];
}


template <class Value, class Key, class HashAlg, class KeyVal, class KeyComp>
class hashtable {
public:
	typedef Key					key_type;
	typedef Value				value_type;
	typedef HashAlg				hasher;
	typedef KeyComp				key_equal;

	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;

	hasher		m_hash() const { return m_hash_alg; }
	key_equal	key_eq() const { return m_equals; }

private:
	typedef hashtable_node<Value> Node;
	hasher				m_hash_alg;
	key_equal			m_equals;
	KeyVal				m_get_key;
	vector<Node*>		m_buckets;
	size_type			m_size;

public:
	typedef hashtable_iterator<Value, Key, HashAlg, KeyVal, KeyComp>			iterator;
	typedef hashtable_const_iterator<Value, Key, HashAlg, KeyVal, KeyComp>		const_iterator;

	friend struct hashtable_iterator<Value, Key, HashAlg, KeyVal, KeyComp>;
	friend struct hashtable_const_iterator<Value, Key, HashAlg, KeyVal, KeyComp>;

	// CONSTRUCTORS
	hashtable(size_type _n, const HashAlg& _h, const KeyComp& _e, const KeyVal& _ext) : m_hash_alg(_h), m_equals(_e), m_get_key(_ext), m_size(0) {
		m_create_buckets(_n);
	}

	hashtable(size_type _n, const HashAlg& _h, const KeyComp& _e) : m_hash_alg(_h), m_equals(_e), m_get_key(KeyVal()), m_size(0) {
		m_create_buckets(_n);
	}

	hashtable(const hashtable& _ht) : m_hash_alg(_ht.m_hash_alg), m_equals(_ht.m_equals), m_get_key(_ht.m_get_key),	m_size(0) {
		m_copy(_ht);
	}

	~hashtable() { clear(); }
	
	// ASIGN OPERATOR
	hashtable& operator= (const hashtable& _ht) {
		if (&_ht != this) {
			clear();
			m_hash_alg = _ht.m_hash_alg;
			m_equals = _ht.m_equals;
			m_get_key = _ht.m_get_key;
			m_copy(_ht);
		}

		return *this;
	}

	inline size_type			size() const { return m_size; }
	inline size_type			max_size() const { return size_type(-1); }
	inline bool					empty() const { return size() == 0; }

	inline void swap(hashtable& _ht) {
		__ztl::swap(m_hash_alg, _ht.m_hash_alg);
		__ztl::swap(m_equals, _ht.m_equals);
		__ztl::swap(m_get_key, _ht.m_get_key);
		m_buckets.swap(_ht.m_buckets);
		__ztl::swap(m_size, _ht.m_size);
	}

	// ACCESS

	inline iterator begin() {
		for (size_type n = 0; n < m_buckets.size(); ++n)
			if (m_buckets[n])
				return iterator(m_buckets[n], this);
		return end();
	}

	inline iterator end() { return iterator(NULL, this); }

	inline const_iterator cbegin() const {
		for (size_type n = 0;n < m_buckets.size(); ++n)
			if (m_buckets[n])
				return const_iterator(m_buckets[n], this);
		return cend();
	}

	inline const_iterator cend() const { return const_iterator(NULL, this); }

	inline size_type bucket_count() const { return m_buckets.size(); }
	inline size_type max_bucket_count() const { return primes[primes_size - 1]; }
	inline size_type elems_in_bucket(size_type _bucket) const {
		size_type result = 0;
		for (Node* cur = m_buckets[_bucket]; cur; cur = cur->m_next)
			result += 1;
		return result;
	}


	// INSERT / FIND

	inline pair<iterator, bool> insert_unique(const value_type& _v) {
		this->resize(m_size + 1);
		const size_type n = m_buckets_count(_v);
		Node* first = m_buckets[n];

		for (Node* cur = first; cur; cur = cur->m_next)
			if (m_equals(cur->m_value.first, _v.first))
				return pair<iterator, bool>(iterator(cur, this), false);

		Node* tmp = m_create_node(_v);
		tmp->m_next = first;
		m_buckets[n] = tmp;
		++m_size;

		return pair<iterator, bool>(iterator(tmp, this), true);
	}

	inline iterator insert_equal(const value_type& _v) {
		this->resize(m_size + 1);
		const size_type n = m_buckets_count(_v);
		Node* first = m_buckets[n];

		for (Node* cur = first; cur; cur = cur->m_next)
			if (m_equals(cur->m_value.first, _v.first)) {
				Node* tmp = m_create_node(_v);
				tmp->m_next = cur->m_next;
				cur->m_next = tmp;
				++m_size;
				return iterator(tmp, this);
			}

		Node* tmp = m_create_node(_v);
		tmp->m_next = first;
		m_buckets[n] = tmp;
		++m_size;
		return iterator(tmp, this);
	}

	inline reference find_or_insert(const value_type& _v) {
		size_type n = m_buckets_count(_v);
		Node* first = m_buckets[n];

		for (Node* cur = first; cur; cur = cur->m_next)
			if (m_equals(m_get_key(cur->m_value), m_get_key(_v))) 
				return cur->m_value;

		this->resize(m_size + 1);
		Node* tmp = m_create_node(_v);
		tmp->m_next = first;
		m_buckets[n] = tmp;
		++m_size;
		return tmp->m_value;
	}

	inline iterator find(const key_type& _k) {
		size_type n = m_buckets_countKey(_k);
		Node* first;
		for (first = m_buckets[n]; first && !m_equals(m_get_key(first->m_value), _k); first = first->m_next) { }
		return iterator(first, this);
	}

	inline const_iterator find(const key_type& _k) const {
		size_type n = m_buckets_countKey(_k);
		const Node* first;
		for (first = m_buckets[n]; first && !m_equals(m_get_key(first->m_value), _k); first = first->m_next) { }

		return const_iterator(first, this);
	}

	inline size_type count(const key_type& _k) const {
		const size_type n = m_buckets_countKey(_k);
		size_type result = 0;

		for (const Node* cur = m_buckets[n]; cur; cur = cur->m_next)
			if (m_equals(m_get_key(cur->m_value), _k.first))
				++result;
		return result;
	}



	pair<iterator, iterator> equal_range(const key_type& _k) {
		const size_type n = m_buckets_countKey(_k);
		for (Node* first = m_buckets[n]; first; first = first->m_next)
			if (m_equals(m_get_key(first->m_value), _k)) {
				for (Node* cur = first->m_next; cur; cur = cur->m_next)
					if (!m_equals(m_get_key(cur->m_value), _k))
						return make_pair(iterator(first, this), iterator(cur, this));
				for (size_type m = n + 1; m < m_buckets.size(); ++m)
					if (m_buckets[m])
						return make_pair(iterator(first, this),
							iterator(m_buckets[m], this));
				return make_pair(iterator(first, this), end());
			}

		return make_pair(end(), end());
	}

	pair<const_iterator, const_iterator> equal_range(const key_type& _k) const {
		const size_type n = m_buckets_countKey(_k);

		for (const Node* first = m_buckets[n]; first;
			first = first->m_next) {
			if (m_equals(m_get_key(first->m_value), _k)) {
				for (const Node* cur = first->m_next; cur; cur = cur->m_next)
					if (!m_equals(m_get_key(cur->m_value), _k))
						return make_pair(const_iterator(first, this), const_iterator(cur, this));
				for (size_type m = n + 1; m < m_buckets.size(); ++m)
					if (m_buckets[m])
						return make_pair(const_iterator(first, this),
							const_iterator(m_buckets[m], this));
				return make_pair(const_iterator(first, this), end());
			}
		}
		return make_pair(end(), end());
	}



	inline size_type erase(const key_type& _k) {
		const size_type n = m_buckets_countKey(_k);
		Node* first = m_buckets[n];
		size_type erased = 0;

		if (first) {
			Node* cur = first, *next = cur->m_next;
			while (next) {
				if (m_equals(m_get_key(next->m_value), _k)) {
					cur->m_next = next->m_next;
					delete next;
					next = cur->m_next;
					++erased;
					--m_size;
				}
				else {
					cur = next;
					next = cur->m_next;
				}
			}
			if (m_equals(m_get_key(first->m_value), _k)) {
				m_buckets[n] = first->m_next;
				delete first;
				++erased;
				--m_size;
			}
		}
		return erased;
	}

	inline void erase(const iterator& _it) {
		Node* p = _it.ptr;
		if (p) {
			const size_type n = m_buckets_count(p->m_value);
			Node* cur = m_buckets[n];

			if (cur == p) {
				m_buckets[n] = cur->m_next;
				delete cur;
				--m_size;
			}
			else {
				Node* next = cur->m_next;
				while (next) {
					if (next == p) {
						cur->m_next = next->m_next;
						delete next;
						--m_size;
						break;
					}
					else {
						cur = next;
						next = cur->m_next;
					}
				}
			}
		}
	}

	inline void erase(iterator _first, iterator _last) {
		size_type f_bucket = _first.ptr ? m_buckets_count(_first.ptr->m_value) : m_buckets.size();
		size_type l_bucket = _last.ptr ? m_buckets_count(_last.ptr->m_value) : m_buckets.size();

		if (_first.ptr == _last.ptr) return;
		else if (f_bucket == l_bucket) m_destroy_bucket(f_bucket, _first.ptr, _last.ptr);
		else { 
			m_destroy_bucket(f_bucket, _first.ptr, 0);
			for (size_type n = f_bucket + 1; n < l_bucket; ++n) m_destroy_bucket(n, 0);
			if (l_bucket != m_buckets.size()) m_destroy_bucket(l_bucket, _last.ptr);
		}
	}

	inline void erase(const_iterator _first, const_iterator _last) {
		this->erase(iterator(const_cast<Node*>(_first.ptr), const_cast<hashtable*>(_first.m_table)), iterator(const_cast<Node*>(_last.ptr), const_cast<hashtable*>(_last.m_table)));
	}

	inline void erase(const const_iterator& _it) {
		erase(iterator(const_cast<Node*>(_it.ptr), const_cast<hashtable*>(_it.m_table)));
	}


	void resize(size_type num_elements_hint) {
		const size_type old = m_buckets.size();
		if (num_elements_hint > old) {
			const size_type n = m_next_size(num_elements_hint);
			if (n > old) {
				vector<Node*> tmp(n, (Node*)(0));
				try {
					for (size_type bucket = 0; bucket < old; ++bucket) {
						Node* first = m_buckets[bucket];
						while (first) {
							size_type new_bucket = m_buckets_count(first->m_value, n);
							m_buckets[bucket] = first->m_next;
							first->m_next = tmp[new_bucket];
							tmp[new_bucket] = first;
							first = m_buckets[bucket];
						}
					}
					m_buckets.swap(tmp);
				}
				catch (...) {
					for (size_type bucket = 0; bucket < tmp.size(); ++bucket) {
						while (tmp[bucket]) {
							Node* next = tmp[bucket]->m_next;
							delete tmp[bucket];
							tmp[bucket] = next;
						}
					}
					throw;
				}
			}
		}
	}

private:
	inline size_type m_next_size(size_type _n) const { return next_prime(_n); }
	inline void m_create_buckets(size_type _n) {
		const size_type n = m_next_size(_n);
		m_buckets.reserve(n);
		for (int i = 0; i < n; i++) m_buckets.push_back(NULL);		//TO DO insert
		m_size = 0;
	}

	inline size_type m_buckets_countKey(const key_type& _k) const {
		return m_buckets_countKey(_k, m_buckets.size());
	}

	size_type m_buckets_count(const value_type& _v) const {
		return m_buckets_countKey(_v.first);
	}

	size_type m_buckets_countKey(const key_type& _k, size_t n) const {
		return m_hash_alg(_k) % n;
	}

	size_type m_buckets_count(const value_type& _v, size_t n) const {
		return m_buckets_countKey(_v.first, n);
	}

	Node* m_create_node(const value_type& _v) {
		Node* n = new Node();
		n->m_next = 0;
		n->m_value = _v;

		return n;
		//delete n;
	}



	void m_destroy_bucket(const size_type n, Node* first, Node* _last) {
		Node* cur = m_buckets[n];
		if (cur == first) this->m_destroy_bucket(n, _last);
		else {
			Node* next;
			for (next = cur->m_next;
			next != first;
				cur = next, next = cur->m_next)
				;
			while (next != _last) {
				cur->m_next = next->m_next;
				delete next;
				next = cur->m_next;
				--m_size;
			}
		}
	}

	void m_destroy_bucket(const size_type n, Node* _last) {
		Node* cur = m_buckets[n];
		while (cur != _last) {
			Node* next = cur->m_next;
			delete cur;
			cur = next;
			m_buckets[n] = cur;
			--m_size;
		}
	}

	void clear() {
		for (size_type i = 0; i < m_buckets.size(); ++i) {
			Node* cur = m_buckets[i];
			while (cur != 0) {
				Node* next = cur->m_next;
				delete cur;
				cur = next;
			}
			m_buckets[i] = 0;
		}
		m_size = 0;
	}


	void m_copy(const hashtable& _ht) {
		m_buckets.clear();
		m_buckets.reserve(_ht.m_buckets.size());
		m_buckets.insert(m_buckets.end(), _ht.m_buckets.size(), (Node*)0);
		try {
			for (size_type i = 0; i < _ht.m_buckets.size(); ++i) {
				const Node* cur = _ht.m_buckets[i];
				if (cur) {
					Node* copy = m_create_node(cur->m_value);
					m_buckets[i] = copy;

					for (Node* next = cur->m_next;
					next;
						cur = next, next = cur->m_next) {
						copy->m_next = m_create_node(next->m_value);
						copy = copy->m_next;
					}
				}
			}
			m_size = _ht.m_size;
		}
		catch (...) {
			this->clear();
		};
	}

public:
	// OPERATORS
	inline bool operator==(const hashtable& _x) {
		if (this->m_buckets.size() != _x.m_buckets.size()) return false;
		for (int n = 0; n < this->m_buckets.size(); ++n) {
			Node* cur1 = this->m_buckets[n];
			Node* cur2 = _x.m_buckets[n];
			for (; cur1 && cur2 && cur1->m_value == cur2->m_value; cur1 = cur1->m_next, cur2 = cur2->m_next)
			{ }
			if (cur1 || cur2) return false;
		}
		return true;
	}

	inline bool operator!=(const hashtable& _x) { return !(_x == *this); }
};




template <class V, class K, class H, class X, class E>
inline void swap(hashtable<V, K, H, X, E>& _x, hashtable<V, K, H, X, E>& _y) { _x.swap(_y); }


__ztl_namespace_end

#endif
