#pragma once

#include <cstdlib>
#include <cstring>
#include <new>

namespace mstd {

	const size_t PoolSize = 20;

	// ��һ��������
	template <int inst>
	class malloc_allocator
	{
	public:
		typedef void(*oom_handler_t)();

	public:
		static void* allocate(size_t num_bytes) {
			if (0 == num_bytes) { return nullptr; }
			void* result = std::malloc(num_bytes);
			if (nullptr == result) oom_malloc(num_bytes);
			return result;
		}

		static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
			if (nullptr == p) { return; }
			void* result = std::realloc(p, new_sz);
			if (nullptr == result) oom_realloc(p, new_sz);
			return result;
		}

		static void deallocate(void* p, size_t) {
			if (nullptr != p) { std::free(p); }
		}
		// �����ڴ治��ʱ�Ĵ�����
		static oom_handler_t set_oom_handler(oom_handler_t);

		static void* oom_malloc(size_t);
		static void* oom_realloc(void*, size_t);
		static oom_handler_t oom_handler;
	};

	template<int inst>
	typename malloc_allocator<inst>::oom_handler_t malloc_allocator<inst>::oom_handler = nullptr;

	template <int inst>
	void* malloc_allocator<inst>::oom_realloc(void* p, size_t n)
	{
		void* result = nullptr;
		while (true) {
			if (nullptr == oom_handler) throw std::bad_alloc{};
			oom_handler();
			result = std::realloc(p, n);
			if (result) return result;
		}
	}

	template <int inst>
	void* malloc_allocator<inst>::oom_malloc(size_t n)
	{
		void* result = nullptr;
		while (true) {
			if (nullptr == oom_handler) throw std::bad_alloc{};
			oom_handler();
			result = std::malloc(n);
			if (result) return result;
		}
	}

	template <int inst>
	typename malloc_allocator<inst>::oom_handler_t malloc_allocator<inst>::
		set_oom_handler(oom_handler_t func)
	{
		oom_handler_t old = oom_handler;
		oom_handler = func;
		return old;
	}

	// �ڶ���������
	template <int inst>
	class pool_allocator
	{
	private:
		enum { UP_BORDER = 8 };
		enum { MAX_BYTES = 128 };
		enum { NFREELISTS = 16 }; // MAX_BYTES/UP_BORDER

		struct obj { //free-lists �Ľڵ�
			obj* next;
		};
		//16��free-list
		static obj* free_list[NFREELISTS];

		//round_up�������ڴ��С�ϵ���8�ı���
		static size_t round_up(size_t size) {
			return (size + UP_BORDER - 1) & ~(UP_BORDER - 1);
		}

		//free_list_index���������ڴ��С������������������
		static size_t free_list_index(size_t size) {
			return (size + UP_BORDER - 1) / UP_BORDER - 1;
		}

		//��¼�ڴ�ص�״̬�� end - start ��ʾ�ڴ�ص�����
		static char* start;
		static char* end;
		static size_t heap_size; // ��¼����Ķ��ڴ�����

		//ÿ��malloc 2*n_obj*size ��С���ڴ��
		static const size_t n_obj;

		//chunk_alloc����һ���ڴ棬�����ڴ��׵�ַ���������ڴ��״̬
		static void* chunk_alloc(size_t size, size_t& m_n_obj);

		//refill��chunk_alloc���ص��ڴ���и��С�飬����������ͬʱ����������ڴ���׵�ַ
		static void* refill(size_t size);

	public:
		static void* allocate(size_t size);
		static void* reallocate(void* p, size_t old_sz, size_t new_sz);
		static void deallocate(void* p, size_t size);
	};

	template <int inst>
	typename pool_allocator<inst>::obj*
		pool_allocator<inst>::free_list[NFREELISTS]{};

	template <int inst>
	char* pool_allocator<inst>::start = nullptr;

	template <int inst>
	char* pool_allocator<inst>::end = nullptr;

	template <int inst>
	size_t pool_allocator<inst>::heap_size = 0;

	template <int inst>
	const size_t pool_allocator<inst>::n_obj = PoolSize;

	template <int inst>
	void* pool_allocator<inst>::chunk_alloc(size_t size, size_t& m_n_obj)
	{
		size_t total_bytes = m_n_obj * size;
		size_t bytes_left = end - start;
		char* result = nullptr;
		if (bytes_left >= total_bytes) {
			result = start;
			start += total_bytes;
			return result;
		}
		else if (bytes_left >= size) {
			result = start;
			m_n_obj = bytes_left / size;
			start += m_n_obj * size;
			return result;
		}
		else {
			if (bytes_left > 0) { // ��С��size��С���ڴ���ռ�����Ӧ����������
				auto m_free_list = free_list + free_list_index(bytes_left);
				auto temp = reinterpret_cast<obj*>(start);
				temp->next = *m_free_list;
				*m_free_list = temp;
			}
			size_t bytes_to_get = 2 * m_n_obj * size;
			result = static_cast<char*>(std::malloc(bytes_to_get));
			if (nullptr == result) {
				//Ѱ�ұ�size����ڴ����������������Ƿ��п��пռ�
				for (auto sz = size; sz <= 128; sz += UP_BORDER) {
					auto m_free_list = free_list_index(sz) + free_list;
					if (nullptr != *m_free_list) {
						start = reinterpret_cast<char*>(*m_free_list);
						end = start + sz;
						*m_free_list = (*m_free_list).next;
						return chunk_alloc(size, m_n_obj);
					}
				}
				//ʵ���Ҳ����ڴ棬����һ�������������ڴ棬����� out of memory handler
				//�����׳� bad_alloc �쳣
				start = static_cast<char*>(malloc_allocator<inst>::
					allocate(bytes_to_get));
				if (nullptr != start) {
					end = start + bytes_to_get;
					heap_size += bytes_to_get;
					return chunk_alloc(size, m_n_obj);
				}
			}
			end = result + bytes_to_get;
			heap_size += bytes_to_get;
			return chunk_alloc(size, m_n_obj);
		}
	}

	template <int inst>
	void* pool_allocator<inst>::refill(size_t size)
	{
		size_t m_n_obj = n_obj;
		void* result = chunk_alloc(size, m_n_obj);
		if (1 == m_n_obj) { return result; }
		auto temp = static_cast<char*>(result);
		size_t i = 1;
		for (; i < m_n_obj - 1; ++i) {
			reinterpret_cast<obj*>(temp + i * size).next =
				reinterpret_cast<obj*>(temp + (i + 1) * size);
		}
		reinterpret_cast<obj*>(temp + i * size).next = nullptr;
		auto m_free_list = free_list + free_list_index(size);
		*m_free_list = reinterpret_cast<obj*>(temp + size);
		return result;
	}

	template <int inst>
	void* pool_allocator<inst>::allocate(size_t size)
	{
		if (0 == size) { return; }
		if (size > static_cast<size_t>(MAX_BYTES))
		{//>128bytes ����һ�����������ڴ����뺯��
			malloc_allocator<inst>::allocate(size);
		}
		obj** m_free_list = free_list + free_list_index(size);
		obj* result = *m_free_list;
		if (nullptr == result) {
			result = refill(round_up(size));
			return result;
		}
		*m_free_list = result->next;
		return result;
	}

	template <int inst>
	void* pool_allocator<inst>::
		reallocate(void* p, size_t old_sz, size_t new_sz)
	{
		if (nullptr == p) { return; }
		if (old_sz > static_cast<size_t>(MAX_BYTES) &&
			new_sz > static_cast<size_t>(MAX_BYTES)) {
			return malloc_allocator<inst>::reallocate(p, new_sz);
		}
		else {
			void* result = nullptr;
			if (new_sz > static_cast<size_t>(MAX_BYTES)) {
				result = malloc_allocator<inst>::allocate(new_sz);
			}
			result = allocate(new_sz);
			auto copy_sz = (old_sz > new_sz) ? new_sz : old_sz;
			std::memcpy(result, p, copy_sz);
			deallocate(p, old_sz);
			return result;
		}
	}

	template <int inst>
	void pool_allocator<inst>::deallocate(void* p, size_t size)
	{
		if (nullptr == p) { return; }
		if (size > static_cast<size_t>(NFREELISTS))
		{//>128bytes ����һ�����������ڴ��ͷź���
			malloc_allocator<inst>::deallocate(p, size);
			return;
		}
		obj** m_free_list = free_list + free_list_index(size);
		auto mp = static_cast<obj*>(p);
		mp->next = *m_free_list;
		*m_free_list = mp;
	}

}

