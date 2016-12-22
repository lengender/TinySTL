#ifndef _COSNTRUCT_H_
#define _CONSTRUCT_H_

#include"type_traits.h"
#include<new>

namespace TinySTL
{
	template<class T1, class T2>
	inline void construct(T1 *ptr, const T2 &value)
	{
		new (ptr) T1(value);  //placement new, 调用T1::T1(value)
	}

	template<class T>
	inline void destroy(T* ptr)
	{
		ptr->~T();  //调用dtor ~T()
	}

	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type)
	{}

	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type)
	{
		for (; first < last; ++first)
			destroy(&*first);
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
		_destroy(first, last, is_POD_type());
	}
}
#endif