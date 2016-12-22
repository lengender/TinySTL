#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include<cstddef>
/*
	STL�ؼ����ڣ����������˼ά��������������containers�����㷨��algorithm���ֿ����˴˶�����
				�������һ�����ż������Ǵ����һ��

	��������һ������ָ��Ķ��󣬶�ָ��ĸ�����Ϊ�����Ҳ������Ҫ�ı�����������(dereference)��
	��Ա����(member access),��ˣ�����������Ҫ�ı�̹����Ƕ�operator*��operator->�������ع�����

	����������Ӧ�ͱ𣬸��ݾ��飬��õ���Ӧ�ͱ������֣�
	1��value type : ��������ָ������ͱ�
	2��difference type �� ��ʾ����������֮��ľ��룬�����Ҳ����������ʾһ�����׵������������Ϊ
						�������ռ���������ԣ�ͷβֱ�ӵ�������������������
	3��reference type : ���Ҫ������ֵ��������by reference�ķ�ʽ����
	4��pointer type : �����ܹ�����һ��pointer,ָ�����������ָ֮��
	5��iterator_category : ����������Ӧ�ͱ�
*/

namespace TinySTL{

	//���ֵ�����
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

	//Ϊ������һ©�������п����ı�������ü̳��������std::iterator
	template<class Category, class T, class Distance = ptrdiff_t,
	class Pointer = T*, class Reference = T&>
	struct iterator{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	differnce_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	//ե֭�� traits
	template<class Iterator>
	struct iterator_traits{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		
	};

	//���ԭ��ָ��(native pointer)����Ƶ�traitsƫ�ػ���
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

	//�жϵ���������
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&){
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//��ȡ��������distance_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type *
		distance_type(const Iterator&){
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//��ȡĳ����������value type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type *
		value_type(const Iterator&){
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	template <class InputIterator, class Distance>
	inline void __distance(InputIterator first, InputIterator last, Distance& n,
		input_iterator_tag) {
		while (first != last) { ++first; ++n; }
	}

	template <class RandomAccessIterator, class Distance>
	inline void __distance(RandomAccessIterator first, RandomAccessIterator last,
		Distance& n, random_access_iterator_tag) {
		n += last - first;
	}

	template <class InputIterator, class Distance>
	inline void distance(InputIterator first, InputIterator last, Distance& n) {
		__distance(first, last, n, iterator_category(first));
	}
}
//
#endif