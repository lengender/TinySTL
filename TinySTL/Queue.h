#ifndef _QUEUE_H_
#define _QUEUE_H_

#include"Vector.h"
#include"Deque.h"
#include"algorithm.h"
#include"Functional.h"

namespace TinySTL{
	//class of Queue
	template<class T, class Container = TinySTL::Deque<T>>
	class Queue{
	public:
		typedef T value_type;
		typedef Container container_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef typename Container::size_type size_type;
	private:
		container_type c;
	public:
		bool empty() const{ return c.empty(); }
		size_type size() const{ return c.size(); }
		reference front()	{ return c.front(); }
		const_reference front() { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() { return c.back(); }
		void push(const value_type &x){ c.push_back(x); }
		void pop() { c.pop_front(); }
	public:
		template<class T, class Container>
		friend bool operator==(const Queue<T, Container>& lhs, const Queue<T, Container>& rhs);
		template<class T, class Container>
		friend bool operator!=(const Queue<T, Container>& lhs, const Queue<T, Container>& rhs);
		template<class T, class Container>
		friend void swap(Queue<T, Container>& x, Queue<T, Container>& y);
	};

	template<class T, class Container>
	bool operator==(const Queue<T, Container>& lhs, const Queue<T, Container>& rhs){
		return lhs.c == rhs.c;
	}

	template<class T, class Container>
	bool operator!=(const Queue<T, Container>& lhs, const Queue<T, Container>& rhs){
		return lhs.c != rhs.c; 
	}

	template<class T, class Container>
	void swap(Queue<T, Container>& x, Queue<T, Container>& y){
		TinySTL::swap(x.c, y.c);
	}


	//class of priority_queue
	template<class T, class Container = TinySTL::Vector<T>,
		class Compare = TinySTL::less<typename Container::value_type>>
		class priority_queue{
		public:
			typedef T value_type;
			typedef Container container_type;
			typedef typename Container::reference reference;
			typedef typename Container::const_reference const_reference;
			typedef typename Container::size_type size_type;
		private:
			container_type container;
			Compare comp;
		public:
			explicit priority_queue(const Compare& comp = Compare(),
				const Container & cntr = Container())
				:container(cntr), comp(comp){}
			template<class InputIterator>
			priority_queue(InputIterator first, InputIterator last,
				const Compare &comp = Compare(),
				const Container &cntr = Container())
				: container(cntr) comp(comp){
				container.insert(container.end(), first, last);
				TinySTL::make_heap(container.begin(), container.end());
			}

			bool empty() const{ return container.empty(); }
			size_type size() const{ return container.size(); }
			reference top() { return container.front(); }
			void push(const value_type& val){
				container.push_back(val);
				TinySTL::push_heap(container.begin(), container.end(), comp);
			}

			void pop(){
				TinySTL::pop_heap(container.begin(), container.end(), comp);
				container.pop_back();
			}

			void swap(priority_queue& x){
				TinySTL::swap(container, x.container);
				TinySTL::swap(comp, x.comp);
			}
		public:
			template<class T, class Container, class Compare>
			friend void swap(priority_queue<T, Container, Compare>& x,
				priority_queue<T, Container, Compare>& y);
	};

		template<class T, class Container, class Compare>
		void swap(priority_queue<T, Container, Compare>& x,
			priority_queue<T, Container, Compare>& y){
			x.swap(y);
		}
}
#endif