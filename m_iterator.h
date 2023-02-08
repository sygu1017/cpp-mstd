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
		std::string error_mesg = std::string("Verify fail: ") + mesg + ", at file " + file + " in line " + std::to_string(line) + ".";
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

	// 容器迭代器 定义 _Prevent_inheriting_unwarp，is_same_v<>防止继承性的可解包装
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
			_MSTD_VERIFY(Coff <= static_cast<Commom_diff>(_Max_possible_v<Iter_diff>) && (is_unsigned_v<Diff> || static_cast<Commom_diff>(_Min_possible_v<Iter_diff>) <= Coff), "integer overflow");

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
	constexpr void _Seek_wrapped_iter(Iter& iter, UIter&& uiter) {
		if constexpr (_Wrapped_seekable_v<Iter, UIter>) {
			iter._Wrapped_iter(uiter);
		}
		else {
			iter = mstd::forward<UIter>(uiter);
		}
	}

	template<class IptIter, class Diff>
	constexpr void advance(IptIter& iter, const Diff off) {
		if constexpr (_Is_ranges_rdm_iter_v<IptIter>) {
			iter += off;
		}
		else {
			if constexpr (is_signed_v<Diff> && !_Is_ranges_bid_iter_v<IptIter>) {
				_MSTD_VERIFY(off >= 0, "non-bidirectional iterator can not be decrease by advance");
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
	constexpr _Iter_difference_t<IptIter> distance(IptIter first, IptIter last) {
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
	constexpr IptIter next(IptIter first, _Iter_difference_t<IptIter> off = 1) {
		static_assert(_Is_ranges_ipt_iter_v<IptIter>, "next requires input iterator");
		mstd::advance(first, off);
		return first;
	}

	template<class BidIter>
	constexpr BidIter prev(BidIter first, _Iter_difference_t<BidIter> off = 1) {
		static_assert(_Is_ranges_bid_iter_v<BidIter>, "prev requires bidirectional iterator");
		mstd::advance(first, -off);
		return first;
	}

	template<class Iter, class Pointer, bool = mstd::is_pointer_v<Iter>>
	constexpr bool _Has_nothrow_operator_arrow = false;

	template<class Iter, class Pointer>
	constexpr bool _Has_nothrow_operator_arrow<Iter, Pointer, false> = noexcept(_Fake_copy_init<Pointer>(mstd::declval<Iter&>().operator->()));

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

	public:
		template<class>
		friend class reverse_iterator;

		constexpr reverse_iterator() = default;
		constexpr explicit reverse_iterator(BidIter iter) // 传递参数异常 是否影响noexcept?
			noexcept(mstd::is_nothrow_move_constructible<BidIter>::value)
			: base_iter_(mstd::move(iter)) {}

		template<class Other>
		constexpr reverse_iterator(const reverse_iterator<Other>& right)
			noexcept(mstd::is_nothrow_constructible<BidIter, const Other&>::value)
			: base_iter_(right.base_iter_) {}

		constexpr reverse_iterator& operator=(const reverse_iterator& right)
			noexcept(mstd::is_nothrow_assignable<BidIter, const BidIter&>::value) {
			base_iter_ = right.base_iter_;
			return *this;
		}

		constexpr BidIter base() const
			noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value) {
			return base_iter_;
		}

		constexpr reference operator*() const
			noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value
				&& noexcept(*--(mstd::declval<BidIter&>()))) {
			BidIter temp = base_iter_;
			return *--temp;
		}

		constexpr pointer operator->() const
			noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value
				&& noexcept(--(mstd::declval<BidIter&>()))
				&& _Has_nothrow_operator_arrow<BidIter&, pointer>) {

			BidIter temp = base_iter_;
			--temp;
			if constexpr (mstd::is_pointer_v<BidIter>) {
				return base_iter_;
			}
			else {
				return base_iter_.operator->();
			}
		}

		constexpr reverse_iterator& operator++() noexcept(noexcept(--base_iter_)) {
			--base_iter_;
			return *this;
		}

		constexpr reverse_iterator operator++(int)
			noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value
				&& noexcept(--base_iter_)) {
			reverse_iterator temp = *this;
			--base_iter_;
			return temp;
		}

		constexpr reverse_iterator& operator--() noexcept(noexcept(++base_iter_)) {
			++base_iter_;
			return *this;
		}

		constexpr reverse_iterator operator--(int)
			noexcept(noexcept(mstd::is_nothrow_copy_constructible<BidIter>::value
				&& noexcept(++base_iter_))) {
			reverse_iterator temp = *this;
			++base_iter_;
			return temp;
		}

		constexpr reverse_iterator& operator+=(const difference_type off)
			noexcept(noexcept(base_iter_ -= off)) {
			base_iter_ -= off;
			return *this;
		}

		constexpr reverse_iterator& operator-=(const difference_type off)
			noexcept(base_iter_ += off) {
			base_iter_ += off;
			return *this;
		}

		constexpr reverse_iterator operator-(const difference_type off) const
			noexcept(noexcept(reverse_iterator(base_iter_ + off))) {
			return reverse_iterator(base_iter_ + off);
		}

		constexpr reverse_iterator operator+(const difference_type off) const
			noexcept(noexcept(reverse_iterator(base_iter_ + off))) {
			return reverse_iterator(base_iter_ - off);
		}

		constexpr reference operator[](const difference_type off) const
			noexcept(noexcept(_Fake_copy_init<reference>(base_iter_[off]))) {
			return base_iter_[static_cast<difference_type>(-off - 1)];
		}

		using _Prevent_inheriting_unwrap = reverse_iterator;

		template<class BidIter2,
			mstd::enable_if_t<_Range_verifiable_v<BidIter, BidIter2>, int> = 0>
		friend constexpr void _Verify_range(const reverse_iterator& first, const reverse_iterator& last) noexcept {
			_Verify_range(last._Get_base_iter(), base_iter_);
		}

		template<class BidIter2 = BidIter,
			mstd::enable_if_t<_Offset_verifiable_v<BidIter2>, int> = 0>
		constexpr void _Verify_offset(const difference_type off) noexcept {
			_MSTD_VERIFY(off != _Min_possible_v<difference_type>, "integer overflow");
			base_iter_._Verify_offset(-off);
		}

		static constexpr bool _Unwrap_when_unverified = _Do_unwrap_when_unverified_v<BidIter>;

		template<class BidIter2 = BidIter,
			mstd::enable_if_t<_Unwrappable_v<BidIter2>, int> = 0>
		constexpr reverse_iterator<_Unwrapped_t<BidIter>> _Unwrapped_iter() &&
			noexcept(noexcept(static_cast<reverse_iterator<_Unwrapped_t<BidIter>>>
				(mstd::move(base_iter_)._Unwrapped_iter()))) {
			return static_cast<reverse_iterator<_Unwrapped_t<BidIter>>>(mstd::move
			(base_iter_)._Unwrapped_iter());
		}

		template<class Source,
			enable_if_t<_Wrapped_seekable_v<BidIter, const Source&>, int> = 0>
		constexpr void _Wrapped_iter(const reverse_iterator<Source>& iter)
			noexcept(noexcept(base_iter_._Wrapped_iter(iter.base_iter_))) {
			base_iter_._Wrapped_iter(iter.base_iter_);
		}

		constexpr const BidIter& _Get_base_iter() const noexcept { return base_iter_; }

	protected:
		BidIter base_iter_{};
	};

	template<class BidIter>
	constexpr reverse_iterator<BidIter>
		operator+(const typename BidIter::difference_type off,
			const reverse_iterator<BidIter>& right) noexcept(noexcept
				(reverse_iterator<BidIter>(right + off))) {
		return reverse_iterator<BidIter>(right + off);
	}

	template<class BidIter1, class BidIter2>
	constexpr auto operator-(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) noexcept(noexcept(left._Get_base_iter()
			- right._Get_base_iter()))
		->decltype(left._Get_base_iter() - right._Get_base_iter()) {
		return left._Get_base_iter() - right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	constexpr bool operator<(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) noexcept(noexcept(left._Get_base_iter()
	> right._Get_base_iter())) {
		return left._Get_base_iter() > right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	constexpr bool operator>(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) noexcept(noexcept(left._Get_base_iter()
			< right._Get_base_iter())) {
		return left._Get_base_iter() < right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	constexpr bool operator<=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) noexcept(noexcept(left._Get_base_iter()
			>= right._Get_base_iter())) {
		return left._Get_base_iter() >= right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	constexpr bool operator>=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) noexcept(noexcept(left._Get_base_iter()
			<= right._Get_base_iter())) {
		return left._Get_base_iter() <= right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	constexpr bool operator==(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) noexcept(noexcept(_Fake_copy_init<bool>
			(left._Get_base_iter() == right._Get_base_iter()))) {
		return left._Get_base_iter() == right._Get_base_iter();
	}

	template<class BidIter1, class BidIter2>
	constexpr bool operator!=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) noexcept(noexcept(left._Get_base_iter
		() != right._Get_base_iter())) {
		return left._Get_base_iter() != right._Get_base_iter();
	}

	template<class BidIter>
	constexpr reverse_iterator<BidIter> make_reverse_iterator(BidIter iter)
		noexcept(mstd::is_nothrow_move_constructible<BidIter>::value) {
		return reverse_iterator<BidIter>(mstd::move(iter));
	}

	struct _Default_sentinel {};  // ??

	// move_iterator

	template<class Iter>
	class move_iterator {
	public:
		using iterator_type = Iter;
		using iterator_category = _Iter_category_t<Iter>;

		using value_type = _Iter_value_t<Iter>;
		using pointer = Iter; // ??
		using difference_type = _Iter_difference_t<Iter>;
		using reference = mstd::conditional_t<mstd::is_reference_v<_Iter_ref_t<Iter>>,
			mstd::remove_reference_t<_Iter_reference_t<Iter>>&&, _Iter_reference_t<Iter>>;

		constexpr move_iterator() = default;
		constexpr explicit move_iterator(Iter iter)
			noexcept(mstd::is_nothrow_move_constructible<Iter>::value)
			: base_iter_(mstd::move(iter)) {}

		template<class Other>
		constexpr move_iterator(const move_iterator<Other>& other) noexcept
			(mstd::is_nothrow_copy_constructible<Iter, const Other&>::value) : base_iter_(other.base()) {}

		template<class Other>
		constexpr move_iterator& operator=(const move_iterator<Other>& other)
			noexcept(mstd::is_nothrow_assignable<Iter, const Other&>::value) {
			base_iter_ = other.base();
			return *this;
		}

		constexpr iterator_type base() const
			noexcept(mstd::is_nothrow_copy_constructible<Iter>::value) {
			return base_iter_;
		}

		constexpr reference operator*() const noexcept(noexcept(static_cast<reference>(*base_iter_))) {
			return static_cast<reference>(*base_iter_);
		}

		constexpr pointer operator->() const noexcept(mstd::is_nothrow_copy_constructible<Iter>::value) {
			return base_iter_;
		}

		constexpr move_iterator& operator++() const noexcept(noexcept(++base_iter_)) {
			++base_iter_;
			return *this;
		}

		constexpr move_iterator operator++(int) const
			noexcept(mstd::is_nothrow_copy_constructible<Iter>::value && noexcept(++base_iter_)) {
			iterator_type temp = *this;
			++base_iter_;
			return temp;
		}

		constexpr move_iterator& operator--() const noexcept(noexcept(--base_iter_)) {
			--base_iter_;
			return *this;
		}

		constexpr move_iterator operator--(int) const
			noexcept(mstd::is_nothrow_copy_constructible<Iter>::value && noexcept(--base_iter_)) {
			iterator_category temp = *this;
			--base_iter_;
			return temp;
		}

		template<class Iter2 = Iter>
		constexpr auto operator==(_Default_sentinel sentinel) const noexcept
			->decltype(mstd::declval<const Iter2&>() == sentinel) {
			return base_iter_ == sentinel;
		}

		template<class Iter2 = Iter>
		constexpr auto operator==(_Default_sentinel sentinel) const noexcept
			->decltype(mstd::declval<const Iter2&>() != sentinel) {
			return base_iter_ != sentinel;
		}

		constexpr move_iterator operator+(const difference_type off) const
			noexcept(noexcept(move_iterator(base_iter_ + off))) {
			return move_iterator(base_iter_ + off);
		}

		constexpr move_iterator& operator+=(const difference_type off)
			noexcept(noexcept(base_iter_ += off)) {
			base_iter_ += off;
			return *this;
		}

		constexpr move_iterator operator-(const difference_type off) const
			noexcept(noexcept(move_iterator(base_iter_ - off))) {
			return move_iterator(base_iter_ - off);
		}

		constexpr move_iterator& operator-=(const difference_type off)
			noexcept(noexcept(base_iter_ -= off)) {
			base_iter_ -= off;
			return *this;
		}

		constexpr reference operator[](const difference_type off) const
			noexcept(mstd::move(base_iter_[off])) {
			return mstd::move(base_iter_[off]);
		}

		template<class Iter2, enable_if_t<_Range_verifiable_v<Iter, Iter2>, int> = 0>
		friend constexpr void _Verify_range(const move_iterator<Iter>& first, const move_iterator<Iter2>& last) noexcept {
			_Verify_range(first._Get_base_iter(), last._Get_base_iter());
		}

		using _Prevent_inheriting_unwrap = move_iterator;

		template<class Iter2 = iterator_type, enable_if_t<_Offset_verifiable_v<Iter2>, int> = 0>
		constexpr void _Verify_offset(const difference_type off) const noexcept {
			base_iter_._Verify_offset(off);
		}


		template<class Iter2 = iterator_type, enable_if_t<_Unwrappable_v<const Iter2&>, int> = 0>
		constexpr move_iterator<_Unwrapped_t<const Iter2&>> _Unwrapped_iter() const&
			noexcept(static_cast<move_iterator<_Unwrapped_t<const Iter2&>>>(base_iter_._Unwrapped_iter())) {
			return static_cast<move_iterator<_Unwrapped_t<const Iter2&>>>(base_iter_._Unwrapped_iter());
		}

		template<class Iter2 = iterator_type, enable_if_t<_Unwrappable_v<Iter2>, int> = 0>
		constexpr move_iterator<_Unwrapped_t<Iter2>> _Unwrapped_iter() &&
			noexcept(static_cast<move_iterator<_Unwrapped_t<Iter2>>>(mstd::move(base_iter_)._Unwrapped_iter())) {
			return static_cast<move_iterator<_Unwrapped_t<Iter2>>>(mstd::move(base_iter_)._Unwrapped_iter());
		}

		static constexpr bool _Unwrap_when_unverified = _Do_unwrap_when_unverified_v<iterator_type>;

		template<class Src, enable_if_t<_Wrapped_seekable_v<iterator_type, const Src&>, int> = 0>
		constexpr void _Wrapped_iter(const move_iterator<Src>& iter)
			noexcept(base_iter_._Wrapped_iter(iter._Get_base_iter())) {
			base_iter_._Wrapped_iter(iter._Get_base_iter());
		}

		template<class Src, enable_if_t<_Wrapped_seekable_v<iterator_type, Src>, int> = 0>
		constexpr void _Wrapped_iter(move_iterator<Src>&& iter)
			noexcept(base_iter_._Wrapped_iter(mstd::move(iter)._Get_base_iter())) {
			base_iter_._Wrapped_iter(mstd::move(iter)._Get_base_iter());
		}

		constexpr const iterator_type& _Get_base_iter() const& noexcept {
			return base_iter_;
		}

		constexpr iterator_type _Get_base_iter() && noexcept {
			return mstd::move(base_iter_);
		}

	protected:
		Iter base_iter_{};
	};

	template<class Iter1, class Iter2>
	constexpr bool operator==(const move_iterator<Iter1>& left, const move_iterator<Iter2>& right)
		noexcept(noexcept(_Fake_copy_init<bool>(left._Get_base_iter() == right._Get_base_iter()))) {
		return left._Get_base_iter() == right._Get_base_iter();
	}

	template<class Iter1, class Iter2>
	constexpr bool operator!=(const move_iterator<Iter1>& left, const move_iterator<Iter2>& right)
		noexcept(noexcept(left == right)) {
		return !(left == right);
	}

	template<class Iter1, class Iter2>
	constexpr bool operator<(const move_iterator<Iter1>& left, const move_iterator<Iter2>& right)
		noexcept(noexcept(_Fake_copy_init<bool>(left._Get_base_iter() < right._Get_base_iter()))) {
		return left._Get_base_iter() < right._Get_base_iter();
	}

	template<class Iter1, class Iter2>
	constexpr bool operator>=(const move_iterator<Iter1>& left, const move_iterator<Iter2>& right)
		noexcept(noexcept(left < right)) {
		return !(left < right);
	}

	template<class Iter1, class Iter2>
	constexpr bool operator>(const move_iterator<Iter1>& left, const move_iterator<Iter2>& right)
		noexcept(noexcept(left < right)) {
		return right < left;
	}

	template<class Iter1, class Iter2>
	constexpr bool operator<=(const move_iterator<Iter1>& left, const move_iterator<Iter2>& right)
		noexcept(noexcept(right < left)) {
		return !(right < left);
	}

	template<class Iter1, class Iter2>
	constexpr auto operator-(const move_iterator<Iter1>& left, const move_iterator<Iter2>& right)
		noexcept(noexcept(left._Get_base_iter() - right._Get_base_iter()))
		->decltype(left._Get_base_iter() - right._Get_base_iter()) {
		return left._Get_base_iter() - right._Get_base_iter();
	}

	template<class Iter>
	constexpr move_iterator<Iter> operator+(const typename move_iterator<Iter>::difference_type off, const move_iterator<Iter>& iter)
		noexcept(noexcept(move_iterator<Iter>(iter.base() + off))) {
		return move_iterator<Iter>(iter.base() + off);
	}

	template<class Iter>
	constexpr move_iterator<Iter> make_move_iterator(Iter iter)
		noexcept(mstd::is_nothrow_move_constructible<Iter>::value) {
		return move_iterator<Iter>(mstd::move(iter));
	}

}

