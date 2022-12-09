#pragma once

#include <cstddef>
#include <string>
#include <iostream>

#include "m_type_traits.h"

namespace mstd {

	struct input_iterator_tag {};
	struct output_interator_tag {};

	struct forward_iterator_tag : input_iterator_tag {};
	struct bidirection_iterator_tag : forward_iterator_tag {};
	struct random_access_iterator_tag : bidirection_iterator_tag {};

	template<class Iter, class = void>
	struct _Iterator_traits_base {};

	template<class Iter>
	struct _Iterator_traits_base <Iter, void_t<typename Iter::iterator_category, typename Iter::value_type, typename Iter::pointer, typename Iter::reference, typename Iter::difference_type>> {

		using iterator_category = typename Iter::iterator_category;
		using value_type = typename Iter::value_type;
		using pointer = typename Iter::pointer;
		using reference = typename Iter::reference;
		using difference_type = typename Iter::difference_type;
	};

	template<class Tp, bool = is_object_v<Tp>>
	struct _Iterator_traits_pointer_base {
		using iterator_category = random_access_iterator_tag;
		using value_type = remove_cv_t<Tp>;
		using pointer = Tp*;
		using reference = Tp&;
		using difference_type = ptrdiff_t;
	};

	template<class Tp>
	struct _Iterator_traits_pointer_base<Tp, false> {};

	template<class Iter>
	struct iterator_traits : _Iterator_traits_base<Iter> {};

	template<class Tp>
	struct iterator_traits<Tp*> : _Iterator_traits_pointer_base<Tp> {};

	// 判断是否是迭代器 _Is_iterator_v
	template<typename Iter, typename = void>
	constexpr bool _Is_iterator_v = false;

	template<typename Iter>
	constexpr bool _Is_iterator_v<Iter, void_t<typename iterator_traits<Iter>::iterator_category>> = true;

	template<typename Iter>
	struct _Is_iterator : bool_constant<_Is_iterator_v<Iter>> {};

	template<class Iter>
	using _Iter_category_t = typename iterator_traits<Iter>::iterator_category;

	template<class Iter>
	using _Iter_value_t = typename iterator_traits<Iter>::value_type;

	template<class Iter>
	using _Iter_difference_t = typename iterator_traits<Iter>::difference_type;

	template<class Iter>
	using _Iter_pointer_t = typename iterator_traits<Iter>::pointer;

	template<class Iter>
	using _Iter_reference_t = typename iterator_traits<Iter>::reference;

	template<class... Iters>
	using _Common_difference_t = mstd::common_type_t<_Iter_difference_t<Iters>...>;

	template<class Iter>
	constexpr bool _Is_ranges_ipt_iter_v = mstd::is_convertible<_Iter_category_t<Iter>, input_iterator_tag>;

	template<class Iter>
	constexpr bool _Is_ranges_fwd_iter_v = mstd::is_convertible<_Iter_category_t<Iter>, forward_iterator_tag>;

	template<class Iter>
	constexpr bool _Is_ranges_bid_iter_v = mstd::is_convertible<_Iter_category_t<Iter>, bidirection_iterator_tag>;

	template<class Iter>
	constexpr bool _Is_ranges_rdm_iter_v = mstd::is_convertible<_Iter_category_t<Iter>, random_access_iterator_tag>;

	// 定义 _MSTD_VERIFY()

	inline void _Verify_fail(const char* mesg, const char* file, int line) {
		std::string error_mesg = std::string("Verify Fail: ") + mesg + ", at file " + file + " in line " + std::to_string(line) + ".";
		std::cout << error_mesg << std::endl;
		std::terminate();
	}

#define _MSTD_VERIFY(cond, mesg)							\
	do {													\
		if (!(cond)) {										\
			mstd::_Verify_fail(mesg, __FILE__, __LINE__);	\
		}													\
	} while (false)

#define _ITERATOR_DEBUG 1

#if _ITERATOR_DEBUG
	// 指针迭代器偏特化
	template<class Tp>
	constexpr void _Verify_range(const Tp* const first, const Tp* const last) noexcept {
		_MSTD_VERIFY(first < last, "transposed pointer range");
	}

#endif // _ITERATOR_DEBUG

