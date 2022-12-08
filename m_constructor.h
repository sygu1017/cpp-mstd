#pragma once

#include <new>			// placement new;
#include <utility>
#include <type_traits>	// is_trivially_destructible<>; remove_reference_t<>;
#include "m_utility.h"

namespace mstd {
	/*
	template<typename T1, typename T2>
	inline void construct(T1* p, const T2& value) {
		if (nullptr != p) { ::new(static_cast<void*>(p)) T1(value); }
	}

	template<typename T1, typename T2>
	inline void construct(T1* p, T2&& value) {
		if (nullptr != p) { ::new(static_cast<void*>(p)) T1(std::move(value)); }
	}
	*/

	// GNU C++ 11.2 construct
	template<typename Tp, typename... Args>
	inline void construct(Tp* ptr, Args&&... args) {
		if (nullptr != ptr) {
			::new(static_cast<void*>(ptr)) Tp(std::forward<Args>(args)...);
		}
	}

	template<typename Tp>
	inline void destroy(Tp* ptr) {
		if (nullptr != ptr) { ptr->~Tp(); }
	}

	template<typename Iter>
	inline void __destroy(Iter first, Iter last, std::false_type) {
		for (; first != last; ++first) {
			destroy(&*first);
		}
	}

	template<typename Iter>
	inline void __destroy(Iter first, Iter last, std::true_type) {}

	template<typename Iter, std::enable_if_t<mstd::is_iterator_v<Iter>, int> = 0>
	inline void destroy(Iter first, Iter last) {
		using type = typename std::is_trivially_destructible<
			std::remove_reference_t<decltype(*first)>>::type;
		__destroy(first, last, type());
	}

}

