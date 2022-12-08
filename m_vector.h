#pragma once

#include "m_alloc.h"			// malloc_allocator;
#include "m_constructor.h"		// construct(); destroy();
#include "m_uninitialized.h"	// uninitialized_copy(); uninitialized_fill_n(); uninitialized_move();
#include "m_utility.h"			// is_iterator_v<>;
#include "m_algorithm.h"		// copy();
#include "m_iterator.h"			// reverse_iterator;

#include <initializer_list>		// std::initialized_list;
#include <exception>			// std::out_of_range;
#include <cstring>				// std::memmove();
#include <type_traits>			// std::enable_if_t<>;

namespace mstd {

	template<typename VecType>
	struct _vector_const_iterator {
		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename VecType::value_type;
		using pointer = typename VecType::const_pointer;
		using reference = typename VecType::const_reference;
		using size_type = typename VecType::size_type;
		using difference_type = typename VecType::difference_type;
		using val_ptr_t = typename VecType::pointer;

		val_ptr_t ptr_{};

		_vector_const_iterator() = default;
		_vector_const_iterator(val_ptr_t ptr) noexcept : ptr_(ptr) {}
		_vector_const_iterator& operator=(val_ptr_t ptr) noexcept {
			this->ptr_ = ptr;
			return *this;
		}

		reference operator*() const noexcept { return *ptr_; }
		pointer operator->() const noexcept { return ptr_; }
		_vector_const_iterator& operator++() noexcept {
			++ptr_;
			return *this;
		}

		_vector_const_iterator operator++(int) noexcept {
			_vector_const_iterator temp = *this;
			++ptr_;
			return temp;
		}

		_vector_const_iterator& operator--() noexcept {
			--ptr_;
			return *this;
		}

		_vector_const_iterator operator--(int) noexcept {
			_vector_const_iterator temp = *this;
			--ptr_;
			return temp;
		}

		_vector_const_iterator operator+(const difference_type off) const noexcept {
			return _vector_const_iterator(ptr_ + off);
		}

		_vector_const_iterator& operator+=(const difference_type off) noexcept {
			ptr_ += off;
			return *this;
		}

		_vector_const_iterator operator-(const difference_type off) const noexcept {
			return _vector_const_iterator(ptr_ - off);
		}

		_vector_const_iterator& operator-=(const difference_type off) noexcept {
			ptr_ -= off;
			return *this;
		}

		difference_type operator-(const _vector_const_iterator& right) const noexcept {
			return ptr_ - right.ptr_;
		}

		reference operator[](const difference_type off) const noexcept {
			return *(*this + off);
		}

		bool operator==(const _vector_const_iterator& right) const noexcept {
			return ptr_ == right.ptr_;
		}
		bool operator!=(const _vector_const_iterator& right) const noexcept {
			return !operator==(right);
		}
		bool operator>(const _vector_const_iterator& right) const noexcept {
			return ptr_ > right.ptr_;
		}
		bool operator<=(const _vector_const_iterator& right) const noexcept {
			return !operator>(right);
		}
		bool operator<(const _vector_const_iterator& right) const noexcept {
			return ptr_ < right.ptr_;
		}
		bool operator>=(const _vector_const_iterator& right) const noexcept {
			return !operator<(right);
		}

		val_ptr_t raw_ptr() const noexcept { return ptr_; }

	};

	template<typename Tp>
	inline _vector_const_iterator<Tp>
		operator+(typename _vector_const_iterator<Tp>::difference_type off,
			const _vector_const_iterator<Tp>& right) noexcept {
		return _vector_const_iterator<Tp>(right.ptr_ + off);
	}

	template<typename VecType>
	struct _vector_iterator : _vector_const_iterator<VecType> {
		using Parent = _vector_const_iterator<VecType>;
		using reference = typename VecType::reference;
		using pointer = typename VecType::pointer;
		using difference_type = typename VecType::difference_type;

		_vector_iterator() noexcept = default;
		_vector_iterator(pointer ptr) noexcept : Parent(ptr) {}
		_vector_iterator(const Parent& right) noexcept : Parent(right.raw_ptr()) {}
		_vector_iterator& operator=(const Parent& right) noexcept {
			this->ptr_ = right.ptr_;
			return *this;
		}