	template<class Iter, class = void>
	constexpr bool _Allow_inheriting_unwrap_v = false;

	// 容器迭代器 定义 _Prevent_inheriting_unwarp
	template<class Iter>
	constexpr bool _Allow_inheriting_unwrap_v<Iter, void_t<typename Iter::_Prevent_inheriting_unwrap>> = mstd::is_same_v<Iter, typename Iter::_Prevent_inheriting_unwrap>;

	template<class Iter, class Sentinel = Iter, class = void>
	constexpr bool _Range_verifiable_v = false;

	template<class Iter, class Sentinel>	// 多定义一个 Sentinel 类型 ？
	constexpr bool _Range_verifiable_v<Iter, Sentinel, void_t<decltype(_Verify_range(mstd::declval<const Iter&>(), mstd::declval<const Sentinel&>()))>> = _Allow_inheriting_unwrap_v<Iter>;

	// ADL(Argument Dependent Lookup) 参数依赖查找?

	template<class Iter, class Sentinel>
	inline constexpr void _Mstd_adl_verify_range(const Iter& first, const Sentinel& last) {
		if constexpr (_Range_verifiable_v<Iter, Sentinel>) {
			_Verify_range(first, last);	 // 会调用迭代器定义的_Verify_range()友元重载函数
		}
	}

	template<class Iter, class = void>
	constexpr bool _Unwrappable_v = false;

	template<class Iter>
	constexpr bool _Unwrappable_v<Iter, void_t<decltype(mstd::declval<_Remove_cvref_t<Iter>&>()._Wrapped_iter(mstd::declval<Iter>()._Unwrapped_iter()))>> = _Allow_inheriting_unwrap_v<_Remove_cvref_t<Iter>>;

	template<class Iter, class = void>
	constexpr bool _Has_nothrow_unwrapped = false;

	template<class Iter>
	constexpr bool _Has_nothrow_unwrapped<Iter, void_t<decltype(mstd::declval<Iter>()._Unwrapped_iter())>> = noexcept(mstd::declval<Iter>()._Unwrapped_iter());

	template<class Iter>
	inline constexpr decltype(auto) _Get_unwrapped_iter(Iter&& iter) noexcept(!_Unwrappable_v<Iter> || _Has_nothrow_unwrapped<Iter>) {
		if constexpr (is_pointer_v<mstd::decay_t<Iter>>) {
			return iter + 0;
		}
		else if constexpr (_Unwrappable_v<Iter>) {
			return static_cast<Iter&&>(iter)._Unwrapped_iter();
		}
		else {
			return static_cast<Iter&&>(iter);
		}
	}

	template<class Iter>
	using _Unwrapped_t = _Remove_cvref_t<decltype(mstd::_Get_unwrapped_iter(mstd::declval<Iter>()))>;

	template<class Iter, class = bool>
	constexpr bool _Do_unwrap_when_unverified_v = false;

	template<class Iter>
	constexpr bool _Do_unwrap_when_unverified_v<Iter, decltype(static_cast<bool>(Iter::_Unwrap_when_unverified))> = static_cast<bool>(Iter::_Unwrap_when_unverified);

	template<class Iter>
	constexpr bool _Unwrappable_for_unverified_v = _Unwrappable_v<Iter> && _Do_unwrap_when_unverified_v<_Remove_cvref_t<Iter>>;

	template<class Iter>
	inline constexpr decltype(auto) _Get_unwrapped_iter_when_unverified(Iter&& iter) {
		if constexpr (is_pointer_v<Iter>) {
			return iter + 0;
		}
		else if constexpr (_Unwrappable_for_unverified_v<Iter>) {
			return static_cast<Iter&&>(iter)._Unwrapped_iter();
		}
		else {
			return static_cast<Iter&&>(iter);
		}
	}

	template<class Iter>
	using _Unwrapped_unverified_t = _Remove_cvref_t<decltype(_Get_unwrapped_iter_when_unverified(mstd::declval<Iter>()))>;

	template<class Iter, class = void>
	constexpr bool _Offset_verifiable_v = false;

	template<class Iter>
	constexpr bool _Offset_verifiable_v<Iter, void_t<decltype(mstd::declval<const Iter&>()._Verify_offset(mstd::declval<const _Iter_difference_t<Iter>>()))>> = true;

