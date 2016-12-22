#include"String.h"
#include<iostream>
namespace TinySTL{
	//ctor
	String::String()
		:start(0), finish(0), end_of_storage(0){
		empty_init();
	}

	String::String(const String& str)
		: start(0), finish(0), end_of_storage(0){
		size_type len = str.length();
		size_type capacity_sz = alloc_capacity(len);
		start = dataAllocator::allocate(capacity_sz);
		TinySTL::uninitialized_copy(str.start, str.finish, start);
		finish = start + len;
		end_of_storage = start + capacity_sz;
	}

	String::String(const char *s)
		:start(0), finish(0), end_of_storage(0){
		size_type len = strlen(s);
		size_type capacity_sz = alloc_capacity(len);
		start = dataAllocator::allocate(capacity_sz);
		TinySTL::uninitialized_copy(s, s + len, start);
		finish = start + len;
		end_of_storage = start + capacity_sz;


	}

	String::String(String &&str)
		:start(0), finish(0), end_of_storage(0){
		//move ctor
		start = str.start;
		finish = str.finish;
		end_of_storage = str.end_of_storage;
		str.finish = str.start = str.end_of_storage = 0;
	}

	String::String(const String& str, size_t pos, size_t len)
		:start(0), finish(0), end_of_storage(0){
		size_t sz = changeVarWhenEqualNPOS(len, str.size(), pos);
		size_t csz = alloc_capacity(sz);
		start = dataAllocator::allocate(csz);
		TinySTL::uninitialized_copy(str.start + pos, str.start + sz, start);
		finish = start + sz;
		end_of_storage = start + csz;
	}

	String::String(const char *s, size_t n)
		: start(0), finish(0), end_of_storage(0){
		size_t csz = alloc_capacity(n);
		start = dataAllocator::allocate(csz);
		TinySTL::uninitialized_copy(s, s + n, start);
		finish = start + n;
		end_of_storage = start + csz;
	}

	String::String(size_t n, char c)
		: start(0), finish(0), end_of_storage(0){
		size_t csz = alloc_capacity(n);
		start = dataAllocator::allocate(csz);
		finish = TinySTL::uninitialized_fill_n(start, n, c);
		end_of_storage = start + csz;
	}
	String& String::operator=(const String &str){
		if (this != &str){
			dataAllocator::destroy(start, finish);
			dataAllocator::deallocate(start, end_of_storage - start);
			size_t sz = str.size();
			size_t csz = alloc_capacity(sz);
			start = dataAllocator::allocate(csz);
			finish = TinySTL::uninitialized_copy(str.start, str.finish, start);
			end_of_storage = start + csz;
		}

		return *this;
	}

	String& String::operator=(String &&str){
		if (this != &str){
			start = str.start;
			finish = str.finish;
			end_of_storage = str.end_of_storage;
			str.finish = str.start = str.end_of_storage = 0;
		}

		return *this;
	}

	String& String::operator=(const char* s){
		dataAllocator::destroy(start, finish);
		dataAllocator::deallocate(start, end_of_storage - start);
		size_t sz = strlen(s);
		size_t csz = alloc_capacity(sz);
		start = dataAllocator::allocate(csz);
		finish = TinySTL::uninitialized_copy(s, s + sz, start);
		end_of_storage = start + csz;

		return *this;
	}

	String& String::operator=(char c){
		dataAllocator::destroy(start, finish);
		dataAllocator::deallocate(start, end_of_storage - start);
		size_t csz = alloc_capacity(1);
		start = dataAllocator::allocate(csz);
		finish = TinySTL::uninitialized_fill_n(start, 1, c);
		end_of_storage = start + csz;

		return *this;
	}

	String::~String(){
		dataAllocator::destroy(start, finish);
		dataAllocator::deallocate(start, end_of_storage - start);
	}
	//modifiers
	String& String::operator+=(const String& str){
		insert(size(), str);
		return *this;
	}

	String& String::operator+=(const char *s){
		insert(size(), s);
		return *this;
	}

	String& String::operator+=(char c){
		insert(end(), c);
		return *this;
	}
	String& String::insert(size_t pos, const String& str){
		insert(start + pos, str.begin(), str.end());
		return *this;
	}