		_vector_iterator& operator=(pointer ptr) noexcept {
			Parent::operator=(ptr);
			return *this;
		}

		reference operator*() const noexcept {
			return const_cast<reference>(Parent::operator*());
		}

		pointer operator->() const noexcept {
			return const_cast<pointer>(Parent::operator->());
		}

		_vector_iterator& operator++() noexcept {
			Parent::operator++();
			return *this;
		}

		_vector_iterator operator++(int) noexcept {
			_vector_iterator temp{ *this };
			Parent::operator++();
			return temp;
		}

		_vector_iterator& operator--() noexcept {
			Parent::operator--();
			return *this;
		}

		_vector_iterator operator--(int) noexcept {
			_vector_iterator temp{ *this };
			Parent::operator--();
			return temp;
		}

		_vector_iterator& operator-=(const difference_type off) noexcept {
			Parent::operator-=(off);
			return *this;
		}

		using Parent::operator-;

		_vector_iterator operator-(const difference_type off) noexcept {
			_vector_iterator temp = *this;
			temp -= off;
			return temp;
		}

		_vector_iterator& operator+=(const difference_type off) noexcept {
			Parent::operator+=(off);
			return *this;
		}

		_vector_iterator operator+(const difference_type off) noexcept {
			_vector_iterator temp = *this;
			temp += off;
			return temp;
		}

	};

	template<typename Tp>
	inline _vector_iterator<Tp> operator+(const typename _vector_iterator<Tp>::difference_type off,
		const _vector_iterator<Tp>& right) noexcept {
		return _vector_iterator<Tp>{right.ptr_ + off};
	}


	template<typename Tp, typename Alloc = malloc_allocator<0>>
	class vector {
	public:
		using data_allocator = Alloc;
		using value_type = Tp;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		using iterator = _vector_iterator<vector>;
		using const_iterator = _vector_const_iterator<vector>;
		using reverse_iterator = mstd::reverse_iterator<iterator>;
		using const_reverse_iterator = mstd::reverse_iterator<const_iterator>;

	protected:
		iterator start_{};
		iterator finish_{};
		iterator end_of_storage_{};

		// 分配n个元素的空间，原迭代器失效
		void alloc_n(size_type size) {
			start_ = static_cast<pointer>
				(data_allocator::allocate(size * sizeof(value_type)));
			end_of_storage_ = start_ + size;
		}

		void fill_init(size_type n, const value_type& value) {
			if (0 == n) { return; }
			alloc_n(n);
			finish_ = uninitialized_fill_n(start_, n, value);
		}

		template <typename IptIter>
		void range_init(IptIter begin, IptIter end) {
			difference_type n = std::distance(begin, end);
			if (0 == n) { return; }
			alloc_n(n);
			finish_ = uninitialized_copy(begin, end, start_);
		}

		void exchange(vector& other) noexcept {
			this->start_ = other.start_;
			this->finish_ = other.finish_;
			this->end_of_storage_ = other.end_of_storage_;
			other.start_ = other.finish_ = other.end_of_storage_ = nullptr;
		}

		//请求n个元素的空间，若剩余空间不足则分配新的内存，原迭代器可能失效
		void check_and_alloc(size_type n) {
			if (end_of_storage_ - finish_ >= static_cast<difference_type>(n)) { return; }
			auto need_sz = 2 * size() > (size() + n) ? 2 * size() : size() + n;
			vector temp = std::move(*this);
			alloc_n(need_sz);
			finish_ = uninitialized_move(temp.begin(), temp.end(), start_);
		}

		//将[pos，finish)之间的元素向前移动n个位置
		void move_forward_n(iterator pos, size_type n) {
			auto dest = pos - static_cast<difference_type>(n);
			iterator new_finish = mstd::copy(pos, finish_, dest);
			if (new_finish < finish_)
				destroy(new_finish, finish_);
			finish_ = new_finish;
		}

