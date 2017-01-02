#ifndef _LIST_H_
#define _LIST_H_

#include"allocator.h"
#include"iterator.h"
#include"type_traits.h"
#include"utility.h"

namespace TinySTL{
	template<class T>
	class List;

	namespace Detail{
		//the class of node
		template<class T>
		struct _list_node{
			T data;
			_list_node* prev;
			_list_node* next;
			List<T> *container;
			_list_node(const T& d, node *p, node *n, List<T>* c)
				:data(d), prev(p), next(n), container(c){}
			bool operator==(const _list_node& n){
				return data == n.data && prev == n.prev && 
					next == n.next && container == n.container;
			}
		};

		//the class of list iterator
		template<class T>
		class ListIterator : public iterator < bidirectional_iterator_tag, T>{
		public:
			template<class T> friend class List;
			typedef _list_node<T>* nodePtr;
			nodePtr p;
		public:
			explicit ListIterator(nodePtr ptr = nullptr) :p(ptr){}

			ListIterator& operator++();
			ListIteartor& operator++(int);
			ListIterator& operator--();
			ListIterator& operator--(int);
			T& operator*(){ return p->data; }
			T* operator->(){ return &(operator*()); }

			template<class T>
			friend bool operator==(const ListIterator<T>& lhs, const ListIterator<T>& rhs); 
			template<class T>
			friend bool operator!=(const ListIterator<T>& lhs, const ListIterator<T>& rhs);
		};
	}//end of Detail namepace

	template<class T>
	class List{
		template<class T>
		friend class ListIterator;
	public:
		typedef allocator<Detail::_list_node<T>> nodeAllocator;
		typedef Detail::_list_node<T>* nodePtr;
	public:
		typedef T								value_type;
		typedef Detail::ListIterator<T>			iterator;
		typedef Detail::ListIterator<const T>	const_iterator;
		typedef reverse_iterator_t<iterator>	reverse_iterator;
		typedef T&								reference;
		typedef size_t							size_type;
	private:
		iterator head;
		iterator tail;
	public:
		List();
		explicit List(size_type n, const value_type& val = value_type());
		template<class InputIterator>
		List(InputIterator first, InputIterator last);
		List(const List& l);
		List& operator=(const List& l);
		~List();

		bool empty() const { return head == tail; }
		size_type size() const;
		reference front() { return (head.p->data); }
		reference back() { return (tail.p->prev->data); }

		void push_front(const value_type& val);
		void pop_front();
		void push_back(const value_type& val);
		void pop_back();
		
		iterator begin(){ return head; };
		iterator end() { return tail; };
		const_iterator begin() const;
		const_iterator end() const;
		reverse_iterator rbegin() { return reverse_iterator(tail); };
		reverse_iterator rend() { return reverse_iterator(head); };

		iterator insert(iterator pos, const value_type& val);
		void insert(iterator pos, size_type n, const value_type& val);
		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		void swap(List& x);
		void clear();
		void splice(iterator pos, List& x);
		void splice(iterator pos, List& x, iterator i);
		void splice(iterator pos, List& x, iterator first, iterator last);
		void remove(const value_type& val);
		template<class Predicate>
		void remove_if(Predicate pred);
		void unique();
		template<class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(List& x);
		template<class Compare>
		void merge(List& x, Compare comp);
		void sort();
		template<class Compare>
		void sort(Compare comp);
	private:
		nodePtr newNode(const T& val = T());
		void ctorAux(size_type n, const value_type &val, std::true_type);
		template<class InputIterator>
		void ctorAux(InputIterator first, InputIterator last, std::false_type);
		void deleteNode(nodePtr p);
		const_iterator changeIteratorToConstIterator(iterator& it) const; 
		void insert_aux(iterator pos, size_type n, const T& val, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type);
	public:
		template<class T>
		friend void swap(List<T>& x, List<T>& y);
		template<class T>
		friend bool operator==(const List<T>& lhs, const List<T>& rhs);
		template<class T>
		friend bool operator!=(const List<T>& lhs, const List<T>& rhs);
	};
}
#include"List.impl.h"
#endif