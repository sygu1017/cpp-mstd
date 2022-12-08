#pragma once

#include <algorithm>  // 使用 std::copy std::fill std::fill_n 函数
#include "m_constructor.h"
//#include "m_type_traits.h"
#include <type_traits>
#include <string.h>

/*
实现 uninitialized_copy(); uninitialized_fill(); uninitialized_fill_n(); uninitialized_move();
*/

namespace mstd {

	template<typename IptIter, typename FwdIter>
	FwdIter __copy_dispath(IptIter first, IptIter last,
		FwdIter result, std::true_type)
	{
		return std::copy(first, last, result);
	}

	template<typename IptIter, typename FwdIter>
	FwdIter __copy_dispath(IptIter first, IptIter last,
		FwdIter result, std::false_type)
	{
		FwdIter begin = result;
		for (; first < last; ++first, ++begin) {
			mstd::construct(&*begin, *first);
		}
		return begin;
	}

	template<typename IptIter, typename FwdIter>
	FwdIter uninitialized_copy(IptIter first, IptIter last, FwdIter result)
	{
		using type = typename std::is_pod<std::remove_reference_t<decltype(*first)>>::type;
		return __copy_dispath(first, last, result, type());
	}

	template<typename FwdIter, typename Tp>
	FwdIter __fill_dispath(FwdIter first,
		FwdIter last, const Tp& value, std::true_type)
	{
		return std::fill(first, last, value);
	}

	template<typename FwdIter, typename Tp>
	FwdIter __fill_dispath(FwdIter first,
		FwdIter last, const Tp& value, std::false_type)
	{
		FwdIter begin = first;
		for (; begin < last; ++begin) {
			mstd::construct(&*begin, value);
		}
		return begin;
	}

	template<typename FwdIter, typename Tp>
	FwdIter uninitialized_fill(FwdIter first, FwdIter last, const Tp& value)
	{
		using type = typename std::is_pod<std::remove_reference_t<decltype(*first)>>::type;
		return __fill_dispath(first, last, value, type());
	}

	template<typename FwdIter, typename Size, typename Tp>
	FwdIter __fill_n_dispath(FwdIter first,
		Size n, const Tp& value, std::true_type)
	{
		return std::fill_n(first, n, value);
	}

	template<typename FwdIter, typename Size, typename Tp>
	FwdIter __fill_n_dispath(FwdIter first,
		Size n, const Tp& value, std::false_type)
	{
		FwdIter begin = first;
		for (Size index = 0; index < n; ++index, ++begin) {
			mstd::construct(&*begin, value);
		}
		return begin;
	}

	template<typename FwdIter, typename Size, typename Tp>
	FwdIter uninitialized_fill_n(FwdIter first, Size n, const Tp& value)
	{
		using type = typename std::is_pod<std::remove_reference_t<decltype(*first)>>::type;
		return __fill_n_dispath(first, n, value, type());
	}

	template<typename IptIter, typename FwdIter>
	FwdIter uninitialized_move(IptIter first, IptIter last, FwdIter result)
	{
		auto value = std::is_pod_v<std::remove_reference_t<decltype(*first)>>;
		if (value) {
			result = std::copy(first, last, result);
		}
		else {
			for (; first != last; ++first, ++result) {
				mstd::construct(&*result, std::move(*first));
			}
		}
		return result;
	}

}
