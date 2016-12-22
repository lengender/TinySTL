#ifndef _UNINITIALIZEDFUNCTIONS_H_
#define _UNINITIALIZEDFUNCTIONS_H_

#include"iterator.h"
#include"type_traits.h"
#include<string.h>

namespace TinySTL{

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type){
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type){
		ForwardIterator cur = result;

		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);   //必须一个一个构造元素，无法批量进行

		return cur;
	}

	template<class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result, T*){
		typedef typename _type_traits<T>::is_POD_type is_POD;
		return _uninitialized_copy_aux(first, last, result, is_POD());
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result){
		return _uninitialized_copy(first, last, result, value_type(result));
	}

	//以下是针对const char*的特化版本
	inline char* uninitialized_copy(const char* first, const char* last, char *result){
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//以下是针对const wchar_t*的特化版本
	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t*result){
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator fill_n(ForwardIterator first, Size n, T val){
		assert(first != 0);

		while (n--)
			*first++ = val;

		return first;
	}

	template<class ForwardIterator, class T>
	inline ForwardIterator fill(ForwardIterator first, ForwardIterator last,
		const T& val){
		assert(first != 0 && last != 0);

		while (first != last)
			*first++ = val;

		return first;
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, _true_type){
		return fill_n(first, n, x);
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, _false_type){
		ForwardIterator cur = first;

		for (; n > 0; --n, ++cur)
			construct(&*cur, x);

		return cur;
	}

	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n,
		const T& x, T1*){
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n,
		const T& x){

		return _uninitialized_fill_n(first, n, x, value_type(first));
	}

	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, _true_type){
		return fill(first, last, x);
	}

	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, _false_type){
		ForwardIterator cur = first;

		for (; cur != last; ++cur)
			construct(&*cur, x);

	}

	template<class ForwardIterator, class T, class T1>
	void _uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x, T1 *){
		typedef typename _type_traits::is_POD_type is_POD;
		_uninitialized_fill_aux(first, last, x, is_POD());
	}

	template<class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x){
		_uninitialized_fill_aux(first, last, x, value_type(first));
	}
}
#endif