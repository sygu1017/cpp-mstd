#pragma once

#include "m_alloc.h"		// malloc_allocator;
#include "m_iterator.h"		// reverse_iterator;
#include "m_constructor.h"	// construct(); destroy();
#include "m_algorithm.h"	// equal(); lexicographical_compare();
#include "m_utility.h"		// is_iterator_v<>;
#include "m_type_traits.h"	// enable_if_t<>;

#include <type_traits>		// bidirectional_iterator_tag; 
#include <initializer_list>	// initializer_list;
#include <exception>		// out_of_range<>;

namespace mstd {

	template<class ListType>
	struct _list_const_iterator {

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename ListType::value_type;
		using pointer = typename ListType::const_pointer;
		using reference = typename ListType::const_reference;
		using difference_type = typename ListType::difference_type;

		using _Node_ptr = typename ListType::_Node_ptr;

		_Node_ptr ptr_{};

		_list_const_iterator() noexcept = default;
		_list_const_iterator(_Node_ptr ptr) noexcept : ptr_(ptr) {}

		reference operator*() const noexcept {
			return ptr_->data_;
		}

		pointer operator->() const noexcept {
			return std::addressof(ptr_->data_);
		}

		_list_const_iterator& operator++() noexcept {
			ptr_ = ptr_->next_;
			return *this;
		}

		_list_const_iterator operator++(int) noexcept {
			_list_const_iterator temp = *this;
			ptr_ = ptr_->next_;
			return temp;
		}

		_list_const_iterator& operator--() noexcept {
			ptr_ = ptr_->prev_;
			return *this;
		}

		_list_const_iterator operator--(int) noexcept {
			_list_const_iterator temp = *this;
			ptr_ = ptr_->prev_;
			return temp;
		}

		bool operator==(const _list_const_iterator& right) const noexcept {
			return this->ptr_ == right.ptr_;
		}

		bool operator!=(const _list_const_iterator& right) const noexcept {
			return !operator==(right);
		}

		_Node_ptr raw_ptr() const noexcept { return ptr_; }
	};

	template<class ListType>
	struct _list_iterator : _list_const_iterator<ListType> {

		using Parent = _list_const_iterator<ListType>;
		using pointer = typename ListType::pointer;
		using reference = typename ListType::reference;

		using _list_const_iterator<ListType>::_list_const_iterator;

		reference operator*() const noexcept {
			return const_cast<reference>(Parent::operator*());
		}

		pointer operator->() const noexcept {
			return const_cast<pointer>(Parent::operator->());
		}

		_list_iterator& operator++() noexcept {
			Parent::operator++();
			return *this;
		}

		_list_iterator operator++(int) noexcept {
			_list_iterator temp = *this;
			Parent::operator++();
			return temp;
		}

		_list_iterator& operator--() noexcept {
			Parent::operator--();
			return *this;
		}

		_list_iterator operator--(int) noexcept {
			_list_iterator temp = *this;
			Parent::operator--();
			return temp;
		}

	};

	template<class Tp, class Alloc>
	struct _list_node {
		using data_allocator = Alloc;
		using value_type = Tp;
		using size_type = size_t;
		using _Node_ptr = _list_node*;

		_Node_ptr prev_{};
		_Node_ptr next_{};
		value_type data_;

		_list_node() = default;

		static _Node_ptr get_node() {
			return static_cast<_Node_ptr>
				(data_allocator::allocate(sizeof(_list_node)));
		}

		static void put_node(_Node_ptr ptr) noexcept {
			data_allocator::deallocate(ptr, sizeof(_list_node));
		}

		template<class... Args>
		static _Node_ptr create_node(Args&&...args) {
			_Node_ptr ptr = get_node();
			mstd::construct(&(ptr->data_), std::forward<Args>(args)...);
			return ptr;
		}

		static void delete_node(_Node_ptr ptr) noexcept {
			mstd::destroy(std::addressof(ptr->data_));
			put_node(ptr);
		}

