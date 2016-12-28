#ifndef _String_h_
#define _String_h_

#include"allocator.h"
#include"iterator.h"
#include"reverseIterator.h"
#include"type_traits.h"
#include"uninitializedFunctions.h"
#include"utility.h"
#include<cassert>
#include<cstring>
#include<type_traits>
//using namespace TinySTL;
namespace TinySTL{
	class String
	{
	public:
		typedef char				value_type;
		typedef char *				iterator;
		typedef const char *		const_iterator;
		typedef char&				reference;
		typedef const char&			const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef reverse_iterator_t<char*> reverse_iterator;
		typedef reverse_iterator_t<const char *> const_reverse_iterator;
		//npos is a static member constant value
		static const size_t npos = -1;
	private:
		char *start;
		char *finish;
		char *end_of_storage;

		typedef TinySTL::allocator<char> dataAllocator;
	private:
		size_t alloc_capacity(size_t old_size) const;   //容量分配函数
		void empty_init(); //初始化
		size_t changeVarWhenEqualNPOS(size_t var, size_t minuend, size_t minue) const;


	public:
		String();//默认构造函数
		String(const String& str);		//拷贝构造函数
		String(String &&str);
		String(const String& str, size_t pos, size_t len = npos);
		String(const char *s);
		String(const char *s, size_t n);
		String(size_t n, char c);
		template<class InputIterator>
		String(InputIterator first, InputIterator last);

		String& operator=(const String& str);
		String& operator=(String &&str);
		String& operator=(const char *s);
		String& operator=(char c);

		~String();
	public:
		size_type size() const { return finish - start; }
		size_type length() const { return size(); }
		size_type max_size() const { return size_type(-1) / sizeof(value_type) - 1; }
		size_type capacity() const { return end_of_storage - start - 1; }
		iterator begin() { return start; }
		iterator end() { return finish; }
		const_iterator begin() const { return start; }
		const_iterator end() const { return finish; }
		reverse_iterator rbegin() { return reverse_iterator(finish); }
		reverse_iterator rend() { return reverse_iterator(start); }
		const_reverse_iterator rbegin() const { return reverse_iterator(finish); }
		const_reverse_iterator rend() const { return reverse_iterator(start); }
		const_iterator cbegin() const { return start; }
		const_iterator cend() const { return finish; }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(finish); }
		const_reverse_iterator crend() const { return const_reverse_iterator(start); }
		char &back(){ return *(finish - 1); }
		const char& back() const { return *(finish - 1); }
		char &front() { return *start; }
		const char &front() const { *start; }
		void clear() {
			dataAllocator::destroy(start, finish);
			start = finish;
		}
		char & operator[](size_t pos) { return *(start + pos); }
		const char & operator[](size_t pos) const { return *(start + pos); }
		bool empty() const { return begin() == end(); }
		void resize(size_t n);
		void resize(size_t n, char c);
		void reverse(size_t n = 0);
		void shrink_to_fit(){
			dataAllocator::deallocate(finish, end_of_storage - finish);
			end_of_storage = finish;
		}
		char& at(size_t pos){ return *(start + pos); }
		const char& at(size_t pos) const { return *(start + pos); }

