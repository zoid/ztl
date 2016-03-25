#ifndef _list_hpp_1337420
#define _list_hpp_1337420

#include <iterator>
#include <type_traits>

#include "../ztl_base.hpp"

__ztl_namespace_start
	template<typename Key>
	struct list_node
	{
		list_node* m_next;
		list_node* m_prev;
		Key m_data;

		void hook(list_node* const _position) {
			this->m_prev = _position->m_prev;
			this->m_next = _position;
			this->m_prev->m_next = this;
			_position->m_prev = this;
		}

		void unhook() {
			this->m_prev->m_next = this->m_next;
			this->m_next->m_prev = this->m_prev;
		}
	};

	template<typename Key>
	struct list_iterator
	{
		typedef list_iterator<Key>			Self;
		typedef list_node<Key>				Node;
		typedef ptrdiff_t				difference_type;
		typedef __stl::bidirectional_iterator_tag	iterator_category;
		typedef Key					value_type;
		typedef Key*					pointer;
		typedef Key&					reference;

		list_iterator() { }
		list_iterator(Node* _x) : ptr(_x) { }

		inline reference	operator*() const { return ptr->m_data; }
		inline pointer		operator->() const { return &ptr->m_data; }
		inline Self&		operator++() { ptr = ptr->m_next; return *this; }
		inline Self		operator++(int) { Self tmp = *this; ptr = ptr->m_next; return tmp; }
		inline Self&		operator--() { ptr = ptr->m_prev; return *this; }
		inline Self		operator--(int) { Self tmp = *this; ptr = ptr->m_prev; return tmp; }
		inline bool		operator==(const Self& _x) const { return ptr == _x.ptr; }
		inline bool		operator!=(const Self& _x) const { return ptr != _x.ptr; }

		inline Self&		operator+(int _x) { for (; _x > 0; --_x) ptr = ptr->m_next; return *this; }
		inline Self&		operator-(int _x) { for (; _x > 0; --_x) ptr = ptr->m_prev; return *this; }

		Node* ptr;
	};


	template<typename Key>
	struct list_const_iterator
	{
		typedef list_const_iterator<Key>						Self;
		typedef const list_node<Key>							Node;
		typedef list_iterator<Key>							iterator;
		typedef ptrdiff_t								difference_type;
		typedef __stl::bidirectional_iterator_tag					iterator_category;
		typedef const Key								value_type;
		typedef const Key*								pointer;
		typedef const Key&								reference;

		list_const_iterator() { }
		list_const_iterator(const Node* _x) : ptr(_x) { }
		list_const_iterator(const iterator& _x) : ptr(_x.ptr) { }

		inline reference	operator*() const { return ptr->m_data; }
		inline pointer		operator->() const { return &ptr->m_data; }
		inline Self&		operator++() { ptr = ptr->m_next; return *this; }
		inline Self		operator++(int) { Self tmp = *this; ptr = ptr->m_next; return tmp; }
		inline Self&		operator--() { ptr = ptr->m_prev; return *this; }
		inline Self		operator--(int) { Self tmp = *this; ptr = ptr->m_prev;	return tmp; }
		inline bool		operator==(const Self& _x) const { return ptr == _x.ptr; }
		inline bool		operator!=(const Self& _x) const { return ptr != _x.ptr; }

		inline Self&		operator+(int _x) { for (; _x > 0; --_x) ptr = ptr->m_next; return *this; }
		inline Self&		operator-(int _x) { for (; _x > 0; --_x) ptr = ptr->m_prev; return *this; }

		const Node* ptr;
	};

	template<typename Key>
	class list
	{
	public:
		typedef Key						value_type;
		typedef Key*						pointer;
		typedef const Key*					const_pointer;
		typedef Key&						reference;
		typedef const Key&					const_reference;

		typedef list_iterator<Key>				iterator;
		typedef list_const_iterator<Key>			const_iterator;
		typedef __stl::reverse_iterator<const_iterator>		const_reverse_iterator;
		typedef __stl::reverse_iterator<iterator>		reverse_iterator;
		typedef size_t						size_type;
		typedef ptrdiff_t					difference_type;

	protected:
		typedef list_node<Key>					Node;
		Node init;

		inline Node* m_create_node(const Key& _x) {
			Node* tmp = (Node*)malloc(sizeof(Node));
			tmp->m_data = _x;
			return tmp;
		}

		inline Node*			m_create_node() { return (Node*)malloc(sizeof(Node)); }
		inline void			m_clear() { this->erase(begin(), end()); }
		inline void			m_init() { this->init.m_next = this->init.m_prev = &this->init; }

		inline void			m_fill_insert(iterator _pos, size_type _n, const Key& _x) {
			for (; _n > 0; --_n) this->m_insert(_pos, _x);
		}

		inline void			m_insert(iterator _position, const value_type& _x) {
			this->m_create_node(_x)->hook(_position.ptr);
		}

		inline void			m_erase(iterator _position) {
			_position.ptr->unhook();
			free(_position.ptr);
		}

		template<typename __Int>
		inline void			m_insert_dispatch(iterator _pos, __Int _n, __Int _x, __stl::true_type) {
			this->m_fill_insert(_pos, static_cast<size_type>(_n), static_cast<Key>(_x));
		}

		template<typename Iter>
		inline void			m_insert_dispatch(iterator _pos, Iter _first, Iter _last, __stl::false_type) {
			for (; _first != _last; ++_first) this->m_insert(_pos, *_first);
		}

		inline void			m_transfer(iterator _position, iterator _first, iterator _last) {
			if (_position != _last) {
				_last.ptr->m_prev->m_next = _position.ptr;
				_first.ptr->m_prev->m_next = _last.ptr;
				_position.ptr->m_prev->m_next = _first.ptr;

				Node* tmp = _position.ptr->m_prev;
				_position.ptr->m_prev = _last.ptr->m_prev;
				_last.ptr->m_prev = _first.ptr->m_prev;
				_first.ptr->m_prev = tmp;
			}
		}

	public:
		explicit list() : init(*m_create_node()) { m_init(); }
		explicit list(size_type _n) : init(*m_create_node()) { m_init(); this->insert(begin(), _n, Key()); }
		list(size_type _n, const Key& _value) : init(*m_create_node()) { m_init(); this->insert(begin(), _n, _value); }
		list(const list& _x) : init(*m_create_node()) { m_init(); this->insert(begin(), _x.begin(), _x.end()); }

		template<typename Iter>
		list(Iter _first, Iter _last) : init(*m_create_node()) { m_init(); this->insert(begin(), _first, _last); }

		~list() {
			/*Node* node, *tmp = &this->init;
			while (tmp) {
			node = tmp->m_next;
			delete node;
			}*/
		}

		/* Iterator operations */
		inline iterator			begin() { return (iterator)this->init.m_next; }
		inline const_iterator		cbegin() const { return (iterator)this->init.m_next; }
		inline iterator			end() { return (iterator)&this->init; }
		inline const_iterator		cend() const { return (iterator)&this->init; }
		inline reverse_iterator		rbegin() { return reverse_iterator(end()); }
		inline const_reverse_iterator	crbegin() const { return const_reverse_iterator(cend()); }
		inline reverse_iterator		rend() { return reverse_iterator(begin()); }
		inline const_reverse_iterator	crend() const { return const_reverse_iterator(cbegin()); }


		/* Size Operations */
		inline bool			empty() const { return this->init.m_next == &this->init; }
		inline size_type		size() const { return __stl::distance(begin(), end()); }
		inline size_type		max_size() const { return size_type(-1); }

		/* Element access */
		inline reference		front() { return *begin(); }
		inline const_reference		front() const { return *begin(); }
		inline reference		back() { return *(--end()); }
		inline const_reference		back() const { return *(--end()); }

		/* Modifiers */
		inline void			push_front(const Key& _x) { this->m_insert(begin(), _x); }
		inline void			pop_front() { this->m_erase(begin()); }
		inline void			push_back(const Key& _x) { this->m_insert(end(), _x); }
		inline void			pop_back() { this->m_erase(this->init.m_prev); }

		inline iterator			insert(iterator _pos, const Key& _x) { this->m_insert(_pos, _x); return --_pos; }
		inline void			insert(iterator _pos, size_type _n, const Key& _x) { this->m_fill_insert(_pos, _n, _x); }

		template<typename Iter>
		inline void			insert(iterator _pos, Iter _first, Iter _last) { this->m_insert_dispatch(_position, _first, _last, __stl::is_integral<Iter>::value); }

		inline iterator			erase(iterator _pos) { this->m_erase(_pos++); return _pos; }
		inline iterator			erase(iterator _first, iterator _last) { while (_first != _last) _first = erase(_first); return _last; }
		inline void			clear() { this->m_clear(); this->m_init(); }



		inline void			remove(const Key& _value) {
			for (iterator it = begin(); it != end(); ++it) {
				if (it.ptr->m_data == _value)
					it = this->erase(it);
			}
		}

		template <class Predicate>
		inline void			remove_if(Predicate _pred) {
			for (iterator it = begin(); it != end(); ++it) {
				if (_pred(*it))
					it = this->erase(it);

				// Fix last item delete
				if (_pred(back())) pop_back();
			}
		}


		/* Operators */
		inline bool operator==(const list& _x) {
			const_iterator end1 = end();
			const_iterator end2 = _x.end();

			const_iterator it1 = begin();
			const_iterator it2 = _x.begin();
			while (it1 != end1 && it2 != end2 && *it1 == *it2) {
				++it1;
				++it2;
			}

			return it1 == end1 && it2 == end2;
		}

		inline bool operator<(const list& _x) { return __stl::lexicographical_compare(_x.begin(), _x.end(), begin(), end()); }
		inline bool operator>(const list& _x) { return _x < *this; }
		inline bool operator!=(const list<Key>& _x) { return !(_x == *this); }
		inline bool operator<=(const list<Key>& _x) { return !(_x < *this); }
		inline bool operator>=(const list<Key>& _x) { return !(*this < _x); }
	};
__ztl_namespace_end

#endif /* _list_hpp_1337420 */