		//将[pos,finish)之间的元素向后移动n个位置，如果pos==finish_，++finish_；
		void move_backward_n(iterator pos, size_type n) {
			if (pos < finish_) {
				iterator new_finish = finish_ + n;
				iterator new_pos = pos + n;
				if (std::is_pod_v<value_type>) {
					std::memmove(new_pos.raw_ptr(), pos.raw_ptr(),
						n * sizeof(value_type));
				}
				else {
					iterator src{ finish_ }, dst{ new_finish };
					if (new_pos < finish_) {
						while (dst != finish_) {
							mstd::construct(&*--dst, std::move(*--src));
						}
						while (dst != new_pos) {
							*--dst = std::move(*--src);
						}
						destroy(pos, new_pos);
					}
					else {
						while (dst != new_pos) {
							mstd::construct(&*--dst, std::move(*--src));
						}
						destroy(pos, finish_);
					}
				}
				finish_ = new_finish;
			}
			else if (pos == finish_) {
				++finish_;
			}
		}

		bool is_invalid_index(size_type index) const noexcept {
			return index >= size();
		}

		bool is_invalid_iterator(const_iterator pos) const noexcept {
			return pos < start_ || pos >= finish_;
		}

		bool is_invalid_insert_iterator(const_iterator pos) const noexcept {
			return pos < start_ || pos > finish_;
		}

	public:
		vector() = default;
		explicit vector(size_type size) { fill_init(size, value_type()); }
		vector(size_type size, const value_type& value) { fill_init(size, value); }
		// 使用 mstd::is_iterator_v 让构造函数专用化
		template <typename IptIter, std::enable_if_t<mstd::is_iterator_v<IptIter>, int> = 0>
		vector(IptIter first, IptIter last) { range_init(first, last); }
		// 使用 std::initializer_list
		vector(std::initializer_list<value_type> ilist) { range_init(ilist.begin(), ilist.end()); }
		vector(const vector& other) { range_init(other.begin(), other.end()); }
		vector& operator=(const vector& other) {
			vector temp(other.begin(), other.end());
			this->swap(temp);
			return *this;
		}
		vector(vector&& other) noexcept { exchange(other); }
		vector& operator=(vector&& other) noexcept {
			if (this == &other) return *this;
			clear();
			exchange(other);
			return *this;
		}
		vector& operator=(std::initializer_list<value_type> ilist) {
			vector temp(ilist.begin(), ilist.end());
			this->swap(temp);
			return *this;
		}
		~vector() {
			if (start_.raw_ptr() != nullptr) {
				destroy(start_, finish_);
				data_allocator::deallocate(start_.raw_ptr(),
					(end_of_storage_ - start_) * sizeof(value_type));
				start_ = finish_ = end_of_storage_ = nullptr;
			}
		}

	public:
		iterator begin() noexcept { return start_; }
		iterator end() noexcept { return finish_; }
		const_iterator begin() const noexcept { return start_; }
		const_iterator end() const  noexcept { return finish_; }
		const_iterator cbegin() const noexcept { return start_; }
		const_iterator cend() const  noexcept { return finish_; }
		// rbegin rend
		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

		reference at(size_type index) {
			if (is_invalid_index(index)) {
				throw std::out_of_range
				{ "vector member func at() error: index out of range" };
			}
			return *(start_ + index);
		}
		const_reference at(size_type index) const {
			if (is_invalid_index(index)) {
				throw std::out_of_range
				{ "vector member func at() error: index out of range" };
			}
			return *(start_ + index);
		}
		reference front() { return *start_; }
		const_reference front() const { return *start_; }
		reference back() { return *(--finish_); }
		const_reference back() const { return *(--finish_); }
		size_type size() const noexcept { return size_type(finish_ - start_); }
		size_type capacity() const noexcept { return size_type(end_of_storage_ - start_); }
		bool empty() const noexcept { return start_ == finish_; }

		template<typename IptIter, std::enable_if_t<mstd::is_iterator_v<IptIter>, int> = 0>
		void assign(IptIter first, IptIter last) {  // std::vector 调用 copy assignment operator
			vector temp{ first, last };
			this->swap(temp);
		}

		void assign(std::initializer_list<value_type> ilist) {
			assign(ilist.begin(), ilist.end());
		}

		void assign(size_type size, const value_type& value) {
			vector temp = vector(size, value);
			this->swap(temp);
		}

