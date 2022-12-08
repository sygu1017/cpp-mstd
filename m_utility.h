#pragma once

#include "m_type_traits.h"
#include <cassert>			// assert();

namespace mstd {

	template<class Tp>
	constexpr remove_reference_t<Tp>&& move(Tp&& val) noexcept {
		return static_cast<remove_reference_t<Tp>&&>(val);
	}

	template<class Tp>
	constexpr Tp&& forward(remove_reference_t<Tp>& arg) noexcept {
		return static_cast<Tp&&>(arg);
	}

	template<class Tp>
	constexpr Tp&& forward(remove_reference_t<Tp>&& arg) noexcept {
		return static_cast<Tp&&>(arg);
	}

	template <typename Tp>
	inline void swap(Tp& left, Tp& right) noexcept
	{
		Tp temp = mstd::move(left);
		left = mstd::move(right);
		right = mstd::move(temp);
	}

	template<typename Tp, size_t N>
	inline void swap(Tp(&left)[N], Tp(&right)[N]) noexcept {
		for (size_t index = 0; index < N; ++index)
			swap(left[index], right[index]);
	}

	// 判断是否是迭代器 is_iterator_v
	template<typename Iter, typename = void>
	constexpr bool is_iterator_v = false;

	template<typename Iter>
	constexpr bool is_iterator_v<Iter,
		std::void_t<typename std::iterator_traits<Iter>::iterator_category>> = true;

	// 实现std::memmove,std::memcpy
	/*
	void* memcpy(void *dst, const void *src, size_t count);
	void* memmove(void *dst, const void *src, size_t count);
	*/

	void* memcpy(void* dst, const void* src, size_t count)
	{
		assert(dst != nullptr && src != nullptr);
		char* ptr_dst = (char*)dst;
		char* ptr_src = (char*)src;
		if (ptr_dst == ptr_src) return dst;
		while (count--)
			*ptr_dst++ = *ptr_src++;
		return dst;
	}

	void* memmove(void* dst, const void* src, size_t count)
	{
		assert(dst != nullptr && src != nullptr);
		if (dst < src) {
			char* ptr_dst = (char*)dst;
			char* ptr_src = (char*)src;
			while (count--)
				*ptr_dst++ = *ptr_src++;
		}
		else {
			char* ptr_dst = (char*)dst + count;
			char* ptr_src = (char*)src + count;
			while (count--)
				*--ptr_dst = *--ptr_src;
		}
		return dst;
	}

	template<class Iter>
	struct is_bitcopy_assignment : std::bool_constant<false> {};

	template<class Iter>
	using is_bitcopy_assignment_t = typename is_bitcopy_assignment<Iter>::type;




}