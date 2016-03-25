#ifndef _rb_tree_hpp_1337420
#define _rb_tree_hpp_1337420

#include <iterator>
#include <type_traits>
#include "../ztl_base.hpp"
#include "../container/pair.hpp"

__ztl_namespace_start
	typedef bool rb_color;
	const rb_color RED = false;
	const rb_color BLACK = true;

	template<class Key, class Value>
	class rb_node {
	public:
		typedef rb_node			Node;
		typedef rb_color		Color;

		rb_node() : m_left(NULL), m_right(NULL), m_parent(NULL) {}
		rb_node(Key _key, Value _val) : m_value(_val), m_key(_key) { }
		rb_node(const Node& _x) : m_key(_x.m_key), m_value(_x.m_value), m_color(_x.m_color), m_left(NULL), m_right(NULL) {};

		Color m_color;
		Node* m_parent;
		Node* m_left;
		Node* m_right;

		inline static Node* min(Node* _x) {
			while (_x->m_left != 0) _x = _x->m_left;
			return _x;ptr
		}

		inline static Node* max(Node* _x) {
			while (_x->m_right != 0) _x = _x->m_right;
			return _x;
		}

		inline Node* m_next() {
			if (this->m_right != 0) {
				Node* y = this->m_right;
				while (y->m_left != 0) y = y->m_left;
				return y;
			}
			else {
				Node* y = this->m_parent, *x = this;
				while (x == y->m_right) {
					x = y;
					y = y->m_parent;
				}
				if (x->m_right != y) return y;
				return x;
			}
		}

		inline Node* m_prev() {
			if (this->m_color == RED && this->m_parent->m_parent == this)
				return this->m_right;
			else if (this->m_left != 0) {
				Node* y = this->m_left;
				while (y->m_right != 0)
					y = y->m_right;
				return y;
			}
			else {
				Node* y = this->m_parent, *x = this;
				while (x == y->m_left) {
					x = y;
					y = y->m_parent;
				}
				return y;
			}
		}

		Value m_value;
		Key m_key;
	};


	template <class Key, class Value>
	struct rb_iterator {
		typedef rb_iterator<Key, Value>					Self;
		typedef rb_node<Key, Value>					Node;
		typedef __stl::bidirectional_iterator_tag			iterator_category;
		typedef __stl::ptrdiff_t						difference_type;
		typedef Value							value_type;
		typedef Value&							reference;
		typedef Value							pointer;


		rb_iterator() { }
		rb_iterator(Node* _x) : ptr(_x) { }

		inline reference	operator*() const { return ptr->m_value; }
		inline pointer		operator->() const { return &(operator*()); }

		inline Self&		operator++() { ptr = ptr->m_next(); return *this; }
		inline Self		operator++(int) { Self tmp = *this; tmp.ptr = tmp.ptr->m_next(); return tmp; }
		inline Self&		operator--() { ptr = ptr->m_prev(); return *this; }
		inline Self		operator--(int) { Self tmp = *this; tmp.ptr = tmp.ptr->m_prev(); return tmp; }

		inline bool		operator== (const Self&_x) { return _x.ptr == this->ptr; }
		inline bool		operator!= (const Self& _x) { return _x.ptr != this->ptr; }

		Node* ptr;
	};

	template <class Key, class Value>
	struct const_rb_iterator {
		typedef const_rb_iterator					Self;
		typedef rb_node<Key, Value>					Node;
		typedef __stl::bidirectional_iterator_tag			iterator_category;
		typedef __stl::ptrdiff_t						difference_type;
		typedef Value							value_type;
		typedef Value&							reference;
		typedef Value							pointer;
		typedef rb_iterator<Key, Value>					iterator;

		const_rb_iterator() { }
		const_rb_iterator(Node* _x) : ptr(_x) { }
		const_rb_iterator(const iterator& _it) : ptr(_it.ptr) { }

		inline reference	operator*() const { return ptr->m_value; }
		inline pointer		operator->() const { return &(operator*()); }

		inline Self&		operator++() { ptr = ptr->m_next(); return *this; }
		inline Self		operator++(int) { Self tmp = *this; tmp.ptr = tmp.ptr->m_next(); return tmp; }
		inline Self&		operator--() { ptr = ptr->m_prev(); return *this; }
		inline Self		operator--(int) { Self tmp = *this; tmp.ptr = tmp.ptr->m_prev(); return tmp; }

		inline bool		operator== (Self& _x) { return _x.ptr == this->ptr; }
		inline bool		operator!= (Self& _x) { return _x.ptr != this->ptr; }

		Node* ptr;
	};

	template<class Key, class Value, class Compare = __stl::less<Key>>
	class rb_tree {
	public:
		typedef Key							key_type;
		typedef Value							value_type;
		typedef value_type*						pointer;
		typedef const value_type*					const_pointer;
		typedef value_type&						reference;
		typedef const value_type&					const_reference;
		typedef __stl::size_t						size_type;
		typedef __stl::ptrdiff_t						difference_type;

		typedef rb_iterator<Key, Value>					iterator;
		typedef const_rb_iterator<Key, Value>				const_iterator;
		typedef __stl::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef __stl::reverse_iterator<iterator>			reverse_iterator;

		rb_tree() : m_size(0), m_comparer(), m_header(new Node()) { this->m_initialize(); }
		rb_tree(const Compare& _comp) : m_size(0), m_comparer(_comp), m_header(new Node()) { this->m_initialize(); }
		rb_tree(const rb_tree<Key, Value, Compare>& _x) : m_size(_x.m_size), m_comparer(_x.m_comparer), m_header(new Node()) {
			if (_x.m_root() == 0)
				this->m_initialize();
			else {
				s_color(m_header) = RED;
				m_root() = m_copy(_x.m_root(), m_header);
				m_leftmost() = min(m_root());
				m_rightmost() = max(m_root());
			}
			m_size = _x.m_size;
		}

		~rb_tree() { clear(); }

	private:
		typedef rb_node<Key, Value>			Node;

		inline void m_rotate_left(Node* _x, Node*& _root) {
			Node* y = _x->m_right;
			_x->m_right = y->m_left;
			if (y->m_left != 0)
				y->m_left->m_parent = _x;

			y->m_parent = _x->m_parent;

			if (_x == _root)			_root = y;
			else if (_x == _x->m_parent->m_left)	_x->m_parent->m_left = y;
			else					_x->m_parent->m_right = y;

			y->m_left = _x;
			_x->m_parent = y;
		}

		inline void m_rotate_right(Node* _x, Node*& _root) {
			Node* y = _x->m_left;
			_x->m_left = y->m_right;
			if (y->m_right != 0)
				y->m_right->m_parent = _x;
			y->m_parent = _x->m_parent;

			if (_x == _root)			_root = y;
			else if (_x == _x->m_parent->m_right)	_x->m_parent->m_right = y;
			else					_x->m_parent->m_left = y;

			y->m_right = _x;
			_x->m_parent = y;
		}

		inline void m_rebalance(Node* _x, Node*& _root) {
			_x->m_color = RED;
			while (_x != _root && _x->m_parent->m_color == RED) {
				if (_x->m_parent == _x->m_parent->m_parent->m_left) {
					Node* y = _x->m_parent->m_parent->m_right;
					if (y && y->m_color == RED) {
						_x->m_parent->m_color = BLACK;
						y->m_color = BLACK;
						_x->m_parent->m_parent->m_color = RED;
						_x = _x->m_parent->m_parent;
					}
					else {
						if (_x == _x->m_parent->m_right) {
							_x = _x->m_parent;
							this->m_rotate_left(_x, _root);
						}
						_x->m_parent->m_color = BLACK;
						_x->m_parent->m_parent->m_color = RED;
						this->m_rotate_right(_x->m_parent->m_parent, _root);
					}
				}
				else {
					Node* y = _x->m_parent->m_parent->m_left;
					if (y && y->m_color == RED) {
						_x->m_parent->m_color = BLACK;
						y->m_color = BLACK;
						_x->m_parent->m_parent->m_color = RED;
						_x = _x->m_parent->m_parent;
					}
					else {
						if (_x == _x->m_parent->m_left) {
							_x = _x->m_parent;
							this->m_rotate_right(_x, _root);
						}
						_x->m_parent->m_color = BLACK;
						_x->m_parent->m_parent->m_color = RED;
						this->m_rotate_left(_x->m_parent->m_parent, _root);
					}
				}
			}
			_root->m_color = BLACK;
		}



		inline Node* m_rebalance_erase(Node* z, Node*& _root, Node*& _leftmost, Node*& _rightmost) {
			Node* y = z;
			Node* x = 0;
			Node* parent = 0;
			if (y->m_left == 0) x = y->m_right;
			else
				if (y->m_right == 0) x = y->m_left;    
				else {                   
					y = y->m_right;   
					while (y->m_left != 0)
						y = y->m_left;
					x = y->m_right;
				}
				if (y != z) {          
					z->m_left->m_parent = y;
					y->m_left = z->m_left;
					if (y != z->m_right) {
						parent = y->m_parent;
						if (x) x->m_parent = y->m_parent;
						y->m_parent->m_left = x;     
						y->m_right = z->m_right;
						z->m_right->m_parent = y;
					}
					else
						parent = y;
					if (_root == z)
						_root = y;
					else if (z->m_parent->m_left == z)
						z->m_parent->m_left = y;
					else
						z->m_parent->m_right = y;
					y->m_parent = z->m_parent;
					_sTD::swap(y->m_color, z->m_color);
					y = z;
				}
				else {                        // y == z
					parent = y->m_parent;
					if (x) x->m_parent = y->m_parent;
					if (_root == z)
						_root = x;
					else
						if (z->m_parent->m_left == z)
							z->m_parent->m_left = x;
						else
							z->m_parent->m_right = x;
					if (_leftmost == z)
						if (z->m_right == 0)        
							_leftmost = z->m_parent;
						else
							_leftmost = rb_node::min(x);
					if (_rightmost == z)
						if (z->m_left == 0)         
							_rightmost = z->m_parent;
						else                      
							_rightmost = rb_node::max(x);
				}
				if (y->m_color != RED) {
					while (x != _root && (x == 0 || x->m_color == BLACK))
						if (x == parent->m_left) {
							Node* w = parent->m_right;
							if (w->m_color == RED) {
								w->m_color = BLACK;
								parent->m_color = RED;
								m_rotate_left(parent, _root);
								w = parent->m_right;
							}
							if ((w->m_left == 0 ||
								w->m_left->m_color == BLACK) &&
								(w->m_right == 0 ||
									w->m_right->m_color == BLACK)) {
								w->m_color = RED;
								x = parent;
								parent = parent->m_parent;
							}
							else {
								if (w->m_right == 0 ||
									w->m_right->m_color == BLACK) {
									if (w->m_left) w->m_left->m_color = BLACK;
									w->m_color = RED;
									m_rotate_right(w, _root);
									w = parent->m_right;
								}
								w->m_color = parent->m_color;
								parent->m_color = BLACK;
								if (w->m_right) w->m_right->m_color = BLACK;
								m_rotate_left(parent, _root);
								break;
							}
						}
						else {                 
							Node* w = parent->m_left;
							if (w->m_color == RED) {
								w->m_color = BLACK;
								parent->m_color = RED;
								m_rotate_right(parent, _root);
								w = parent->m_left;
							}
							if ((w->m_right == 0 ||
								w->m_right->m_color == BLACK) &&
								(w->m_left == 0 ||
									w->m_left->m_color == BLACK)) {
								w->m_color = RED;
								x = parent;
								parent = parent->m_parent;
							}
							else {
								if (w->m_left == 0 ||
									w->m_left->m_color == BLACK) {
									if (w->m_right) w->m_right->m_color = BLACK;
									w->m_color = RED;
									m_rotate_left(w, _root);
									w = parent->m_left;
								}
								w->m_color = parent->m_color;
								parent->m_color = BLACK;
								if (w->m_left) w->m_left->m_color = BLACK;
								m_rotate_right(parent, _root);
								break;
							}
						}
						if (x) x->m_color = BLACK;
				}
				return y;
		}



		inline Node* m_create_node(const Key& _key, const Value& _val) {
			Node* tmp = new Node(_key, _val);
			return tmp;
		}

		void m_destroy_node(Node* p) {
			delete p;
		}

		Node* m_header;
		size_t m_size;
		Compare m_comparer;

		Node*& m_root() const { return this->m_header->m_parent; }
		Node*& m_leftmost() const { return this->m_header->m_left; }
		Node*& m_rightmost() const { return this->m_header->m_right; }

		static Node*& s_left(Node* _x) { return _x->m_left; }
		static Node*& s_right(Node* _x) { return _x->m_right; }
		static Node*& s_parent(Node* _x) { return _x->m_parent; }

		static reference s_value(Node* _x) { return _x->m_value; }
		static const Key& s_key(Node* _x) { return _x->m_key; }
		static rb_color& s_color(Node* _x) { return _x->m_color; }

		static Node* min(Node* _x) { return Node::min(_x); }
		static Node* max(Node* _x) { return Node::max(_x); }


		void m_initialize() {
			s_color(this->m_header) = RED;

			m_root() = 0;
			m_leftmost() = m_header;
			m_rightmost() = m_header;
		}

		inline rb_iterator<Key, Value> m_insert(Node* _x, Node* _y, const Key& _k, const Value& _v) {
			Node* z;

			if (_y == this->m_header || _x != 0 || m_comparer(_k, s_key(_y))) {
				z = m_create_node(_k, _v);
				s_left(_y) = z;					
										
				if (_y == this->m_header) {
					m_root() = z;
					m_rightmost() = z;
				}
				else if (_y == m_leftmost())
					m_leftmost() = z;   
			}
			else {
				z = this->m_create_node(_k, _v);
				s_right(_y) = z;
				if (_y == m_rightmost())
					m_rightmost() = z;  
			}
			s_parent(z) = _y;
			s_left(z) = 0;
			s_right(z) = 0;
			m_rebalance(z, m_header->m_parent);
			++m_size;
			return iterator(z);
		}

		Node* m_copy(Node* _x, Node* _p)
		{
			Node* top = _x;
			top->m_parent = _p;

			try {
				if (_x->m_right) top->m_right = this->m_copy(s_right(_x), top);

				_p = top;
				_x = s_left(_x);

				while (_x != 0) {
					Node* y = _x;
					_p->m_left = y;
					y->m_parent = _p;

					if (_x->m_right) y->m_right = this->m_copy(s_right(_x), y);

					_p = y;
					_x = s_left(_x);
				}
			}
			catch {
				this->m_erase(top);
			}

			return top;
		}

		inline void m_erase(Node* _x) {
			while (_x != 0) {
				this->m_erase(s_right(_x));
				Node* y = s_left(_x);
				this->m_destroy_node(_x);
				_x = y;
			}
		}



	public:

		// ACCESS
		inline Compare					key_comp() const { return m_comparer; }
		inline iterator					begin() { return m_leftmost(); }
		inline const_iterator				cbegin() const { return m_leftmost(); }
		inline iterator					end() { return m_header; }
		inline const_iterator				cend() const { return m_header; }
		inline reverse_iterator				rbegin() { return reverse_iterator(end()); }
		inline const_reverse_iterator			crbegin() const { return const_reverse_iterator(end()); }
		inline reverse_iterator				rend() { return reverse_iterator(begin()); }
		inline const_reverse_iterator			crend() const { return const_reverse_iterator(begin()); }


		inline bool					empty() const { return m_size == 0; }
		inline size_type				size() const { return m_size; }
		inline size_type				maxsize() const { return (size_t)-1; }


		// MODIFY
		inline void swap(rb_tree<Key, Value, Compare>& _t) {
			__stl::swap(m_header, _t.m_header);
			__stl::swap(m_size, _t.m_size);
			__stl::swap(m_comparer, _t.m_comparer);
		}

		inline void erase(iterator _position) {
			Node* y = this->m_rebalance_erase(_position.ptr, m_header->m_parent, m_header->m_left, m_header->m_right);
			this->m_destroy_node(y);
			--this->m_size;
		}

		inline size_type erase(const Key& _x)
		{
			pair<iterator, iterator> p = equal_range(_x);
			size_type n = 0;
			distance(p.first, p.second, n);
			erase(p.first, p.second);
			return _n;
		}


		inline void erase(iterator _first, iterator _last)
		{
			if (_first == begin() && _last == end())
				clear();
			else
				while (_first != _last) this->erase(_first++);
		}

		inline void erase(const Key* _first, const Key* _last) {
			while (_first != _last) erase(*_first++);
		}

		inline void clear() {
			if (this->m_size != 0) {
				this->m_erase(m_root());
				this->m_leftmost() = m_header;
				this->m_root() = 0;
				this->m_rightmost() = m_header;
				this->m_size = 0;
			}
		}

		inline iterator insert_equal(const Key& _k, const Value& _v)
		{
			Node* y = this->m_header;
			Node* x = this->m_root();

			while (x != NULL) {
				y = x;
				x = this->m_comparer(_k, s_key(x)) ? s_left(x) : s_right(x);
			}

			return this->m_insert(x, y, _k, _v);
		}

		pair<iterator, bool> insert_unique(const Key& _k, const Value& _v) {
			Node* y = this->m_header;
			Node* x = this->m_root();

			bool comp = true;
			while (x != 0) {
				y = x;
				comp = this->m_comparer(_k, s_key(x));
				x = comp ? s_left(x) : s_right(x);
			}

			iterator j = iterator(y);
			if (comp)
				if (j == begin())
					return pair<iterator, bool>(this->m_insert(x, y, _k, _v), true);
				else
					--j;
			if (this->m_comparer(s_key(j.ptr), _k))
				return pair<iterator, bool>(this->m_insert(x, y, _k, _v), true);

			return pair<iterator, bool>(j, false);
		}

		iterator insert_unique(iterator _position, const Key& _k, const Value& _v) {
			if (_position.ptr == this->m_header->m_left) { // begin()
				if (size() > 0 && this->m_comparer(_k, _position.ptr->m_k))
					return this->m_insert(_position.ptr, _position.ptr, _k, _v);
				else
					return this->insert_unique(_k, _v).first;
			}
			else if (_position.ptr == this->m_header) { // end()
				if (this->m_comparer(m_rightmost()->m_key, _k))
					return this->m_insert(0, m_rightmost(), _k, _v);
				else
					return this->insert_unique(_k, _v).first;
			}
			else {
				iterator before = _position;
				--before;
				if (this->m_comparer(before.ptr->m_key, _k) && this->m_comparer(_k, _position.ptr->m_key)) {
					if (before.ptr->m_right == 0)
						return this->m_insert(0, before.ptr, _k, _v);
					else
						return this->m_insert(_position.ptr, _position.ptr, _k, _v);
				}
				else
					return this->insert_unique(_k, _v).first;
			}
		}


		inline iterator find(const Key& _k) {
			Node* y = m_header;
			Node* x = m_root();

			while (x != 0) {
				if (!m_comparer(s_key(x), _k))
					y = x, x = s_left(x);
				else
					x = s_right(x);
			}
			iterator j = iterator(y);
			return (j == end() || m_comparer(_k, s_key(j.ptr))) ? end() : j;
		}

		inline const_iterator find(const Key& _k) const {
			Node* y = m_header;
			Node* x = m_root();

			while (x != 0) {
				if (!m_comparer(s_key(x), _k))
					y = x, x = s_left(x);
				else
					x = s_right(x);
			}
			const_iterator j = const_iterator(y);
			return (j == cend() || m_comparer(_k, j.ptr->m_key)) ? cend() : j;
		}

		size_type count(const Key& _k) const {
			pair<const_iterator, const_iterator> p = equal_range(_k);
			size_type n = 0;
			__stl::distance(p.first, p.second, n);
			return n;
		}

		inline iterator lower_bound(const Key& _k)
		{
			Node* y = m_header;
			Node* x = m_root();

			while (x != 0)
				if (!m_comparer(s_key(x), _k))
					y = x, x = s_left(x);
				else
					x = s_right(x);

			return iterator(y);
		}

		inline const_iterator lower_bound(const Key& _k) const
		{
			Node* y = m_header;
			Node* x = m_root();

			while (x != 0)
				if (!m_comparer(s_key(x), _k))
					y = x, x = s_left(x);
				else
					x = s_right(x);

			return const_iterator(y);
		}

		inline iterator upper_bound(const Key& _k)
		{
			Node* y = m_header;
			Node* x = m_root();

			while (x != 0)
				if (m_comparer(_k, s_key(x)))
					y = x, x = s_left(x);
				else
					x = s_right(x);

			return iterator(y);
		}

		inline const_iterator upper_bound(const Key& _k) const
		{
			Node* y = m_header;
			Node* x = m_root();

			while (x != 0)
				if (m_comparer(_k, s_key(x)))
					y = x, x = s_left(x);
				else
					x = s_right(x);

			return const_iterator(y);
		}

		inline pair<iterator, iterator> equal_range(const Key& _k) {
			return pair<iterator, iterator>(lower_bound(_k), upper_bound(_k));
		}

		inline pair<const_iterator, const_iterator> equal_range(const Key& _k) const {
			return pair<const_iterator, const_iterator>(lower_bound(_k), upper_bound(_k));
		}


	};
__ztl_namespace_end

#endif /* _rb_tree_hpp_1337420 */