		static void delete_nodes_non_head(_Node_ptr head) noexcept {
			if (head == nullptr) return;
			_Node_ptr node{ head->next_ };
			_Node_ptr temp{};
			while (node != head) {
				temp = node->next_;
				delete_node(node);
				node = temp;
			}
		}

		static _Node_ptr link_node(_Node_ptr link_point, _Node_ptr node) noexcept {
			_Node_ptr next_point = link_point->next_;
			link_point->next_ = node;
			node->next_ = next_point;
			next_point->prev_ = node;
			node->prev_ = link_point;
			return node;
		}

		// 将[first,last)节点插入到pos位置前面
		static void transfer(_Node_ptr pos, _Node_ptr first, _Node_ptr last) noexcept {
			_Node_ptr pos_prev = pos->prev_;
			_Node_ptr first_prev = first->prev_;
			pos->prev_ = last->prev_;
			last->prev_->next_ = pos;
			pos_prev->next_ = first;
			first->prev_ = pos_prev;
			first_prev->next_ = last;
			last->prev_ = first_prev;
		}

		static void delete_range(_Node_ptr first, _Node_ptr last) noexcept {
			_Node_ptr prev = first->prev_;
			_Node_ptr next{};
			size_type del_size{};
			for (; first != last;) {
				next = first->next_;
				delete_node(first);
				first = next;
			}
			prev->next_ = last;
			last->prev_ = prev;
		}

		template<class Binary_Pred>
		static _Node_ptr _sort_merge(_Node_ptr first, _Node_ptr mid,
			_Node_ptr last, Binary_Pred pred) {


			return first;
		}

		template<class Binary_Pred>
		static _Node_ptr sort_impl(_Node_ptr& first, size_type sz, Binary_Pred pred) {
			if (sz == 0) {
				return first;
			}
			else if (sz == 1) {
				return first->next_;
			}
			auto mid = sort_impl(first, sz / 2, pred);
			const auto last = sort_impl(mid, sz - sz / 2, pred);
			first = _sort_merge(first, mid, last, pred);
			return last;
		}

	};

	template<class Tp, class Alloc = malloc_allocator<0>>
	class list {
	public:
		static_assert(!mstd::is_const_v<Tp>, "The container element is not allowed to be const type.");

		using data_allocator = Alloc;
		using value_type = Tp;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		using iterator = _list_iterator<list>;
		using const_iterator = _list_const_iterator<list>;
		using reverse_iterator = mstd::reverse_iterator<iterator>;
		using const_reverse_iterator = mstd::reverse_iterator<const_iterator>;

		using _Node = _list_node<Tp, Alloc>;
		using _Node_ptr = _Node*;

	protected:

		_Node_ptr head_{};
		size_type size_{};

		void empty_init() noexcept {
			head_->next_ = head_;
			head_->prev_ = head_;
		}

		template<class... Args>
		void alloc_node_and_link(_Node_ptr pos, size_type num, Args&&... args) {
			_Node_ptr ptr{};
			for (; num > 0; --num) {
				ptr = _Node::create_node(std::forward<Args>(args)...); // 不能进行多次 std::move(value_type())；
				pos = _Node::link_node(pos, ptr);
			}
		}

		template<class IptIter, mstd::enable_if_t<mstd::_Is_iterator_v<IptIter>, int> = 0>
		void alloc_node_and_link(_Node_ptr pos, IptIter first, IptIter last) {
			_Node_ptr ptr{};
			for (; first != last; ++first) {
				ptr = _Node::create_node(*first);
				pos = _Node::link_node(pos, ptr);
			}
		}

		void exchange(list& other) noexcept {
			mstd::swap(other.head_, this->head_);
			mstd::swap(other.size_, this->size_);
		}

		bool is_invalid_iterator(const_iterator pos) noexcept {
			return false;
		}

	public:
		list() : head_(_Node::get_node()) { empty_init(); }
		explicit list(size_type num) : head_(_Node::get_node()), size_(num) {
			empty_init();
			alloc_node_and_link(head_, num);
		}
		list(size_type num, const value_type& val) : head_(_Node::get_node()), size_(num) {
			empty_init();
			alloc_node_and_link(head_, num, val);
		}

