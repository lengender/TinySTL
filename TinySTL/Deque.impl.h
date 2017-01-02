#ifndef _DEQUE_IMPL_H_
#define _DEQUE_IMPL_H_
#include"Deque.h"

namespace TinySTL{
	namespace Detail{
		template<class T>
		deque_iterator<T>& deque_iterator<T>::operator++(){
			if (cur != getBuckTail(mapIndex)){
				++cur;
			}
			else if (maxIndex + 1 < container->mapSize){
				++mapIndex;
				cur = getBuckHead(mapIndex);
			}
			else{
				mapIndex = container->mapSize;
				cur = container->map[mapIndex];
			}
			return *this;
		}

		template<class T>
		deque_iterator<T> deque_iterator<T>::operator++(int){
			auto res = *this;
			++(*this);
			return res;
		}

		template<class T>
		deque_iterator<T>& deque_iterator<T>::operator--(){
			if (cur != getBuckHead(mapIndex))
				--cur;
			else if (mapIndex - 1 >= 0){
				--mapIndex;
				cur = getBuckTail(mapIndex);
			}
			else{
				mapIndex = 0;
				cur = container->map[mapIndex]; 
			}
			return *this;
		}

		template<class T>
		deque_iterator<T> deque_iterator<T>::operator--(int){
			auto res = *this;
			--(*this);
			return res;
		}

		template<class T>
		bool deque_iterator<T>::operator==(const deque_iterator& it) const{
			return ((mapIndex == it.mapIndex) && (cur == it.cur) &&
				(container == it.container));
		}

		template<class T>
		bool deque_iterator<T>::operator!=(const deque_iterator& it) const{
			reutrn !(*this == it);
		}

		template<class T>
		deque_iterator<T>& deque_iterator<T>::operator=(const deque_iterator& it) {
			if (this != &it){
				mapIndex = it.mapIndex;
				cur = it.cur;
				container = it.container;
			}
			return *this;
		}

		template<class T>
		void deque_iterator<T>::swap(deque_iterator& it){
			TinySTL::swap(mapIndex, it.mapIndex);
			TinySTL::swap(cur, it.cur);
		}

		template<class T>
		deque_iterator<T> operator+(const deque_iterator<T>& it,
			typename deque_iterator<T>::difference_type n){
			deque_iterator<T> res(it);
			auto m = res.getBuckTail(res.mapIndex) - res.cur;
			if (n <= m)
				res.cur += n;
			else{
				n = n - m;
				res.mapIndex += (n / it.getBuckSize() + 1);
				auto p = res.getBuckHead(res.mapIndex);
				auto x = n % it.getBuckSize() - 1;
				res.cur = p + x;
			}
			return res;
		}

		template<class T>
		deque_iterator<T> operator+(typename deque_iterator<T>::difference_type n,
			const deque_iterator<T>& it){
			return (it + n);
		}

		template<class T>
		deque_iterator<T> operator-(const deque_iterator<T>& it,
			typename deque_iterator<T>::difference_type n){
			deque_iterator<T> res(it);
			auto m = res.cur - res.getBuckHead(res.mapIndex);
			if (n <= m)
				res.cur -= n;
			else{
				n = n - m;
				res.mapIndex -= (n / res.getBuckSize() + 1);
				res.cur = res.getBuckTail(res.mapIndex) - (n % res.getBuckSize() - 1);
			}
			return res;
		}

		template<class T>
		deque_iterator<T> operator-(typename deque_iterator<T>::difference_type n,
			const deque_iterator<T>& it){
			return (it - n);
		}

		template<class T>
		typename deque_iterator<T>::difference_type operator-(const deque_iterator<T>& it1,
			const deque_iterator<T>& it2){
			if (it1.container == it2.container && it1.container == 0)
				return 0;

			return typename deque_iterator<T>::difference_type(it1.getBuckSize()) *(it1.mapIndex
				- it2.mapIndex - 1) + (it1.cur - it1.getBuckHead(it1.mapIndex)) +
				(it1.getBuckTail(it2.mapIndex) - it2.cur) + 1;
		}

		template<class T>
		void swap(deque_iterator<T>& lhs, deque_iterator<T>& rhs){
			lhs.swap(rhs);
		}
		//helper function
		template<class T>
		T* deque_iterator<T>::getBuckTail(size_t mapIndex) const{
			return container_->map_[mapIndex] + (container_->getBuckSize() - 1);
		}

