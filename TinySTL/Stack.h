#ifndef _STACK_H_
#define _STACK_H_

#include"Deque.h"

namespace TinySTL{
	template<class T, class Container = TinySTL::Deque<T>>
	class Stack{
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::reference reference;
		typedef typename Container::size_type size_type;
		typedef Container container_type;
	private:
		container_type container;
	public:
		explicit Stack(const container_type& ctnr = container_type())
			:container(ctnr){}

		bool empty() const{ return container.empty(); }
		size_type size() const{ return container.size(); }
		reference top() { return container.back(); }
		const_reference top() const{ return container.back(); }
		void push(const value_type& x) { container.push_back(val); }
		void pop() { container.pop_back(); }
		void swap(Stack& x){ TinySTL::swap(container, x.container); }
	public:
		template<class T, class Container>
		friend bool operator==(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);
		template<class T, class Container>
		friend bool operator!=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);
		template<class T, class Container>
		friend void swap(Stack<T, Container>& x, Stack<T, Container>& y);
	};

	template<class T, class Container>
	bool operator==(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs){
		return lhs.container == rhs.container;
	}

	template<class T, class Container>
	bool operator!=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs){
		return lhs.container != rhs.container;
	}

	template<class T, class Container>
	void swap(Stack<T, Container>& x, Stack<T, Container>& x){
		x.swap(y);
	}
}
#endif