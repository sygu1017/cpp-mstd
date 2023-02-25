#pragma once

#include <cassert>			// assert();
#include <utility>			// std::addressof();

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
	constexpr void swap(Tp& left, Tp& right)
		noexcept(is_nothrow_move_constructible<Tp>::value)
	{
		Tp temp = mstd::move(left);
		left = mstd::move(right);
		right = mstd::move(temp);
	}

	template<typename Tp, size_t N>
	constexpr void swap(Tp(&left)[N], Tp(&right)[N])
		noexcept(is_nothrow_move_constructible<Tp>::value) {
		for (size_t index = 0; index < N; ++index)
			swap(left[index], right[index]);
	}

	template<class Tp, class Other = Tp>
	constexpr Tp exchange(Tp& val, Other&& other)
		noexcept(mstd::conjunction_v<mstd::is_nothrow_move_constructible<Tp>, mstd::is_nothrow_assignable<Tp&, Other>>) {
		Tp old_val = static_cast<Tp&&>(val);
		val = static_cast<Tp&&>(other);
		return old_val;
	}
}

#include "m_iterator.h"

namespace mstd {

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
			return Tp::pointer_to(val);  // 返回fancy pointer Tp中定义的pointer_to()函数的返回值，有哪些pointer中定义了pointer_to()函数??
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
	constexpr auto _Unfancy(Ptr_type ptr) noexcept { return std::addressof(*ptr); }

	template<class Tp>
	constexpr Tp* _Unfancy(Tp* ptr) noexcept { return ptr; }

	template<class Pointer, mstd::enable_if_t<!mstd::is_pointer_v<Pointer>, int> = 0>
	constexpr Pointer _Refancy(typename pointer_traits<Pointer>::element_type* ptr) noexcept {
		return pointer_traits<Pointer>::pointer_to(*ptr);
	}

	template<class Pointer, mstd::enable_if_t<mstd::is_pointer_v<Pointer>, int> = 0>
	constexpr Pointer _Refancy(Pointer ptr) noexcept {
		return ptr;
	}

	// copy(); copy_n(); copy_backward(); move(); move_backward(); fill(); fill_n(); equal(); 
	// lexicographical_compare; find(); count(); reverse(); rotate(); find_if(); 

	// 实现std::memmove(); std::memcpy();
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
	constexpr auto _To_address(const Iter& val) noexcept {
		return val;
	}

	template<class Iter>  // move_iterator<>偏特化，
	constexpr auto _To_address(const move_iterator<Iter>& val) noexcept {
		return _To_address(val.base());
	}

	template<class Iter>
	constexpr bool _Iterator_is_contiguous = is_pointer_v<Iter>;

	template<class Iter1, class Iter2>
	constexpr bool _Iterators_are_contiguous = _Iterator_is_contiguous<Iter1> && _Iterator_is_contiguous<Iter2>;

	template<class Iter>
	constexpr bool _Iterator_is_volatile = is_volatile_v<remove_reference_t<_Iter_reference_t<Iter>>>;

	template<class Source, class Dest>
	constexpr bool _Is_pointer_address_convertible = is_void_v<Source> || is_void_v<Dest> ||
		is_same_v<remove_cv_t<Source>, remove_cv_t<Dest>>;

	template<class Tp, bool Is_enum = mstd::is_enum_v<Tp>>
	struct _Unwrap_enum {
		using type = underlying_type_t<Tp>;
	};

	template<class Tp>
	struct _Unwrap_enum<Tp, false> {
		using type = Tp;
	};

	template<class Tp>
	using _Unwarp_enum_t = typename _Unwrap_enum<Tp>::type;  // ??

	template<class Source, class Dest, class SourceRef, class DestRef>
	struct _Trivial_cat {
		using USource = _Unwarp_enum_t<Source>;  // 为什么要Unwrap_enum??
		using UDest = _Unwarp_enum_t<Dest>;

