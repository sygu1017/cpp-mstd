#pragma once

#include <exception>		// std::bad_alloc;

#include "m_type_traits.h"	// is_trivially_destructible<>;
#include "m_utility.h"		// pointer_traits;


namespace mstd {

	// C++11 memory

	// 包装 operator new、operator delete 实现内存分配函数
	struct _Default_allocate_traits {
		static void* _Allocate(const size_t bytes) {
			return ::operator new(bytes);
		}
		static void* _Allocate_aligned(const size_t bytes, const size_t align) {
			return ::operator new(bytes, std::align_val_t{ align });
		}
	};

	constexpr size_t __MSTDCPP_DEFAULT_NEW_ALIGNMENT__ = 8u;

	constexpr size_t _Big_allocation_threshold = 4096;
	constexpr size_t _Big_allocation_alignment = 32;

	template<size_t Tp_size>
	inline constexpr size_t _Get_size_of_n(const size_t count) {
		return Tp_size * count;
	}

	template<size_t Align, class Traits = _Default_allocate_traits, enable_if_t<(Align > __MSTDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0>
		inline void* _Allocate(const size_t bytes) {
		if (bytes == 0) return nullptr;
		size_t passed_align = std::max(Align, _Big_allocation_alignment);
		return Traits::_Allocate_aligned(bytes, passed_align);
	}

	template<size_t Align, enable_if_t<(Align > __MSTDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0 >
		inline void _Deallocate(void* ptr, const size_t bytes) noexcept {
		size_t passed_align = std::max(Align, _Big_allocation_alignment);
		::operator delete(ptr, bytes, std::align_val_t{ passed_align });
	}

	template<size_t Align, class Traits = _Default_allocate_traits, enable_if_t<(Align <= __MSTDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0>
	inline void* _Allocate(const size_t bytes) {
		if (bytes >= _Big_allocation_threshold) {
			return std::_Allocate_manually_vector_aligned<Traits>(bytes);
		}
		if (bytes != 0) return Traits::_Allocate(bytes);
		return nullptr;
	}

	template<size_t Align, enable_if_t<(Align <= __MSTDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0>
	inline void _Deallocate(void* ptr, size_t bytes) noexcept {
		if (bytes >= _Big_allocation_threshold) {
			return std::_Adjust_manually_vector_aligned(ptr, bytes);
		}
		::operator delete(ptr, bytes);
	}

	template<class Tp>
	constexpr size_t _New_alignof = std::max(alignof(Tp), __MSTDCPP_DEFAULT_NEW_ALIGNMENT__);

	template<class Tp, class... Args>
	inline Tp* _Globle_new(Args&&...args) {
		struct _Guard_type {
			void* result;
			~_Globle_new() {
				if (result != nullptr) {
					_Deallocate<_New_alignof<Tp>>(result, sizeof(Tp));
				}
			}
		};
		_Guard_type guard{ _Allocate<_New_alignof<Tp>>(sizeof(Tp)) };
		::new(guard.result) Tp(mstd::forward<Args>(args)...);
		return static_cast<Tp*>(mstd::exchange(guard.result, nullptr));
	}

	template<class Iter>
	constexpr void* _Voidify_iter(Iter it) noexcept {
		if constexpr (mstd::is_pointer_v<Iter>) {
			return const_cast<void*>(static_cast<const volatile void*>(it));
		}
		else {
			return const_cast<void*>(static_cast<const volatile void*>(std::addressof(*it)));
		}
	}

	// 包装 palcement new
	template<class Tp, class... Args>
	inline void _Construct_in_place(Tp& obj, Args&&... args) noexcept(mstd::is_nothrow_constructible<Tp, Args...>::value) {
		::new (_Voidify_iter(std::addressof(obj))) Tp(mstd::forward<Args>(args)...);
	}

	template<class Tp>
	inline void _Default_construct_in_place(Tp& obj) noexcept(mstd::is_nothrow_default_constructible<Tp>::value) {
		::new (_Voidify_iter(std::addressof(obj))) Tp;
	}


	// 实现 allocator; allocator_traits; 

	template<class Alloc, class = void>
	struct _Get_pointer_type {
		using type = typename Alloc::value_type*;
	};

	template<class Alloc>
	struct _Get_pointer_type<Alloc, void_t<typename Alloc::pointer>> {
		using type = typename Alloc::pointer;
	};

	template<class Alloc, class = void>
	struct _Get_const_pointer {
		using _Ptr_type = typename _Get_pointer_type<Alloc>::type;
		using _Val_type = typename Alloc::value_type;
		using type = typename pointer_traits<_Ptr_type>::template rebind<const _Val_type>;
	};

	template<class Alloc>
	struct _Get_const_pointer<Alloc, void_t<typename Alloc::const_pointer>> {
		using type = typename Alloc::const_pointer;
	};

	template<class Alloc, class = void>
	struct _Get_void_pointer {
		using _Ptr_type = typename _Get_pointer_type<Alloc>::type;
		using type = typename pointer_traits<_Ptr_type>::template rebind<void>;
	};

	template<class Alloc>
	struct _Get_void_pointer<Alloc, void_t<typename Alloc::void_pointer>> {
		using type = typename Alloc::void_pointer;
	};

	template<class Alloc, class = void>
	struct _Get_const_void_pointer {
		using _Ptr_type = typename _Get_pointer_type<Alloc>::type;
		using type = typename pointer_traits<_Ptr_type>::template rebind<const void>;
	};

	template<class Alloc>
	struct _Get_const_void_pointer<Alloc, void_t<typename Alloc::const_void_pointer>> {
		using type = typename Alloc::const_void_pointer;
	};

	template <class Alloc, class = void>
	struct _Get_difference_type {
		using _Ptr_type = typename _Get_pointer_type<Alloc>::type;
		using type = typename pointer_traits<_Ptr_type>::difference_type;
	};

	template <class Alloc>
	struct _Get_difference_type<Alloc, void_t<typename Alloc::difference_type>> {
		using type = typename Alloc::difference_type;
	};

	template<class Alloc, class = void>
	struct _Get_size_type {
		using type = mstd::make_unsigned_t<typename _Get_difference_type<Alloc>::type>;
	};

	template<class Alloc>
	struct _Get_size_type<Alloc, void_t<typename Alloc::size_type>> {
		using type = typename Alloc::size_type;
	};

	template<class Alloc, class = void>
	struct _Get_propagate_on_container_copy {
		using type = false_type;
	};

	template<class Alloc>
	struct _Get_propagate_on_container_copy<Alloc, void_t<typename Alloc::propagate_on_container_on_copy_assignment>> {
		using type = typename Alloc::propagate_on_container_copy_assignment;
	};

	template<class Alloc, class = void>
	struct _Get_propagate_on_container_move {
		using type = false_type;
	};

	template<class Alloc>
	struct _Get_propagate_on_container_move<Alloc, void_t<typename Alloc::propagate_on_container_move_assignment>> {
		using type = typename Alloc::propagate_on_container_move_assignment;
	};

	template<class Alloc, class = void>
	struct _Get_propagate_on_container_swap {
		using type = false_type;
	};

	template<class Alloc>
	struct _Get_propagate_on_container_swap<Alloc, void_t<typename Alloc::propagate_on_container_swap>> {
		using type = typename Alloc::propagate_on_container_swap;
	};

	template<class Alloc, class = void>
	struct _Get_is_always_equal {
		using type = typename is_empty<Alloc>::type;
	};

	template<class Alloc>
	struct _Get_is_always_equal<Alloc, void_t<typename Alloc::is_always_equal>> {
		using type = typename Alloc::is_always_equal;
	};

	template<class Tp, class Other, class = void>
	struct _Get_rebind_type {
		using type = typename _Replace_first_parameter<Other, Tp>::type;
	};

	template<class Tp, class Other>
	struct _Get_rebind_type<Tp, Other, void_t<typename Tp::template rebind<Other>::other>> {
		using type = typename Tp::template rebind<Other>::other;
	};

	template<class Tp>
	class allocator;

	template<class Alloc, class = void>
	struct _Is_default_allocator : false_type {};

	template<class Tp>
	struct _Is_default_allocator<allocator<Tp>, void_t<typename allocator<Tp>::_From_primary>> : mstd::is_same<allocator<Tp>, typename allocator<Tp>::_From_primary>::type {};

	// 判断用户定义分配器是否定义了相关函数
	template<class Void, class... Args>
	struct _Has_no_alloc_construct : true_type {};

	template<class Alloc, class Ptr, class... Args>
	struct _Has_no_alloc_construct<void_t<decltype(mstd::declval<Alloc&>().construct(mstd::declval<Ptr>(), mstd::declval<Args>()...))>, Alloc, Ptr, Args...> : false_type {};

	template<class Alloc, class Ptr, class...Args>
	using _Uses_default_construct = mstd::disjunction<_Is_default_allocator<Alloc>, _Has_no_alloc_construct<void, Alloc, Ptr, Args...>>;

	template<class Alloc, class Ptr, class = void>
	struct _Has_no_alloc_destroy : true_type {};

	template<class Alloc, class Ptr>
	struct _Has_no_alloc_destroy<Alloc, Ptr, void_t<decltype(mstd::declval<Alloc&>().destroy(mstd::declval<Ptr>()))>> : false_type {};

	template<class Alloc, class Ptr>
	using _Uses_default_destroy = mstd::disjunction<_Is_default_allocator<Alloc>, _Has_no_alloc_destroy<Alloc, Ptr>>;

	template<class Alloc, class Size_type, class Const_void_pointer, class = void>
	struct _Has_allocate_hint : false_type {};

	template<class Alloc, class Size_type, class Const_void_pointer>
	struct _Has_allocate_hint<Alloc, Size_type, Const_void_pointer, void_t<decltype(mstd::declval<Alloc&>().allocate(mstd::declval<const Size_type&>(), mstd::declval<const Const_void_pointer&>()))>> : true_type {};

	template<class Alloc, class = void>
	struct _Has_max_size : false_type {};

	template<class Alloc>
	struct _Has_max_size<Alloc, void_t<decltype(mstd::declval<Alloc&>.max_size())>> : true_type {};

	template<class Alloc, class = void>
	struct _Has_select_on_container_copy_construction : false_type {};

	template<class Alloc>
	struct _Has_select_on_container_copy_construction<Alloc, void_t<decltype(mstd::declval<const Alloc&>().select_on_container_copy_construction())>> : true_type {};  // const Alloc&

	template<class Alloc>
	struct allocator_traits;

	// 分配器萃取器
	template<class Alloc>
	struct _Normal_allocator_traits { // can be used for all allocators

		using allocator_type = Alloc;
		using value_type = typename Alloc::value_type;

		using pointer = typename _Get_pointer_type<Alloc>::type;
		using const_pointer = typename _Get_const_pointer<Alloc>::type;
		using void_pointer = typename _Get_void_pointer<Alloc>::type;
		using const_void_pointer = typename _Get_const_void_pointer<Alloc>::type;

		using size_type = typename _Get_size_type<Alloc>::type;
		using difference_type = typename _Get_difference_type<Alloc>::type;

		using propagate_on_container_copy_assignment = typename _Get_propagate_on_container_copy<Alloc>::type;
		using propagate_on_container_move_assignment = typename _Get_propagate_on_container_move<Alloc>::type;
		using propagate_on_container_swap = typename _Get_propagate_on_container_swap<Alloc>::type;
		using is_always_equal = typename _Get_is_always_equal<Alloc>::type;

		template<class Other>
		using rebind_alloc = typename _Get_rebind_type<Alloc, Other>::type;

		template<class Other>
		using rebind_traits = allocator_traits<rebind_alloc<Other>>;

		static pointer allocate(Alloc& alloc, const size_type count) {
			return alloc.allocate(count);
		}

		static pointer allocate(Alloc& alloc, const size_type count, const const_void_pointer& hint) {
			if constexpr (_Has_allocate_hint<Alloc, size_type, const_void_pointer>::value) {
				return alloc.allocate(count, hint);
			}
			else {
				return alloc.allocate(count);
			}
		}

		static void deallocate(Alloc& alloc, pointer ptr, const size_type count) {
			alloc.deallocate(ptr, count);
		}

		template<class Tp, class... Args>
		static void construct(Alloc& alloc, Tp* ptr, Args&&... args) {
			if constexpr (_Uses_default_construct<Alloc, Tp*, Args...>::value) {
				::new (static_cast<void*>(ptr)) Tp(mstd::forward<Args>(args)...);
			}
			else {
				alloc.construct(ptr, mstd::forward<Args>(args)...);
			}
		}

		template<class Tp>
		static void destroy(Alloc& alloc, Tp* ptr) {
			if constexpr (_Has_no_alloc_destroy<Alloc, Tp*>::value) {
				ptr->~Tp();
			}
			else {
				alloc.destroy(ptr);
			}
		}

		static size_type max_size(const Alloc& alloc) noexcept {
			if constexpr (_Has_max_size<Alloc>::value) {
				return alloc.max_size();
			}
			else {
				return (std::numeric_limits<size_type>::max)() / sizeof(value_type);
			}
		}

		static Alloc select_on_container_copy_construction(const Alloc& alloc) {
			if constexpr (_Has_select_on_container_copy_construction<Alloc>::value) {
				return alloc.select_on_container_copy_construction();
			}
			else {
				return alloc;
			}
		}

	};

	// 默认分配器的萃取器
	template<class Alloc>
	struct _Default_allocator_traits {	// traits for allocator<Tp>;

		using allocator_type = Alloc;
		using value_type = typename Alloc::value_type;

		using pointer = value_type*;
		using const_pointer = const value_type*;
		using void_pointer = void*;
		using const_void_pointer = const void*;

		using size_type = size_t;
		using difference_type = ptrdiff_t;

		using propagate_on_container_copy_assignment = false_type;
		using propagate_on_container_move_assignment = true_type;
		using propagate_on_container_swap = false_type;
		using is_always_equal = true_type;

		template<class Other>
		using rebind_alloc = allocator<Other>;

		template<class Other>
		using rebind_traits = allocator_traits<allocator<Other>>;

		static pointer allocate(Alloc& alloc, const size_type count) {
			return _Allocate<_New_alignof<value_type>>(_Get_size_of_n<sizeof(value_type)>(count));
		}

		static void deallocate(Alloc& alloc, const pointer ptr, const size_type count) {
			_Deallocate<_New_alignof<size_type>>(_Get_size_of_n<sizeof(value_type)>(count));
		}

		template<class Tp, class...Args>
		static void construct(Alloc& alloc, Tp* const ptr, Args&&...args) {
			::new (_Voidify_iter(ptr)) Tp(mstd::forward<Args>(args)...);
		}

		template<class Tp>
		static void destroy(Alloc& alloc, Tp* const ptr) {
			ptr->~Tp();
		}

		static size_type max_size(const Alloc&) noexcept {
			return static_cast<size_type>(-1) / sizeof(value_type);
		}

		static Alloc select_on_container_copy_construction(const Alloc& alloc) {
			return alloc;
		}

	};

	template<class Alloc>
	struct allocator_traits : conditional_t<_Is_default_allocator<Alloc>::value, _Default_allocator_traits<Alloc>, _Normal_allocator_traits<Alloc>> {};

	// 标准默认分配器
	template<class Tp>
	class allocator {
	public:
		static_assert(!mstd::is_const_v<Tp>, "allocator<const Tp> is ill-formed.");

		using _From_primary = allocator;

		using value_type = Tp;
		using pointer = Tp*;
		using const_pointer = const Tp*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		using propagate_on_container_move_assignment = true_type;
		using is_always_equal = true_type;

		template<class Other>
		struct rebind {
			using other = allocator<Other>;
		};

		Tp* address(Tp& obj) const noexcept { return std::addressof(obj); }
		const Tp* address(const Tp& obj) const noexcept { return std::addressof(obj); }

		constexpr allocator() noexcept {}
		constexpr allocator(const allocator&) noexcept = default;

		template<class Other>
		constexpr allocator(const allocator<Other>&) noexcept {}
		allocator& operator=(const allocator&) noexcept = default;
		~allocator() = default;

		Tp* allocate(const size_type count) {
			return static_cast<Tp*>(_Allocate<_New_alignof<Tp>>(_Get_size_of_n<sizeof(Tp)>(count)));
		}

		Tp* allocate(const size_type count, const void*) {
			return allocate(count);
		}

		void deallocate(Tp* const ptr, const size_type count) {
			assert(ptr != nullptr || count == 0);
			_Deallocate<_New_alignof<Tp>>(ptr, sizeof(Tp) * count);
		}

		template<class Tp, class... Args>
		void construct(Tp* const ptr, Args&&...args) {
			::new (_Voidify_iter(ptr)) Tp(mstd::forward<Args>(args)...);
		}

		void destroy(Tp* const ptr) { ptr->~Tp(); }

		size_type max_size() const noexcept { return static_cast<size_type>(-1) / sizeof(Tp); }

	};

	template<class Tp, class Other>
	bool operator==(const allocator<Tp>&, const allocator<Other>&) noexcept { return true; }

	template<class Tp, class Other>
	bool operator!=(const allocator<Tp>&, const allocator<Other>&) noexcept { return false; }

	template <class Alloc, class Value_type>
	using _Rebind_alloc_t = typename allocator_traits<Alloc>::template rebind_alloc<Value_type>;

	template <class Alloc>
	constexpr bool _Is_simple_alloc_v = is_same_v<typename allocator_traits<Alloc>::size_type, size_t> &&
		is_same_v<typename allocator_traits<Alloc>::difference_type, ptrdiff_t> &&
		is_same_v<typename allocator_traits<Alloc>::pointer, typename Alloc::value_type*> &&
		is_same_v<typename allocator_traits<Alloc>::const_pointer, const typename Alloc::value_type*>;

	// _Is_simple_alloc_v<Alloc> = true 条件下，容器成员类型为_Simple_types
	template <class Value_type>
	struct _Simple_types {
		using value_type = Value_type;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using pointer = value_type*;
		using const_pointer = const value_type*;
	};

	// 判断分配器拷贝赋值元素的兼容性
	template <class Alloc>
	constexpr bool _Choose_pocca_v = allocator_traits<Alloc>::propagate_on_container_copy_assignment::value
		&& !allocator_traits<Alloc>::is_always_equal::value;

	enum class _Pocma_values {
		_Equal_allocators, // 可以移动元素，甚至是交换，allocator<Tp>
		_Propagate_allocators, // 通常分配器可以增值，可以移动元素
		_No_propagate_allocators, // 分配器不兼容，将移动转换为拷贝
	};

	// 判断分配器移动赋值元素的兼容性
	template <class Alloc>
	constexpr _Pocma_values
		_Choose_pocma_v = allocator_traits<Alloc>::is_always_equal::value
		? _Pocma_values::_Equal_allocators
		: (allocator_traits<Alloc>::propagate_on_container_move_assignment::value
			? _Pocma_values::_Propagate_allocators
			: _Pocma_values::_No_propagate_allocators);

	template <class Alloc>
	void _Pocca(Alloc& left, const Alloc& right) noexcept {
		if constexpr (allocator_traits<Alloc>::propagate_on_container_copy_assignment::value) {
			left = right;
		}
	}

	template <class Alloc>
	void _Pocma(Alloc& left, Alloc& right) noexcept {
		if constexpr (allocator_traits<Alloc>::propagate_on_container_move_assignment::value) {
			left = mstd::move(right);
		}
	}

	template <class Alloc>
	void _Pocs(Alloc& left, Alloc& right) noexcept {
		if constexpr (allocator_traits<Alloc>::propagate_on_container_swap::value) {
			swap(left, right);
		}
	}

	template<class FwdIter>
	inline void _Destroy_range(FwdIter first, FwdIter last) noexcept;

	template<class Tp>
	inline void _Destroy_in_place(Tp& obj) noexcept {
		if constexpr (mstd::is_array_v<Tp>) {
			_Destroy_range(obj, obj + mstd::extent_v<Tp>);
		}
		else {
			obj.~Tp();
		}
	}

	template<class Tp>
	inline void destroy_at(Tp* const ptr) noexcept {
		ptr->~Tp();
	}

	template<class Alloc>
	using _Alloc_ptr_t = typename allocator_traits<Alloc>::pointer;

	template<class Alloc>
	using _Alloc_size_t = typename allocator_traits<Alloc>::size_type;


	template<class FwdIter>
	inline void _Destroy_range(FwdIter first, const FwdIter last) noexcept {
		if constexpr (!is_trivially_destructible<_Iter_value_t<FwdIter>>::value) {
			for (; first != last; ++first) {
				_Destroy_in_place(*first);
			}
		}
	}

	template<class Alloc>
	inline void _Destroy_range(_Alloc_ptr_t<Alloc> first, const _Alloc_ptr_t<Alloc> last, Alloc& alloc) noexcept {
		using _Tp = typename Alloc::value_type;
		if constexpr (!conjunction_v<is_trivially_destructible<_Tp>, _Uses_default_destroy<Alloc, _Tp*>>) {
			for (; first != last; ++first) {
				allocator_traits<Alloc>::destroy(alloc, _Unfancy(first));
			}
		}
	}

	template<class Alloc>
	inline void _Deallocate_plain(Alloc& alloc, typename Alloc::value_type* const ptr) noexcept {
		using _Alloc_traits = allocator_traits<Alloc>;
		if constexpr (is_same_v<_Alloc_ptr_t<Alloc>, typename Alloc::value_type*>) {
			_Alloc_traits::deallocate(alloc, ptr, 1);
		}
		else {
			using _Ptr_traits = pointer_traits<_Alloc_ptr_t<Alloc>>;
			_Alloc_traits::deallocate(alloc, _Ptr_traits::pointer_to(*ptr), 1);
		}
	}

	template<class Alloc>
	inline void _Delete_plain_internal(Alloc& alloc, typename Alloc::value_type* const ptr) noexcept {
		using Tp = typename Alloc::value_type;
		ptr->~Tp();
		_Deallocate_plain(alloc, ptr);
	}

	template<class Container>
	struct _Tidy_guard {
		Container* cont_;

		void _Release() const noexcept {
			cont_ = nullptr;
		}
		~_Tidy_guard() {
			if (cont_) cont_->_Tidy();  // Container需要定义_Tidy()函数
		}
	};

	template<class Container>
	struct _Tidy_deallocate_guard {
		Container* cont_;

		void _Release() const noexcept {
			cont_ = nullptr;
		}
		// Container需要定义_Tidy_deallocate()函数
		~_Tidy_deallocate_guard() {
			if (cont_) cont_->_Tidy_deallocate();
		}
	};

	template<class Iter>
	struct _Uninitialized_backout {
		Iter first_;
		Iter last_;

		explicit _Uninitialized_backout(Iter iter) : first_(iter), last_(iter) {}
		_Uninitialized_backout(Iter first, Iter last) : first_(first), last_(last) {}

		_Uninitialized_backout(const _Uninitialized_backout&) = delete;
		_Uninitialized_backout& operator=(const _Uninitialized_backout&) = delete;

		~_Uninitialized_backout() {
			_Destroy_range(first_, last_);
		}

		template<class... Args>
		void _Emplace_back(Args&&... args) {
			_Construct_in_place(*last_, mstd::forward<Args>(args)...);
			++last_;
		}

		Iter _Release() {
			first_ = last_;
			return last_;
		}
	};

	template<class Alloc>
	struct _Uninitialized_backout_alloc {
	private:
		using pointer = _Alloc_ptr_t<Alloc>;

		pointer first_;
		pointer last_;
		Alloc& alloc_;

	public:

		explicit _Uninitialized_backout_alloc(pointer iter, Alloc& alloc)
			: first_(iter), last_(iter), alloc_(alloc) {}

		_Uninitialized_backout_alloc(const _Uninitialized_backout_alloc&) = delete;
		_Uninitialized_backout_alloc& operator=(const _Uninitialized_backout_alloc&) = delete;

		~_Uninitialized_backout_alloc() {
			_Destroy_range(first_, last_, alloc_);
		}

		template<class... Args>
		void _Emplace_back(Args&&... args) {
			allocator_traits<Alloc>::construct(alloc_, _Unfancy(last_), mstd::forward<Args>(args)...);
			++last_;
		}

		constexpr pointer _Release() {
			first_ = last_;
			return last_;
		}
	};

	template<class InIter, class OutIter>
	inline OutIter _Uninitialized_copy_unchecked(InIter first, InIter last, OutIter dest) {
		if constexpr (_Iter_copy_cat<InIter, OutIter>::_Bitcopy_constructible) {
			return _Copy_with_memmove(first, last, dest);
		}
		_Uninitialized_backout<OutIter> backout{ dest };
		for (; first != last; ++first) {
			backout._Emplace_back(*first);
		}
		return backout._Release();
	}

	template<class InIter, class OutIter>
	inline OutIter uninitialized_copy(InIter first, InIter last, OutIter dest) {

		_Mstd_adl_verify_range(first, last);
		auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);
		auto udest = _Get_unwrapped_iter_n(dest, _Idl_distance(ufirst, ulast));

		_Seek_wrapped_iter(dest, _Uninitialized_copy_unchecked(ufirst, ulast, udest));
		return dest;
	}

	template<class Iter, class Tp>
	void uninitialized_fill(Iter first, Iter last, const Tp& val) {

		_Mstd_adl_verify_range(first, last);
		auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);

		if constexpr (_Fill_memset_is_safe<_Unwrapped_t<const Iter&>, Tp>) {
			_Fill_memset(ufirst, val, static_cast<size_t>(ulast - ufirst));
		}
		else {
			if constexpr (_Fill_zero_memset_is_safe<_Unwrapped_t<const Iter&>, Tp>) {
				if (_Is_all_bits_zero(val)) {
					_Fill_zero_memset(ufirst, static_cast<size_t>(ulast - ufirst));
					return;
				}
			}
			_Uninitialized_backout<_Unwrapped_t<const Iter&>> backout{ ufirst };
			for (; ufirst != ulast; ++ufirst) {
				backout._Emplace_back(val);
			}
			backout._Release();
		}
	}

	template <class Iter, class Sentinel, class Alloc>
	inline _Alloc_ptr_t<Alloc> _Uninitialized_copy(Iter first, Sentinel last, _Alloc_ptr_t<Alloc> dest, Alloc& alloc) {
		using Ptr = typename Alloc::value_type*;
		auto ufirst = _Get_unwrapped_iter(mstd::move(first));
		auto ulast = _Get_unwrapped_iter(mstd::move(last));

		constexpr bool can_memmove = _Sentinel_copy_cat<decltype(ufirst), decltype(ulast), Ptr>::_Bitcopy_constructible && _Uses_default_construct<Alloc, Ptr, decltype(*ufirst)>::value;
		if constexpr (can_memmove) {
			if constexpr (is_same_v<decltype(ufirst), decltype(ulast)>) {
				_Copy_with_memmove(_To_address(ufirst), _To_address(ulast), _Unfancy(dest));
				dest += ulast - ufirst;
			}
			else {
				const auto count = static_cast<size_t>(ulast - ufirst);
				_Copy_n_with_memmove(_To_address(ufirst), count, _Unfancy(dest));
				dest += count;
			}
			return dest;
		}
		_Uninitialized_backout_alloc<Alloc> backout{ dest, alloc };
		for (; ufirst != ulast; ++ufirst) {
			backout._Emplace_back(*ufirst);
		}
		return backout._Release();
	}

	template<class Iter, class Alloc>
	inline _Alloc_ptr_t<Alloc> _Uninitialized_copy_n(Iter first, size_t count, _Alloc_ptr_t<Alloc> dest, Alloc& alloc) {
		using Ptr = typename Alloc::value_type*;
		auto ufirst = _Get_unwrapped_iter(mstd::move(first));
		constexpr bool can_memmove = conjunction_v<bool_constant<_Iter_copy_cat<decltype(ufirst), Ptr>::_Bitcopy_constructible>, _Uses_default_construct<Alloc, Ptr, decltype(*ufirst)>>;

		if constexpr (can_memmove) {
			_Copy_n_with_memmove(ufirst, count, _Unfancy(dest));
			dest += count;
			return dest;
		}

		_Uninitialized_backout_alloc<Alloc> backout{ dest,alloc };
		for (; count != 0; --count, ++ufirst) {
			backout._Emplace_back(*ufirst);
		}
		return backout._Release();
	}

	template<class Iter, class Alloc>
	inline _Alloc_ptr_t<Alloc> _Uninitialized_move(Iter first, Iter last, _Alloc_ptr_t<Alloc> dest, Alloc& alloc) {
		using Ptr = typename Alloc::value_type*;
		auto ufirst = _Get_unwrapped_iter(first);
		const auto ulast = _Get_unwrapped_iter(last);
		constexpr bool can_memmove = conjunction_v<bool_constant<_Iter_move_cat<decltype(ufirst), Ptr >::_Bitcopy_constructible>, _Uses_default_construct<Alloc, Ptr, decltype(mstd::move(*ufirst))>>;
		if constexpr (can_memmove) {
			_Copy_with_memmove(ufirst, ulast, _Unfancy(dest));
			return dest + (ulast - ufirst);
		}
		_Uninitialized_backout_alloc<Alloc> backout{ dest,alloc };
		for (; ufirst != ulast; ++ufirst) {
			backout._Emplace_back(mstd::move(*ufirst));
		}
		return backout._Release();
	}

	template<class Alloc>
	inline _Alloc_ptr_t<Alloc> _Uninitialized_fill_n(_Alloc_ptr_t<Alloc> first, _Alloc_size_t<Alloc> count, const typename Alloc::value_type& val, Alloc& alloc) {
		using Tp = typename Alloc::value_type;
		if constexpr (_Fill_memset_is_safe<Tp*, Tp> && _Uses_default_construct<Alloc, Tp*, Tp>::value) {
			_Fill_memset(_Unfancy(first), val, static_cast<size_t>(count));
			return first + count;
		}
		else if constexpr (_Fill_zero_memset_is_safe<Tp*, Tp> && _Uses_default_construct<Alloc, Tp*, Tp>::value) {
			if (_Is_all_bits_zero(val)) {
				_Fill_zero_memset(_Unfancy(first), static_cast<size_t>(count));
				return first + count;
			}
		}
		_Uninitialized_backout_alloc<Alloc> backout{ first,alloc };
		for (; count > 0; --count) {
			backout._Emplace_back(val);
		}
		return backout._Release();
	}

	// 实现 ITERATOR_DEBUG 下迭代器核查器
	struct _Fake_allocator {};

	struct _Container_base_fake {
		void _Orphan_all() noexcept {}
		void _Swap_proxy_and_iterators(_Container_base_fake&) noexcept {}
		void _Alloc_proxy(const _Fake_allocator&) noexcept {}
		void _Reload_proxy(const _Fake_allocator&, const _Fake_allocator&) noexcept {}
	};

	struct _Iterator_base_fake {
		void _Adopt(const void*) noexcept {}
		const _Iterator_base_fake* _Getcont() const noexcept {
			return nullptr;
		}

		static constexpr bool _Unwrap_when_unverified = true;
	};

	struct _Container_base_real;

	struct _Container_proxy {
		_Container_proxy() noexcept = default;
		_Container_proxy(_Container_base_real* cont) noexcept : cont_(cont) {}

		const _Container_base_real* cont_ = nullptr;
		mutable _Iterator_base_real* firstiter_ = nullptr;
	};

	struct _Container_base_real {
	public:
		_Container_proxy* proxy_ = nullptr;

		_Container_base_real() noexcept = default;
		_Container_base_real(const _Container_base_real&) = delete;
		_Container_base_real& operator=(const _Container_base_real&) = delete;

		// 重新分配内存时，迭代器会失效，将所有旧迭代器设置失效
		void _Orphan_all() noexcept;

		// 交换容器基类时，需要交换容器代理，以及容器代理中保存的容器指针
		void _Swap_proxy_and_iterators(_Container_base_real& right) noexcept;

		template <class Alloc>
		void _Alloc_proxy(Alloc&& alloc) {
			_Container_proxy* const new_proxy = _Unfancy(alloc.allocate(1));
			_Construct_in_place(*new_proxy, this);
			proxy_ = new_proxy;
			new_proxy->proxy_ = this;
		}

		template <class Alloc>
		void _Reload_proxy(Alloc&& old_alloc, Alloc&& new_alloc) {
			_Container_proxy* const new_proxy = _Unfancy(new_alloc.allocate(1));
			_Construct_in_place(*new_proxy, this);
			new_proxy->cont_ = this;
			_Delete_plain_internal(old_alloc, mstd::exchange(proxy_, new_proxy));
		}

	};

	struct _Iterator_base_real {
	public:
		mutable _Container_proxy* proxy_ = nullptr;
		mutable _Iterator_base_real* nextiter_ = nullptr;

		_Iterator_base_real() noexcept = default;

		_Iterator_base_real(const _Iterator_base_real& right) noexcept {
			*this = right;
		}

		_Iterator_base_real& operator=(const _Iterator_base_real& right) noexcept {
			_Assign(right);
			return *this;
		}

		~_Iterator_base_real() noexcept {
			_Orphan_me();
		}

		const _Container_base_real* _Getcont() const noexcept {
			return proxy_ ? proxy_->cont_ : nullptr;
		}

		// 迭代器基类初始化调用
		void _Adopt(const _Container_base_real* parent) noexcept {
			if (!parent) { _Orphan_me(); return; }
			_Container_proxy* parent_proxy = parent->proxy_;
			if (proxy_ != parent_proxy) {
				if (!proxy_) _Orphan_me();
				nextiter_ = parent_proxy->firstiter_;
				parent_proxy->firstiter_ = this;
				proxy_ = parent_proxy;
			}
		}

	private:

		void _Assign(const _Iterator_base_real& right) {
			if (proxy_ == right.proxy_) return;
			if (right.proxy_) {
				_Adopt(right.proxy_->cont_);
			}
			else {
				_Orphan_me();
			}
		}

		// 将自身从迭代器链表中移除，将proxy_置为空
		void _Orphan_me() {
			if (!proxy_) return;
			_Iterator_base_real** pnext = &proxy_->firstiter_;
			while (*pnext && *pnext != this) {
				pnext = &(*pnext)->nextiter_;
			}
			_MSTD_VERIFY(*pnext, "iterator list corrupted");
			*pnext = nextiter_;
			proxy_ = nullptr;
		}

	};

	void _Container_base_real::_Orphan_all() noexcept {
		if (!proxy_) { return; }
		auto pnext = mstd::exchange(proxy_->firstiter_, nullptr);
		for (; pnext; pnext = pnext->nextiter_) {
			pnext->proxy_ = nullptr;
		}
	}

	void _Container_base_real::_Swap_proxy_and_iterators(_Container_base_real& right) noexcept {
		_Container_proxy* temp = proxy_;
		proxy_ = right.proxy_;
		right.proxy_ = temp;

		if (proxy_) {
			proxy_->cont_ = this;
		}

		if (right.proxy_) {
			right.proxy_->cont_ = &right;
		}
	}

#if _ITERATOR_DEBUG != 0

	using _Container_base = _Container_base_real;
	using _Iterator_base = _Iterator_base_real;

#define _CHANGE_TO_PROXY_ALLOC(_Alloc_t, alloc) static_cast<_Rebind_alloc_t<_Alloc_t, _Container_proxy>>(alloc)

#else
	using _Container_base = _Container_base_fake;
	using _Iterator_base = _Iterator_base_fake;

	constexpr _Fake_allocator _fake_alloc{};
#define _CHANGE_TO_PROXY_ALLOC(_Alloc_t, _fake_alloc) _fake_alloc

#endif


	// 容器基类代理器生成器，防止分配内存失败出现内存泄漏
	struct _Basic_container_proxy_ptr {
		_Container_proxy* ptr_{};
		constexpr void _Release() noexcept { ptr_ = nullptr; }
	protected:
		_Basic_container_proxy_ptr() = default;
		_Basic_container_proxy_ptr(const _Basic_container_proxy_ptr&) = delete;
		_Basic_container_proxy_ptr(_Basic_container_proxy_ptr&&) = delete;
	};

	// 标签类，指示容器基类代理器按默认方式构造
	struct _Leave_proxy_unbound {
		explicit _Leave_proxy_unbound() = default;
	};

	template<class Alloc>
	struct _Container_proxy_ptr : _Basic_container_proxy_ptr {
		Alloc& alloc_;
		_Container_proxy_ptr(Alloc& alloc, _Leave_proxy_unbound) : alloc_(alloc) {
			ptr_ = _Unfancy(alloc.allocate(1));
			_Construct_in_place(*ptr_);
		}

		_Container_proxy_ptr(Alloc& alloc, _Container_base_real& cont) : alloc_(alloc) {
			ptr_ = _Unfancy(alloc.allocate(1));
			_Construct_in_place(*ptr_, std::addressof(cont));
			cont->proxy_ = ptr_;
		}

		void _Bind(Alloc& old_alloc, _Container_base_real* cont) noexcept {
			ptr_->cont_ = cont;
			_Delete_plain_internal(old_alloc, mstd::exchange(cont->proxy_, mstd::exchange(ptr_, nullptr)));
		}

		~_Container_proxy_ptr() {
			if (ptr_) _Delete_plain_internal(alloc_, ptr_);
		}

	};


	// 标签类，用于指示重载函数调用决议
	struct _Zero_then_variadic_args_t {
		explicit _Zero_then_variadic_args_t() = default;
	};

	struct _One_then_variadic_args_t {
		explicit _One_then_variadic_args_t() = default;
	};

	// 聚合类，用于节省存放对象内存空间（节省为了区分空类对象而设定的内存空间）
	template<class Tp1, class Tp2, bool = is_empty<Tp1>::value && !is_final<Tp1>::value>
	struct _Compressed_pair final : private Tp1 {
	public:
		Tp2 second_;
		using base = Tp1;

		template<class... Other2>
		constexpr explicit _Compressed_pair(_Zero_then_variadic_args_t, Other2&&...vals2)
			noexcept(mstd::conjunction_v<is_nothrow_default_constructible<Tp1>, is_nothrow_constructible<Tp2, Other2...>>)
			: Tp1(), second_(mstd::forward<Other2>(vals2)...) {}

		template<class Other1, class... Other2>
		constexpr explicit _Compressed_pair(_One_then_variadic_args_t, Other1&& val1, Other2&&... vals2)
			noexcept(mstd::conjunction_v<is_nothrow_constructible<Tp1, Other1>, is_nothrow_constructible<Tp2, Other2...>>)
			: Tp1(mstd::forward<Other1>(val1)), second_(mstd::forward<Other2>(vals2)...) {}

		constexpr Tp1& _Get_first() noexcept {
			return *this;
		}

		constexpr const Tp1& _Get_first() const noexcept {
			return *this;
		}
	};

	template<class Tp1, class Tp2>
	struct _Compressed_pair<Tp1, Tp2, false> final {
	public:
		Tp1 first_;
		Tp2 second_;

		template<class... Other2>
		constexpr explicit _Compressed_pair(_Zero_then_variadic_args_t, Other2&&...vals2)
			noexcept(mstd::conjunction_v<is_nothrow_default_constructible<Tp1>, is_nothrow_constructible<Tp2, Other2...>>)
			: first_(), second_(mstd::forward<Other2>(vals2)...) {}

		template<class Other1, class... Other2>
		constexpr explicit _Compressed_pair(_One_then_variadic_args_t, Other1&& val1, Other2&&... vals2)
			noexcept(mstd::conjunction_v<is_nothrow_constructible<Tp1, Other1>, is_nothrow_constructible<Tp2, Other2...>>)
			: first_(mstd::forward<Other1>(val1)), second_(mstd::forward<Other2>(vals2)...) {}

		constexpr Tp1& _Get_first() noexcept {
			return first_;
		}

		constexpr const Tp1& _Get_first() const noexcept {
			return first_;
		}
	};















}