	template<class Iter>
	constexpr bool _Unwrappable_for_offset_v = _Unwrappable_v<Iter> && _Offset_verifiable_v<Iter>;

	template<class Diff>
	constexpr Diff _Max_possible_v{ static_cast<mstd::make_unsigned_t<Diff>>(-1) >> 1 };

	template<class Diff>
	constexpr Diff _Min_possible_v{ -_Max_possible_v<Diff> -1 };

	template<class Iter, class Diff>
	inline constexpr decltype(auto) _Get_unwrapped_iter_n(Iter&& iter, const Diff off) {
		if constexpr (is_pointer_v<Iter>)
			return iter + 0;
		else if constexpr (_Unwrappable_for_offset_v<Iter> && is_integral_v<Diff>) {
			using Iter_diff = _Iter_difference_t<_Remove_cvref_t<Iter>>;
			using Commom_diff = common_type_t<Iter_diff, Diff>;

			const auto Coff = static_cast<Commom_diff>(off);
			_MSTD_VERIFY(Coff <= static_cast<Commom_diff>(_Max_possible_v<Iter_diff>) && (is_unsigned_v<Diff> || static_cast<Commom_diff>(_Min_possible_v<Iter_diff>) <= Coff), "Integer overflow");

			iter._Verify_offset(static_cast<Iter_diff>(off));
			return static_cast<Iter&&>(iter)._Unwrapped_iter();
		}
		else if constexpr (_Unwrappable_for_unverified_v<Iter>) {
			return static_cast<Iter&&>(iter)._Unwrapped_iter();
		}
		else {
			return static_cast<Iter&&>(iter);
		}
	}

	template<class Iter, class UIter, class = void>
	constexpr bool _Wrapped_seekable_v = false;

	template<class Iter, class UIter>
	constexpr bool _Wrapped_seekable_v<Iter, UIter, void_t<decltype(mstd::declval<Iter&>()._Wrapped_iter(mstd::declval<UIter&&>()))>> = true;

	template<class Iter, class UIter>
	inline constexpr void _Seek_wrapped_iter(Iter& iter, UIter&& uiter) {
		if constexpr (_Wrapped_seekable_v<Iter, UIter>) {
			iter._Wrapped_iter(uiter);
		}
		else {
			iter = mstd::forward<UIter>(uiter);
		}
	}

	template<class IptIter, class Diff>
	inline void advance(IptIter& iter, const Diff off) {
		if constexpr (_Is_ranges_rdm_iter_v<IptIter>) {
			iter += off;
		}
		else {
			if constexpr (is_signed_v<Diff> && !_Is_ranges_bid_iter_v<IptIter>) {
				_MSTD_VERIFY(off >= 0, "Non-bidirectional iterator can not be decrease by advance.");
			}
			decltype(auto) unwrapped_iter = _Get_unwrapped_iter_n(iter, off);
			constexpr bool need_rewrap = !is_reference_v<decltype(_Get_unwrapped_iter_n(mstd::move(iter), off))>;
			if constexpr (is_signed_v<Diff> && _Is_ranges_bid_iter_v<IptIter>) {
				for (; off < 0; ++off) {
					--unwrapped_iter;
				}
			}
			for (; off > 0; --off) {
				++unwrapped_iter;
			}
			if constexpr (need_rewrap) {
				_Seek_wrapped_iter(iter, mstd::move(unwrapped_iter));
			}
		}
	}

	template<class IptIter>
	inline _Iter_difference_t<IptIter> distance(IptIter first, IptIter last) {
		if constexpr (_Is_ranges_rdm_iter_v<IptIter>) {
			return last - first;	// 迭代器需自己定义检查范围是否合法
		}
		else {
			_Mstd_adl_verify_range(first, last);

			auto ufirst = _Get_unwrapped_iter(first);
			const auto ulast = _Get_unwrapped_iter(last);
			_Iter_category_t<IptIter> off = 0;
			for (; ufirst != ulast; ++ufirst) {
				++off;
			}
			return off;
		}
	}

