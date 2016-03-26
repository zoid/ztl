#ifndef _trie_hpp_1337420
#define _trie_hpp_1337420

#include "../ztl_base.hpp"

#ifdef __stl_compatibility
	#include <string>
	#include <vector>
#endif

#include "../container/string.hpp"
#include "../container/vector.hpp"
#include "../container/map.hpp"

__ztl_namespace_start
	struct trie_node  {
		map<char, trie_node*> m_nodes;
		char m_value;
		bool m_end;

		trie_node() : m_value(NULL), m_end(false), m_nodes(map<char, trie_node*>()) { }
		trie_node(char _v) : m_value(_v), m_end(false), m_nodes(map<char, trie_node*>()) {	}

		trie_node* operator[](char c) { return  m_nodes[c]; }
		
		inline trie_node* add(char c) {
			if (m_nodes.find(c) == m_nodes.end()) 
				m_nodes.insert(make_pair(c, new trie_node(c)));

			return m_nodes[c];
		}

		inline void remove(char c) {
			if (m_nodes.find(c) != m_nodes.end()) {
				delete m_nodes[c];
				m_nodes.erase(m_nodes.find(c));
			}
		}
	};

	class trie  {
	private:
		typedef trie_node		Node;
		Node* root;

	public:
		trie() : root(new Node()) {}
		~trie() { delete root; }

		inline bool empty()	{ return this->root->m_nodes.empty(); }

		inline void insert(string s) {
			Node* node = root;
			for (char c : s) node = node->add(c);
			
			node->m_end = true;
		}

		inline void insert(vector<string> sv) {
			for (string s : sv) insert(s);
		}

		inline Node* find(string s, bool match_words = false) const {
			Node* node = root;
			for (char c : s) {
				if (node == NULL) return NULL;
				node = (*node)[c];
			}

			return (match_words) ? ((node->m_end) ? node : NULL) : node;
		}

		inline bool remove(string s, bool partial_remove = false) {
			if (find(s, !partial_remove)) return false;

			Node* node = root, *lw2 = root;
			char lw2_c = s[0];

			for (char c : s) {
				if (!(node->m_nodes.size() > 1)) {
					lw2 = node;
					lw2_c = c;
				}
				node = (*node)[c];
			}

			lw2->remove(lw2_c);
			return true;
		}


#ifdef __stl_compatibility
		inline void insert(__stl::string s) {
			Node* node = root;
			for (char c : s) node = node->add(c);

			node->m_end = true;
		}

		inline void insert(__stl::vector<__stl::string> sv) {
			for (__stl::string s : sv) insert(s);
		}

		inline Node* find(__stl::string s, bool match_words = false) const {
			Node* node = root;
			for (char c : s) {
				if (node == NULL) return NULL;
				node = (*node)[c];
			}

			return (match_words) ? ((node->m_end) ? node : NULL) : node;
		}

		inline bool remove(__stl::string s, bool partial_remove = false) {
			if (find(s, !partial_remove)) return false;

			Node* node = root, *lw2 = root;
			char lw2_c = s[0];

			for (char c : s) {
				if (!(node->m_nodes.size() > 1)) {
					lw2 = node;
					lw2_c = c;
				}
				node = (*node)[c];
			}

			lw2->remove(lw2_c);
			return true;
		}
#endif
		
	};
__ztl_namespace_end

#endif /* _trie_hpp_1337420 */
