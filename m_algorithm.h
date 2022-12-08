#pragma once

/*
Functions in <algorithm>

Non-modifying sequence operations:
all_of			 Test condition on all elements in range (function template )
any_of			 Test if any element in range fulfills condition (function template )
none_of			 Test if no elements fulfill condition (function template )
for_each		 Apply function to range (function template )
find			 Find value in range (function template )
find_if			 Find element in range (function template )
find_if_not		 Find element in range (negative condition) (function template )
find_end		 Find last subsequence in range (function template )
find_first_of	 Find element from set in range (function template )
adjacent_find	 Find equal adjacent elements in range (function template )
count			 Count appearances of value in range (function template )
count_if		 Return number of elements in range satisfying condition (function template )
mismatch		 Return first position where two ranges differ (function template )
equal			 Test whether the elements in two ranges are equal (function template )
is_permutation	 Test whether range is permutation of another (function template )
search			 Search range for subsequence (function template )
search_n		 Search range for elements (function template )

Modifying sequence operations:
copy			 Copy range of elements (function template )
copy_n			 Copy elements (function template )
copy_if			 Copy certain elements of range (function template )
copy_backward	 Copy range of elements backward (function template )
move			 Move range of elements (function template )
move_backward	 Move range of elements backward (function template )
swap			 Exchange values of two objects (function template )
swap_ranges		 Exchange values of two ranges (function template )
iter_swap		 Exchange values of objects pointed to by two iterators (function template )
transform		 Transform range (function template )
replace			 Replace value in range (function template )
replace_if		 Replace values in range (function template )
replace_copy	 Copy range replacing value (function template )
replace_copy_if	 Copy range replacing value (function template )
fill			 Fill range with value (function template )
fill_n			 Fill sequence with value (function template )
generate		 Generate values for range with function (function template )
generate_n		 Generate values for sequence with function (function template )
remove			 Remove value from range (function template )
remove_if		 Remove elements from range (function template )
remove_copy		 Copy range removing value (function template )
remove_copy_if	 Copy range removing values (function template )
unique			 Remove consecutive duplicates in range (function template )
unique_copy		 Copy range removing duplicates (function template )
reverse			 Reverse range (function template )
reverse_copy	 Copy range reversed (function template )
rotate			 Rotate left the elements in range (function template )
rotate_copy		 Copy range rotated left (function template )
random_shuffle	 Randomly rearrange elements in range (function template )
shuffle			 Randomly rearrange elements in range using generator (function template )

Partitions:
is_partitioned	 Test whether range is partitioned (function template )
partition		 Partition range in two (function template )
stable_partition Partition range in two - stable ordering (function template )
partition_copy	 Partition range into two (function template )
partition_point  Get partition point (function template )

Sorting:
sort			 Sort elements in range (function template )
stable_sort		 Sort elements preserving order of equivalents (function template )
partial_sort	 Partially sort elements in range (function template )
partial_sort_cop yCopy and partially sort range (function template )
is_sorted		 Check whether range is sorted (function template )
is_sorted_until	 Find first unsorted element in range (function template )
nth_element		 Sort element in range (function template )

Binary search (operating on partitioned/sorted ranges):
lower_bound		 Return iterator to lower bound (function template )
upper_bound		 Return iterator to upper bound (function template )
equal_range		 Get subrange of equal elements (function template )
binary_search	 Test if value exists in sorted sequence (function template )

Merge (operating on sorted ranges):
merge			 Merge sorted ranges (function template )
inplace_merge	 Merge consecutive sorted ranges (function template )
includes		 Test whether sorted range includes another sorted range (function template )
set_union		 Union of two sorted ranges (function template )
set_intersection Intersection of two sorted ranges (function template )
set_difference	 Difference of two sorted ranges (function template )
set_symmetric_difference  Symmetric difference of two sorted ranges (function template )

Heap:
push_heap		 Push element into heap range (function template )
pop_heap		 Pop element from heap range (function template )
make_heap		 Make heap from range (function template )
sort_heap		 Sort elements of heap (function template )
is_heap			 Test if range is heap (function template )
is_heap_until	 Find first element not in heap order (function template )

Min/max:
min				 Return the smallest (function template )
max				 Return the largest (function template )
minmax			 Return smallest and largest elements (function template )
min_element		 Return smallest element in range (function template )
max_element		 Return largest element in range (function template )
minmax_element	 Return smallest and largest elements in range (function template )

Other:
lexicographical_compare	  Lexicographical less-than comparison (function template )
next_permutation		  Transform range to next permutation (function template )
prev_permutation		  Transform range to previous permutation (function template )
*/


