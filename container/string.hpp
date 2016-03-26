#ifndef m_data_hpp_1337420
#define m_data_hpp_1337420

#include <cstdlib>
#include <cmath>
#include <memory>

#ifdef __stl_compatibility
	#include <string>
	#include <sstream>
#endif

#include "../ztl_base.hpp"

__ztl_namespace_start

class string {
public:
	typedef unsigned long long						          size_type;
	typedef int										                  difference_type;
	typedef char*									                  iterator;
	typedef const char*								              const_iterator;
	typedef __stl::reverse_iterator<iterator>		    reverse_iterator;
	typedef __stl::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef __stl::random_access_iterator_tag		    iterator_category;
	
private:
	int m_capacity;
	int m_length; 
	char* m_data;

public:
	static string empty, npos;

	string() : m_capacity(16), m_length(0), m_data(new char[m_capacity + 1]) {
		memset(m_data, 0, m_capacity + 1);
	}

	string(const char * c_str) : m_length(0) {
		while (c_str[m_length] != '\0') m_length++;;
		m_capacity = m_length <= 16 ? 16 : m_length;
		m_data = new char[m_capacity + 1];
		memset(m_data, 0, m_capacity + 1);
		memcpy(m_data, c_str, m_length);
	}

	string(size_type _size) : m_capacity(_size), m_length(0), m_data(new char[m_capacity + 1]) {
		memset(m_data, 0, m_capacity + 1);
	}

	string::string(const string & _x) : m_capacity(_x.m_capacity), m_length(_x.m_length) {
		if (_x != string::empty) {
			m_data = new char[m_capacity + 1];
			memset(m_data, 0, m_capacity + 1);
			memcpy(m_data, _x.m_data, m_length);
		}
		else {
			m_data = NULL;
			m_length = 0;
			m_capacity = 0;
		}
	}

	~string()
	{
		m_length = 0;
		m_capacity = 0;
		delete[] m_data;
	}

#ifdef __stl_compatibility
	string(const __stl::string& _s) : m_length(_s.length()), m_capacity(m_length), m_data(new char[m_capacity + 1]) {
		memcpy(m_data, _s.c_str(), m_length);
		m_data[m_capacity] = '\0';
	}
#endif

	inline char& operator[](const int _i) { return m_data[_i]; }
	inline const char& operator[](const int _i) const { return m_data[_i]; }

	void operator+= (const string & _x) {
		if ((m_length + _x.m_length) > m_capacity) {
			m_capacity = m_length + _x.m_length;
			char *buffer = new char[m_capacity + 1];
			memset(buffer, 0, m_capacity + 1);
			memcpy(buffer, m_data, m_length);
			delete[] m_data;
			m_data = buffer;
		}

		memcpy(m_data + m_length, _x.m_data, _x.m_length);
		m_length += _x.m_length;
	}

	void operator+= (const char* const _x) {
		size_type strlen = 0;
		while (_x[strlen] != '\0') strlen++;
		if ((m_length + strlen) > m_capacity) {
			m_capacity = m_length + strlen;
			char *buffer = new char[m_capacity + 1];
			memset(buffer, 0, m_capacity + 1);
			memcpy(buffer, m_data, m_length);
			delete[] m_data;
			m_data = buffer;
		}

		memcpy(m_data + m_length, _x, strlen);
		m_length += strlen;
	}

	string& operator=(const string & _x)
	{
		if (m_capacity < _x.m_length)
			this->clear(_x.m_length);

		memcpy(m_data, _x.m_data, _x.m_length);
		m_length = _x.m_length;
		m_data[m_length] = 0;

		return *this;
	}

	string& operator=(const char * const _x)
	{
		m_length = 0;
		while (_x[m_length] != '\0') m_length++;
		
		delete[] m_data;
		m_capacity = m_length < 16 ? 16 : m_length;
		m_data = new char[m_capacity + 1];
		
		memset(m_data, 0, m_capacity + 1);
		memcpy(m_data, _x, m_length);

		return *this;
	}
	
	void clear() {
		delete[] m_data;
		m_data = new char[m_capacity + 1];
		memset(m_data, 0, m_capacity + 1);
		m_length = 0;
	}
	
	void clear(size_type _capacity) {
		delete[] m_data;
		m_capacity = _capacity;
		m_data = new char[m_capacity + 1];
		memset(m_data, 0, m_capacity + 1);
		m_length = 0;
	}

	inline void	resize(size_type n) {
		m_capacity = n;
		char *buffer = new char[m_capacity + 1];
		memset(buffer, 0, n + 1);
		memcpy(buffer, m_data, m_length);
		delete[] m_data;
		m_data = buffer;
		m_length = __min(m_length, n);
		m_data[m_length] = '\0';
	}
	
	inline void	resize(size_type n, char c) {
		m_capacity = n;
		char *buffer = new char[m_capacity + 1];
		memset(buffer, c, n + 1);
		memcpy(buffer, m_data, m_length);
		delete[] m_data;
		m_data = buffer;
		m_length = m_capacity;
		m_data[m_capacity] = '\0';
	}