		//modifiers
		String& operator+=(const String& str);
		String& operator+=(const char *s);
		String& operator+= (char c);
		void push_back(char c){ insert(end(), c); };
		String &insert(size_t pos, const String& str);
		String &insert(size_t pos, const String& str, size_t subpos, size_t sublen = npos);
		String &insert(size_t pos, const char* s);
		String &insert(size_t pos, const char *s, size_t n);
		String &insert(size_t pos, size_t n, char c);
		iterator insert(iterator p, size_t n, char c);
		iterator insert(iterator p, char c);
		template<class InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);
		String& append(const String& str);
		String& append(const String& str, size_t subpos, size_t sublen = npos);
		String& append(const char* s);
		String& append(const char* s, size_t n);
		String& append(size_t n, char c);
		template<class InputIterator>
		String& append(InputIterator first, InputIterator last);
		String& erase(size_t pos = 0, size_t len = npos);
		iterator erase(iterator p);
		iterator erase(iterator first, iterator last);
		String& replace(size_t pos, size_t len, const String& str);
		String& replace(iterator i1, iterator i2, const String& str);
		String& replace(size_t pos, size_t len, const String& str, size_t subpos, size_t sublen = npos);
		String& replace(size_t pos, size_t len, const char *s);
		String& replace(iterator i1, iterator i2, const char *s);
		String& replace(size_t pos, size_t len, const char *s, size_t n);
		String& replace(iterator i1, iterator i2, const char* s, size_t n);
		String& replace(size_t pos, size_t len, size_t n, char c);
		String& replace(iterator i1, iterator i2, size_t n, char c);
		template<class InputIterator>
		String& replace(iterator i1, iterator i2, InputIterator first, InputIterator last);
		void swap(String& str){
			TinySTL::swap(start, str.start);
			TinySTL::swap(finish, str.finish);
			TinySTL::swap(end_of_storage, str.end_of_storage);
		}
		void pop_back(){ erase(end() - 1, end());}
		const_iterator c_str() const{
			*finish = 0;
			return start;
		}
		size_t copy(char *s, size_t len, size_t pos = 0) const{
			auto ptr = TinySTL::uninitialized_copy(begin() + pos, begin() + pos + len, s);
			return (size_t)(ptr - s);
		}
		size_t find(const String& str, size_t pos = 0) const;
		size_t find(const char* s, size_t pos = 0) const;
		size_t find(const char* s, size_t pos, size_t n) const;
		size_t find(char c, size_t pos = 0) const;
		size_t rfind(const String& str, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos, size_t n) const;
		size_t rfind(char c, size_t pos = npos) const;
		size_t find_first_of(const String& str, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_of(char c, size_t pos = 0) const;
		size_t find_last_of(const String& str, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_of(char c, size_t pos = npos) const;
		size_t find_first_not_of(const String& str, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_not_of(char c, size_t pos = 0) const;
		size_t find_last_not_of(const String& str, size_t pos = npos) const;
		size_t find_last_not_of(const char *s, size_t pos = npos) const;
		size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_not_of(char c, size_t pos = npos) const;

		String substr(size_t pos = 0, size_t len = npos) const{
			len = changeVarWhenEqualNPOS(len, size(), pos);
			return String(begin() + pos, begin() + pos + len);
		}

		int compare(const String& str) const;
		int compare(size_t pos, size_t len, const String& str) const;
		int compare(size_t pos, size_t len, const String& str,
			size_t subpos, size_t sublen = npos) const;
		int compare(const char * s) const;
		int compare(size_t pos, size_t len, const char* s) const;
		int compare(size_t pos, size_t len, const char *s, size_t n) const;
	public:
		friend std::ostream& operator<<(std::ostream &os, const String& str);
		friend std::istream& operator>>(std::istream &is, String& str);
		friend String operator+(const String& lhs, const String& rhs);
		friend String operator+(const String& lhs, const char* rhs);
		friend String operator+(const char* lhs, const String& rhs);
		friend String operator+(const String& lhs, char rhs);
		friend String operator+(char lhs, const String& rhs);
		friend bool operator==(const String& lhs, const String& rhs);
		friend bool operator==(const char* lhs, const String& rhs);
		friend bool operator==(const String& lhs, const char* rhs);
		friend bool operator!=(const String& lhs, const String& rhs);
		friend bool operator!=(const char* lhs, const String& rhs);
		friend bool operator!=(const String& lhs, const char* rhs);
		friend bool operator<(const String& lhs, const String& rhs);
		friend bool operator<(const char* lhs, const String& rhs);
		friend bool operator<(const String& lhs, const char* rhs);
		friend bool operator<=(const String& lhs, const String& rhs);
		friend bool operator<=(const char* lhs, const String& rhs);
		friend bool operator<=(const String& lhs, const char* rhs);
		friend bool operator>(const String& lhs, const String& rhs);
		friend bool operator>(const char* lhs, const String& rhs);
		friend bool operator>(const String& lhs, const char* rhs);
		friend bool operator>=(const String& lhs, const String& rhs);
		friend bool operator>=(const char* lhs, const String& rhs);
		friend bool operator>=(const String& lhs, const char* rhs);
		friend std::istream& getline(std::istream& is, String& str, char delim);
		friend std::istream& getline(std::istream& is, String& str);
		friend void swap(String &x, String& y);
	private:
		size_t getNewCapacity(size_t len) const;
		template<class InputIterator>
		iterator insert_aux_copy(iterator p, InputIterator first, InputIterator last);
		iterator insert_aux_filln(iterator p, size_t n, value_type c);
		size_t find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond) const;
		size_t rfind_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond) const;
		bool isContained(char ch, const_iterator first, const_iterator last) const;
		int compare_aux(size_t pos, size_t len, const_iterator cit, size_t subpos, size_t sublen) const;
	};

	template<class InputIterator>
	String::String(InputIterator first, InputIterator last)
		:start(0), finish(0), end_of_storage(0){
		size_type len = last - first;
		size_t csz = alloc_capacity(len);
		start = dataAllocator::allocate(csz);
		finish = uninitialized_copy(first, last, start);
		end_of_storage = start + csz;
	}

	template<class InputIterator>
	String::iterator String::insert_aux_copy(iterator p, InputIterator first, InputIterator last){
		size_t len = last - first;
		size_t newCap = getNewCapacity(len);
		iterator newStart = dataAllocator::allocate(newCap);
		iterator newFinish = TinySTL::uninitialized_copy(start, p, newStart);
		newFinish = TinySTL::uninitialized_copy(first, last, newFinish);
		auto res = finish;
		newFinish = TinySTL::uninitialized_copy(p, finish, newFinish);
		dataAllocator::destroy(start, finish);
		dataAllocator::deallocate(start, end_of_storage - start);

		start = newStart;
		finish = newFinish;
		end_of_storage = start + newCap;
		return res;
	}

	template<class InputIterator>
	String::iterator String::insert(iterator p, InputIterator first, InputIterator last){
		auto oldLen = capacity() - size();
		size_t len = distance(first, last);

		if (len <= oldLen){
			for (iterator it = finish - 1; it >= p; --it)
				*(it + len) = *it;

			TinySTL::uninitialized_copy(first, last, p);
			finish += len;
			return p + len;
		}
		else{
			return insert_aux_copy(p, first, last);
		}
	}

	template<class InputIterator>
	String& String::append(InputIterator first, InputIterator last){
		insert(end(), first, last);
		return *this;
	}

	template<class InputIterator>
	String& String::replace(iterator i1, iterator i2, InputIterator first,
		InputIterator last){
		auto ptr = erase(i1, i2);
		insert(ptr, first, last);
		return *this;
	}
}
#endif