#include "m_utility.h"
#include "m_type_traits.h"
#include <cstring>			// memmove()
#include <type_traits>		// 
#include <cstddef>
#include <functional>

namespace mstd {

	// algobase: 
	// copy/copy_backward/equal/fill/fill_n/iter_swap/lexicographical_compare/max/min/mismatch/swap/swap_ranges

	// Non-modifying sequence operations:

	template<class IptIter, class UnaryPred>
	inline bool all_of(IptIter first, IptIter last, UnaryPred pred)
	{
		while (first != last) {
			if (!pred(*first)) return false;
			++first;
		}
		return true;
	}

	template<class IptIter, class UnaryPred>
	inline bool any_of(IptIter first, IptIter last, UnaryPred pred)
	{
		while (first != last) {
			if (pred(*first)) return true;
			++first;
		}
		return false;
	}

	template<class IptIter, class UnaryPred>
	inline bool none_of(IptIter first, IptIter last, UnaryPred pred)
	{
		while (first != last) {
			if (pred(*first)) return false;
			++first;
		}
		return true;
	}

	template<class IptIter, class Function>
	inline Function for_each(IptIter first, IptIter last, Function fn)
	{
		while (first != last) {
			fn(*first);
			++first;
		}
		return fn;
	}

	template<class IptIter, class Tp>
	inline IptIter find(IptIter first, IptIter last, const Tp& val)
	{
		while (first != last) {
			if (*first == val) return first;
			++first;
		}
		return last;
	}

	template<class IptIter, class UnaryPred>
	inline IptIter find_if(IptIter first, IptIter last, UnaryPred pred)
	{
		while (first != last) {
			if (pred(*first)) return first;
			++first;
		}
		return last;
	}

	template<class IptIter, class UnaryPred>
	inline IptIter find_if_not(IptIter first, IptIter last, UnaryPred pred)
	{
		while (first != last) {
			if (!pred(*first)) return first;
			++first;
		}
		return last;
	}

	template<class FwdIter1, class FwdIter2, class BinaryPred>
	inline FwdIter1 find_end(FwdIter1 first1, FwdIter1 last1,
		FwdIter2 first2, FwdIter2 last2, BinaryPred pred)
	{
		if (first2 == last2) return last1;
		FwdIter1 ret = last1;
		while (first1 != last1) {
			FwdIter1 it1 = first1;
			FwdIter2 it2 = first2;
			while (pred(*it1, *it2)) {
				++it1; ++it2;
				if (it2 == last2) { ret = first1; break; }
				if (it1 == last1) return ret;
			}
			++first1;
		}
		return ret;
	}

	template<class FwdIter1, class FwdIter2>
	inline FwdIter1 find_end(FwdIter1 first1, FwdIter1 last1,
		FwdIter2 first2, FwdIter2 last2)
	{
		return find_end(first1, last1, first2, last2, std::equal_to<>{});
	}

	template<class IptIter, class FwdIter, class BinaryPred>
	inline IptIter find_first_of(IptIter first1, IptIter last1,
		FwdIter first2, FwdIter last2, BinaryPred pred)
	{
		while (first1 != last1) {
			for (FwdIter iter = first2; iter != last2; ++iter) {
				if (pred(*iter, *first1))
					return first1;
			}
			++first1;
		}
		return last1;
	}

	template<class IptIter, class FwdIter>
	inline IptIter find_first_of(IptIter first1, IptIter last1,
		FwdIter first2, FwdIter last2) {
		return find_first_of(first1, last1, first2, last2, std::equal_to<>{});
	}

	template <class FwdIter, class BinaryPred>
	inline FwdIter adjacent_find(FwdIter first, FwdIter last, BinaryPred pred)
	{
		if (first != last) {
			FwdIter next = first; ++next;
			while (next != last) {
				if (pred(*first, *next))
					return first;
				++first; ++next;
			}
		}
		return last;
	}

	template <class FwdIter>
	inline FwdIter adjacent_find(FwdIter first, FwdIter last)
	{
		return adjacent_find(first, last, std::equal_to<>{});
	}

