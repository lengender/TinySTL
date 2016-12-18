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
			//此list上面还有空间
			free_list[index] = list->next;
			return list;
		}
		else{
			//没有空间，需要从重新分配空间
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

		node->next = free_list[index];   //直接接到内存区块的第一块
		free_list[index] = node;
	}

	//重新分配内存空间
	void *_default_alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz){
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}

	//返回一个大小为n的对象，并且有时候会为适当的free_list增加节点
	//假设bytes已经上调为8的倍数
	void* _default_alloc::refill(size_t n){
		int nobjs = _NOBJS;
		
		//从内存池中取
		char *chunk = chunk_alloc(n, nobjs);
		obj *volatile *my_free_list = 0;
		obj *result = 0;
		obj *current_obj = 0, *next_obj = 0;

		//如果只获得一个区块，这个区块分配给调用者，free list无新节点
		if (nobjs == 1){
			return chunk;
		}

		//否则准备调整free list, 纳入新节点
		my_free_list = free_list + FREELIST_INDEX(n);

		//以下在chunk空间内建立free_list
		result = (obj*)chunk;   //返回个客端
		
		//以下引导free_list指向新配置的空间
		*my_free_list = next_obj = (obj*)(chunk + n);
		//以下将free_list各节点串接起来
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

	//假设size已经适当上调至8的倍数，注意参数nobjs是pass by reference
	char * _default_alloc::chunk_alloc(size_t size, int &nobjs){
		char *result = 0;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;   //内存池剩余空间

		if (bytes_left >= total_bytes){
			//内存池剩余空间完全满足需求量
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size){
			//内存池剩余空间不能完全满足需求量，但足够供应一个（含）以上的区块
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else{
			//内存池剩余空间连一个区块也无法提供
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//试着让内存池中的残余零头还有利用价值
			if (bytes_left > 0){
				obj *volatile*mylist = free_list + FREELIST_INDEX(bytes_left);
				//调整free_list,将内存池中剩余空间插入
				((obj*)start_free)->next = *mylist;
				*mylist = (obj*)start_free;
			}

			//配置heap空间，用来补充内存池
			start_free = (char*)malloc(bytes_to_get);
			if (!start_free){
				//heap空间不足，malloc失败
				int i;
				obj *volatile *my_free_list = 0, *p = 0;
				for (i = size; i <= _MAX_BYTES; i += _ALIGN){
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != NULL){  //free list 内尚有未用区块
						//调整free list以释放未用区块
						*my_free_list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;  //如果出现意外，真的没有内存了
				//调用第一级内存配置器
				start_free = (char*)_malloc_alloc::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//递归调用自己，为了修正nobjs
			return chunk_alloc(size, nobjs);
		}
	}
}