	template<class IptIter>
	inline constexpr IptIter next(IptIter first, _Iter_difference_t<IptIter> off = 1) {
		static_assert(_Is_ranges_ipt_iter_v<IptIter>, "The next() requires input iterator.");
		mstd::advance(first, off);
		return first;
	}

	template<class BidIter>
	inline constexpr BidIter prev(BidIter first, _Iter_difference_t<BidIter> off = 1) {
		static_assert(_Is_ranges_bid_iter_v<BidIter>, "The prev() requires bidirectional iterator.");
		mstd::advance(first, -off);
		return first;
	}

	template<class Iter, class Pointer, bool = mstd::is_pointer_v<Iter>>
	constexpr bool _Has_nothrow_operator_arrow = false;

	template<class Iter, class Pointer>
	constexpr bool _Has_nothrow_operator_arrow<Iter, Pointer, false> = noexcept(_Fake_copy_init<Pointer>(mstd::declval<Iter&>().operator->()));


	// move_iterator







	// reverse_iterator 

	template<class BidIter>
	class reverse_iterator {
	public:
		using iterator_type = BidIter;

		using iterator_category = _Iter_category_t<BidIter>;
		using value_type = _Iter_value_t<BidIter>;
		using pointer = _Iter_pointer_t<BidIter>;
		using reference = _Iter_reference_t<BidIter>;
		using difference_type = _Iter_difference_t<BidIter>;

	protected:
		BidIter base_iter_{};

	public:
		template<class>
		friend class reverse_iterator;

		constexpr reverse_iterator() = default;
		explicit reverse_iterator(BidIter iter) noexcept(mstd::is_nothrow_move_constructible<BidIter>::value) : base_iter_(mstd::move(iter)) {}

		template<class Other>
		reverse_iterator(const reverse_iterator<Other>& right) noexcept(mstd::is_nothrow_constructible<BidIter, const Other&>::value) : base_iter_(right.base_iter_) {}

		reverse_iterator& operator=(const reverse_iterator& right) noexcept(mstd::is_nothrow_assignable<BidIter, const BidIter&>::value) {
			base_iter_ = right.base_iter_;
			return *this;
		}

		BidIter base() const
			noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value) {
			return base_iter_;
		}

