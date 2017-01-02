#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_

namespace TinySTL{
	namespace Detail{
		template<class T>
		ListIterator<T>& ListIterator<T>::operator++(){
			p = p->next;
			return *this;
		}

		template<class T>
		ListIterator<T>& ListIterator<T>::operator++(int){
			auto res = *this;
			++*this;
			return res;
		}

		template<class T>
		ListIterator<T>& ListIterator<T>::operator--(){
			p = p->prev;
			return *this;
		}

		template<class T>
		ListIterator<T>& ListIterator<T>::operator--(int){
			auto res = *this;
			--*this;
			return res;
		}

		template<class T>
		bool operator==(const ListIterator<T>& lhs, const ListIterator<T>& rhs){
			return lhs.p == rhs.p;
		}

		template<class T>
		bool operator!=(const ListIterator<T>& lhs, const ListIterator<T>& rhs){
			return !(lhs == rhs);
		}
	}// end of Detail namespace

	//ctor
	template<class T>
	List<T>::List(){
		head.p = newNode();
		tail.p = head.p;
	}

	template<class T>
	List<T>::List(size_type n, const value_type& val = value_type()){
		ctorAux(n, val, std::is_integral<value_type>());
	}

	template<class T>
	template<class InputIterator>
	List<T>::List(InputIterator first, InputIterator last){
		ctorAux(first, last, std::is_integral<InputIterator>());
	}

	template<class T>
	List<T>::List(const List& l){
		head.p = newNode();
		tail.p = head.p;
		for (auto node = l.head.p; node != l.tail.p; node = node->next){
			push_back(node->data);
		}
	}

	template<class T>
	List<T>& List<T>::operator=(const List& l){
		if (this != &l){
			List(l).swap(*this);
		}
		return *this;
	}

	template<class T>
	List <T>::~List(){
		for (; head != tail;){
			auto tmp = head++;
			nodeAllocator::destroy(tmp.p);
			nodeAllocator::deallocate(tmp.p);
		}
		nodeAllocator::deallocate(tail.p);
	}

	template<class T>
	typename List<T>::size_type List<T>::size() const{
		size_type length = 0;
		for (auto h = head; h != tail; ++h)
			++length;

		return length;
	}

	template<class T>
	void List<T>::push_front(const value_type& val){
		auto node = newNode(val);
		head.p->prev = node;
		node->next = head.p;
		head.p = node;
	}

	template<class T>
	void List<T>::pop_front(){
		auto oldNode = head.p;
		head.p = oldNode->next;
		head.p->prev = nullptr;
		deleteNode(oldNode);
	}

	template<class T>
	void List<T>::push_back(const value_type){
		auto node = newNode();
		(tail.p)->data = val;
		(tail.p)->next = node;
		node.prev = tail.p;
		tail.p = node;
	}

	template<class T>
	void List<T>::pop_back(){
		auto newTail = tail.p->prev;
		newTail->next = nullptr;
		deleteNode(tail.p);
		tail.p = newTail;
	}

	template<class T>
	typename List<T>::const_iterator List<T>::begin() const{
		auto tmp = (List* const) this;
		return changeIteratorToConstIterator(tmp->head);
	}

	template<class T>
	typename List<T>::const_iterator List<T>::end() const{
		auto tmp = (List* const) this;
		return changeIteratorToConstIterator(tmp->tail);
	}

	template<class T>
	typename List<T>::iterator List<T>::insert(iterator pos, const value_type& val){
		if (pos == begin()){
			push_front(val);
			return begin();
		}
		else if (pos == end()){
			auto ret = pos;
			push_back(val);
			return ret;
		}

		auto node = newNode(val);
		auto prev = pos.p->prev;
		node->next = pos.p;
		node->prev = prev;
		prev->next = node;
		pos.p->prev = node;
		return iterator(node);
	}

	template<class T>
	void List<T>::insert(iterator pos, size_type n, const value_type& val){
		insert_aux(pos, n, val, std::is_integral<value_type>());
	}

	template<class T>
	template<class InputIterator>
	void List<T>::insert(iterator pos, InputIterator first, InputIterator last){
		insert_aux(pos, first, last, std::is_integral<InputIterator>());
	}

	template<class T>
	typename List<T>::iterator List<T>::erase(iterator pos){
		if (pos == head){
			pop_front();
			return head;
		}
		else{
			auto prev = pos.p->prev;
			prev->next = pos.p->next;
			pos.p->next->prev = prev;
			deleteNode(pos.p);
			return iterator(prev->next);
		}
	}

	template<class T>
	typename List<T>::iterator List<T>::erase(iterator first, iterator last){
		typename List<T>::iterator res;
		for (; first != last;){
			auto tmp = first++;
			res = erase(tmp);
		}
		return res;
	}

	template<class T>
	void List<T>::clear(){
		erase(begin(), end());
	}

	template<class T>
	void List<T>::swap(List& x){
		TinySTL::swap(head.p, x.head.p);
		TinySTL::swap(tail.p, x.tail.p);
	}

	template<class T>
	void List<T>::splice(iterator pos, List& x){
		this->insert(pos, x.begin(), x.end());
		x.head.p = x.tail.p;
	}

	template<class T>
	void List<T>::splice(iterator pos, List& x, iterator first, iterator last){
		if (first.p == last.p) return;
		auto tailNode = last.p->prev;
		if (x.head.p == first.p){
			x.head.p = last.p;
			x.head.p->prev = nullptr;
		}
		else{
			first.p->prev->next = last.p;
			last.p->prev = first.p->prev;
		}

		if (pos.p == head.p){
			first.p->prev = nullptr;
			tailNode->next = head.p;
			head.p->prev = tailNode;
			head.p = first.p;
		}
		else{
			pos.p->prev->next = first.p;
			first.p->prev = pos.p->prev;
			tailNode->next = pos.p;
			pos.p->prev = tailNode;
		}
	}