		template<class IptIter, mstd::enable_if_t<mstd::_Is_iterator_v<IptIter>, int> = 0>
		list(IptIter first, IptIter last) : head_(_Node::get_node()), size_(std::distance(first, last)) {
			empty_init();
			alloc_node_and_link(head_, first, last);
		}
		list(std::initializer_list<Tp> ilist) : head_(_Node::get_node()), size_(ilist.size()) {
			empty_init();
			alloc_node_and_link(head_, ilist.begin(), ilist.end());
		}
		list(const list& other) : head_(_Node::get_node()), size_(other.size()) {
			empty_init();
			alloc_node_and_link(head_, other.cbegin(), other.cend());
		}

		list& operator=(const list& other) {
			if (this != &other) {
				list temp{ other };
				this->swap(temp);
			}
			return *this;
		}
		list(list&& other) noexcept : head_(_Node::get_node()) {
			empty_init();
			exchange(other);
		}
		list& operator=(list&& other) noexcept {
			if (this != &other) {
				clear();
				exchange(other);
			}
			return *this;
		}

		list& operator=(std::initializer_list<value_type> ilist) {
			assign(ilist.begin(), ilist.end());
			return *this;
		}

		~list() {
			if (head_ != nullptr) {
				_Node::delete_nodes_non_head(head_);
				_Node::put_node(head_);
				head_ = nullptr;
			}
		}

	public:
		iterator begin() noexcept { return iterator(head_->next_); }
		const_iterator begin() const noexcept { return const_iterator(head_->next_); }
		iterator end() noexcept { return iterator(head_); }
		const_iterator end() const noexcept { return const_iterator(head_); }
		const_iterator cbegin() const noexcept { return const_iterator(head_->next_); }
		const_iterator cend() const noexcept { return const_iterator(head_); }
		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

		bool empty() const noexcept { return size_ == 0; }
		size_type size() const noexcept { return size_; }

		reference front() noexcept {
			return head_->next_->data_;
		}

		const_reference front() const noexcept {
			return head_->next_->data_;
		}

		reference back() noexcept {
			return head_->prev_->data_;
		}

		const_reference back() const noexcept {
			return head_->prev_->data_;
		}

		template<class IptIter, std::enable_if_t<mstd::_Is_iterator_v<IptIter>, int> = 0>
		void assign(IptIter first, IptIter last) {
			list temp(first, last);
			this->swap(temp);
		}

		void assign(size_type num, const value_type& val) {
			list temp(num, val);
			this->swap(temp);
		}

		void assign(std::initializer_list<value_type> ilist) {
			assign(ilist.begin(), ilist.end());
		}

		template<class... Args>
		void emplace(const_iterator pos, Args&&... args) {
			alloc_node_and_link((--pos).raw_ptr(), 1, std::forward<Args>(args)...);
			++size_;
		}

		template<class... Args>
		void emplace_back(Args&&... args) {
			emplace(end(), std::forward<Args>(args)...);
		}

		template<class... Args>
		void emplace_front(Args&&... args) {
			emplace(begin(), std::forward<Args>(args)...);
		}

		void push_front(const value_type& val) {
			emplace(begin(), val);
		}

		void push_back(const value_type& val) {
			emplace(end(), val);
		}

		void pop_front() noexcept {
			_Node::delete_range(head_->next_, head_->next_->next_);
			--size_;
		}

		void pop_back() noexcept {
			_Node::delete_range(head_->prev_, head_);
			--size_;
		}

		void erase(const_iterator pos) noexcept {
			_Node_ptr ptr = pos.raw_ptr();
			_Node::delete_range(ptr, ptr->next_);
			--size_;
		}

		void erase(const_iterator first, const_iterator last) noexcept {
			_Node_ptr fptr = first.raw_ptr();
			_Node_ptr lptr = last.raw_ptr();
			auto sz = std::distance(first, last);
			_Node::delete_range(fptr, lptr);
			size_ -= sz;
		}