		reference operator*() const noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value && noexcept(*--(mstd::declval<BidIter&>()))) {
			BidIter temp = base_iter_;
			return *--temp;
		}

		pointer operator->() const noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value && noexcept(--(mstd::declval<BidIter&>())) && _Has_nothrow_operator_arrow<BidIter&, pointer>) {
			BidIter temp = base_iter_;
			--temp;
			if constexpr (mstd::is_pointer_v<BidIter>) {
				return base_iter_;
			}
			else {
				return base_iter_.operator->();
			}
		}

		reverse_iterator& operator++() noexcept(noexcept(--base_iter_)) {
			--base_iter_;
			return *this;
		}

		reverse_iterator operator++(int) noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value && noexcept(--base_iter_)) {
			reverse_iterator temp = *this;
			--base_iter_;
			return temp;
		}

		reverse_iterator& operator--() noexcept(noexcept(++base_iter_)) {
			++base_iter_;
			return *this;
		}

		reverse_iterator operator--(int) noexcept(noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value && noexcept(++base_iter_))) {
			reverse_iterator temp = *this;
			++base_iter_;
			return temp;
		}

		reverse_iterator& operator+=(const difference_type off) noexcept(noexcept(base_iter_ -= off)) {
			base_iter_ -= off;
			return *this;
		}

		reverse_iterator& operator-=(const difference_type off) noexcept(base_iter_ += off) {
			base_iter_ += off;
			return *this;
		}

		reverse_iterator operator-(const difference_type off) const noexcept(noexcept(reverse_iterator(base_iter_ + off))) {
			return reverse_iterator(base_iter_ + off);
		}

		reverse_iterator operator+(const difference_type off) const noexcept(noexcept(reverse_iterator(base_iter_ + off))) {
			return reverse_iterator(base_iter_ - off);
		}

		reference operator[](const difference_type off) const noexcept(noexcept(_Fake_copy_init<reference>(base_iter_[off]))) {
			return base_iter_[-off - 1];
		}

















		constexpr const BidIter& _Get_base_iter() const noexcept { return base_iter_; }
	};

	template<class BidIter>
	reverse_iterator<BidIter>
		operator+(const typename BidIter::difference_type off,
			const reverse_iterator<BidIter>& right) {
		return reverse_iterator<BidIter>(right + off);
	}

	template<class BidIter>
	typename reverse_iterator<BidIter>::difference_type
		operator-(const reverse_iterator<BidIter>& left,
			const reverse_iterator<BidIter>& right) {
		return left._Get_base_iter() - right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator<(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left._Get_base_iter() > right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator>(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left._Get_base_iter() < right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator<=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left._Get_base_iter() >= right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator>=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left._Get_base_iter() <= right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator==(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left._Get_base_iter() == right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator!=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left._Get_base_iter() != right._Get_base_iter();
	}
}


namespace gnu_iterator {

	struct output_iterator_tag {};
	struct input_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template<typename Category, typename Tp, typename Distance = ptrdiff_t,
		typename Pointer = Tp*, typename Reference = Tp&>
	struct iterator
	{
		using iterator_category = Category;
		using value_type = Tp;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};

	template<typename Category, typename Tp, typename Distance = ptrdiff_t,
		typename Pointer = const Tp*, typename Reference = const Tp&>
	struct const_iterator
	{
		using iterator_category = Category;
		using value_type = Tp;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};

	template<typename Iter>
	struct iterator_traits
	{
		using iterator_category = typename Iter::iterator_category;
		using value_type = typename Iter::value_type;
		using difference_type = typename Iter::difference_type;
		using pointer = typename Iter::pointer;
		using reference = typename Iter::reference;
	};


	template<typename Tp>
	struct iterator_traits<Tp*>
	{
		using iterator_category = random_access_iterator_tag;
		using value_type = Tp;
		using difference_type = ptrdiff_t;
		using Pointer = Tp*;
		using reference = Tp&;
	};

	template<typename Tp>
	struct iterator_traits<const Tp*>
	{
		using iterator_category = random_access_iterator_tag;
		using value_type = Tp;
		using difference_type = ptrdiff_t;
		using Pointer = Tp*;
		using reference = Tp&;
	};

	template<typename Iter>
	inline typename iterator_traits<Iter>::value_type*
		iterator_value_type(Iter it) {
		using value_type = typename iterator_traits<Iter>::value_type;
		return static_cast<value_type*>(0);
	}

	template<typename Iter>
	inline typename iterator_traits<Iter>::iterator_category
		iterator_category(Iter it)
	{
		using category = typename iterator_traits<Iter>::iterator_category;
		return category();
	}

	template<typename IptIter>
	inline typename iterator_traits<IptIter>::difference_type
		__distance(IptIter first, IptIter last, input_iterator_tag)
	{
		typename iterator_traits<IptIter>::difference_type n = 0;
		for (; first < last; ++first) {
			n += 1;
		}
		return n;
	}

	template<typename RdmIter>
	inline typename iterator_traits<RdmIter>::difference_type
		__distance(RdmIter first, RdmIter last, random_access_iterator_tag)
	{
		return last - first;
	}

	template<typename IptIter>
	inline typename iterator_traits<IptIter>::difference_type
		iterator_distance(IptIter first, IptIter last)
	{
		using category = typename iterator_traits<IptIter>::iterator_category;
		return __distance(first, last, category());
	}


	template<typename IptIter, typename Distance = ptrdiff_t>
	inline void __advance(IptIter& first, Distance n, input_iterator_tag)
	{
		for (; n > 0; --n, ++first);
	}

	template<typename IptIter, typename Distance = ptrdiff_t>
	inline void __advance(IptIter& first, Distance n, bidirectional_iterator_tag)
	{
		if (n > 0) {
			for (; n > 0; --n, ++first);
		}
		else {
			for (; n < 0; ++n, ++first);
		}
	}

	template<typename IptIter, typename Distance = ptrdiff_t>
	inline void __advance(IptIter& first, Distance n, random_access_iterator_tag)
	{
		first += n;
	}

	template<typename IptIter, typename Distance = ptrdiff_t>
	inline void iterator_advance(IptIter& first, Distance n)
	{
		using category = typename iterator_traits<IptIter>::iterator_category;
		__advance(first, n, category());
	}
}