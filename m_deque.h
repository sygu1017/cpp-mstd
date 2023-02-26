#pragma once

#include "m_utility.h"			// 
#include "m_iterator.h"			// reverse_iterator;
#include "m_memory.h"			// _Tidy_guard;


namespace mstd {

	template<class Deque_type>
	struct _Deque_unchecked_const_iterator {

	};





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

	template <class Value_type, class Size_type, class Difference_type, class Pointer, class Const_pointer, class Reference, class Const_reference, class Mapptr_type>
	struct _Deque_iter_types {
		using value_type = Value_type;
		using size_type = Size_type;
		using difference_type = Difference_type;
		using pointer = Pointer;
		using const_pointer = Const_pointer;
		using _Map_ptr = Mapptr_type;
	};

	template <class Tp>
	struct _Deque_simple_types : _Simple_types<Tp> {
		using _Map_ptr = Tp**;
	};

	template<class Val_type>
	class _Deque_val : public _Container_base_real {  // 继承了proxy_/firstiter_数据成员
	public:
		using value_type = typename Val_type::value_type;
		using size_type = typename Val_type::size_type;
		using difference_type = typename Val_type::difference_type;
		using pointer = typename Val_type::pointer;
		using const_pointer = typename Val_type::const_pointer;
		using reference = value_type&;
		using const_reference = const value_type&;
		using _Map_ptr = typename Val_type::_Map_ptr;

	private:
		static constexpr size_type _Bytes = sizeof(value_type);

	public:
		static constexpr int _Block_size =
			_Bytes <= 1 ? 16
			: _Bytes <= 2 ? 8
			: _Bytes <= 4 ? 4
			: _Bytes <= 8 ? 2
			: 1;

		_Map_ptr map_{};		// 循环队列头指针
		size_type mapsize_{};	// 循环队列大小
		size_type off_{};		// 第一个元素相对于头部位置的偏移量
		size_type size_{};		// 元素总数

		_Deque_val() noexcept = default;

		// 求得偏移量为off的元素所处内存块头指针 在循环列表中的位置
		size_type _Get_block(size_type off) const noexcept { return (off / _Block_size) & (mapsize_ - 1); }
	};

	template<class Tp, class Alloc = mstd::allocator<Tp>>
	class deque {
	private:
		friend _Tidy_guard<deque>;

		using _Alloc_t = _Rebind_alloc_t<Alloc, Tp>;
		using _Alloc_traits = allocator_traits<_Alloc_t>;

		using _Alloc_ptr_t = _Rebind_alloc_t<Alloc, typename _Alloc_traits::pointer>;
		using _Alloc_ptr_traits = allocator_traits<_Alloc_ptr_t>;

		using _Alloc_proxy_t = _Rebind_alloc_t<Alloc, _Container_proxy>;
		using _Alloc_proxy_traits = allocator_traits<_Alloc_proxy_t>;

		using _Map_ptr = typename _Alloc_ptr_traits::pointer;

		using _Internal_val = _Deque_val<conditional_t<_Is_simple_alloc_v<_Alloc_t>, _Deque_simple_types<Tp>, _Deque_iter_types<typename _Alloc_traits::value_type, typename _Alloc_traits::size_type, typename _Alloc_traits::difference_type, typename _Alloc_traits::pointer, typename _Alloc_traits::const_pointer, Tp&, const Tp&, _Map_ptr>>>;

		static constexpr int _Minimum_map_size = 8;
		static constexpr int _Block_size = _Internal_val::_Block_size;

		// deque内部数据成员
		_Compressed_pair<_Alloc_t, _Internal_val> m_pair_;

	public:

		using data_allocator = Alloc;
		using value_type = Tp;
		using size_type = typename _Alloc_traits::size_type;
		using difference_type = typename _Alloc_traits::difference_type;
		using pointer = typename _Alloc_traits::pointer;
		using const_pointer = typename _Alloc_traits::const_pointer;
		using reference = value_type&;
		using const_reference = const value_type&;

		using iterator = _Deque_iterator<_Internal_val>;
		using const_iterator = _Deque_const_iterator<_Internal_val>;
		using reverse_iterator = mstd::reverse_iterator<iterator>;
		using const_reverse_iterator = mstd::reverse_iterator<const_iterator>;

		using _unchecked_const_iterator = _Deque_unchecked_const_iterator<_Internal_val>;
		using _unchecked_iterator = _Deque_unchecked_iterator<_Internal_val>;

	public:
		deque() : m_pair_(_Zero_then_variadic_args_t{}) {

		}

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







		_Internal_val& _Get_data() noexcept { return m_pair_.second_; }

		const _Internal_val& _Get_data() const noexcept { return m_pair_.second_; }

		size_type _Get_block(size_type off) const noexcept { _Get_data()._Get_block(off); }

		void _Orphan_all() noexcept { _Get_data()._Orphan_all(); }

		Alloc& _Get_alloc() noexcept { _Get_data()._Get_first(); }

		const Alloc& _Get_alloc() const noexcept { _Get_data()._Get_first(); }

		_Map_ptr& _Map() noexcept { return _Get_data().map_; }

		const _Map_ptr& _Map() const noexcept { return _Get_data().map_; }

		size_type& _Mapsize() noexcept { return _Get_data().mapsize_; }

		const size_type& _Mapsize() const noexcept { return _Get_data().mapsize_; }

		size_type& _Myoff() noexcept { return _Get_data().off_; }

		const size_type& _Myoff() const noexcept { return _Get_data().off_; }

		size_type& _Mysize() noexcept { return _Get_data().size_; }

		const size_type& _Mysize() const noexcept { return _Get_data().size_; }











	};

}

