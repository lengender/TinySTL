#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

#include"Vector.h"
#include<cassert>

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

	template<class T, class Alloc>
	bool Vector<T, Alloc>::operator==(const Vector& vec) const{
		if (size() != vec.size())
			return false;
		else{
			auto ptr1 = start;
			auto ptr2 = vec.start;
			for (; ptr1 != finish && ptr2 != vec.finish; ++ptr1, ++ptr2){
				if (*ptr1 != *ptr2)
					return false;
			}
			return true;
		}
	}

	template<class T, class Alloc>
	bool Vector<T, Alloc>::operator!=(const Vector& vec) const{
		reurn !(*this == vec);
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

	//修改容器相关动作
	//清空容器，销毁容器中的所有对象并使容器的size为0，但不回收容器已有空间
	template<class T, class Alloc>
	void Vector<T, Alloc>::clear(){
		dataAllocator::destroy(start, finish);
		finish = start;
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::swap(Vector& vec){
		if (this != &vec){
			TinySTL::swap(start, vec.start);
			TinySTL::swap(finish, vec.finish);
			TinySTL::swap(end_of_stroage, vec.end_of_stroage);
		}
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::pop_back(){
		--finish;
		dataAllocator::destroy(finish);
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::push_back(const value_type& val){
		insert(end(), val);
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::insert(iterator pos, const size_type n,
		const value_type& val){
		insert_aux(pos, n, val, typename std::is_integral<size_type>::type());
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void Vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last){
		insert_aux(pos, first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(iterator pos, const value_type& val){
		const auto index = pos - begin();
		insert(pos, 1, val);
		return begin() + index;
	}

	template<class T, class Alloc>
	typename Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(iterator pos){
		erase(pos, pos + 1);
	}

	template<class T, class Alloc>
	typename Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(iterator first, iterator last){
		difference_type len_tail = end() - last;
		difference_type len_remove = last - first;
		finish = finish - len_remove;

		for (; len_tail != 0; --len_tail){
			auto tmp = (last - len_remove);
			*tmp = *(last++);
		}

		return first;
	}

	//friend function
	template<class T, class Alloc>
	bool operator==(const Vector<T, Alloc>& vec1, const Vector<T, Alloc>& vec2){
		return vec1.operator==(vec2);
	}

	template<class T, class Alloc>
	bool operator!=(const Vector<T, Alloc>& vec1, const Vector<T, Alloc>& vec2){
		reurn !(vec1 == vec2);
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
	typename Vector<T, Alloc>::size_type Vector<T, Alloc>::getNewCapacity(size_type len) const{
		size_type oldCap = end_of_stroage - start;
		auto res = TinySTL::max(oldCap, len);
		size_type newCap = (oldCap != 0 ? (oldCap + res) : len);
		return newCap;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void Vector<T, Alloc>::insert_aux(iterator pos, InputIterator first,
		InputIterator last, std::false_type){
		difference_type locationLeft = end_of_stroage - finish;
		difference_type locationNeed;
		distance(first, last, locationNeed);

		if (locationLeft >= locationNeed){
			if (finish - pos > locationNeed){
				TinySTL::uninitialized_copy(finish - locationNeed, finish, finish);
				std::copy_backward(pos, finish - locationNeed, finish);
				std::copy(first, last, pos);
			}
			else{
				iterator tmp = TinySTL::uninitialized_copy(first + (finish - pos), last, finish);
				TinySTL::uninitialized_copy(pos, finish, tmp);
				std::copy(first, first + (finish - pos), pos);
			}
			finish += locationNeed;
		}
		else{
			reallocate_and_copy(pos, first, last);
		}
	}

	template<class T, class Alloc>
	template<class Integer>
	void Vector<T,Alloc>::insert_aux(iterator pos, Integer n, 
		const value_type& val, std::true_type){
		assert(n != 0);

		difference_type locLeft = end_of_stroage - finish;
		difference_type locNeed = n;

		if (locLeft >= locNeed){
			auto tmpPtr = end() - 1;
			for (; tmpPtr - pos >= 0; --tmpPtr){
				construct(tmpPtr + locNeed, *tmpPtr);
			}

			TinySTL::uninitialized_fill_n(pos, n, val);
			finish += locNeed;
		}
		else{
			reallocate_and_fill_n(pos, n, val);
		}
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void Vector<T, Alloc>::reallocate_and_copy(iterator pos, InputIterator first, InputIterator last){
		difference_type newCap = getNewCapacity(last - first);

		T *newStart = dataAllocator::allocate(newCap);
		T *newFinish = TinySTL::uninitialized_copy(begin(), pos, newStart);
		newFinish = TinySTL::uninitialized_copy(first, last, newFinish);
		newFinish = TinySTL::uninitialized_copy(pos, end(), newFinish);

		destroy_and_deallocate();
		start = newStart;
		finish = newFinish;
		end_of_stroage = start + newCap;
	}

	template<class T, class Alloc>
	void Vector<T, Alloc>::reallocate_and_fill_n(iterator pos, const size_type n, const value_type &val){
		difference_type newCap = getNewCapacity(n);

		T *newStart = dataAllocator::allocate(newCap);
		T *newFinish = TinySTL::uninitialized_copy(begin(), pos, newStart);
		newFinish = TinySTL::uninitialized_fill_n(newFinish, n, val);
		newFinish = TinySTL::uninitialized_copy(pos, end(), newFinish);

		destroy_and_deallocate();
		start = newStart;
		finish = newFinish;
		end_of_stroage = start + newCap;
	}
}

#endif