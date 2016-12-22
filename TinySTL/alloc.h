#ifndef _ALLOC_H_
#define _ALLOC_H_

/*
	空间配置器，仍然采用两级配置。
	第一级：_malloc_alloc   直接使用malloc 和 free 函数， 当要求内存大于128bytes时
	第二级：_default_alloc 内存池，维护16个自由链表，负责16中小型区块的次配置能力
			内存池（memory pool）以malloc配置得到，如果内存不足，转调用第一级配置器
*/

#include<stdlib.h>
#include<iostream>

namespace TinySTL{

	/*
		第一级空间配置器
		*/
	class _malloc_alloc{
	public:
		static void *allocate(size_t n)
		{
			void *result = malloc(n);
			if (result == NULL)
			{
				std::cout << "memory lack" << std::endl;
				exit(0);
			}

			return result;
		}

		static void deallocate(void *p, size_t n)
		{
			free(p);
		}

		static void* reallocate(void *p, size_t old_sz, size_t new_sz)
		{
			void *result = realloc(p, new_sz);
			if (result == NULL)
			{
				std::cout << "realloc error." << std::endl;
				exit(0);
			}
			return result;
		}

		//SGI STL中若内存分配失败，会不停的尝试申请，直到出错，或成功申请到
		//在此，不考虑内存分配失败的情况。
	};

	/*
		第二级空间配置器，实现内存池的配置，分配、回收
		*/
	enum { _ALIGN = 8 };  //小型区块的上调边界
	enum { _MAX_BYTES = 128 };  //小型区块的上限，超过的区块，转由第一级配置器分配
	enum { _NFTEELISTS = _MAX_BYTES / _ALIGN }; // free-lists个数
	enum { _NOBJS = 20 };   //每次增加的节点数

	class _default_alloc{
	private:
		//free-lists的节点构造
		union obj{
			union obj *next;
			char client[1];
		};
	private:
		//16个free-lists
		static obj *volatile free_list[_NFTEELISTS];
		static char *start_free;  //内存池起始位置
		static char *end_free;   //内存池结束位置
		static size_t heap_size;
	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes){
			return (((bytes)+_ALIGN - 1) & ~(_ALIGN - 1));
		}

		//根据区块大小，决定使用第n号free-list,n从0开始计算
		static size_t FREELIST_INDEX(size_t bytes){
			return (((bytes)+_ALIGN - 1) / _ALIGN - 1);
		}

		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void *refill(size_t n);

		//配置一大块空间，可容纳nobjs个大小为"size"的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, int &nobjs);

	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void* reallocate(void *p, size_t old_sz, size_t new_sz);
	};

	typedef _default_alloc alloc;

	/*template < class T, class Alloc >
	class Simple_Alloc{
	public:
		static T* allocate(size_t n){
			return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
		}

		static T* allocate(void){
			return (T*)Alloc::allocate(sizeof(T));
		}

		static void deallocate(T *p, size_t n){
			if (0 != n)
				Alloc::deallocate(p, n * sizeof(T));
		}

		static void deallocate(T *p){
			Alloc::deallocate(p, sizeof(T));
		}
	};*/
}

#endif