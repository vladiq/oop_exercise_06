#ifndef D_ALLOCATOR_H_
#define D_ALLOCATOR_H_ 1

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <list>

#include "list.h"

namespace allocator {

	template<class T, size_t ALLOC_SIZE>
	struct my_allocator {
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using is_always_equal = std::false_type;

		template<class L>
		struct rebind {
			using other = my_allocator<L, ALLOC_SIZE>;
		};

		my_allocator() :
			pool_begin(new char[ALLOC_SIZE]),
			pool_end(pool_begin + ALLOC_SIZE),
			pool_tail(pool_begin)
		{}

		my_allocator(const my_allocator&) = delete;
		my_allocator(my_allocator&&) = delete;

		~my_allocator() {
			delete[] pool_begin;
		}

		T* allocate(std::size_t n);
		void deallocate(T* ptr, std::size_t n);

	private:
		char* pool_begin;
		char* pool_end;
		char* pool_tail;
		std::list<char*> free_blocks;
	};

	template<class T, size_t ALLOC_SIZE>
	T* my_allocator<T, ALLOC_SIZE>::allocate(std::size_t n) {
		if (n != 1) {
			throw std::logic_error("Allocating arrays is unavaliable");
		}
		if (size_t(pool_end - pool_tail) < sizeof(T)) {
			if (free_blocks.size()) {
				auto it = free_blocks.begin();
				char* ptr = *it;
				free_blocks.pop_front();
				return reinterpret_cast<T*>(ptr);
			}
			throw std::bad_alloc();
		}
		T* result = reinterpret_cast<T*>(pool_tail);
		pool_tail += sizeof(T);
		return result;
	}

	template<class T, size_t ALLOC_SIZE>
	void my_allocator<T, ALLOC_SIZE>::deallocate(T* ptr, std::size_t n) {
		if (n != 1) {
			throw std::logic_error("Allocating arrays is unavaliable, thus deallocating is unavalivable as well");
		}
		if (ptr == nullptr) {
			return;
		}
		free_blocks.push_back(reinterpret_cast<char*>(ptr));
	}
};

#endif // D_ALLOCATOR_H_
