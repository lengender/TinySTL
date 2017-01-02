#ifndef _DEQUE_H_
#define _DEQUE_H_

#include"allocator.h"
#include"iterator.h"
#include"type_traits.h"
#include"utility.h"

namespace TinySTL{
	template<class T, class Alloc = allocator<T>>
	class Deque;
	namespace Detail{
		template<class T>
		class deque_iterator : public iterator < bidirectional_iterator_tag, T > {
		private:
			template<class T, class Alloc>
			friend class TinySTL::Deque;

		private:
			typedef const TinySTL::Deque<T>* cntrPtr;
			size_t mapIndex;
			T *cur;
			cntrPtr container;
		public:
			deque_iterator() :mapIndex(-1), cur(0), container(0){}
			deque_iterator(size_t index, T *ptr, cntrPtr container)
				:mapIndex(index), cur(ptr), container(container){}
			deque_iterator(const deque_iterator& it)
				:mapIndex(it.mapIndex), cur(it.cur), container(it.container){}
			deque_iterator& operator=(const deque_iterator& it);
			void swap(deque_iterator &it);
			reference operator*(){ return *cur; }
			const reference operator*() const{ return *cur; }
			pointer operator->(){ return &(operator*()); }
			const pointer operator->(){ return &(operator*()); }
			deque_iterator& operator++();
			deque_iterator operator++(int);
			deque_iterator& operator--();
			deque_iterator operator--(int);
			bool operator==(const deque_iterator& it) const;
			bool operator!=(const deque_iterator& it) const;
		private:
			T* getBuckTail(size_t mapIndex) const;
			T *getBuckHead(size_t mapIndex) const;
			size_t getBuckSize() const;
		public:
			template<class T>
			friend deque_iterator<T> operator+(const deque_iterator<T>& it,
				typename deque_iterator<T>::difference_type n);
			template<class T>
			friend deque_iterator<T> operator+(typename deque_iterator<T>::difference_type n,
				const deque_iterator<T>& it);
			template<class T>
			friend deque_iterator<T> operator-(const deque_iterator<T>& it,
				typename deque_iterator<T>::difference_type n);
			template<class T>
			friend deque_iterator<T> operator-(typename deque_iterator<T>::difference_type n,
				const deque_iterator<T>& it);
			template<class T>
			friend typename deque_iterator<T>::difference
				operator-(const deque_iterator<T>& it1, const deque_iterator<T>& it2);
			template<class T>
			friend void swap(deque_iterator<T>& lhs, deque_iterator<T>& rhs);
		};
	}
	//class of Deque
	template<class T, class Alloc>
	class Deque{
	private:
		template<class T>
		friend class TinySTL::Detail::deque_iterator;
	public:
		typedef T								value_type;
		typedef TinySTL::Detail::deque_iterator<T>				iterator;
		typedef TinySTL::Detail::deque_iterator<const T>			const_iterator;
		typedef T&								reference;
		typedef const reference					const_reference;
		typedef size_t							size_type;
		typedef ptrdiff_t						difference_type;
		typedef Alloc							allocator_type;
	private:
		typedef Alloc	dataAllocator;
		enum class EBucksSize{ BUCKSIZE = 64 };
	private:
		iterator start, finish;
		size_t mapSize;
		T **map;
	public:
		Deque();
		explicit Deque(size_type n, const value_type& val = value_type());
		template<class InputIterator>
		Deque(InputIterator first, InputIterator last);
		Deque(const Deque& x);
		Deque& operator=(const Deque& x);
	//	Deque& operator=(Deque&& x);
		~Deque();

		iterator begin() { return start; };
		iterator end() { return finish; };
		const_iterator begin() const { return start; };
		const_iterator end() const { return finish; };

	public:
		size_type size() const{ return end() - begin(); }
		bool empty() const { return begin() == end(); }

		reference operator[](size_type n);
		reference front();
		reference back();
		const_reference operator[](size_type n) const;
		const_reference front() const;
		const_reference back() const;

		void push_back(const value_type& val);
		void push_front(const value_type& val);
		void pop_back();
		void pop_front();
		void swap(Deque& x);
		void clear();
	private:
		T *getANewBuck();
		T** getANewMap(const size_t size);
		size_t getNewMapSize(const size_t size);
		size_t getBuckSize() const;
		void init();
		bool back_full() const;
		bool front_full() const;
		void deque_aux(size_t n, const value_type& val, std::true_type);
		template<class Iterator>
		void deque_aux(Iterator first, Iterator last, std::false_type);
		void reallocateAndCopy();
	public:
		template<class T, class Alloc>
		friend bool operator==(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs);
		template<class T, class Alloc>
		friend bool operator!=(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs);
		template<class T, class Alloc>
		friend void swap(Deque<T, Alloc>& x, Deque<T, Alloc>& y);
	};
}

#include"Deque.impl.h"
#endif