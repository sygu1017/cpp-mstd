#pragma once

#include "m_alloc.h"			// malloc_allocator;
#include "m_constructor.h"		// 
#include "m_iterator.h"			// reverse_iterator;
#include "m_memory.h"			// _Tidy_guard;

#include <type_traits>



namespace mstd {

	template<class Deque_type>
	struct _Deque_const_iterator {

		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename Deque_type::value_type;
		using size_type = typename Deque_type::size_type;
		using difference_type = typename Deque_type::difference_type;
		using pointer = typename Deque_type::const_pointer;
		using reference = const value_type&;

		static constexpr int block_size = Deque_type::block_size;

		const Deque_type* Mycont_{};
		size_type Myoff_{};

		Deque_type() noexcept = default;
		Deque_type(const Deque_type* cont_ptr, size_type off) noexcept
			: Mycont_(cont_ptr), Myoff_(off) {}

		reference operator*() const noexcept {
			size_type block = Mycont_->_Get_block();
			size_type off = Myoff_ % block_size;
			return Mycont_->_Get_map()[block][off];
		}

		pointer operator->() const noexcept {
			size_type block = Mycont_->_Get_block();
			size_type off = Myoff_ % block_size;
			return Mycont_->_Get_map()[block] + off;
		}

		_Deque_const_iterator& operator++() const noexcept {
			++Myoff_;
			return *this;
		}

		_Deque_const_iterator operator++(int) const noexcept {
			_Deque_const_iterator temp = *this;
			++Myoff_;
			return temp;
		}

		_Deque_const_iterator& operator--() const noexcept {
			--Myoff_;
			return *this;
		}

		_Deque_const_iterator operator--(int) const noexcept {
			_Deque_const_iterator temp = *this;
			--Myoff_;
			return temp;
		}

		_Deque_const_iterator& operator+=(const difference_type off) const noexcept {
			Myoff_ += off;
			return *this;
		}

		_Deque_const_iterator& operator-=(const difference_type off) const noexcept {
			Myoff_ -= off;
			return *this;
		}

		_Deque_const_iterator operator+(const difference_type off) const noexcept {
			_Deque_const_iterator temp = *this;
			temp += off;
			return temp;
		}

		_Deque_const_iterator operator-(const difference_type off) const noexcept {
			_Deque_const_iterator temp = *this;
			temp -= off;
			return temp;
		}

		difference_type operator-(const _Deque_const_iterator& right) const noexcept {
			return static_cast<difference_type>(Myoff_ - right->Myoff_);
		}

		bool operator==(const _Deque_const_iterator& right) const noexcept {
			return Myoff_ == right.Myoff_;
		}

		bool operator!=(const _Deque_const_iterator& right) const noexcept {
			return !(*this == right);
		}

		bool operator<(const _Deque_const_iterator& right) const noexcept {
			return Myoff_ < right.Myoff_;
		}

		bool operator>(const _Deque_const_iterator& right) const noexcept {
			return Myoff_ > right.Myoff_;
		}

		bool operator<=(const _Deque_const_iterator& right) const noexcept {
			return !(*this > right);
		}

		bool operator>=(const _Deque_const_iterator& right) const noexcept {
			return !(*this < right);
		}

	};

	template<class Deque_type>
	struct _Deque_iterator : _Deque_const_iterator<Deque_type> {

		using value_type = typename Deque_type::value_type;
		using size_type = typename Deque_type::size_type;
		using difference_type = typename Deque_type::difference_type;
		using pointer = typename Deque_type::pointer;
		using reference = value_type&;

		using _MyParent = _Deque_const_iterator<Deque_type>;

		using _MyParent::_MyParent;  // 使用父辈构造函数

		reference operator*() const noexcept {
			return const_cast<reference>(_MyParent::operator*());
		}

		pointer operator->() const noexcept {
			return const_cast<pointer>(_MyParent::operator->());
		}

		_Deque_iterator& operator++() const noexcept {
			_MyParent::operator++();
			return *this;
		}

		_Deque_iterator operator++(int) const noexcept {
			_Deque_iterator temp = *this;
			_MyParent::operator++();
			return temp;
		}

		_Deque_iterator& operator--() const noexcept {
			_MyParent::operator--();
			return *this;
		}

		_Deque_iterator operator--(int) const noexcept {
			_Deque_iterator temp = *this;
			_MyParent::operator--();
			return temp;
		}

		_Deque_iterator& operator+=(const difference_type off) const noexcept {
			_MyParent::operator+=(off);
			return *this;
		}

		_Deque_iterator& operator-=(const difference_type off) const noexcept {
			_MyParent::operator-=(off);
			return *this;
		}

		_Deque_iterator operator+(const difference_type off) const noexcept {
			_Deque_iterator temp = *this;
			temp += off;
			return temp;
		}

		using _MyParent::operator-;

		_Deque_iterator operator-(const difference_type off) const noexcept {
			_Deque_iterator temp = *this;
			temp -= off;
			return temp;
		}

	};


	template<class Tp, class Alloc = malloc_allocator<0>>
	class deque {
	public:
		static_assert(!mstd::is_const_v<Tp>, "The container element is not allowed to be const type.");

		using data_allocator = Alloc;
		using value_type = Tp;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using _Map_ptr = Tp**;

		using iterator = _Deque_iterator<deque>;
		using const_iterator = _Deque_const_iterator<deque>;
		using reverse_iterator = mstd::reverse_iterator<iterator>;
		using const_reverse_iterator = mstd::reverse_iterator<const_iterator>;

	private:
		static constexpr size_type _Bytes = sizeof(Tp);

	protected:
		static constexpr int _Block_size =
			_Bytes <= 1 ? 16
			: _Bytes <= 2 ? 8
			: _Bytes <= 4 ? 4
			: _Bytes <= 8 ? 2
			: 1;

		_Map_ptr Mycont_{};
		size_type Mapsize_{};
		size_type Myoff_{};
		size_type Mysize_{};

	public:
		deque() noexcept = default;
		deque(size_type num) {

		}

	private:
		template<class... Args>
		void _Emplace_back_internal(Args&&... args) {

		}

		template<class... Args>
		void _Emplace_front_internal(Args&&... args) {

		}

		void _Grow_map(size_type count) {

		}

		template<class IptIter>
		void _Construct(IptIter first, IptIter last) {
			_Tidy_guard guard{ this };
			for (; first != last; ++first) {
				_Emplace_back_internal(*first);
			}
			guard._Release();
		}

		void _Construct(size_type count, const value_type& val) {
			_Tidy_guard guard{ this };
			for (; count > 0; --count) {
				_Emplace_back_internal(val);
			}
			guard._Release();
		}





	};

}

