#pragma once

#include <exception>		// std::bad_alloc;

#include "m_type_traits.h"	// is_trivially_destructible<>;
#include "m_utility.h"		// pointer_traits;
#include "m_iterator.h"		// _Iter_value_t<>;


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
		Container* _Target;

		void _Release() const noexcept {
			_Target = nullptr;
		}
		~_Tidy_guard() {
			if (_Target) _Target->_Tidy();  // Container需要定义_Tidy()函数
		}
	};

	template<class Container>
	struct _Tidy_deallocate_guard {
		Container* _Target;

		void _Release() const noexcept {
			_Target = nullptr;
		}
		// Container需要定义_Tidy_deallocate()函数
		~_Tidy_deallocate_guard() {
			if (_Target) _Target->_Tidy_deallocate();
		}
	};

	template<class Iter>
	struct _Initialized_backout {
		Iter first_;
		Iter last_;

		explicit _Initialized_backout(Iter iter) : first_(iter), last_(iter) {}
		_Initialized_backout(Iter first, Iter last) : first_(first), last_(last) {}

		_Initialized_backout(const _Initialized_backout&) = delete;
		_Initialized_backout& operator=(const _Initialized_backout&) = delete;

		~_Initialized_backout() {
			_Destroy_range(first_, last_);
		}

		template<class... Args>
		void _Emplace_back(Args&&... args) {
			_Construct_in_place(*last_, mstd::forward<Args>(args)...);
			++last_;
		}

		Iter _Release() {
			first_ = last_;
			return last_
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
			allocator_traits<Alloc>::construct(alloc_, _Unfancy(last), mstd::forward<Args>(args)...);
			++last_;
		}

		constexpr pointer _Release() {
			first_ = last_;
			return last_
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
		using Ptr = typename _Alloc::value_type*;
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
				_Copy_n_with_memmove(_To_address(ufirst), count, _Unfancy(_Dest));
				dest += count;
			}
			return dest;
		}
		_Uninitialized_backout_alloc<_Alloc> backout{ dest, alloc };
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
















}