		void push_back(const value_type& val) {
			emplace_back(val);
		}

		void push_back(value_type&& val) noexcept {
			emplace_back(std::move(val));
		}

		void pop_back() noexcept {
			destroy(&*--finish_);
		}

		template <typename... Args>
		iterator emplace(const_iterator pos, Args&&... args) {
			if (is_invalid_insert_iterator(pos)) {
				throw std::out_of_range
				{ "vector member func emplace() error: iterator out of range" };
			}
			difference_type index = std::distance(cbegin(), pos); //使用index记录偏移量
			check_and_alloc(1);
			iterator new_iter{ start_ + index };  // check_and_alloc后 原迭代器可能失效
			move_backward_n(new_iter, 1);
			construct(new_iter.raw_ptr(), std::forward<Args>(args)...);
			return new_iter;
		}

		template <typename... Args>
		void emplace_back(Args&&... args) {
			emplace(finish_, std::forward<Args>(args)...);
		}

		iterator erase(const_iterator pos) {
			if (is_invalid_iterator(pos)) {
				throw std::out_of_range
				{ "vector member func erase() error: iterator out of range" };
			}
			move_forward_n(pos + 1, 1);
			return pos;
		}

		iterator erase(const_iterator first, const_iterator last) {
			if (is_invalid_iterator(first) || is_invalid_iterator(last)) {
				throw std::out_of_range
				{ "vector member func erase() error: iterator out of range" };
			}
			move_forward_n(last, static_cast<size_type>(last - first));
			return first;
		}

		iterator insert(const_iterator pos, const value_type& val) {
			if (is_invalid_insert_iterator(pos)) {
				throw std::out_of_range
				{ "vector member func insert() error: iterator out of range" };
			}
			difference_type index = std::distance(cbegin(), pos);
			check_and_alloc(1);
			iterator new_iter{ start_ + index };
			move_backward_n(new_iter, 1);
			construct(new_iter.raw_ptr(), val);
			return new_iter;
		}

		iterator insert(const_iterator pos, value_type&& val) {
			if (is_invalid_insert_iterator(pos)) {
				throw std::out_of_range
				{ "vector member func insert() error: iterator out of range" };
			}
			difference_type index = std::distance(cbegin(), pos);
			check_and_alloc(1);
			iterator new_iter{ start_ + index };
			move_backward_n(new_iter, 1);
			construct(new_iter.raw_ptr(), std::move(val));
			return new_iter;
		}

		iterator insert(const_iterator pos, size_type num, const value_type& value) {
			if (is_invalid_insert_iterator(pos)) {
				throw std::out_of_range
				{ "vector member func insert() error: iterator out of range" };
			}
			difference_type index = std::distance(cbegin(), pos);
			check_and_alloc(num);
			iterator new_iter{ start_ + index };
			move_backward_n(new_iter, num);
			iterator iter{ new_iter };
			iterator last{ new_iter + num };
			for (; iter != last; ++iter) {
				construct(iter.raw_ptr(), value);
			}
			return new_iter;
		}

		template<typename IptIter, std::enable_if_t<mstd::is_iterator_v<IptIter>, int> = 0>
		iterator insert(const_iterator pos, IptIter first, IptIter last) {
			if (is_invalid_insert_iterator(pos)) {
				throw std::out_of_range
				{ "vector member func insert() error: iterator out of range" };
			}
			difference_type num = std::distance(first, last);
			difference_type index = std::distance(cbegin(), pos);
			check_and_alloc(num);
			iterator new_iter{ start_ + index };
			move_backward_n(new_iter, num);
			iterator iter{ new_iter };
			for (; first != last; ++first, ++iter) {
				construct(iter.raw_ptr(), *first);
			}
			return new_iter;
		}

		iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
			return insert(pos, ilist.begin(), ilist.end());
		}

		Tp* data() noexcept { return start_.raw_ptr(); }

		const Tp* data() const noexcept { return start_.raw_ptr(); }

		void resize(size_type new_sz) {
			resize(new_sz, value_type{});
		}

