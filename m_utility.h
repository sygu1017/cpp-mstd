#pragma once

#include <cassert>			// assert();
#include <utility>			// std::addressof();
#include <iterator>

#include "m_type_traits.h"

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

	template<class Tp, class Other = Tp>
	inline constexpr Tp exchange(Tp& val, Other&& other) noexcept(mstd::conjunction_v<mstd::is_nothrow_move_constructible<Tp>, mstd::is_nothrow_assignable<Tp&, Other>>) {
		Tp old_val = static_cast<Tp&&>(val);
		val = static_cast<Tp&&>(other);
		return old_val;
	}


	template<class Tp>
	struct _Get_first_parameter;

	template<template<class, class...> class Tp, class First, class...Rest>
	struct _Get_first_parameter<Tp<First, Rest...>> {
		using type = First;
	};

	template<class Tp, class = void>
	struct _Get_element_type {
		using type = typename _Get_first_parameter<Tp>::type;
	};

	template<class Tp>
	struct _Get_element_type<Tp, mstd::void_t<typename Tp::element_type>> {
		using type = typename Tp::element_type;
	};

	template <class Tp, class = void>
	struct _Get_ptr_difference_type {
		using type = ptrdiff_t;
	};

	template <class Tp>
	struct _Get_ptr_difference_type<Tp, void_t<typename Tp::difference_type>> {
		using type = typename Tp::difference_type;
	};

	template<class New_first, class Tp>
	struct _Replace_first_parameter;

	template<class New_first, template<class, class...> class Tp, class First, class... Rest>
	struct _Replace_first_parameter<New_first, Tp<First, Rest...>> {
		using type = Tp<New_first, Rest...>;
	};

	template<class Tp, class Other, class = void>
	struct _Get_rebind_alias {
		using type = typename _Replace_first_parameter<Other, Tp>::type;
	};

	template<class Tp, class Other>
	struct _Get_rebind_alias<Tp, Other, mstd::void_t<typename Tp::template rebind<Other>>> {
		using type = typename Tp::template rebind<Other>;
	};

	template<class Tp>	// fancy pointer
	struct pointer_traits {

		using pointer = Tp;
		using element_type = typename _Get_element_type<Tp>::type;
		using difference_type = typename _Get_ptr_difference_type<Tp>::type;

		template<class Other>
		using rebind = typename _Get_rebind_alias<Tp, Other>::type;
		using _Reftype = mstd::conditional_t<mstd::is_void_v<element_type>, char, element_type>&;

		static pointer pointer_to(_Reftype val) noexcept(noexcept(Tp::pointer_to(val))) {
			return Tp::pointer_to(val);
		}
	};

	template<class Tp>	// 原生指针 plain pointer
	struct pointer_traits<Tp*> {

		using pointer = Tp*;
		using element_type = Tp;
		using different_type = ptrdiff_t;

		template<class Other>
		using rebind = Other*;

		using _Reftype = mstd::conditional_t<mstd::is_void_v<Tp>, char, Tp>&;

		static pointer pointer_to(_Reftype val) noexcept {
			return std::addressof(val);
		}
	};

	template<class Ptr, class Other>
	using _Rebind_pointer_t = typename pointer_traits<Ptr>::template rebind<Other>;

	template<class Ptr_type>
	inline constexpr auto _Unfancy(Ptr_type ptr) noexcept { return std::addressof(*ptr); }

	template<class Tp>
	constexpr Tp* _Unfancy(Tp* ptr) noexcept { return ptr; }

	template<class Pointer, mstd::enable_if_t<!mstd::is_pointer_v<Pointer>, int> = 0>
	inline constexpr Pointer _Refancy(typename pointer_traits<Pointer>::element_type* ptr) noexcept {
		return pointer_traits<Pointer>::pointer_to(*ptr);
	}

	template<class Pointer, mstd::enable_if_t<mstd::is_pointer_v<Pointer>, int> = 0>
	inline constexpr Pointer _Refancy(Pointer ptr) noexcept {
		return ptr;
	}

	//template<class Pointer, mstd::enable_if_t<!mstd::is_pointer_v<Pointer>, int> = 0>
	//constexpr











	template<class Iter>
	struct is_bitcopy_assignment : mstd::bool_constant<false> {};

	template<class Iter>
	using is_bitcopy_assignment_t = typename is_bitcopy_assignment<Iter>::type;







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

}