#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include<cstddef>
/*
	STL关键所在：迭代器设计思维，将数据容器（containers）和算法（algorithm）分开，彼此独立，
				最后再以一帖胶着剂将它们撮合在一起。

	迭代器是一种类似指针的对象，而指针的各种行为中最常见也是最重要的便是内容提领(dereference)和
	成员访问(member access),因此，迭代器最重要的编程工作是对operator*和operator->进行重载工作。

	迭代器的相应型别，根据经验，最常用的相应型别由五种：
	1、value type : 迭代器所指对象的型别
	2、difference type ： 表示两个迭代器之间的距离，因此它也可以用来表示一个容易的最大容量，因为
						对连续空间的容器而言，头尾直接的最大距离就是其最大容量
	3、reference type : 如果要传回左值，都是以by reference的方式进行
	4、pointer type : 我们能够传回一个pointer,指向迭代器的所指之物
	5、iterator_category : 迭代器的相应型别
*/

namespace TinySTL{

	//五种迭代器
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag : public input_iterator_tag{};
	struct bidirectional_iterator_tag : public forward_iterator_tag{};
	struct random_access_iterator_tag : public bidirectional_iterator_tag{};

	template<class T, class Distance>
	struct input_iterator{
		typedef input_iterator_tag		iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};

	template<class T, class Distance>
	struct output_iterator{
		typedef output_iterator_tag		iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};

	template<class T, class Distance>
	struct forward_iterator{
		typedef forward_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};

	template<class T, class Distance>
	struct bidirectional_iterator{
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<class T, class Distance>
	struct random_access_iterator{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//为避免万一漏掉，自行开发的编译器最好继承下面这个std::iterator
	template<class Category, class T, class Distance = ptrdiff_t,
	class Pointer = T*, class Reference = T&>
	struct iterator{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	differnce_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	//榨汁机 traits
	template<class Iterator>
	struct iterator_traits{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		
	};

	//针对原生指针(native pointer)而设计的traits偏特化版
	template<class T>
	struct iterator_traits < T* > {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<class T>
	struct iterator_traits < const T* > {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef const T*					pointer;
		typedef const T&					reference;
	};

	//判断迭代器类型
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&){
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//获取迭代器的distance_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type *
		distance_type(const Iterator&){
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//获取某个迭代器的value type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type *
		value_type(const Iterator&){
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type 
		__distance(InputIterator first,
		InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last) { ++first; ++n; }
		return n;
	}

	template <class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator first, RandomAccessIterator last,
		random_access_iterator_tag) {
		return last - first;
	}

	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {
		return __distance(first, last, iterator_category(first));
	}

	//advance函数
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag){
		while (n--) ++i;
	}

	template<class BidirectionalIterator, class Distance>
	inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag){
		if (n >= 0)
			while (n--) ++i;
		else
			while (n++) --i;
	}

	template<class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag){
		i += n;
	}

	template<class InputIterator, class Distance>
	inline void advance(InputIterator &i, Distance n){
		__advance(i, n, iterator_category(i));
	}
}
//
#endif