		iterator insert(const_iterator pos, const value_type& val) {
			_Node_ptr ptr = (--pos).raw_ptr();
			alloc_node_and_link(ptr, 1, val);
			++size_;
			return iterator(ptr->next_);
		}

		iterator insert(const_iterator pos, size_type num, const value_type& val) {
			_Node_ptr ptr = (--pos).raw_ptr();
			alloc_node_and_link(ptr, num, val);
			size_ += num;
			return iterator(ptr->next_);
		}

		template<class IptIter, std::enable_if_t<mstd::_Is_iterator_v<IptIter>, int> = 0>
		iterator insert(const_iterator pos, IptIter first, IptIter last) {
			_Node_ptr ptr = (--pos).raw_ptr();
			alloc_node_and_link(ptr, first, last);
			size_ += std::distance(first, last);
			return iterator(ptr->next_);
		}

		iterator insert(const_iterator pos, value_type&& val) {
			_Node_ptr ptr = (--pos).raw_ptr();
			alloc_node_and_link(ptr, 1, std::move(val));
			++size_;
			return iterator(ptr->next_);
		}

		iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
			_Node_ptr ptr = (--pos).raw_ptr();
			alloc_node_and_link(ptr, ilist.begin(), ilist.end());
			size_ += ilist.size();
			return iterator(ptr->next_);
		}

		void resize(size_type num) {
			if (num > size()) {
				alloc_node_and_link((--end()).raw_ptr(), num - size());
			}
			else {
				_Node_ptr ptr{ head_->next_ };
				for (; num > 0; --num, ptr = ptr->next_);
				_Node::delete_range(ptr, head_);
			}
			size_ = num;
		}

		void resize(size_type num, const value_type& val) {
			if (num > size()) {
				alloc_node_and_link((--end()).raw_ptr(), num - size(), val);
			}
			else {
				_Node_ptr ptr{ head_->next_ };
				for (; num > 0; --num, ptr = ptr->next_);
				_Node::delete_range(ptr, head_);
			}
			size_ = num;
		}

		void clear() noexcept {
			_Node::delete_range(head_->next_, head_);
			size_ = 0;
		}

		void swap(list& right) noexcept {
			if (this != &right) {
				mstd::swap(head_, right.head_);
				mstd::swap(size_, right.size_);
			}
		}

		/*
		Operations:
		splice		Transfer elements from list to list (public member function)
		remove		Remove elements with specific value (public member function)
		remove_if	Remove elements fulfilling condition (public member function template)
		unique		Remove duplicate values (public member function)
		merge		Merge sorted lists (public member function)
		sort		Sort elements in container (public member function)
		reverse		Reverse the order of elements (public member function)
		*/

		void splice(const_iterator pos, list& other) noexcept {
			_Node::transfer(pos.raw_ptr(), other.begin().raw_ptr(), other.end().raw_ptr());
			size_ += other.size_;
			other.size_ = 0;
		}

		void splice(const_iterator pos, list&& other) noexcept {
			splice(pos, other);
		}

		void splice(const_iterator pos, list& other,
			const_iterator other_pos) noexcept {
			_Node::transfer(pos.raw_ptr(), other_pos.raw_ptr(),
				other_pos.raw_ptr()->next_);
			++size_;
			--other.size_;
		}

		void splice(const_iterator pos, list&& other,
			const_iterator other_pos) noexcept {
			splice(pos, other, other_pos);
		}

		void splice(const_iterator pos, list& other,
			const_iterator first, const_iterator last) noexcept {
			_Node::transfer(pos.raw_ptr(), first.raw_ptr(), last.raw_ptr());
			auto sz = std::distance(first, last);
			size_ += sz;
			other.size_ -= sz;
		}

		void splice(const_iterator pos, list&& other,
			const_iterator first, const_iterator last) noexcept {
			splice(pos, first, last);
		}

