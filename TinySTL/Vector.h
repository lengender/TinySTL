#ifndef _VECTOR_H_
#define _VECTOR_H_

#include"allocator.h"
#include"iterator.h"
#include"reverseIterator.h"
#include"type_traits.h"
#include"uninitializedFunctions.h"
#include"utility.h"
#include"algorithm.h"
#include<cstddef>

namespace TinySTL{
	template<class T, class Alloc = allocator<T>>
	class Vector{
	private:
		T* start;
		T* finish;
		T* end_of_stroage;

		typedef Alloc dataAllocator;
	public:
		typedef T										value_type;
		typedef T*										iterator;
		typedef const T*								const_iterator;
		typedef reverse_iterator_t<T*>					reverse_iterator;
		typedef reverse_iterator_t<const T*>			const_reverse_iterator;
		typedef T&										reference;
		typedef iterator								pointer;
		typedef const T&								const_reference;
		typedef size_t									size_type;
		typedef ptrdiff_t								difference_type;
	public:
		//ctor,copy,dtor
		Vector() :statr(0), finish(0), end_of_stroage(0){}
		explicit Vector(const size_type n);
		Vector(const size_type n, const value_type& value);
		template<class InputIterator>
		Vector(InputIterator first, InputIterator last);
		Vector(const Vector& vec);
		Vector(Vector&& vec);
		Vector& operator=(const Vector& vec);
		Vector& operator=(Vector&& vec);
		~Vector();
	public:
		//比较操作相关
		bool operator==(const Vector& vec) const;
		bool operator!=(const Vector& vec) const;

		//容器相关
		difference_type capacity(){ return end_of_stroage - start; }
		difference_type size() { return finish - start; }
		bool empty() const { return start == finish; }
		void resize(size_type n, value_type value = value_type());
		void reserve(size_type n);
		void shrink_to_fit();

		//迭代器相关
		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		const_iterator cbegin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		const_iterator cend() const{ return finish; }
		reverse_iterator rbegin() { return reverse_iterator_t(finish); }
		const_reverse_iterator crbegin() const{ return reverse_iterator_t(finish); }
		reverse_iterator rend() { return reverse_iterator_t(start); }
		const_reverse_iterator crend() const{ return reverse_iterator_t(start); }

		//访问元素相关
		reference operator[](const difference_type i) { return *(begin() + i); }
		const_reference operator[](const difference_type i) const{ return *(cbegin() + i); }
		reference front(){ return *(begin()); }
		reference back(){ return *(end() - 1); }
		pointer data() { return start; }

		//修改容器相关动作
		//清空容器，销毁容器中的所有对象并使容器的size为0，但不回收容器已有空间
		void clear();
		void swap(Vector& vec);
		void push_back(const value_type& val);
		void pop_back();
		iterator insert(iterator pos, const value_type& val);
		void insert(iterator pos, const size_type n, const value_type& val);
		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);

		//容器的空间配置器相关
		Alloc get_allocate(){ return dataAllocator; }
	private:
		void destroy_and_deallocate();
		void allocate_and_fill(size_type n, const T& x);
		template<class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);
		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void vector_aux(Integer n, const value_type& value, std::true_type);
		size_type getNewCapacity(size_type len) const;
		template<class InputIterator>
		void insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void insert_aux(iterator pos, Integer n, const value_type& value, std::true_type);
		template<class InputIterator>
		void reallocate_and_copy(iterator pos, InputIterator first, InputIterator last);
		void reallocate_and_fill_n(iterator pos, const size_type n, const value_type &val);
	public:
		template<class T, class Alloc>
		friend bool operator==(const Vector<T, Alloc>& vec1, const Vector<T, Alloc>& vec2);
		template<class T, class Alloc>
		friend bool operator!=(const Vector<T, Alloc>& vec1, const Vector<T, Alloc>& vec2);
	};
}

#include"Vector.impl.h"
#endif