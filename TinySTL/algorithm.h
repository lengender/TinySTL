#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include"Functional.h"

namespace TinySTL{
	//max
	template<class T>
	const T& max(const T& a, const T& b){
		return (a < b) ? b : a;
	}

	template<class T, class Compare>
	const T& max(const T& a, const T& b, Compare comp){
		return (comp(a, b)) ? b : a;
	}

	//make_heap
	template<class RandomAccessIterator, class Compare>
	//heap上溯算法
	static void up(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp){
		// [first, last], head pointer the header of the heap
		if (first != last){
			int index = last - head;
			auto parentIndex = (index - 1) / 2;
			for (auto cur = last; parentIndex >= 0 && cur != head; parentIndex = (index - 1) / 2){
				auto parent = head + parentIndex;
				if (comp(*parent, *cur)){
					TinySTL::swap(*parent, *cur);
				}
				cur = parent;
				index = cur - head;
			}
		}
	}

	template<class RandomAccessIterator, class Compare>
	//heap下降算法
	static void down(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp){
		if (first != last){
			auto index = first - head;
			auto leftChildIndex = index * 2 + 1;
			for (auto cur = first; leftChildIndex < (last - head + 1) && cur < last;
				leftChildIndex = index * 2 + 1){
				auto child = head + leftChildIndex;
				if ((child + 1) <= last && *(child + 1) > *child)
					child = child + 1;
				if (comp(*cur, *child))
					TinySTL::swap(*cur, *child);
				cur = child;
				index = cur - head;
			}
		}
	}

	template<class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last){
		TinySTL::make_heap(first, last,
			typename TinySTL::less<TinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
		const auto range = last - first;
		for (auto cur = first + range / 2 - 1; cur >= first; --cur){
			TinySTL::down(cur, last - 1, first, comp);
			if (cur == first) return;
		}
	}

	//push_heap
	template<class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last){
		TinySTL::push_heap(first, last,
			TinySTL::less<typename TinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
		TinySTL::up(first, last - 1, first, comp);
	}

	//pop_heap
	template<class RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last){
		TinySTL::pop_heap(first, last,
			TinySTL::less<typename TinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator, class COmpare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
		TinySTL::swap(*first, *(last - 1));
		if (last - first >= 2)
			TinySTL::down(first, last - 2, first, comp);
	}

	//sort_heap
	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last){
		return TinySTL::sort_heap(first, last,
			TinySTL::less<typename TinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
		for (auto cur = last; cur != first; --cur)
			TinySTL::pop_heap(first, cur, comp);
	}

	//is_heap
	template<class RandomAccessIterator>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last){
		return TinySTL::is_heap(first, last,
			TinySTL::less<typename TinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
		const auto range = last - first;
		auto index = range / 2 - 1;
		for (auto cur = first + range / 2 - 1; cur >= first; --cur, --index){
			if (*(first + (index * 2 + 1)) > *cur ||
				((first + (index * 2 + 2)) <= last && *(first + (index * 2 + 1)) > *cur))
				return false;
			if (cur == first)
				break;
		}
		return true;
	}
}
#endif