		void remove(const value_type& val) {
			remove_if([&](const value_type& other) { return other == val; });
		}

		template<class Pred>
		void remove_if(Pred pred) {
			_Node_ptr begin_ptr = head_->next_;
			_Node_ptr temp{};
			while (begin_ptr != head_) {
				temp = begin_ptr->next_;
				if (pred(begin_ptr->data_)) {
					begin_ptr->prev_->next_ = begin_ptr->next_;
					begin_ptr->next_->prev_ = begin_ptr->prev_;
					_Node::delete_node(begin_ptr);
					--size_;
				}
				begin_ptr = temp;
			}
		}

		void unique() {
			unique(std::equal_to<>{});
		}

		template<class Binary_Pred>
		void unique(Binary_Pred pred) {
			if (size_ > 1) {
				_Node_ptr prev = head_->next_;
				_Node_ptr curr = head_->next_->next_;
				while (curr != head_) {
					if (pred(curr->data_, prev->data_)) {
						prev->prev_->next_ = curr;
						curr->prev_ = prev->prev_;
						_Node::delete_node(prev);
						--size_;
					}
					prev = curr;
					curr = curr->next_;
				}
			}
		}

		void merge(list& other) {
			merge(other, std::less<>{});
		}

		template<class Compare>
		void merge(list& other, Compare Cmp) {
			_Node_ptr this_ptr = head_->next_;
			_Node_ptr other_ptr = other.head_->next_;
			_Node_ptr temp{};
			while (this_ptr != head_ && other_ptr != other.head_) {
				if (Cmp(this_ptr->data_, other_ptr->data_)) {
					temp = other_ptr->next_;
					_Node::transfer(this_ptr, other_ptr, temp);
					++size_; --other.size_;
					other_ptr = temp;
				}
				else {
					this_ptr = this_ptr->next_;
				}
			}
			if (other_ptr != other.head_) {
				_Node::transfer(this_ptr, other_ptr, other.head_);
				++size_; --other.size_;
			}
		}

		void sort() {
			sort(std::less<>{});
		}

		template<class Compare>
		void sort(Compare Cmp) {
			_Node_ptr ptr = head_->next_;
			_Node::sort_impl(ptr, size_, Cmp);
		}

		void reverse() noexcept {
			const _Node_ptr phead = head_;
			_Node_ptr pnode = phead;
			for (;;) {
				const _Node_ptr pnext = pnode->next_;
				pnode->next_ = pnode->prev_;
				pnode->prev_ = pnext;
				if (pnext == phead) break;
				pnode = pnext;
			}
		}

	};

	template<class Tp, class Alloc>
	inline void swap(list<Tp, Alloc>& left,
		list<Tp, Alloc>& right) noexcept {
		left.swap(right);
	}

	template<class Tp, class Alloc>
	inline bool operator==(list<Tp, Alloc>& left,
		list<Tp, Alloc>& right) noexcept {
		return left.size() == right.size() &&
			mstd::equal(left.begin(), left.end(), right.begin());
	}

	template<class Tp, class Alloc>
	inline bool operator!=(list<Tp, Alloc>& left,
		list<Tp, Alloc>& right) noexcept {
		return !(left == right);
	}

	template<class Tp, class Alloc>
	inline bool operator<(list<Tp, Alloc>& left,
		list<Tp, Alloc>& right) noexcept {
		return mstd::lexicographical_compare(left.cbegin(),
			left.cend(), right.cbegin(), right.cend());
	}

	template<class Tp, class Alloc>
	inline bool operator>(list<Tp, Alloc>& left,
		list<Tp, Alloc>& right) noexcept {
		return right < left;
	}

	template<class Tp, class Alloc>
	inline bool operator<=(list<Tp, Alloc>& left,
		list<Tp, Alloc>& right) noexcept {
		return !(left > right);
	}

	template<class Tp, class Alloc>
	inline bool operator>=(list<Tp, Alloc>& left,
		list<Tp, Alloc>& right) noexcept {
		return !(left < right);
	}

}