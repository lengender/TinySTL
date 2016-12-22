#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

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
}
#endif