	inline char& at(int _i) {
		if (_i < 0 || _i >= m_length) {
#ifdef __stl_compatibility
			throw __stl::out_of_range("Index out of range");
#else
			return NULL;
#endif
		}
		return m_data[_i]; 
	}

	inline const char& at(int _i) const {
		if (_i < 0 || _i >= m_length) {
#ifdef __stl_compatibility
			throw __stl::out_of_range("Index out of range");
#else
			return NULL;
#endif
		}
		return m_data[_i];
	}

	inline size_type					          length() const { return m_length; }
	inline size_type					          size() const { return m_length;	}
	inline size_type					          max_size() const { return size_type(-1); }
	inline size_type					          capacity() const { return m_capacity; }
	
	inline iterator						          begin() { return m_data; }
	inline iterator						          end() { return m_data + m_length; }
	inline const_iterator				        cbegin() const { return m_data; }
	inline const_iterator				        cend() const { return m_data + m_length; }
	inline reverse_iterator				      rbegin() { return reverse_iterator(end()); }
	inline reverse_iterator				      rend() { return reverse_iterator(begin()); }
	inline const_reverse_iterator		    crbegin() const { return const_reverse_iterator(cend()); }
	inline const_reverse_iterator		    crend() const { return const_reverse_iterator(cbegin()); }

	inline const char*					        c_str() const { return m_data; }

	string& substr(int start_index, int length) const {
		if (start_index + length > m_length) {
#ifdef __stl_compatibility
			throw __stl::invalid_argument("string::substr: length exceed string length");
#else
			return string::empty;
#endif
		}

		char *buffer = new char[length + 1];
		memcpy(buffer, m_data + start_index, length);
		buffer[length] = '\0';
		string* temp = new string(buffer);
		delete[] buffer;
		return *temp;
	}

	// OPERATORS
	friend bool operator==(const string&, const string&);
	friend bool operator==(const string&, const char* const);
	friend bool operator> (const string&, const string&);
	friend bool operator> (const string&, const char*);
	friend bool operator< (const string&, const string&);
	friend bool operator< (const string&, const char*);
	friend bool operator>= (const string&, const string&);
	friend bool operator>= (const string&, const char*);
	friend bool operator<= (const string&, const string&);
	friend bool operator<= (const string&, const char*);	
	friend bool operator!=(const string&, const string&);
	friend bool operator!=(const string&, const char*);


#ifdef __stl_compatibility

	friend __stl::ostream & operator<< (__stl::ostream & os, const string & _x);
	friend __stl::istream & operator>> (__stl::istream & is, string & _x);

#endif

};

string string::empty = string();
string string::npos = string();

bool operator> (const string& _x, const string& _y) {
	for (int i = 0; i < __min(_x.m_length, _y.m_length); i++) {
		if (_x[i] == _y[i]) continue;
		return _x[i] > _y[i];
	}
	return _x.length() > _y.length();
}

bool operator> (const string& _x, const char* _y) {
	int i;
	for (i = 0; i < _x.m_length; i++) {
		if (_x[i] == _y[i]) continue;
		return _y[i] == '\0' || _x[i] > _y[i];
	}
	return _y[i+1] != '\0';
}

bool operator== (const string& _x, const string& _y) {
	if (_x.m_length != _y.m_length) return false;
	for (int i = 0; i < _x.m_length; i++)
		if (_x[i] != _y[i]) return false;

	return true;
}

bool operator==(const string& _x, const char* _y) {
	int i;
	for (i = 0; i < _x.m_length; i++)
		if (_y[i] == '\0' || _x[i] != _y[i]) return false;

	return _y[++i] == '\0';
}

bool operator< (const string& _x, const string& _y) { return !(_x > _y || _x == _y); }
bool operator< (const string& _x, const char* _y) { return !(_x > _y || _x == _y); }
bool operator>= (const string& _x, const string& _y) { return !(_x < _y); }
bool operator>= (const string& _x, const char* _y) { return !(_x < _y); }
bool operator<= (const string& _x, const string& _y) { return !(_x > _y); }
bool operator<= (const string& _x, const char* _y) { return !(_x > _y); }
bool operator!=(const string& _x, const string& _y) { return !(_y == _x); }
bool operator!=(const string& _x, const char* _y) { return !(_y == _x); }







#ifdef __stl_compatibility

__stl::ostream & operator<< (__stl::ostream & os, const string & _x) {
	os << _x.m_data;
	return os;
}

__stl::istream & operator>> (__stl::istream & is, string& _x) {
	char *buffer = new char[1024];
	is.getline(buffer, 1024);
	
	if (is.gcount() > 0) {
		_x.clear(is.gcount());
		_x = buffer;
	}
	else
		_x.clear();

	return is;
}

#endif


__ztl_namespace_end


#endif