	template <class IptIter, class Tp>
	inline typename std::iterator_traits<IptIter>::difference_type
		count(IptIter first, IptIter last, const Tp& val)
	{
		typename iterator_traits<IptIter>::difference_type ret = 0;
		while (first != last) {
			if (*first == val) ++ret;
			++first;
		}
		return ret;
	}

	template <class IptIter, class UnaryPred>
	inline typename std::iterator_traits<IptIter>::difference_type
		count_if(IptIter first, IptIter last, UnaryPred pred)
	{
		typename iterator_traits<IptIter>::difference_type ret = 0;
		while (first != last) {
			if (pred(*first)) ++ret;
			++first;
		}
		return ret;
	}

	template <class IptIter1, class IptIter2, class BinaryPred>
	inline std::pair<IptIter1, IptIter2>
		mismatch(IptIter1 first1, IptIter1 last1,
			IptIter2 first2, BinaryPred pred)
	{
		while ((first1 != last1) && (pred(*first1, *first2))) {
			++first1; ++first2;
		}
		return std::make_pair(first1, first2);
	}

	template <class IptIter1, class IptIter2>
	inline std::pair<IptIter1, IptIter2>
		mismatch(IptIter1 first1, IptIter1 last1, IptIter2 first2)
	{
		return mismatch(first1, last1, first2, std::equal_to<>{});
	}

	template <class IptIter1, class IptIter2, class BinaryPred>
	inline bool equal(IptIter1 first1, IptIter1 last1,
		IptIter2 first2, BinaryPred pred)
	{
		while (first1 != last1) {
			if (!pred(*first1, *first2))
				return false;
			++first1; ++first2;
		}
		return true;
	}

	template <class IptIter1, class IptIter2>
	inline bool equal(IptIter1 first1, IptIter1 last1, IptIter2 first2)
	{
		return equal(first1, last1, first2, std::equal_to<>{});
	}

	template <class IptIter1, class IptIter2, class BinaryPred>
	inline bool is_permutation(IptIter1 first1, IptIter1 last1,
		IptIter2 first2, BinaryPred pred)
	{

	}

	template <class IptIter1, class IptIter2>
	inline bool is_permutation(IptIter1 first1, IptIter1 last1, IptIter2 first2)
	{
		std::tie(first1, first2) = std::mismatch(first1, last1, first2);
		if (first1 == last1) return true;
		IptIter2 last2 = first2;
		std::advance(last2, std::distance(first1, last1));
		for (IptIter1 it1 = first1; it1 != last1; ++it1) {
			if (std::find(first1, it1, *it1) == it1) {
				auto n = std::count(first2, last2, *it1);
				if (n == 0 || std::count(it1, last1, *it1) != n) return false;
			}
		}
		return true;
	}

	template<class FwdIter1, class FwdIter2, class BinaryPred>
	inline FwdIter1 search(FwdIter1 first1, FwdIter1 last1,
		FwdIter2 first2, FwdIter2 last2, BinaryPred pred)
	{
		if (first2 == last2) return first1;
		while (first1 != last1)
		{
			FwdIter1 it1 = first1;
			FwdIter2 it2 = first2;
			while (pred(*it1, *it2)) {
				++it1; ++it2;
				if (it2 == last2) return first1;
				if (it1 == last1) return last1;
			}
			++first1;
		}
		return last1;
	}

	template<class FwdIter1, class FwdIter2>
	inline FwdIter1 search(FwdIter1 first1, FwdIter1 last1,
		FwdIter2 first2, FwdIter2 last2)
	{
		return search(first1, last1, first2, last2, std::equal_to<>{});
	}

	template<class FwdIter, class Size, class Tp, class BinaryPred>
	inline FwdIter search_n(FwdIter first, FwdIter last,
		Size count, const Tp& val, BinaryPred pred)
	{
		FwdIter it, limit;
		Size i;
		limit = first; std::advance(limit, std::distance(first, last) - count);
		while (first != limit) {
			it = first; i = 0;
			while (pred(*it, val)) {
				++it;
				if (++i == count) return first;
			}
			++first;
		}
		return last;
	}

	template<class FwdIter, class Size, class Tp>
	inline FwdIter search_n(FwdIter first, FwdIter last, Size count, const Tp& val)
	{
		return search_n(first, last, count, val, std::equal_to<>{});
	}


