#ifndef _ALLOC_H_
#define _ALLOC_H_

/*
	�ռ�����������Ȼ�����������á�
	��һ����_malloc_alloc   ֱ��ʹ��malloc �� free ������ ��Ҫ���ڴ����128bytesʱ
	�ڶ�����_default_alloc �ڴ�أ�ά��16��������������16��С������Ĵ���������
			�ڴ�أ�memory pool����malloc���õõ�������ڴ治�㣬ת���õ�һ��������
*/

#include<stdlib.h>
#include<iostream>

namespace TinySTL{

	/*
		��һ���ռ�������
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

		//SGI STL�����ڴ����ʧ�ܣ��᲻ͣ�ĳ������룬ֱ��������ɹ����뵽
		//�ڴˣ��������ڴ����ʧ�ܵ������
	};

	/*
		�ڶ����ռ���������ʵ���ڴ�ص����ã����䡢����
		*/
	enum { _ALIGN = 8 };  //С��������ϵ��߽�
	enum { _MAX_BYTES = 128 };  //С����������ޣ����������飬ת�ɵ�һ������������
	enum { _NFTEELISTS = _MAX_BYTES / _ALIGN }; // free-lists����
	enum { _NOBJS = 20 };   //ÿ�����ӵĽڵ���

	class _default_alloc{
	private:
		//free-lists�Ľڵ㹹��
		union obj{
			union obj *next;
			char client[1];
		};
	private:
		//16��free-lists
		static obj *volatile free_list[_NFTEELISTS];
		static char *start_free;  //�ڴ����ʼλ��
		static char *end_free;   //�ڴ�ؽ���λ��
		static size_t heap_size;
	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes){
			return (((bytes)+_ALIGN - 1) & ~(_ALIGN - 1));
		}

		//���������С������ʹ�õ�n��free-list,n��0��ʼ����
		static size_t FREELIST_INDEX(size_t bytes){
			return (((bytes)+_ALIGN - 1) / _ALIGN - 1);
		}

		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void *refill(size_t n);

		//����һ���ռ䣬������nobjs����СΪ"size"������
		//�������nobjs�������������㣬nobjs���ܻή��
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