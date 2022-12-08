#pragma once

#include "m_iterator.h"		// reverse_iterator;
#include <cstddef>


namespace mstd {

	template<class ArrType>
	struct _array_const_iterator {
		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename ArrType::value_type;
		using pointer = typename ArrType::const_pointer;
		using reference = typename ArrType::const_reference;
		using difference_type = typename ArrType::difference_type;
		using size_type = typename ArrType::size_type;
		using base_ptr_t = typename ArrType::pointer;

		base_ptr_t ptr_{};

		_array_const_iterator() = default;
		explicit _array_const_iterator(base_ptr_t ptr) :ptr_(ptr) {}

		reference operator*() const noexcept { return *ptr_; }
		pointer operator->() const noexcept { return ptr_; }
		_array_const_iterator& operator++() noexcept { ++ptr_; return *this; }
		_array_const_iterator operator++(int) noexcept {
			_array_const_iterator temp = *this;
			++ptr_;
			return temp;
		}
		_array_const_iterator& operator--() noexcept { --ptr_; return *this; }
		_array_const_iterator operator--(int) noexcept {
			_array_const_iterator temp = *this;
			--ptr_;
			return temp;
		}
		_array_const_iterator& operator+=(const difference_type off) noexcept {
			ptr_ += off;
			return *this;
		}
		_array_const_iterator& operator-=(const difference_type off) noexcept {
			ptr_ -= off;
			return *this;
		}
		difference_type operator-(const _array_const_iterator& right) const noexcept {
			return (ptr_ - right.ptr_);
		}
		_array_const_iterator operator-(const difference_type off) const noexcept {
			return _array_const_iterator(ptr_ - off);
		}
		_array_const_iterator operator+(const difference_type off) const noexcept {
			return _array_const_iterator(ptr_ + off);
		}

	};

	template<class ArrType>
	_array_const_iterator<ArrType> operator+(const typename _array_const_iterator<ArrType>::
		difference_type off, const _array_const_iterator<ArrType>& right) {
		return _array_const_iterator<ArrType>(right.ptr_ + off);
	}

	template<class ArrType>
	inline bool operator<(const _array_const_iterator<ArrType>& left,
		const _array_const_iterator<ArrType>& right) {
		return left.ptr_ < right.ptr_;
	}

	template<class ArrType>
	inline bool operator>(const _array_const_iterator<ArrType>& left,
		const _array_const_iterator<ArrType>& right) {
		return left.ptr_ > right.ptr_;
	}

	template<class ArrType>
	inline bool operator<=(const _array_const_iterator<ArrType>& left,
		const _array_const_iterator<ArrType>& right) {
		return !(left > right);
	}

	template<class ArrType>
	inline bool operator>=(const _array_const_iterator<ArrType>& left,
		const _array_const_iterator<ArrType>& right) {
		return !(left < right);
	}

	template<class ArrType>
	inline bool operator==(const _array_const_iterator<ArrType>& left,
		const _array_const_iterator<ArrType>& right) {
		return left.ptr_ == right.ptr_;
	}

	template<class ArrType>
	inline bool operator!=(const _array_const_iterator<ArrType>& left,
		const _array_const_iterator<ArrType>& right) {
		return !(left == right);
	}

	template<class ArrType>
	struct _array_iterator : _array_const_iterator {
		using mybase = _array_const_iterator<ArrType>;
		using pointer = typename ArrType::pointer;
		using reference = typename ArrType::reference;

		_array_iterator() = default;
		_array_iterator(base_ptr_t ptr) : mybase(ptr) {}

		reference operator*() const noexcept {
			return const_cast<reference>(mybase::operator*());
		}

		pointer operator->() const noexcept {
			return const_cast<pointer>(mybase::operator->());
		}

		_array_iterator& operator++() noexcept {
			mybase::operator++();
			return *this;
		}
		_array_iterator operator++(int) noexcept {
			_array_iterator temp = *this;
			mybase::operator++();
			return temp;
		}
		_array_iterator& operator--() noexcept {
			mybase::operator--();
			return *this;
		}
		_array_iterator operator++(int) noexcept {
			_array_iterator temp = *this;
			mybase::operator--();
			return temp;
		}
		_array_iterator& operator+=(const difference_type off) noexcept {
			mybase::operator+=(off);
			return *this;
		}
		_array_iterator& operator-=(const difference_type off) noexcept {
			mybase::operator-=(off);
			return *this;
		}
		using operator-;
		_array_iterator operator-(const difference_type off) const noexcept {
			_array_iterator temp = *this;
			temp -= off;
			return temp;
		}
		_array_iterator operator+(const difference_type off) const noexcept {
			_array_iterator temp = *this;
			temp += off;
			return temp;
		}

	};

	template<class ArrType>
	_array_iterator<ArrType> operator+(const typename _array_iterator<ArrType>::
		difference_type off, const _array_iterator<ArrType>& right) {
		return _array_iterator<ArrType>(right.ptr_ + off);
	}


	template<class Tp, size_t Size>
	class array {
	public:
		using value_type = Tp;
		using pointer = Tp*;
		using reference = Tp&;
		using const_pointer = const Tp*;
		using const_reference = const Tp&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		using iterator = _array_iterator<array>;
		using const_iterator = _array_const_iterator<array>;
		using reverse_iterator = reverse_iterator<iterator>;
		using const_reverse_iterator = reverse_iterator<const_iterator>;

	protected:
		Tp elems[Size];

	public:
		iterator begin() noexcept { return iterator(elems); }
		iterator end() noexcept { return iterator(elems + Size); }
		const_iterator begin() const noexcept { return const_iterator(elems); }
		const_iterator end() const noexcept { return const_iterator(elems + Size); }
		const_iterator cbegin() const noexcept { return const_iterator(elems); }
		const_iterator cend() const noexcept { return const_iterator(elems + Size); }
		reverse_iterator rbegin() noexcept { return reverse_iterator(elems); }
		reverse_iterator cend() noexcept { return reverse_iterator(elems + Size); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(elems); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(elems + Size); }

		size_type size() const noexcept { return Size; }
		size_type empty() const noexcept { return Size == 0; }

		reference operator[](size_type index) noexcept {
			if (index >= Size) throw std::out_of_range{ "index out of range" };
			return *elems[index];
		}
		const_reference operator[](size_type index) const noexcept {
			if (index >= Size) throw std::out_of_range{ "index out of range" };
			return *elems[index];
		}
		reference at(size_type index) noexcept {
			if (index >= Size) throw std::out_of_range{ "index out of range" };
			return *elems[index];
		}
		const_reference at(size_type index) const noexcept {
			if (index >= Size) throw std::out_of_range{ "index out of range" };
			return *elems[index];
		}
		reference front() noexcept { return *elems; }
		const_reference front() const noexcept { return *elems; }
		reference back() noexcept { return *(elems + Size); }
		const_reference back() const noexcept { return *(elems + Size); }
		pointer data() noexcept { return elems; }
		const_pointer data() const noexcept { return elems; }
		void fill(const value_type& value) {
			std::fill_n(elems, Size, value);
		}
		void swap(array& right) {
			for (auto iter1 = begin(), iter2 = right.begin();
				iter1 != end(); ++iter1, ++iter2) {
				std::iter_swap(iter1, iter2);
			}
		};

	};
}