	// Modifying sequence operations:

	template<typename IptIter, typename OptIter>
	inline OptIter __copy(IptIter first, IptIter last, OptIter dest,
		std::input_iterator_tag)
	{
		for (; first != last; ++first, ++dest) {
			*dest = *first;
		}
		return dest;
	}

	template<typename RdmIter, typename OptIter, typename Distance>
	inline OptIter __copy_d(RdmIter first, RdmIter last, OptIter dest, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++first, ++dest) {
			*dest = *first;
		}
		return dest;
	}

	template<typename RdmIter, typename OptIter>
	inline OptIter __copy(RdmIter first, RdmIter last, OptIter dest,
		std::random_access_iterator_tag)
	{
		return __copy_d(first, last, dest, (ptrdiff_t*)0);
	}

	template<typename Tp>
	inline Tp* __copy_t(const Tp* first, const Tp* last, Tp* dest, std::true_type)
	{
		std::memmove(dest, first, sizeof(Tp) * (last - first));
		return dest + (last - first);
	}

	template<typename Tp>
	inline Tp* __copy_t(const Tp* first, const Tp* last, Tp* dest, std::false_type)
	{
		return __copy_d(first, last, dest, (ptrdiff_t*)0);
	}

	template<typename IptIter, typename OptIter>
	struct __copy_dispatch {
		OptIter operator()(IptIter first, IptIter last, OptIter dest)
		{
			using type = typename std::iterator_traits<IptIter>::iterator_category;
			return __copy(first, last, dest, type{});
		}
	};

	template<typename Tp>
	struct __copy_dispatch<Tp*, Tp*> {
		Tp* operator()(const Tp* first, const Tp* last, Tp* dest)
		{
			using type = mstd::is_bitcopy_assignment_t<decltype(*first)>;
			return __copy_t(first, last, dest, type{});
		}
	};

	template<typename IptIter, typename OptIter>
	inline OptIter copy(IptIter first, IptIter last, OptIter dest)
	{
		return __copy_dispatch<IptIter, OptIter>()(first, last, dest);
	}

	inline char* copy(const char* first, const char* last, char* dest)
	{
		std::memmove(dest, first, last - first);
		return dest + (last - first);
	}

	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* dest)
	{
		std::memmove(dest, first, sizeof(wchar_t) * (last - first));
		return dest + (last - first);
	}

	template<class IptIter, class Size, class OptIter>
	inline OptIter copy_n(IptIter first, Size n, OptIter result)
	{
		while (n > 0) {
			*result = *first;
			++result; ++first;
			--n;
		}
		return result;
	}

	template <class IptIter, class OptIter, class UnaryPred>
	inline OptIter copy_if(IptIter first, IptIter last, OptIter result, UnaryPred pred)
	{
		while (first != last) {
			if (pred(*first)) {
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template<class BidIter1, class BidIter2>
	inline BidIter2 copy_backward(BidIter1 first, BidIter1 last, BidIter2 result)
	{
		while (last != first)
			*(--result) = *(--last);
		return result;
	}

	template<class IptIter, class OptIter>
	inline OptIter move(IptIter first, IptIter last, OptIter result)
	{
		while (first != last) {
			*result = std::move(*first);
			++result; ++first;
		}
		return result;
	}

	template<class BidIter1, class BidIter2>
	inline BidIter2 move_backward(BidIter1 first, BidIter1 last, BidIter2 result)
	{
		while (last != first)
			*(--result) = std::move(*(--last));
		return result;
	}

	template<class FwdIter1, class FwdIter2>
	inline FwdIter2 swap_ranges(FwdIter1 first1, FwdIter1 last1, FwdIter2 first2)
	{
		while (first1 != last1) {
			swap(*first1, *first2);
			++first1; ++first2;
		}
		return first2;
	}

	template <class FwdIter1, class FwdIter2>
	inline void iter_swap(FwdIter1 left, FwdIter2 right)
	{
		swap(*left, *right);
	}

	template <class IptIter, class OptIter, class UnaryPred>
	inline OptIter transform(IptIter first1, IptIter last1, OptIter result, UnaryPred pred)
	{
		while (first1 != last1) {
			*result = pred(*first1);
			++result; ++first1;
		}
		return result;
	}

	template <class IptIter, class OptIter, class BinaryPred>
	inline OptIter transform(IptIter first1, IptIter last1, IptIter first2,
		OptIter result, BinaryPred pred)
	{
		while (first1 != last1) {
			*result = pred(*first1, *first2);
			++result; ++first1;
		}
		return result;
	}

	template <class FwdIter, class T>
	inline void replace(FwdIter first, FwdIter last,
		const T& old_value, const T& new_value)
	{
		while (first != last) {
			if (*first == old_value) *first = new_value;
			++first;
		}
	}

	template < class FwdIter, class UnaryPred, class T >
	void replace_if(FwdIter first, FwdIter last, UnaryPred pred, const T& new_value)
	{
		while (first != last) {
			if (pred(*first)) *first = new_value;
			++first;
		}
	}

	template <class IptIter, class OptIter, class T>
	OptIter replace_copy(IptIter first, IptIter last,
		OptIter result, const T& old_value, const T& new_value)
	{
		while (first != last) {
			*result = (*first == old_value) ? new_value : *first;
			++first; ++result;
		}
		return result;
	}

	template <class IptIter, class OptIter, class UnaryPred, class T>
	OptIter replace_copy_if(IptIter first, IptIter last,
		OptIter result, UnaryPred pred,
		const T& new_value)
	{
		while (first != last) {
			*result = (pred(*first)) ? new_value : *first;
			++first; ++result;
		}
		return result;
	}

	template<typename IptIter, typename Tp>
	inline void fill(IptIter first, IptIter last, const Tp& value)
	{
		for (; first != last; ++first) {
			*first = value;
		}
	}

	template<typename OptIter, typename Tp>
	inline OptIter fill_n(OptIter first, size_t n, const Tp& value)
	{
		while (n--) {
			*first++ = value;
		}
		return first;
	}

	template <class fwdIter, class Generator>
	inline void generate(fwdIter first, fwdIter last, Generator gen)
	{
		while (first != last) {
			*first = gen();
			++first;
		}
	}

	template <class OptIter, class Size, class Generator>
	inline void generate_n(OptIter first, Size n, Generator gen)
	{
		while (n > 0) {
			*first = gen();
			++first; --n;
		}
	}

	template <class FwdIter, class Tp>
	inline FwdIter remove(FwdIter first, FwdIter last, const Tp& val)
	{
		FwdIter result = first;
		while (first != last) {
			if (!(*first == val)) {
				if (result != first)
					*result = std::move(*first);
				++result;
			}
			++first;
		}
		return result;
	}

	template <class FwdIter, class UnaryPred>
	inline FwdIter remove_if(FwdIter first, FwdIter last, UnaryPred pred)
	{
		FwdIter result = first;
		while (first != last) {
			if (!pred(*first)) {
				if (result != first)
					*result = std::move(*first);
				++result;
			}
			++first;
		}
		return result;
	}

	template <class IptIter, class OptIter, class Tp>
	inline OptIter remove_copy(IptIter first, IptIter last, OptIter result, const Tp& val)
	{
		while (first != last) {
			if (!(*first == val)) {
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template <class IptIter, class OptIter, class UnaryPred>
	OptIter remove_copy_if(IptIter first, IptIter last, OptIter result, UnaryPred pred)
	{
		while (first != last) {
			if (!pred(*first)) {
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}









	template<typename IptIter1, typename IptIter2>
	inline bool lexicographical_compare(IptIter1 first1, IptIter1 last1,
		IptIter1 first2, IptIter2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (*first1 < *first2) {
				return true;
			}
			if (*first2 < *first1) {
				return false;
			}
		}
		return first1 == last1 && first2 != last2;
	}

	template<typename Tp>
	inline const Tp& min(const Tp& v1, const Tp& v2)
	{
		if (v1 < v2)
			return v1;
		return v2;
	}

	template<typename Tp, typename Compare>
	inline const Tp& min(const Tp& v1, const Tp& v2, Compare comp)
	{
		if (comp(v1, v2))
			return v1;
		return v2;
	}

	template<typename Tp>
	inline const Tp& max(const Tp& v1, const Tp& v2)
	{
		if (v1 < v2)
			return v2;
		return v1;
	}

	template<typename Tp, typename Compare>
	inline const Tp& max(const Tp& v1, const Tp& v2, Compare comp)
	{
		if (comp(v1, v2))
			return v2;
		return v1;
	}



}