	template<class T>
	void List<T>::splice(iterator pos, List& x, iterator i){
		auto next = i;
		this->splice(pos, x, i, ++next);
	}

	template<class T>
	void List<T>::remove(const value_type& val){
		for (auto it = begin(); it != end();){
			if (*it == val)
				it = erase(it);
			else
				++it;
		}
	}

	template<class T>
	template<class Predicate>
	void List<T>::remove_if(Predicate pred){
		for (auto it = begin(); it != end();){
			if (pred(*it))
				it = erase(it);
			else
				++it;
		}
	}

	template<class T>
	void List<T>::unique(){
		nodePtr curNode = head.p;
		while (curNode != tail.p){
			nodePtr nextNode = curNode->next;
			if (curNode->data == nextNode->data){
				if (nextNode == tail.p){
					curNode->next = nullptr;
					tail.p = curNode;
				}
				else{
					curNode->next = nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
			}
			else{
				curNode = nextNode;
			}
		}
	}

	template<class T>
	template<class BinaryPredicate>
	void List<T>::unique(BinaryPredicate binary_pred){
		nodePtr curNode = head.p;
		while (curNode != tail.p){
			nodePtr nextNode = curNode->next;
			if (binary_pred(curNode->data, nextNode->data)){
				if (nextNode == tail.p){
					curNode->next = nullptr;
					tail.p = curNode;
				}
				else{
					curNode->next = nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
			}
			else{
				curNode = nextNode;
			}
		}
	}

	template<class T>
	void List<T>::merge(List& x){
		auto it1 = begin(), it2 = x.begin();
		while (it1 != end() && it2 != x.end()){
			if (*it1 <= *it2)
				++it1;
			else{
				auto tmp = it2++;
				this->splice(it1, x, tmp);
			}
		}
		if (it1 == end()){
			this->splice(it1, x, it2, x.end());
		}
	}

	template<class T>
	template<class Compare>
	void List<T>::merge(List& x, Compare comp){
		auto it1 = begin(), it2 = x.begin();
		while (it1 != end() && it2 != x.end()){
			if (comp(*it2, *it1)){
				auto tmp = it2++;
				this->splice(it1, x, tmp);
			}
			else{
				++it1;
			}
		}
		if (it1 == end()){
			this->splice(it1, x, it2, x.end());
		}
	}

	template<class T>
	template<class Compare>
	void List<T>::sort(Compare comp){
		if (empty() || head.p->next == tail.p)
			return;

		List carry;
		List counter[64];
		int fill = 0;
		while (!empty()){
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()){
				counter[i].merge(carry, comp);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
				++fill;
		}
		for (int i = 0; i != fill; ++i){
			counter[i].merge(counter[i - 1], comp);
		}
		swap(counter[fill - 1]);
	}

	template<class T>
	void List<T>::sort(){
		sort(TinySTL::less<T>());
	}
	//helper function
	template<class T>
	typename List<T>::nodePtr List<T>::newNode(const T& val = T()){
		nodePtr res = nodeAllocator::allocate();
		nodeAllocator::construct(res, Detail::_list_node<T>(val, nullptr, nullptr, this));
		return res;
	}

	template<class T>
	void List<T>::ctorAux(size_type n, const value_type& val, std::true_type){
		head.p = newNode();
		tail.p = head.p;
		while (n--)
			push_back(val);
	}

	template<class T>
	template<class InputIterator>
	void List<T>::ctorAux(InputIterator first, InputIterator last, std::false_type){
		head.p = newNode();
		tail.p = head.p;
		for (; first != last; ++first){
			push_back(*first);
		}
	}

	template<class T>
	void List<T>::deleteNode(nodePtr p){
		p->prev = p->next = nullptr;
		nodeAllocator::destroy(p);
		nodeAllocator::deallocate(p);
	}

	template<class T>
	typename List<T>::const_iterator List<T>::changeIteratorToConstIterator(iterator& it) const{
		using nodeP = Detail::_list_node<const T>*;
		auto tmp = (List<const T*> const)this;
		auto ptr = it.p;
		Detail::_list_node<const T> node(ptr->data, (nodeP)(ptr->prev), (nodeP)(ptr->next), tmp);
		return const_iterator(&node);
	}

	template<class T>
	void List<T>::insert_aux(iterator pos, size_type n, const value_type& val, std::true_type){
		for (auto i = n; i != 0; --i)
			pos = insert(pos, val);
	}

	template<class T>
	template<class InputIterator>
	void List<T>::insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type){
		for (--last; first != last; --last)
			pos = insert(pos, *last);

		insert(pos, *last);
	}

	//friend function
	template<class T>
	void swap(List<T>& x, List<T>& y){
		x.swap(y);
	}

	template<class T>
	bool operator==(const List<T>& lhs, const List<T>& rhs){
		auto node1 = lhs.head.p, node2 = rhs.head.p;
		for (; node1 != lhs.tail.p && node2 != rhs.tail.p;
			node1 = node1->next, node2 = node2->next){
			if (node1->data != node2->data)
				break;
		}
		if (node1 == lhs.tail.p && node2 == rhs.tail.p)
			return true;
		return false;
	}

	template<class T>
	bool operator!=(const List<T>& lhs, const List<T>& rhs){
		return !(lhs == rhs);
	}
}

#endif