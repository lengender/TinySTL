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
		typedef reverse_iteartor_t<const T*>			const_reverse_iterator;
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
		//ÈÝÆ÷Ïà¹Ø
		difference_type capacity(){ return end_of_stroage - start; }
		difference_type size() { return finish - start; }
		bool empty() const { return start == finish; }
		void resize(size_type n, value_type value = value_type());
		void reserve(size_type n);
		void shrink_to_fit();
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
	};
}


#endif