	String& String::insert(size_t pos, const char* s){
		insert(start + pos, s, s + strlen(s));
		return *this;
	}

	String& String::insert(size_t pos, const String& str, size_t subpos, size_t sublen){
		sublen = changeVarWhenEqualNPOS(sublen, str.size(), subpos);
		insert(begin() + pos, str.begin() + subpos, str.begin() + subpos + sublen);
		return *this;
	}

	String& String::insert(size_t pos, const char* s, size_t n){
		insert(begin() + pos, s, s + n);
		return (*this);
	}

	String::iterator String::insert_aux_filln(iterator p, size_t n, value_type c){
		auto newCapacity = getNewCapacity(n);
		iterator newStart = dataAllocator::allocate(newCapacity);
		iterator newFinish = TinySTL::uninitialized_copy(start, p, newStart);
		newFinish = TinySTL::uninitialized_fill_n(newFinish, n, c);
		auto res = newFinish;
		newFinish = TinySTL::uninitialized_copy(p, finish, newFinish);

		dataAllocator::destroy(start, finish);
		dataAllocator::deallocate(start, end_of_storage - start);
		start = newStart;
		finish = newFinish;
		end_of_storage = start + newCapacity;
		return res;
	}

	String& String::insert(size_t pos, size_t n, char c){
		insert(begin() + pos, n, c);
		return *this;
	}

	String::iterator String::insert(iterator p, size_t n, char c){
		auto oldLen = capacity() - size();
		if (n <= oldLen){
			for (iterator it = finish - 1; it >= p; --it)
				*(it + n) = *it;

			TinySTL::uninitialized_fill_n(p, n, c);
			finish += n;
			return p + n;
		}
		else
		{
			return insert_aux_filln(p, n, c);
		}
	}

	String::iterator String::insert(iterator p, char c){
		return insert(p, 1, c);
	}
	String& String::append(const String& str){
		(*this) += str;
		return *this;
	}

	String& String::append(const String& str, size_t subpos, size_t sublen){
		sublen = changeVarWhenEqualNPOS(sublen, str.size(), subpos);
		insert(size(), str, subpos, sublen);
		return *this;
	}

	String& String::append(const char* s){
		(*this) += s;
		return *this;
	}

	String& String::append(const char* s, size_t n){
		insert(size(), s, n);
		return *this;
	}

	String& String::append(size_t n, char c){
		insert(end(), n, c);
		return *this;
	}

	String::iterator String::erase(iterator first, iterator last){
		size_t len = last - first;
		for (auto i = 0; i != len; ++i){
			*(first + i) = *(last + i);
		}

		dataAllocator::destroy(first + len, finish);
		finish = first + len;
		return first;
	}

	String& String::erase(size_t pos, size_t len){
		len = changeVarWhenEqualNPOS(len, size(), pos);
		erase(begin() + pos, begin() + pos + len);
		return *this;
	}

	String::iterator String::erase(iterator p){
		return erase(p, p + 1);
	}

	String& String::replace(size_t pos, size_t len, const String& str){
		return replace(begin() + pos, begin() + pos + len, str.begin(), str.end());
	}

	String& String::replace(iterator i1, iterator i2, const String& str){
		return replace(i1, i2, str.begin(), str.end());
	}

	String& String::replace(size_t pos, size_t len, const String& str, size_t subpos, size_t sublen){
		sublen = changeVarWhenEqualNPOS(sublen, str.size(), subpos);
		return replace(begin() + pos, begin() + pos + len, str.begin() + subpos, str.begin() + subpos + sublen);
	}

	String& String::replace(size_t pos, size_t len, const char* s){
		return replace(begin() + pos, begin() + pos + len, s, s + strlen(s));
	}

	String& String::replace(iterator i1, iterator i2, const char* s){
		return replace(i1, i2, s, s + strlen(s));
	}

	String& String::replace(size_t pos, size_t len, const char *s, size_t n){
		return replace(begin() + pos, begin() + pos + len, s, s + n);
	}

	String& String::replace(iterator i1, iterator i2, const char* s, size_t n){
		return replace(i1, i2, s, s + n);
	}

