#include"Vector.h"

namespace TinySTL{
	template<class T, class Alloc>
	Vector<T, Alloc>::Vector(const size_type n)
		:start(0), finish(0), end_of_storage(0){
		allocate_and_fill(n, value_type());
	}

	template<class T, class Alloc>
	Vector<T, Alloc>::Vector(const size_type n, const value_type& value)
		: start(0), finish(0), end_of_stroage(0){
		allocate_and_fill(n, value);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	Vector<T, Alloc>::Vector(InputIterator first, InputIterator last){
		//处理指针和数字间的区别的函数
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	Vector<T, Alloc>::Vector(const Vector& vec){
		allocate_and_copy(vec.start, vec.finish);
	}

	template<class T, class Alloc>
	Vector<T, Alloc>::Vector(Vector&& vec){
		start = vec.start;
		finish = vec.finish;
		end_of_stroage = vec.end_of_stroage;
		vec.start = vec.finish = vec.end_of_stroage = 0;
	}

	template<class T, class Alloc>
	Vector<T, Alloc>& Vector<T, Alloc>::operator=(const Vector& vec){
		if (this != &vec){
			allocate_and_copy(vec.start, vec.finish);
		}
		return *this;
	}

	template<class T, class Alloc>
	Vector<T, Alloc>& Vector<T, Alloc>::operator=(Vector&& vec){
		if (this != &vec){
			destroy_and_deallocate();
			start = vec.start;
			finish = vec.finish;
			end_of_stroage = vec.end_of_stroage;
			vec.start = vec.finish = vec.end_of_stroage = 0;
		}
		return *this;
	}

	template<class T, class Alloc>
	Vector<T, Alloc>::~Vector(){
		destroy_and_deallocate();
	}

	//容器相关
	template<class T, class Alloc>
	void Vector<T, Alloc>::resize(size_type n, value_type val){
		if (n < size()){
			dataAllocator::destroy(start + n, finish);
			finish = start + n;
		}
		else if (n > size() && n <= capacity()){
			auto len = n - size();
			finish = TinySTL::uninitialized_fill_n(finish, len, val);
		}
		else if (n > capacity()){
			auto len = n - size();
			T *newStart = dataAllocator::allocate(getNewCapacity(len));
			T *newFinish = TinySTL::uninitialized_copy(begin(), end(), newStart);
			newFinish = TinySTL::uninitialized_fill_n(newFinish, len, val);

			destroy_and_deallocate();
			start = newStart;
			finish = newFinish;
			end_of_stroage = start + n;
		}
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::reserve(size_type n){
		if (n <= capacity())
			return;

		T* newStart = dataAllocator::allocate(n);
		T* newFinish = TinySTL::uninitialized_copy(begin(), end(), newStart);
		destroy_and_deallocate(); 

		start = newStart;
		finish = newFinish;
		end_of_stroage = start + n;
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::shrink_to_fit(){
		T *t = (T*)dataAllocator::allocate(size());
		finish = TinySTL::uninitialized_copy(start, finish, t);
		dataAllocator::deallocate(start, capacity());
		start = t;
		end_of_stroage = finish;
	}
	//helper function
	template<class T, class Alloc>
	void Vector<T, Alloc>::destroy_and_deallocate(){
		if (capacity() != 0){
			dataAllocator::destroy(start, finish);
			dataAllocator::deallocate(start, capacity());
		}
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::allocate_and_fill(const size_type n, const value_type& value){
		start = dataAllocator::allocate(n);
		TinySTL::uninitialized_fill_n(start, n, value);
		finish = end_of_stroage = start + n;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void Vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last){
		start = dataAllocator::allocate(last - first);
		finish = TinySTL::uninitialized_copy(first, last, start);
		end_of_stroage = finish;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void Vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type){
		allocate_and_copy(first, last);
	}

	template<class T, class Alloc>
	template<class Integer>
	void Vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type){
		allocate_and_fill(n, value);
	}

	template<class T, class Alloc>
	Vector<T,Alloc>::size_type Vector<T, Alloc>::getNewCapacity(size_type len) const{
		size_type oldCap = end_of_stroage - start;
		auto res = TinySTL::max(oldCap, len);
		size_type newCap = (oldCap != 0 ? (oldCap + res) : len);
		return newCap;
	}
}