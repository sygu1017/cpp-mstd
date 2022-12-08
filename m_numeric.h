#pragma once

/*
accumulate(); adjacent_difference(); inner_product(); partial_sum(); power(); iota();
*/

#include "m_functional.h"

namespace mstd {

	template<typename IptIter, typename Tp, typename Binary_Operator>
	Tp accumulate(IptIter first, IptIter last, Tp init, Binary_Operator bin_op) {
		for (; first != last; ++first) {
			init = bin_op(init, *first);
		}
		return init;
	}

	template<typename IptIter, typename Tp>
	Tp accumulate(IptIter first, IptIter last, Tp init) {
		return accumulate(first, last, init, mstd::plus<Tp>{});
	}


	template<typename IptIter, typename OptIter, typename Binary_Operator>
	OptIter adjacent_difference(IptIter first, IptIter last,
		OptIter result, Binary_Operator bin_op) {
		if (first == last) return result;
		*result = *first;
		typename IptIter::value_type val = *first;
		while (++first != last) {
			*++result = bin_op(*first, val);
			val = *first;
		}
		return ++result;
	}

	template<typename IptIter, typename OptIter>
	OptIter adjacent_difference(IptIter first, IptIter last, OptIter result) {
		return adjacent_difference(first, last, result, minus<>{})
	}

	template<typename IptIter, typename Tp, typename Binary_Operator1, typename Binary_Operator2>
	Tp inner_product(IptIter first1, IptIter last1, IptIter first2, Tp init,
		Binary_Operator1 fun1, Binary_Operator2 fun2) {
		for (; first1 != last1; ++first1, ++first2) {
			init = fun1(init, fun2(*first1, *first2));
		}
		return init;
	}

	template<typename IptIter, typename Tp>
	Tp inner_product(IptIter first1, IptIter last1, IptIter first2, Tp init) {
		return inner_product(first1, last1, first2, plus<>{}, multiplies<>{});
	}

	template<typename IptIter, typename OptIter, typename Binary_Operator>
	OptIter partial_sum(IptIter first, IptIter last,
		OptIter result, Binary_Operator bin_op) {
		if (first == last) return result;
		*result = *first;
		typename IptIter::value_type val = *first;
		while (++first != last) {
			val = bin_op(val, *first);
			*++result = val;
		}
		return ++result;
	}

	template<typename IptIter, typename OptIter>
	OptIter partial_sum(IptIter first, IptIter last, OptIter result) {
		return partial_sum(first, last, plus<>{});
	}

	template<typename FwdIter, typename Tp>
	void iota(FwdIter first, FwdIter last, Tp value) {
		while (first != last) {
			*first++ = value++;
		}
	}


}