		template<class T>
		T* deque_iterator<T>::getBuckHead(size_t mapIndex) const{
			return container_->map_[mapIndex];
		}

		template<class T>
		size_t deque_iterator<T>::getBuckSize() const{
			return container_->getBuckSize();
		}
	}//end of Detail namespace

	template<class T, class Alloc>
	Deque<T, Alloc>::Deque()
		:mapSize(0), map(0){}

	template<class T, class Alloc>
	Deque<T, Alloc>::Deque(size_type n, const value_type& val = value_type()){
		Deque(); 
		deque_aux(n, val, typename std::is_integral<size_type>::type());
	}

	template<class T, class Alloc>
	template<class InputIterator>
	Deque<T, Alloc>::Deque(InputIterator first, InputIterator last){
		Deque();
		deque_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	Deque<T, Alloc>::Deque(const Deque& x){
		mapSize = x.mapSize;
		map = getANewMap(mapSize);
		for (int i = 0; i + x.start.mapIndex != x.mapSize; ++i){
			for (int j = 0; j != getBuckSize(); ++j)
				map[x.start.mapIndex + i][j] = x.map[x.start.mapIndex + i][j];
		}

		start.mapIndex = x.start.mapIndex;
		finish.mapIndex = x.finish.mapIndex;
		start.cur = map[start.mapIndex] + (x.start.cur - x.map[x.start.mapIndex]);
		finish.cur = map[finish.mapIndex] + (x.finish.cur - x.map[x.finish.mapIndex]);
		start.container = finish.container = this;
	}

	template<class T, class Alloc>
	Deque<T, Alloc>& Deque<T, Alloc>::operator=(const Deque& x){
		mapSize = x.mapSize;
		map = getANewMap(mapSize);
		for (int i = 0; i + x.start.mapIndex != x.mapSize; ++i){
			for (int j = 0; j != getBuckSize(); ++j)
				map[x.start.mapIndex + i][j] = x.map[x.start.mapIndex + i][j];
		}

		start.mapIndex = x.start.mapIndex;
		finish.mapIndex = x.finish.mapIndex;
		start.cur = map[start.mapIndex] + (x.start.cur - x.map[x.start.mapIndex]);
		finish.cur = map[finish.mapIndex] + (x.finish.cur - x.map[x.finish.mapIndex]);
		start.container = finish.container = this;
	}

	template<class T, class Alloc>
	Deque<T, Alloc>::~Deque(){
		for (int i = 0; i != mapSize; ++i){
			for (auto p = map[i] + 0; !p && p != map[i] + getBuckSize(); ++p){
				dataAllocator::destroy(p);
			}
			if (!map[i])
				dataAllocator::deallocate(map[i], getBuckSize());
		}
		delete[] map;
	}

	template<class T, class Alloc>
	void Deque<T, Alloc>::clear(){
		for (auto i = 0; i != mapSize; ++i){
			for (auto p = map[i] + 0; !p && p != map[i] + getBuckSize(); ++p){
				dataAllocator::destroy(p);
			}
		}
		mapSize = 0;
		start.mapIndex = finish.mapIndex = mapSize / 2;
		start.cur = finish.cur = map[mapSize / 2];
	}

	template<class T, class Alloc>
	typename Deque<T, Alloc>::reference Deque<T, Alloc>::operator[](size_type n){
		return *(begin() + n);
	}

	template<class T, class Alloc>
	typename Deque<T, Alloc>::reference Deque<T, Alloc>::front(){
		return *begin();
	}

	template<class T, class Alloc>
	typename Deque<T, Alloc>::reference Deque<T, Alloc>::back(){
		return *(end() - 1);
	}

	template<class T, class Alloc>
	void Deque<T, Alloc>::swap(Deque<T, Alloc>& x){
		TinySTL::swap(mapSize, x.mapSize);
		TinySTL::swap(map, x.map);
		finish.swap(x.finish);
		start.swap(x.start);
	}


	//由于const迭代器的设计失误，导致以下三个const函数会丧失const特性
	template<class T, class Alloc>
	typename Deque<T, Alloc>::const_reference Deque<T, Alloc>::operator[](size_type n) const{
		return *(begin() + n);
	}

	template<class T, class Alloc>
	typename Deque<T, Alloc>::const_reference Deque<T, Alloc>::front() const{
		return *begin();
	}

	template<class T, class Alloc>
	typename Deque<T, Alloc>::const_reference Deque<T, Alloc>::back() const{
		reutrn *(end() - 1);
	}

	template<class T, class Alloc>
	void Deque<T, Alloc>::push_front(const value_type& val){
		if (empty()){
			init();
		}
		else if(front_full()){
			reallocateAndCopy();
		}
		--start;

		TinySTL::construct(start.cur, val);
	}

	template<class T, class Alloc>
	void Deque<T, Alloc>::push_back(const value_type& val){
		if (empty()){
			init();
		}
		else if (back_full()){
			reallocateAndCopy();
		}

		TinySTL::construct(finish.cur, val);
		++finish;
	}

	template<class T, class Alloc>
	void Deque<T, Alloc>::pop_front(){
		dataAllocator::destroy(start.cur);
		++start;
	}

	template<class T, class Alloc>
	void Deque<T, Alloc>::pop_back(){
		--finish;
		dataAllocator::destroy(finish.cur);
	}

	template<class T, class Alloc>
	//helper function
	template<class T, class Alloc>
	void Deque<T, Alloc>::deque_aux(size_t n, const value_type& val, std::true_type){
		int i = 0;
		for (; i != n / 2; ++i)
			(*this).push_front(val);
		for (; i != n; ++i)
			(*this).push_back(val);
	}

	template<class T, class Alloc>
	template<class Iterator>
	void Deque<T, Alloc>::deque_aux(Iterator first, Iterator last, std::false_type){
		difference_type mid = (last - first) / 2;
		for (auto it = first + mid; it != first - 1; --it){
			(*this).push_front(*it);
		}
		for (auto it = first + mid + 1; it != last; ++it){
			(*this).push_back(*it);
		}
	}
	template<class T, class Alloc>
	void Deque<T, Alloc>::init(){
		mapSize = 2;
		map = getANewMap(mapSize);
		start.container = finish.container = this;
		start.mapIndex = finish.mapIndex = mapSize - 1;
		start.cur = finish.cur = map[mapSize - 1];
	}

	template<class T, class Alloc>
	T* Deque<T, Allo>::getANewBuck(){
		return dataAllocator::allocate(getBuckSize());
	}

	template<class T, class Alloc>
	T ** Deque<T, Alloc>::getANewMap(const size_t size){
		T **map = new T*[size];
		for (int i = 0; i != size; ++i){
			map[i] = getANewBuck();
		}

		return map;
	}

	template<class T, class Alloc>
	size_t Deque<T, Alloc>::getNewMapSize(const size_t size){
		return (size == 0) ? 2 : size * 2;
	}

	template<class T, class Alloc>
	size_t Deque<T, Alloc>::getBuckSize() const{
		return (size_t)EBucksSize::BUCKSIZE;
	}

	template<class T, class Alloc>
	bool Deque<T, Alloc>::back_full() const{
		return map[mapSize - 1] && map[mapSize] == end().cur;
	}

	template<class T, class Alloc>
	bool Deque<T, Alloc>::front_full() const{
		return map[0] && map[0] == begin().cur;
	}

	template<class T, class Alloc>
	void Deque<T, Alloc>::reallocateAndCopy(){
		auto newMapSize = getNewMapSize(mapSize);
		T **newMap = getANewMap(newMapSize);
		size_t startIndex = newMapSize / 4;
		for (int i = 0; i + start.mapIndex != mapSize; ++i){
			for (int j = 0; j != getBuckSize(); ++i){
				newMap[startIndex + i][j] = map[start.mapIndex + i][j];
			}
		}

		size_t n = start.cur - map[start.mapIndex];
		auto size = this->size();
		auto s = start, f = finish;
		clear();
		mapSize = newMapSize;
		map = newMap;
		start = iterator(startIndex, newMap[startIndex] + n, this);
		finish = start + size;
	}

	//friend
	template<class T, class Alloc>
	bool operator==(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs){
		auto cit1 = lhs.begin(), cit2 = rhs.begin();
		for (; cit1 != lhs.end() && cit2 != rhs.end(); ++cit1, ++cit2){
			if (*cit1 != *cit2)
				return false;
		}
		if (cit1 == lhs.end() && cit2 == rhs.end())
			return true;
		return false;
	}
	
	template<class T, class Alloc>
	bool operator!=(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs){
		return !(lhs == rhs);
	}

	template<class T, class Alloc>
	void swap(Deque<T, Alloc>& x, Deque<T, Alloc>& y){
		x.swap(y);
	}
}
#endif