		static constexpr bool _Same_size_and_compatible = sizeof(Source) == sizeof(Dest)
			&& is_same_v<bool, USource> >= is_same_v<bool, UDest>
			&& (is_same_v<USource, UDest> || (is_intagral_v<USource> && is_intagral_v<UDest>) || (is_floating_point_v<USource> && is_floating_point_v<UDest>));

		static constexpr bool _Bitcopy_constructible = _Same_size_and_compatible
			&& is_trivially_constructible_v<Dest, SourceRef>;

		static constexpr bool _Bitcopy_assignable = _Same_size_and_compatible
			&& is_trivially_assignable<DestRef, SourceRef>;
	};

	template<class Source, class Dest, class SourceRef, class DestRef>
	struct _Trivial_cat<Source*, Dest*, SourceRef, DestRef> {

		static constexpr bool _Bitcopy_constructible = _Is_pointer_address_convertible<Source, Dest>
			&& is_trivially_constructible_v<Dest*, SourceRef>;

		static constexpr bool _Bitcopy_assignable = _Is_pointer_address_convertible<Source, Dest>
			&& is_trivially_assignable<DestRef, SourceRef>;
	};

	struct _False_trivial_cat {
		static constexpr bool _Bitcopy_constructible = false;
		static constexpr bool _Bitcopy_assignable = false;
	};

	template<class SourceIter, class DestIter, bool Are_contiguous = _Iterators_are_contiguous<SourceIter, DestIter> && !_Iterator_is_volatile<SourceIter> && !_Iterator_is_volatile<DestIter>>
	struct _Iter_copy_cat : _Trivial_cat<_Iter_value_t<SourceIter>, _Iter_value_t<DestIter>, _Iter_reference_t<DestIter>, _Iter_reference_t<SourceIter>> {};

	template<class SourceIter, class DestIter>
	struct _Iter_copy_cat<SourceIter, DestIter, false> : _False_trivial_cat {};

	template<class SourceIter, class DestIter>
	struct _Iter_copy_cat<move_iterator<SourceIter>, DestIter, false> : _Iter_copy_cat<SourceIter, DestIter> {};

	template<class SourceIter, class DestIter, bool Are_contiguous = _Iterators_are_contiguous<SourceIter, DestIter> && !_Iterator_is_volatile<SourceIter> && !_Iterator_is_volatile<DestIter>>
	struct _Iter_move_cat : _Trivial_cat<_Iter_value_t<SourceIter>, _Iter_value_t<DestIter>, remove_reference_t<_Iter_reference_t<DestIter>>&&, _Iter_reference_t<SourceIter>> {};

	template<class SourceIter, class DestIter>
	struct _Iter_move_cat<SourceIter, DestIter, false> : _False_trivial_cat {};

	template<class SourceIter, class DestIter>
	struct _Iter_move_cat<move_iterator<SourceIter>, DestIter, false> : _Iter_move_cat<SourceIter, DestIter> {};