		void resize(size_type new_sz, const value_type& value) {
			if (new_sz > size()) {
				if (new_sz > capacity()) {
					check_and_alloc(new_sz);
				}
				finish_ = uninitialized_fill_n(finish_, new_sz - size(), value);
			}
			else {
				destroy(start_ + new_sz, finish_);
				finish_ = start_ + new_sz;
			}
		}

		void reserve(size_type new_capacity) {
			if (new_capacity > capacity()) {
				vector temp{};
				this->swap(temp);
				alloc_n(new_capacity);
				iterator iter{ temp.begin() }, dest{ start_ };
				for (; iter != temp.end(); ++iter, ++dest) {
					construct(dest.raw_ptr(), std::move(*iter));
				}
				finish_ = dest;
			}
		}

		void clear() noexcept {
			destroy(start_, finish_);
			finish_ = start_;
		}

		void shrink_to_fit() {
			if (size() != capacity()) {
				if (start_ == finish_) {
					data_allocator::deallocate(&start_, size() * sizeof(value_type));
					start_ = finish_ = end_of_storage_ = nullptr;
				}
				else {
					auto num{ size() };
					vector temp = std::move(*this);
					alloc_n(num);
					finish_ = uninitialized_move(temp.begin(), temp.end(), start_);
				}
			}
		}

		reference operator[](size_type index) {
			if (is_invalid_index(index)) {
				throw std::out_of_range
				{ "vector member func operator[]() error: index out of range" };
			}
			return *(start_ + index);
		}

		const_reference operator[](size_type index) const {
			if (is_invalid_index(index)) {
				throw std::out_of_range
				{ "vector member func operator[]() error: index out of range" };
			}
			return *(start_ + index);
		}

		void swap(vector& right) noexcept {
			mstd::swap(start_, right.start_);
			mstd::swap(finish_, right.finish_);
			mstd::swap(end_of_storage_, right.end_of_storage_);
		}

	};

	template<typename Tp, typename Alloc>
	inline void swap(const vector<Tp, Alloc>& left,
		const vector<Tp, Alloc>& right) noexcept {
		left.swap(right);
	}

	template<typename Tp, typename Alloc>
	inline bool operator>(const vector<Tp, Alloc>& left,
		const vector<Tp, Alloc>& right) noexcept {
		auto lhs_begin{ left.begin() };
		auto lhs_end{ left.end() };
		auto rhs_begin{ right.begin() };
		auto rhs_end{ right.end() };
		for (; lhs_begin != lhs_end && rhs_begin != rhs_end;
			++lhs_begin, ++rhs_begin) {
			if (!(*rhs_begin < *lhs_begin)) { return false; }
		}
		return true;
	}

	template<typename Tp, typename Alloc>
	inline bool operator<(const vector<Tp, Alloc>& left,
		const vector<Tp, Alloc>& right) noexcept {
		auto lhs_begin{ left.begin() };
		auto lhs_end{ left.end() };
		auto rhs_begin{ right.begin() };
		auto rhs_end{ right.end() };
		for (; lhs_begin != lhs_end && rhs_begin != rhs_end;
			++lhs_begin, ++rhs_begin) {
			if (!(*lhs_begin < *rhs_begin)) { return false; }
		}
		return true;
	}

	template<typename Tp, typename Alloc>
	inline bool operator>=(const vector<Tp, Alloc>& left,
		const vector<Tp, Alloc>& right) noexcept {
		return !(left < right);
	}

	template<typename Tp, typename Alloc>
	inline bool operator<=(const vector<Tp, Alloc>& left,
		const vector<Tp, Alloc>& right) noexcept {
		return !(left > right);
	}

	template<typename Tp, typename Alloc>
	inline bool operator==(const vector<Tp, Alloc>& left,
		const vector<Tp, Alloc>& right) noexcept {
		if (left.size() != right.size()) return false;
		for (auto iter{ right.begin() }; iter != right.end(); ++iter) {
			if (*iter != right.at(static_cast<size_t>(iter - left.begin())))
				return false;
		}
		return true;
	}

	template<typename Tp, typename Alloc>
	inline bool operator!=(const vector<Tp, Alloc>& left,
		const vector<Tp, Alloc>& right) noexcept {
		return !(left == right);
	}

}