	String& String::replace(size_t pos, size_t len, size_t n, char c){
		return replace(begin() + pos, begin() + pos + len, n, c);
	}

	String& String::replace(iterator i1, iterator i2, size_t n, char c){
		auto ptr = erase(i1, i2);
		insert(ptr, n, c);
		return *this;
	}
	size_t String::find(const String& str, size_t pos) const{
		size_t len = str.size();
		if (size() - pos < len)
			return npos;
		return find_aux(str.cbegin(), pos, len, size());
	}

	size_t String::find(const char *s, size_t pos, size_t n) const{
		size_t len = strlen(s);
		return find_aux(s, pos, n, size());
	}

	size_t String::find(const char* s, size_t pos) const{
		return find(s, pos, strlen(s));
	}

	size_t String::find(char c, size_t pos) const{
		for (auto cit = cbegin() + pos; cit != cend(); ++cit){
			if (*cit == c)
				return cit - cbegin();
		}
		return npos;
	}

	size_t String::rfind_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond) const{
		size_t i, j;
		for (i = pos + lengthOfS; i >= cond; --i){
			for (j = 0; j != lengthOfS; ++j)
				if (*(begin() + i + j) != cit[j])
					break;

			if (j == lengthOfS)
				return i;
		}
		return npos;
	}

	size_t String::rfind(const String& str, size_t pos) const{
		auto len = str.size();
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		return rfind_aux(str.begin(), pos, len, 0);
	}

	size_t String::rfind(const char* s, size_t pos) const{
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		return rfind(s, pos, strlen(s));
	}

	size_t String::rfind(const char* s, size_t pos, size_t n) const{
		auto len = strlen(s);
		return rfind_aux(s, pos, n, 0);
	}

	size_t String::rfind(char c, size_t pos) const{
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		for (auto cit = cbegin() + pos; cit >= cbegin(); --cit){
			if (*cit == c)
				return cit - cbegin();
		}
		return npos;
	}

	size_t String::find_first_of(const char* s, size_t pos, size_t n) const{
		for (size_t i = pos; i != size(); ++i){
			if (isContained((*this)[i], s, s + n))
				return i;
		}
		return npos;
	}

	size_t String::find_first_of(const String& str, size_t pos) const{
		return find_first_of(str.begin(), pos, str.size());
	}

	size_t String::find_first_of(const char* s, size_t pos) const{
		return find_first_of(s, pos, strlen(s));
	}

	size_t String::find_first_of(char c, size_t pos) const{
		return find(c, pos);
	}

	size_t String::find_last_of(const char* s, size_t pos, size_t n) const{
		for (size_t i = pos; i > 0; --i){
			if (isContained((*this)[i], s, s + n))
				return i;
		}
		return npos;
	}

	size_t String::find_last_of(char c, size_t pos) const{
		return rfind(c, pos);
	}

	size_t String::find_last_of(const String& str, size_t pos) const{
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		return find_last_of(str.begin(), pos, str.size());
	}

	size_t String::find_last_of(const char* s, size_t pos) const{
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		return find_last_of(s, pos, strlen(s));
	}

	size_t String::find_first_not_of(char c, size_t pos) const{
		for (size_t i = pos; i != size(); ++i){
			if ((*this)[i] != c)
				return i;
		}
		return npos;
	}

	size_t String::find_first_not_of(const char* s, size_t pos, size_t n) const{
		for (size_t i = pos; i != size(); ++i){
			if (!isContained((*this)[i], s, s + n))
				return i;
		}
		return npos;
	}

	size_t String::find_first_not_of(const String& str, size_t pos) const{
		return find_first_not_of(str.begin(), pos, str.size());
	}

	size_t String::find_first_not_of(const char* s, size_t pos) const{
		return find_first_not_of(s, pos, strlen(s));
	}

	size_t String::find_last_not_of(char c, size_t pos) const{
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		for (size_t i = pos; i >= 0; --i){
			if ((*this)[i] != c)
				return i;
		}
		return npos;
	}

	size_t String::find_last_not_of(const char* s, size_t pos, size_t n) const{
		for (size_t i = pos; i >= 0; --i){
			if (!isContained((*this)[i], s, s + n))
				return i;
		}
		return npos;
	}

	size_t String::find_last_not_of(const String& str, size_t pos) const{
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		return find_last_not_of(str.begin(), pos, str.size());
	}

	size_t String::find_last_not_of(const char* s, size_t pos) const{
		pos = changeVarWhenEqualNPOS(pos, size(), 1);
		return find_last_not_of(s, pos, strlen(s));
	}

	int String::compare(const String& str) const{
		return compare(0, size(), str, 0, str.size());
	}

	int String::compare(const char *s) const{
		return compare(0, size(), s, strlen(s));
	}

	int String::compare(size_t pos, size_t len, const String& str) const{
		return compare(pos, len, str, 0, str.size());
	}

	int String::compare(size_t pos, size_t len, const String& str,
		size_t subpos, size_t sublen) const{
		return compare_aux(pos, len, str.begin(), subpos, sublen);
	}

	int String::compare(size_t pos, size_t len, const char* s) const{
		return compare(pos, len, s, strlen(s));
	}

	int String::compare(size_t pos, size_t len, const char* s, size_t n) const{
		return compare(pos, len, s, 0, n);
	}
	//private member function
	size_t String::alloc_capacity(size_t old_sz) const {
		if (old_sz < 10)
			return 10;
		else if (old_sz < 100)
			return old_sz * 2;
		else if (old_sz < 1024)
			return old_sz + old_sz / 10;
		else
			return old_sz + 1024;
	}

	void String::empty_init(){
		size_t capacity_sz = alloc_capacity(1);
		start = dataAllocator::allocate(capacity_sz);
		finish = start;
		end_of_storage = start + capacity_sz;
	}

	size_t String::changeVarWhenEqualNPOS(size_t var, size_t minuend, size_t minue) const{
		return (var == npos ? minuend - minue : var);
	}

	size_t String::getNewCapacity(size_t len) const{
		size_t oldCap = end_of_storage - start;
		size_t res = oldCap >= len ? oldCap : len;
		return res + oldCap;
	}

	size_t String::find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond) const{
		size_t i, j;
		for (i = pos; i != cond; ++i){
			for (j = 0; j != lengthOfS; ++j){
				if (*(begin() + i + j) != cit[j])
					break;
			}
			if (j == lengthOfS)
				return i;
		}

		return npos;
	}

	bool String::isContained(char ch, const_iterator first, const_iterator last) const{
		for (auto cit = first; cit != last; ++cit){
			if (*cit == ch)
				return true;
		}
		return false;
	}

	int String::compare_aux(size_t pos, size_t len, const_iterator cit,
		size_t subpos, size_t sublen) const{
		size_t i, j;
		for (i = 0, j = 0; i != len && j != sublen; ++i, ++j){
			if ((*this)[pos + i] < cit[subpos + j])
				return -1;
			else if ((*this)[pos + i] > cit[subpos + j])
				return 1;
		}

		if (i == len && j == sublen)
			return 0;
		else if (i == len)
			return -1;
		else
			return 1;
	}

	//friend operator
	std::ostream& operator<<(std::ostream& os, const String &str){
		size_t sz = str.size();

		for (size_t i = 0; i < sz; ++i)
			os << str[i];

		return os;
	}

	std::istream& operator>>(std::istream& is, String& str){
		char ch;
		while (is.get(ch)){
			if (isblank(ch) || ch == '\n')
				continue;
			else
				break;
		}

		is.putback(ch);
		str.clear();
		while (is.get(ch)){
			if (ch != EOF && !isblank(ch) && ch != '\n')
				str.push_back(ch);
			else
				break;
		}

		return is;
	}

	std::istream& getline(std::istream& is, String& str, char delim){
		char ch;
		str.clear();
		while (is.get(ch)){
			if (ch == delim)
				break;
			else
				str.push_back(ch);
		}
		return is;
	}

	std::istream& getline(std::istream& is, String& str){
		return getline(is, str, '\n');
	}

	void swap(String& x, String& y){
		x.swap(y);
	}

	String operator+(const String& lhs, const String& rhs){
		String res(lhs);
		return res += rhs;
	}

	String operator+(const String& lhs, const char * rhs){
		String res(lhs);
		return res += rhs;
	}

	String operator+(const char* lhs, const String& rhs){
		String res(lhs);
		return res += rhs;
	}

	String operator+(const String& lhs, char rhs){
		String res(lhs);
		return res += rhs;
	}

	String operator+(char lhs, const String& rhs){
		String res(1, lhs);
		return res += rhs;
	}

	bool operator==(const String& lhs, const String& rhs){
		if (lhs.size() == rhs.size()){
			for (auto cit1 = lhs.begin(), cit2 = rhs.begin();
				cit1 != lhs.end() && cit2 != rhs.end();
				++cit1, ++cit2){
				if (*cit1 != *cit2)
					return false;
			}
			return true;
		}
		return false;
	}

	bool operator==(const String& lhs, const char* rhs){
		size_t rlen = strlen(rhs);
		if (lhs.size() == rlen){
			const char *p = rhs;
			for (String::const_iterator cit = lhs.cbegin();
				cit != lhs.cend() && p != rhs + rlen;
				++cit, ++p){
				if (*cit != *p)
					return false;
			}
			return true;
		}
		return false;
	}

	bool operator==(const char* lhs, const String& rhs){
		return rhs == lhs;
	}

	bool operator!=(const String& lhs, const String& rhs){
		return !(lhs == rhs); 
	}

	bool operator!=(const String& lhs, const char* rhs){
		return !(lhs == rhs);
	}

	bool operator!=(const char* lhs, const String& rhs){
		return !(lhs == rhs);
	}

	namespace {
		template<class Iterator1, class Iterator2>
		bool lessEqual_aux(Iterator1 first1, Iterator1 last1,
			Iterator2 first2, Iterator2 last2){
			for (; first1 != last1 && first2 != last2; ++first1, ++first2){
				if (*first1 < *first2)
					return true;
				else if (*first1 > *first2)
					return false;
			}
			if ((first1 == last1 && first2 == last2) || (first1 == last1))
				return true;
			else
				return false;
		}

		template<class Iterator1, class Iterator2>
		bool greaterEqual_aux(Iterator1 first1, Iterator1 last1,
			Iterator2 first2, Iterator2 last2){
			for (; first1 != last1 && first2 != last2; ++first1, ++first2){
				if (*first1 > *first2)
					return true;
				else if (*first1 < *first2)
					return false;
			}
			if ((first1 == last1 && first2 == last2) || first2 == last2)
				return true;
			else
				return false; 
		}
	}

	bool operator<(const String& lhs, const String& rhs){
		return !(lhs >= rhs);
	}

	bool operator<(const String& lhs, const char* rhs){
		return !(lhs >= rhs);
	}

	bool operator<(const char* lhs, const String& rhs){
		return !(lhs >= rhs);
	}

	bool operator<=(const String& lhs, const String& rhs){
		return lessEqual_aux(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
	}

	bool operator<=(const char* lhs, const String& rhs){
		return lessEqual_aux(lhs, lhs + strlen(lhs), rhs.cbegin(), rhs.cend());
	}

	bool operator<=(const String& lhs, const char* rhs){
		return lessEqual_aux(lhs.cbegin(), lhs.cend(), rhs, rhs + strlen(rhs));
	}

	bool operator>(const String& lhs, const String& rhs){
		return !(lhs <= rhs);
	}

	bool operator>(const String& lhs, const char* rhs){
		return !(lhs <= rhs);
	}

	bool operator>(const char *lhs, const String& rhs){
		return !(lhs <= rhs);
	}

	bool operator>=(const String& lhs, const String& rhs){
		return greaterEqual_aux(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
	}

	bool operator>=(const String& lhs, const char* rhs){
		return greaterEqual_aux(lhs.cbegin(), lhs.cend(), rhs, rhs + strlen(rhs));
	}

	bool operator>=(const char* lhs, const String& rhs){
		return greaterEqual_aux(lhs, lhs + strlen(lhs), rhs.cbegin(), rhs.cend());
	}
}