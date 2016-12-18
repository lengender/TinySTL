#include"alloc.h"
#include"construct.h"

namespace TinySTL{
	char *_default_alloc::start_free = 0;
	char *_default_alloc::end_free = 0;
	size_t _default_alloc::heap_size = 0;

	_default_alloc::obj *volatile _default_alloc::free_list[_NFTEELISTS]
		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	void* _default_alloc::allocate(size_t n){
		if (n > _MAX_BYTES)
			return _malloc_alloc::allocate(n);

		size_t index = FREELIST_INDEX(n);
		obj *list = free_list[index];
		if (list){
			//��list���滹�пռ�
			free_list[index] = list->next;
			return list;
		}
		else{
			//û�пռ䣬��Ҫ�����·���ռ�
			return refill(ROUND_UP(n));
		}
	}

	void _default_alloc::deallocate(void *ptr, size_t n){
		if (n > _MAX_BYTES){
			_malloc_alloc::deallocate(ptr, n);
			return;
		}

		size_t index = FREELIST_INDEX(n);
		obj *node = static_cast<obj*>(ptr);

		node->next = free_list[index];   //ֱ�ӽӵ��ڴ�����ĵ�һ��
		free_list[index] = node;
	}

	//���·����ڴ�ռ�
	void *_default_alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz){
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}

	//����һ����СΪn�Ķ��󣬲�����ʱ���Ϊ�ʵ���free_list���ӽڵ�
	//����bytes�Ѿ��ϵ�Ϊ8�ı���
	void* _default_alloc::refill(size_t n){
		int nobjs = _NOBJS;
		
		//���ڴ����ȡ
		char *chunk = chunk_alloc(n, nobjs);
		obj *volatile *my_free_list = 0;
		obj *result = 0;
		obj *current_obj = 0, *next_obj = 0;

		//���ֻ���һ�����飬����������������ߣ�free list���½ڵ�
		if (nobjs == 1){
			return chunk;
		}

		//����׼������free list, �����½ڵ�
		my_free_list = free_list + FREELIST_INDEX(n);

		//������chunk�ռ��ڽ���free_list
		result = (obj*)chunk;   //���ظ��Ͷ�
		
		//��������free_listָ�������õĿռ�
		*my_free_list = next_obj = (obj*)(chunk + n);
		//���½�free_list���ڵ㴮������
		for (int i = 1;; ++i){
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i){
				current_obj->next = 0;
				break;
			}

			current_obj->next = next_obj;
		}

		return result;
	}

	//����size�Ѿ��ʵ��ϵ���8�ı�����ע�����nobjs��pass by reference
	char * _default_alloc::chunk_alloc(size_t size, int &nobjs){
		char *result = 0;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;   //�ڴ��ʣ��ռ�

		if (bytes_left >= total_bytes){
			//�ڴ��ʣ��ռ���ȫ����������
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size){
			//�ڴ��ʣ��ռ䲻����ȫ���������������㹻��Ӧһ�����������ϵ�����
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else{
			//�ڴ��ʣ��ռ���һ������Ҳ�޷��ṩ
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//�������ڴ���еĲ�����ͷ�������ü�ֵ
			if (bytes_left > 0){
				obj *volatile*mylist = free_list + FREELIST_INDEX(bytes_left);
				//����free_list,���ڴ����ʣ��ռ����
				((obj*)start_free)->next = *mylist;
				*mylist = (obj*)start_free;
			}

			//����heap�ռ䣬���������ڴ��
			start_free = (char*)malloc(bytes_to_get);
			if (!start_free){
				//heap�ռ䲻�㣬mallocʧ��
				int i;
				obj *volatile *my_free_list = 0, *p = 0;
				for (i = size; i <= _MAX_BYTES; i += _ALIGN){
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != NULL){  //free list ������δ������
						//����free list���ͷ�δ������
						*my_free_list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;  //����������⣬���û���ڴ���
				//���õ�һ���ڴ�������
				start_free = (char*)_malloc_alloc::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//�ݹ�����Լ���Ϊ������nobjs
			return chunk_alloc(size, nobjs);
		}
	}
}