	template<class InIter, class OutIter>
	OutIter _Copy_with_memmove(InIter first, InIter last, OutIter dest) {
		auto firstPtr = _To_address(first);
		auto lastPtr = _To_address(first);
		auto destPtr = _To_address(first);
		const char* const first_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(firstPtr));
		const char* const last_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(lastPtr));
		char* const dest_ch = const_cast<char*>(reinterpret_cast<const volatile char*>(destPtr));
		const auto conut = static_cast<size_t>(last_ch - first_ch);
		mstd::memmove(dest_ch, first_ch, count);
		if constexpr (is_pointer_v<OutIter>) {
			return reinterpret_cast<OutIter>(destPtr + count);
		}
		else {
			return dest + (lastPtr - firstPtr);
		}
	}

	template<class InIter, class OutIter>
	OutIter _Copy_n_with_memmove(InIter first, const size_t count, OutIter dest) {
		const auto result = _Copy_with_memmove(first, first + count, dest);
		if constexpr (is_pointer_v<OutIter>) {
			return result;
		}
		else {
			return dest + static_cast<_Iter_difference_t<OutIter>>(count);
		}
	}

	template<class InIter, class Sentinel, class OutIter>
	using _Sentinel_copy_cat = conditional_t<is_same_v<InIter, Sentinel>, _Iter_copy_cat<InIter, OutIter>, _False_trivial_cat>;

	template<class InIter, class Sentinel, class OutIter>
	inline OutIter _Copy_unchecked(InIter first, Sentinel last, OutIter dest) {
		if constexpr (_Sentinel_copy_cat<InIter, Sentinel, OutIter>::_Bitcopy_assignable) {
			return _Copy_with_memmove(first, last, dest);
		}
		for (; first != last; ++dest, ++first) {
			*dest = *first;
		}
		return dest;
	}

	struct _Distance_Unknow {
		constexpr _Distance_Unknow operator-() const noexcept { return {}; }
	};

	template<class Iter1, class Iter2>
	constexpr auto _Idl_distance(Iter2 first, Iter2 last) {
		if constexpr (_Is_ranges_rdm_iter_v<Iter2>) {
			return static_cast<_Iter_difference_t<Iter1>(last - first);
		}
		else {
			return _Distance_Unknow{};
		}
	}

	template<class InIter, class OutIter>
	inline OutIter copy(InIter first, InIter last, OutIter dest) {
		_Mstd_adl_verify_range(first, last);
		const auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);
		const auto udest = _Get_unwrapped_iter_n(dest, _Idl_distance<InIter>(last, last));
		_Seek_wrapped_iter(dest, _Copy_unchecked(ufirst, ulast, udest));
		return dest;
	}

	template <class Tp>
	using _Algorithm_int_t = conditional_t<is_integral_v<Tp>, Tp, ptrdiff_t>;

	template<class InIter, class Diff, class OutIter>
	inline OutIter copy_n(InIter first, const Diff count_raw, OutIter dest) {
		_Algorithm_int_t<Diff> count = count_raw;
		if (count > 0) {
			auto ufirst = _Get_unwrapped_iter_n(first, count);
			auto udest = _Get_unwrapped_iter_n(dest, count);
			if constexpr (_Iter_copy_cat<decltype(ufirst), decltype(ulast)>::_Bitcopy_assignable) {
				_Seek_wrapped_iter(dest, _Copy_n_with_memmove(ufirst, static_cast<size_t>(count), udest));
				return dest;
			}
			for (;;) {
				*udest = *ufirst;
				++udest;
				--count;
				if (count == 0) break;
				++ufirst;
			}
			_Seek_wrapped_iter(dest, udest);

		}
		return dest;
	}

	template<class BidIter1, class BidIter2>
	BidIter2 _Copy_backward_with_memmove(BidIter1 first, BidIter1 last, BidIter2 dest) {
		auto firstPtr = _To_address(first);
		auto lastPtr = _To_address(first);
		auto destPtr = _To_address(first);
		const char* const first_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(firstPtr));
		const char* const last_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(lastPtr));
		char* const dest_ch = const_cast<char*>(reinterpret_cast<const volatile char*>(destPtr));
		const auto count = static_cast<size_t>(last_ch - first_ch);
		auto result = mstd::memmove(dest_ch - count, first_ch, count);
		if constexpr (is_pointer_v<BidIter2>) {
			return reinterpret_cast<BidIter2>(result);
		}
		else {
			return dest + (lastPtr - firstPtr);
		}
	}

	template <class BidIter1, class BidIter2>
	BidIter2 _Copy_backward_with_memmove(move_iterator<BidIter1> first, move_iterator<BidIter1> last, BidIter2 dest) {
		return _Copy_backward_memmove(first.base(), last.base(), dest);
	}

	template<class BidIter1, class BidIter2>
	inline BidIter2 _Copy_backward_unchecked(BidIter1 first, BidIter1 last, BidIter2 dest) {
		if constexpr (_Iter_copy_cat<BidIter1, BidIter2>::_Bitcopy_assignable) {
			return _Copy_backward_memmove(first, last, dest);
		}
		while (first != last) {
			*--dest = *--last;
		}
		return dest;
	}

	template<class BidIter1, class BidIter2>
	inline BidIter2 copy_backward(BidIter1 first, BidIter1 last, BidIter2 dest) {
		_Mstd_adl_verify_range(first, last);
		const auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);
		const auto udest = _Get_unwrapped_iter_n(dest, -_Idl_distance<BidIter1>(ufirst, ulast));
		_Seek_wrapped_iter(dest, _Copy_backward_unchecked(ufirst, ulast, udest));
		return dest;
	}

	template<class InIter, class OutIter>
	inline OutIter _Move_unchecked(InIter first, InIter last, OutIter dest) {
		if constexpr (_Iter_move_cat<InIter, OutIter>::_Bitcopy_assignable) {
			return _Copy_with_memmove(first, last, dest);
		}
		for (; first != last; ++dest, ++first) {
			*dest = mstd::move(*first);
		}
		return dest;
	}

	template<class InIter, class OutIter>
	inline OutIter move(InIter first, InIter last, OutIter dest) {
		_Mstd_adl_verify_range(first, last);
		const auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);
		const auto udest = _Get_unwrapped_iter_n(dest, _Idl_distance<InIter>(ufirst, ulast));
		_Seek_wrapped_iter(dest, _Move_unchecked(ufirst, ulast, udest));
		return dest;
	}

	template<class InIter, class OutIter>
	inline OutIter _Move_backward_unchecked(InIter first, InIter last, OutIter dest) {
		if constexpr (_Iter_move_cat<InIter, OutIter>::_Bitcopy_assignable) {
			return _Copy_backward_with_memmove(first, last, dest);
		}
		while (first != last) {
			*--dest = mstd::move(*--last);
		}
		return dest;
	}

	template<class InIter, class OutIter>
	inline OutIter move_backward(InIter first, InIter last, OutIter dest) {
		_Mstd_adl_verify_range(first, last);
		const auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);
		const auto udest = _Get_unwrapped_iter_n(dest, -_Idl_distance<InIter>(ufirst, ulast));
		_Seek_wrapped_iter(dest, _Move_backward_unchecked(ufirst, ulast, udest));
		return dest;
	}

	template<class Tp>
	constexpr bool _Is_character_v = _is_any_of_v<remove_cv_t<Tp>, char, unsigned char, signed char>;

	template<class Tp>
	struct _Is_character : bool_constant<_Is_character_v<Tp>> {};

	template<class Tp>
	struct _Is_character_or_bool : _Is_character<Tp> {};

	template<>
	struct _Is_character_or_bool<bool> : true_type {};

	template<class Tp>
	struct _Is_character_or_byte_or_bool : _Is_character_or_bool<Tp> {};

	template<>
	struct _Is_character_or_byte_or_bool<std::byte> : true_type {};

	template<class FwdIter, class Tp, bool = _Iterator_is_contiguous<FwdIter>>
	constexpr bool _Fill_memset_is_safe = conjunction_v<is_scalar<Tp>, _Is_character_or_byte_or_bool<_Unwrap_enum_t<remove_reference_t<_Iter_reference_t<FwdIter>>>>, negation<is_volatile<remove_reference_t<_Iter_reference_t<FwdIter>>>>, is_assignable<_Iter_reference_t<FwdIter>, const Tp&>>;

	template<class FwdIter, class Tp>
	constexpr bool _Fill_memset_is_safe<FwdIter, Tp, false> = false;

	template<class FwdIter, class Tp, bool = _Iterator_is_contiguous<FwdIter>>
	constexpr bool _Fill_zero_memset_is_safe = conjunction_v<is_scalar<Tp>, is_scalar<_Iter_value_t<FwdIter>>, negation<is_member_pointer<_Iter_value_t<FwdIter>>>, negation<is_volatile<remove_reference_t<_Iter_reference_t<FwdIter>>>>, is_assignable<_Iter_reference_t<FwdIter>, const Tp&>>;

	template<class FwdIter, class Tp>
	constexpr bool _Fill_zero_memset_is_safe<FwdIter, Tp, false> = false;

	template<class Iter, class Tp>
	void _Fill_memset(Iter dest, const Tp val, const size_t count) {
		_Iter_value_t<Iter> dest_val = val;
		std::memset(_To_address(dest), static_cast<unsigned char>(dest_val), count);
	}

	template<class Iter>
	void _Fill_zero_memset(Iter dest, const size_t count) {
		std::memset(_To_address(dest), 0, count * sizeof(_Iter_value_t<Iter>));
	}

	template<class Tp>
	bool _Is_all_bits_zero(const Tp& val) {
		if constexpr (is_same_v<Tp, nullptr_t>) {
			return true;
		}
		else {
			constexpr Tp zero{};
			return std::memcmp(&val, &zero, sizeof(Tp)) == 0;
		}
	}

	template<class FwdIter, class Tp>
	inline void fill(const FwdIter first, const FwdIter last, const Tp& val) {
		_Mstd_adl_verify_range(first, last);
		auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);
		if constexpr (_Fill_memset_is_safe<decltype(ufirst), Tp>) {
			_Fill_memset(ufirst, val, static_cast<size_t>(ulast - ufirst));
			return;
		}
		else if constexpr (_Fill_zero_memset_is_safe<decltype(ufirst), Tp>) {
			if (_Is_all_bits_zero(val)) {
				_Fill_zero_memset(ufirst, static_cast<size_t>(ulast - ufirst));
				return;
			}
		}
		for (; ufirst != ulast; ++ufirst) {
			*ufirst = val;
		}
	}

	template<class OutIter, class Diff, class Tp>
	inline OutIter fill_n(OutIter dest, const Diff count_raw, const Tp& val) {
		_Algorithm_int_t<Diff> count = count_raw;
		if (count > 0) {
			auto udest = _Get_unwrapped_iter_n(dest, count);
			if constexpr (_Fill_memset_is_safe<decltype(udest), Tp>) {
				_Fill_memset(udest, val, static_cast<size_t>(count));
				_Seek_wrapped_iter(dest, udest + count);
				return dest;
			}
			else if constexpr (_Fill_zero_memset_is_safe<decltype(udest), Tp>) {
				if (_Is_all_bits_zero(val)) {
					_Fill_zero_memset(udest, static_cast<size_t>(count));
					_Seek_wrapped_iter(dest, udest + count);
					return dest;
				}
			}
			for (; 0 < count; --count, ++udest) {
				*udest = val;
			}
			_Seek_wrapped_iter(dest, udest);
		}
		return dest;
	}

	// 实现 ITERATOR_DEBUG 下迭代器核查器

	struct _Fake_allocator {};

	struct _Container_base_fake {
		void _Orphan_all() noexcept {}
		void _Swap_proxy_and_iterators(_Container_base_fake&) noexcept {}
		void _Alloc_proxy(const _Fake_allocator&) noexcept {}
		void _Reload_proxy(const _Fake_allocator&, const _Fake_allocator&) noexcept {}
	};

	struct _Iterator_base_fake {
		void _Adopt(const void*) noexcept {}
		const _Iterator_base_fake* _Getcont() const noexcept {
			return nullptr;
		}

		static constexpr bool _Unwrap_when_unverified = true;
	};

	struct _Container_base_real;
	struct _Iterator_base_real;

	struct _Container_proxy {
		_Container_proxy() noexcept = default;
		_Container_proxy(_Container_base_real* cont) noexcept : cont_(cont) {}

		const _Container_base_real* cont_ = nullptr;
		mutable _Iterator_base_real* firstiter_ = nullptr;
	};

	struct _Container_base_real {
	public:
		_Container_proxy* proxy_{};

		_Container_base_real() noexcept = default;
		_Container_base_real(const _Container_base_real&) = delete;
		_Container_base_real& operator=(const _Container_base_real&) = delete;

		// 重新分配内存时，迭代器会失效，将所有旧迭代器设置失效
		void _Orphan_all() noexcept {
			if (!proxy_) { return; }
			for (auto pnext = mstd::exchange(proxy_->firstiter_, nullptr); pnext; pnext = pnext->nextiter_) {
				pnext->proxy_ = nullptr;
			}
		}

		// 交换容器基类时，需要交换容器代理，以及容器代理中保存的容器指针
		void _Swap_proxy_and_iterators(_Container_base_real& right) noexcept {
			_Container_proxy* temp = proxy_;
			proxy_ = right.proxy_;
			right.proxy_ = temp;

			if (proxy_) {
				proxy_->cont_ = this;
			}

			if (right.proxy_) {
				right.proxy_->cont_ = &right;
			}
		}

		template <class Alloc>
		void _Alloc_proxy(Alloc&& alloc) {
			_Container_proxy* const new_proxy = _Unfancy(alloc.allocate(1));
			_Construct_in_place(*new_proxy, this);
			proxy_ = new_proxy;
			new_proxy->proxy_ = this;
		}

		template <class Alloc>
		void _Reload_proxy(Alloc&& old_alloc, Alloc&& new_alloc) {
			_Container_proxy* const new_proxy = _Unfancy(new_alloc.allocate(1));
			_Construct_in_place(*new_proxy, this);
			new_proxy->cont_ = this;
			_Delete_plain_internal(old_alloc, mstd::exchange(proxy_, new_proxy));
		}

	};

	struct _Iterator_base_real {
	public:
		_Container_proxy* proxy_{};
		_Iterator_base_real* nextiter_{};

		_Iterator_base_real() noexcept = default;

		_Iterator_base_real(const _Iterator_base_real& right) noexcept {
			*this = right;
		}

		_Iterator_base_real& operator=(const _Iterator_base_real& right) noexcept {
			_Assign(right);
			return *this;
		}

		~_Iterator_base_real() noexcept {
			_Orphan_me();
		}

		const _Container_base_real* _Getcont() const noexcept {
			return proxy_ ? proxy_->cont_ : nullptr;
		}

		// 迭代器基类初始化调用
		void _Adopt(const _Container_base_real* parent) noexcept {
			if (!parent) { _Orphan_me(); return; }
			_Container_proxy* parent_proxy = parent->proxy_;
			if (proxy_ != parent_proxy) {
				if (!proxy_) _Orphan_me();
				nextiter_ = parent_proxy->firstiter_;
				parent_proxy->firstiter_ = this;
				proxy_ = parent_proxy;
			}
		}

	private:

		void _Assign(const _Iterator_base_real& right) {
			if (proxy_ == right.proxy_) return;
			if (right.proxy_) {
				_Adopt(right.proxy_->cont_);
			}
			else {
				_Orphan_me();
			}
		}

		// 将自身从迭代器链表中移除，将proxy_置为空
		void _Orphan_me() {
			if (!proxy_) return;
			_Iterator_base_real** pnext = &proxy_->firstiter_;
			while (*pnext && *pnext != this) {
				pnext = &(*pnext)->nextiter_;
			}
			_MSTD_VERIFY(*pnext, "iterator list corrupted");
			*pnext = nextiter_;
			proxy_ = nullptr;
		}

	};

	// 容器基类代理器生成器，防止分配内存失败出现内存泄漏





	// 标签类，用于指示重载函数调用决议







	// 聚合类，用于节省存放对象内存空间（节省为了区分空类对象而设定的一定内存空间）


















}