#pragma once

#include <cstddef>

namespace mstd {

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

	// reverse_iterator 
	template<class BidIter>
	class reverse_iterator {
	public:
		using iterator_category = typename BidIter::iterator_category;
		using value_type = typename BidIter::value_type;
		using pointer = typename BidIter::pointer;
		using reference = typename BidIter::reference;
		using difference_type = typename BidIter::difference_type;

	protected:
		BidIter raw_iter_{};

	public:
		template<class>
		friend class reverse_iterator;

		reverse_iterator() = default;
		explicit reverse_iterator(BidIter iter) noexcept : raw_iter_(std::move(iter)) {}
		template<class Other>
		reverse_iterator(const reverse_iterator<Other>& right) noexcept : raw_iter_(right.iter_) {}
		reverse_iterator& operator=(const reverse_iterator& right) noexcept {
			raw_iter_ = right.raw_iter_;
			return *this;
		}

		BidIter base() const { return raw_iter_; }

		reference operator*() const {
			BidIter temp = raw_iter_;
			return *--temp;
		}

		pointer operator->() const {
			if constexpr (std::is_pointer_v<BidIter>) {
				return raw_iter_;
			}
			else {
				return raw_iter_.operator->();
			}
		}

		reverse_iterator& operator++() {
			--raw_iter_;
			return *this;
		}

		reverse_iterator operator++(int) {
			reverse_iterator temp = *this;
			--raw_iter_;
			return temp;
		}

		reverse_iterator& operator--() {
			++raw_iter_;
			return *this;
		}

		reverse_iterator operator--(int) {
			reverse_iterator temp = *this;
			++raw_iter_;
			return temp;
		}

		reverse_iterator& operator+=(const difference_type off) {
			raw_iter_ -= off;
			return *this;
		}

		reverse_iterator& operator-=(const difference_type off) {
			raw_iter_ += off;
			return *this;
		}

		reverse_iterator operator-(const difference_type off) const {
			return reverse_iterator(raw_iter_ + off);
		}

		reverse_iterator operator+(const difference_type off) const {
			return reverse_iterator(raw_iter_ - off);
		}

		reference operator[](const difference_type off) const {
			return raw_iter_[-off - 1];
		}

		const BidIter& get_iter() const noexcept { return raw_iter_; }

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
		return left.get_iter() - right.get_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator<(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left.get_iter() > right.get_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator>(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left.get_iter() < right.get_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator<=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left.get_iter() >= right.get_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator>=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left.get_iter() <= right.get_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator==(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left.get_iter() == right.get_iter();
	}

	template<class BidIter1, class BidIter2>
	inline bool operator!=(const reverse_iterator<BidIter1>& left,
		const reverse_iterator<BidIter2>& right) {
		return left.get_iter() != right.get_iter();
	}
}

