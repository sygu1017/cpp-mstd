#pragma once  // 编译器根据文件属性判断两个文件是否相同
#ifndef _M_TYPE_TRAITS_H_
#define _M_TYPE_TRAITS_H_

#include <cstddef> 

/*
* 编译器内部的intrinsic，
* C++11中，需要编译器辅助实现判断：
* is_class;
* is_union;
* is_enum;
* is_trivial;
* is_standard_layout
* is_pod;
* is_literal_type
* is_empty;
* is_abstract;
* is_polymorphic;
* has_trivial_constructor;
* has_trivial_copy;
* has_trivial_assign;
* has_trivial_destructor;
* has_nothrow_constructor;
* has_nothrow_copy;
* has_nothrow_assign;
* is_final; // C++14
*
* underlying_type;
* alignment_of
*/

///  判断类型类别 type predicate    
//
// is_void : 是否是void类型；
// is_integral : 整数 bool/char/char16_t/char32_t/wchar_t/[unsigned] short/int/long/long long；
// is_floating_point : 浮点数（floating point）：float/double/long double；
// is_arithmetic : 算术类型（arithmetic）：整数/浮点数；
// is_signed/is_unsigned : 是否是有符号或无符号类型；
// is_const : 是否是 const；
// is_volatile : 是否是 volatile；
// is_array : 内置数组（非std::array）；
// is_enum : 枚举（enumeration）；
// is_union : 联合体（union）；
// is_class : class/struct 类型（非union）
// is_function : 是否是 函数类型；
//
// is_reference : 引用：左值或右值引用；
// is_lvalue_reference : 左值引用；
// is_rvalue_reference : 右值引用；
// is_pointer : pointer 类型（!is_member_pointer）；
// is_member_pointer : 指向 nostatic 成员的指针（is_member_function_pointer+is_member_object_pointer）；
// is_member_object_pointer : 指向 nostatic 数据成员的指针；
// is_member_function_pointer : 指向 nostatic 成员函数的指针(包括普通成员函数、调用类型转换和调用操作符成员函数)；
//
// is_fundamental : 基础类型：算术类型/void/nullptr_t；
// is_scalar : 标量：算术类型/枚举/pointer/member pointer/nullptr_t；
// is_object : 对象类型（不包括 void/function/reference）（!is_fundamental）；
// is_compound : 数组/enumeration/union/class/function/reference/pointer；
// is_trivial : scalar/trivial class/由 scalar 和 trivial class 构成的数组；
// is_trivially_copyable : scalar/trivial copyable class/由scalar或trivial copyable class构成的数组，safe memcpy()等；
// is_standard_layout : scalar/standard layout class/由 scalar 或 standard layout class 构成的数组；
// is_pod : plain old data type，可以使用 memcpy() memmove()；
// is_literal_type : scalar/reference/class/由 scalar、reference和class 构成的数组；


///  判断 class 类型实现细节
//
// is_empty : class 不带任何成员/virtual 成员函数/virtual base class；
// is_polymorphic : class 带有一个（derived）virtual 成员函数；
// is_abstract : abstract class（至少有一个 pure virtual 函数）；
// has_virtual_destructor : class 带有 虚析构函数；
//
// is_default_constructible : 是否能够完成 默认构造；
// is_copy_constructible : 是够能够完成 拷贝构造；
// is_move_constructible : 是否能够完成 移动构造；
// is_copy_assignable : 是否能够完成 拷贝赋值；
// is_move_assignable : 是否能够完成 移动赋值；
// is_destructiable : 是否能够完成 析构（析构函数不能是 deleted/protected/private）；
//
// is_trivially_default_constructible : 是否能够完成 trivial 默认构造；
// is_trivially_copy_constructible : 是够能够完成 trivial 拷贝构造；
// is_trivially_move_constructible : 是否能够完成 trivial 移动构造；
// is_trivially_copy_assignable : 是否能够完成 trivial 拷贝赋值；
// is_trivially_move_assignable : 是否能够完成 trivial 移动赋值；
// is_trivially_destructiable : 是否能够完成 trivial 析构;
//
// is_nothrow_default_constructible : 是否能够完成 nothrow 默认构造；
// is_nothrow_copy_constructible : 是够能够完成 nothrow 拷贝构造；
// is_nothrow_move_constructible : 是否能够完成 nothrow 移动构造；
// is_nothrow_copy_assignable : 是否能够完成 nothrow 拷贝赋值；
// is_nothrow_move_assignable : 是否能够完成 nothrow 移动赋值；
// is_nothrow_destructiable : 是否能够完成 nothrow 析构;


///  检验类型之间关系
//
// is_same <T1,T2> : T1、T2类型 是否相同（包括 const/volatile）；
// is_base_of <T1,T2> : T1 是否是 T2 的 base class；
// is_convertible <T1,T2> : T1 是否可转换成 T2；
//
// is_constructible <T,Args...> : T 是否可由 Args 初始化；
// is_trivially_constructible <T,Args...> : T 是否可由 Args trivial 初始化；
// is_nothrow_constructible <T,Args...> : T 是否可由 Args nothrow 初始化；
//
// is_assignale <T1,T2> : T2 是否可 赋值给 T1；
// is_trivially_assignable <T1,T2> : T2 是否可 trivial 赋值给 T1；
// is_nothrow_assignable <T1,T2> : T2 是否可 nothrow 赋值给 T1；
//
// uses_allocator <T,Alloc> : Alloc 是否可转换为 T::Allocator；


///  其他 Type Trait
//
// rank <T> : array 类型的维度（或是0）
// extent <T,I=0> : 维度I（或0）的尺度（extent）；
// remove_extent <T> : 获得 array 的元素类型（若T不是array则获得类型T）；
// remove_all_extents <T> : 获得多维 array 的元素类型（若T不是多维array则获得类型T）；
//
// underlying_type <T> : 枚举类型的低层类型；
// decay <T> : 将 T 转化为 “实值类型”（"by-value" type），去掉ref cv修饰符，或将函数或内置数组转换为指针类型；
// enable_if <B,T=void> : 只有当 bool B 为 true 时会产出 type T；
// conditional <B,T,F> : 当 bool B 为 true 会产出 type T，否则产出 type F；
// 
// common_type <T,...> : 所有被传入类型的共通类型（必须有共通类型）；
// result_of <F,Arg> : 将 Arg 作为实参调用 F 后，其所得结果的类型；
//
// aligned_of <T> : 等价于 alignof(T)；
// aligned_sotrage <Len> : Len byte 加上默认对齐规则所可容纳的类型；
// aligned_storage <Len,Align> : Len byte 且以 size_t Align 的约数进行对齐，所可容纳的类型；
// aligned_union <Len,Types...> : Len byte 且以 a union of Types... 进行对齐，所可容纳的类型；


///  进行类型修改
//
// remove_const : 不带 const 的类型；
// remove_volatile : 不带 volatile 的类型；
// remove_cv : 不带 const 和 volatile 的类型；
//
// add_const : 带 const 的类型；
// add_volatile : 带 volatile 的类型；
// add_cv : 带 const 和 volatile 的类型；
//
// make_signed : 对应 signed noreference 的类型；
// make_unsigned : 对应 unsigned noreference 的类型；
//
// remove_reference : 对应 noreference 类型；
// add_lvalue_reference : 对应 左值引用（右值变左值）；
// add_rvalue_reference : 对应 右值引用（左值不变）；
//
// remove_pointer : 得到pointer 所指向的类型（若是非pointer,则是原类型）；
// add_pointer : 得到一个 pointer 类型，指向对应 noreference 类型；


namespace mstd {

	template<class IntglConst, IntglConst Val>
	struct integral_constant {

		static constexpr IntglConst value = Val;

		using value_type = IntglConst;
		using type = integral_constant;

		constexpr operator value_type() const noexcept { return Val; }
		constexpr value_type operator()() const noexcept { return Val; }
	};

	template<bool Val>
	using bool_constant = integral_constant<bool, Val>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	template<bool Val, class = void>
	struct enable_if {};

	template<class Tp>
	struct enable_if<true, Tp> {
		using type = Tp;
	};

	template<bool Val, class Tp = void>
	using enable_if_t = typename enable_if<Val, Tp>::type;

	template<bool, class Tp1, class Tp2>
	struct conditional {
		using type = Tp1;
	};

	template<class Tp1, class Tp2>
	struct conditional<false, Tp1, Tp2> {
		using type = Tp2;
	};

	template<bool Val, class Tp1, class Tp2>
	using conditional_t = typename conditional<Val, Tp1, Tp2>::type;

	template<class Tp1, class Tp2>
	constexpr bool is_same_v = false;

	template<class Tp>
	constexpr bool is_same_v<Tp, Tp> = true;

	template<class Tp1, class Tp2>
	struct is_same : bool_constant<is_same_v<Tp1, Tp2>> {};

	template<class Tp>
	struct remove_const {
		using type = Tp;
	};

	template<class Tp>
	struct remove_const<const Tp> {
		using type = Tp;
	};

	template<class Tp>
	using remove_const_t = typename remove_const<Tp>::type;

	template<class Tp>
	struct remove_volatile {
		using type = Tp;
	};

	template<class Tp>
	struct remove_volatile<volatile Tp> {
		using type = Tp;
	};

	template<class Tp>
	using remove_volatile_t = typename remove_volatile<Tp>::type;

	template<class Tp>
	struct remove_cv {
		using type = Tp;

		template<template<class> class Fn>
		using _Apply = Fn<Tp>;
	};

	template<class Tp>
	struct remove_cv<const Tp> {
		using type = Tp;

		template<template<class> class Fn>
		using _Apply = const Fn<Tp>;
	};

	template<class Tp>
	struct remove_cv<volatile Tp> {
		using type = Tp;

		template<template<class> class Fn>
		using _Apply = volatile Fn<Tp>;
	};

	template<class Tp>
	struct remove_cv<const volatile Tp> {
		using type = Tp;

		template<template<class> class Fn>
		using _Apply = const volatile Fn<Tp>;
	};

	template<class Tp>
	using remove_cv_t = typename remove_cv<Tp>::type;

	template<bool First_Val, class First, class... Rest>
	struct _conjunction {
		using type = First;
	};

	template<class First, class Next, class... Rest>
	struct _conjunction<true, First, Next, Rest...> {
		using type = typename _conjunction<Next::value, Next, Rest...>::type;
	};

	template<class... Types>
	struct conjunction : true_type {};

	template<class First, class... Rest>
	struct conjunction<First, Rest...> : _conjunction<First::value, First, Rest...> {};

	template<class... Types>
	constexpr bool conjunction_v = conjunction<Types...>::value;

	template<bool First_Val, class First, class... Rest>
	struct _disjunction {
		using type = First;
	};

	template<class First, class Next, class... Rest>
	struct _disjunction<false, First, Next, Rest...> {
		using type = typename _disjunction<Next::value, Next, Rest...>::type;
	};

	template<class... Types>
	struct disjunction : false_type {};

	template<class First, class... Rest>
	struct disjunction<First, Rest...> : _disjunction<First::value, First, Rest...>::type {};

	template<class... Types>
	constexpr bool disjunction_v = disjunction<Types...>::value;

	template<class Tp, class... Types>
	constexpr bool _is_any_of_v = disjunction_v<is_same<Tp, Types>...>;

	template<class Tp>
	constexpr bool is_intagral_v = _is_any_of_v<remove_cv_t<Tp>, bool, char, signed char,
		unsigned char, wchar_t, char16_t, char32_t, short, unsigned short, int, unsigned int,
		long, unsigned long, long long, unsigned long long>;

	template<class Tp>
	struct is_intagral : bool_constant<is_intagral_v<Tp>> {};

	template<class Tp>
	constexpr bool is_floating_point_v = _is_any_of_v<remove_cv_t<Tp>, float, double, long double>;

	template<class Tp>
	struct is_floating_point : bool_constant<is_floating_point_v<Tp>> {};

	template<class Tp>
	constexpr bool is_arithmetic_v = is_intagral_v<Tp> || is_floating_point_v<Tp>;

	template<class Tp>
	struct is_arithmetic : bool_constant<is_arithmetic_v<Tp>> {};

	template<class Tp>
	struct remove_reference {
		using type = Tp;
	};

	template<class Tp>
	struct remove_reference<Tp&> {
		using type = Tp;
	};

	template<class Tp>
	struct remove_reference<Tp&&> {
		using type = Tp;
	};

	template<class Tp>
	using remove_reference_t = typename remove_reference<Tp>::type;

	template<class Tp>
	struct negation : bool_constant<!static_cast<bool>(Tp::value)> {};

	template<class Tp>
	constexpr bool is_void_v = is_same_v<remove_cv_t<Tp>, void>;

	template<class Tp>
	struct is_void : bool_constant<is_void_v<Tp>> {};

	template<class... Types>
	using void_t = void;

	template<class Tp>
	struct add_const {
		using type = const Tp;
	};

	template<class Tp>
	using add_const_t = typename add_const<Tp>::type;

	template<class Tp>
	struct add_volatile {
		using type = volatile Tp;
	};

	template<class Tp>
	using add_volatile_t = typename add_volatile<Tp>::type;

	template<class Tp>
	struct add_cv {
		using type = const volatile Tp;
	};

	template<class Tp>
	using add_cv_t = typename add_cv<Tp>::type;

	template<class Tp, class = void>
	struct _add_reference {
		using _Lref = Tp;
		using _Rref = Tp;
	};

	template<class Tp>
	struct _add_reference<Tp, void_t<Tp&>> {	// referenceable
		using _Lref = Tp&;
		using _Rref = Tp&&;
	};

	template<class Tp>
	struct add_lvalue_reference {
		using type = typename _add_reference<Tp>::_Lref;
	};

	template<class Tp>
	struct add_rvalue_reference {
		using type = typename _add_reference<Tp>::_Rref;
	};

	template<class Tp>
	using add_lvalue_reference_t = typename _add_reference<Tp>::_Lref;

	template<class Tp>
	using add_rvalue_reference_t = typename _add_reference<Tp>::_Rref;

	template<class Tp>
	add_rvalue_reference_t<Tp> declval() noexcept {}

	template<class Tp>
	struct remove_extent {
		using type = Tp;
	};

	template<class Tp, size_t Size>
	struct remove_extent<Tp[Size]> {
		using type = Tp;
	};

	template<class Tp>
	struct remove_extent<Tp[]> {
		using type = Tp;
	};

	template<class Tp>
	using remove_extent_t = typename remove_extent<Tp>::type;

	template<class Tp>
	struct remove_all_extents {
		using type = Tp;
	};

	template<class Tp>
	struct remove_all_extents<Tp[]> : remove_all_extents<Tp> {};

	template<class Tp, size_t Size>
	struct remove_all_extents<Tp[Size]> : remove_all_extents<Tp> {};

	template<class Tp>
	using remove_all_extents_t = typename remove_all_extents<Tp>::type;

	template<class Tp>
	struct remove_pointer {
		using type = Tp;
	};

	template<class Tp>
	struct remove_pointer<Tp*> {
		using type = Tp;
	};

	template<class Tp>
	struct remove_pointer<Tp* const> {
		using type = Tp;
	};

	template<class Tp>
	struct remove_pointer<Tp* volatile> {
		using type = Tp;
	};

	template<class Tp>
	struct remove_pointer<Tp* const volatile> {
		using type = Tp;
	};

	template<class Tp, class = void>
	struct _add_pointer {
		using type = Tp;
	};

	template<class Tp>
	struct _add_pointer<Tp, void_t<remove_reference_t<Tp>*>> {
		using type = remove_reference_t<Tp>*;
	};

	template<class Tp>
	struct add_pointer {
		using type = typename _add_pointer<Tp>::type;
	};

	template<class Tp>
	using add_pointer_t = typename add_pointer<Tp>::type;

	template<class Tp>
	constexpr bool is_array_v = false;

	template<class Tp>
	constexpr bool is_array_v<Tp[]> = true;

	template<class Tp, size_t Size>
	constexpr bool is_array_v<Tp[Size]> = true;

	template<class Tp>
	struct is_array : bool_constant<is_array_v<Tp>> {};

	template<class Tp>
	constexpr bool is_lvalue_reference_v = false;

	template<class Tp>
	constexpr bool is_lvalue_reference_v<Tp&> = true;

	template<class Tp>
	struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<Tp>> {};

	template<class Tp>
	constexpr bool is_rvalue_reference_v = false;

	template<class Tp>
	constexpr bool is_rvalue_reference_v<Tp&&> = true;

	template<class Tp>
	struct is_rvalue_reference : bool_constant<is_rvalue_reference_v<Tp>> {};

	template<class Tp>
	constexpr bool is_reference_v = is_lvalue_reference_v<Tp> || is_rvalue_reference_v<Tp>;

	template<class Tp>
	struct is_reference : bool_constant<is_reference_v<Tp>> {};

	template<class Tp>
	constexpr bool is_pointer_v = false;

	template<class Tp>
	constexpr bool is_pointer_v<Tp*> = true;

	template<class Tp>
	constexpr bool is_pointer_v<Tp* const> = true;

	template<class Tp>
	constexpr bool is_pointer_v<Tp* volatile> = true;

	template<class Tp>
	constexpr bool is_pointer_v<Tp* const volatile> = true;

	template<class Tp>
	struct is_pointer : bool_constant<is_pointer_v<Tp>> {};

	template<class Tp>
	constexpr bool is_null_pointer_v = is_same_v<remove_cv_t<Tp>, nullptr_t>;

	template<class Tp>
	struct is_null_pointer : bool_constant<is_null_pointer_v<Tp>> {};

	// 依赖编译器实现
	template<class Tp>
	using is_union = std::is_union<Tp>;		// using std::is_union<>;

	template<class Tp>
	using is_class = std::is_class<Tp>;

	template<class Tp>
	constexpr bool is_fundamental_v = is_arithmetic_v<Tp> || is_void_v<Tp> || is_null_pointer_v<Tp>;

	template<class Tp>
	struct is_fundamental : bool_constant<is_fundamental_v<Tp>> {};

	template<class From, class To>
	using is_convertible = std::is_convertible<From, To>;

	template<class Tp>
	using is_enum = std::is_enum<Tp>;

	template<class Tp>
	struct is_compound : bool_constant<!is_fundamental_v<Tp>> {};

	template<class Tp>
	constexpr bool is_compound_v = !is_fundamental_v<Tp>;

	template<class Tp>
	using is_member_function_pointer = std::is_member_function_pointer<Tp>;

	template<class Tp>
	constexpr bool is_const_v = false;

	template<class Tp>
	constexpr bool is_const_v<const Tp> = true;

	template<class Tp>
	struct is_const : bool_constant<is_const_v<Tp>> {};

	template<class Tp>
	constexpr bool is_volatile_v = false;

	template<class Tp>
	constexpr bool is_volatile_v<volatile Tp> = true;

	template<class Tp>
	struct is_volatile : bool_constant<is_volatile_v<Tp>> {};

	template<class Tp>
	constexpr bool is_function_v = !is_const_v<const Tp> && !is_reference_v<Tp>;

	template<class Tp>
	struct is_function : bool_constant<is_function_v<Tp>> {};

	template<class Tp>
	constexpr bool is_object_v = is_const_v<const Tp> && !is_void_v<Tp>;

	template<class Tp>
	struct is_object : bool_constant<is_object_v<Tp>> {};

	template<class Tp>
	struct _is_member_object_pointer {
		static constexpr bool value = false;
	};

	template<class Tp1, class Tp2>
	struct _is_member_object_pointer<Tp1 Tp2::*> {
		static constexpr bool value = !is_function_v<Tp1>;
	};

	template<class Tp>
	struct is_member_object_pointer : bool_constant<_is_member_object_pointer<Tp>::value> {};

	template<class Tp>
	constexpr bool is_member_object_pointer_v = _is_member_object_pointer<Tp>::value;

	template<class Tp>
	constexpr bool is_member_pointer_v = is_member_function_pointer_v<Tp> || is_member_object_pointer_v<Tp>;

	template<class Tp>
	struct is_member_pointer : bool_constant<is_member_pointer_v<Tp>> {};

	template<class Tp>
	constexpr bool is_scalar_v = is_arithmetic_v<Tp> || is_pointer_v<Tp> ||
		is_member_pointer_v<Tp> || is_enum_v<Tp> || is_null_pointer_v<Tp>;

	template<class Tp>
	struct is_scalar : bool_constant<is_scalar_v<Tp>> {};

	template<class Tp>
	using is_pod = std::is_pod<Tp>;

	template<class Tp>
	using is_empty = std::is_empty<Tp>;

	template<class Tp>
	using is_polymorphic = std::is_polymorphic<Tp>;

	template<class Tp>
	using is_abstract = std::is_abstract<Tp>;

	template<class Tp>
	using is_final = std::is_final<Tp>;

	template<class Tp>
	using is_standard_layout = std::is_standard_layout<Tp>;

	//template<class Tp>
	//using is_literal_type = std::is_literal_type<Tp>;

	template<class Tp>
	using is_trivial = std::is_trivial<Tp>;

	template<class Tp>
	using is_trivially_copyable = std::is_trivially_copyable<Tp>;

	template<class Tp>
	using has_virtual_destructor = std::has_virtual_destructor<Tp>;

	template<class Tp, class... Args>
	using is_constructible = std::is_constructible<Tp, Args...>;

	template<class Tp>
	using is_default_constructible = is_constructible<Tp>;

	template<class Tp>
	using is_copy_constructible = is_constructible<Tp, add_lvalue_reference_t<const Tp>>;

	template<class Tp>
	using is_move_constructible = is_constructible<Tp, Tp>;

	template<class To, class From>
	using is_assignable = std::is_assignable<To, From>;

	template<class Tp>
	using is_copy_assignable = is_assignable<add_lvalue_reference_t<Tp>, add_lvalue_reference_t<const Tp>>;

	template<class Tp>
	using is_move_assignable = is_assignable<add_lvalue_reference_t<Tp>, Tp>;

	template<class Tp>
	using is_destructible = std::is_destructible<Tp>;

	template<class Tp, class... Args>
	using is_trivially_constructible = std::is_trivially_constructible<Tp, Args...>;

	template<class Tp>
	using is_trivially_copy_constructible = is_trivially_constructible<Tp,
		add_lvalue_reference_t<const Tp>>;

	template<class Tp>
	using is_trivially_default_constructible = is_trivially_constructible<Tp>;

	template<class Tp>
	using is_trivially_move_constructible = is_trivially_constructible<Tp, Tp>;

	template<class To, class From>
	using is_trivially_assignable = std::is_trivially_assignable<To, From>;

	template<class Tp>
	using is_trivially_copy_assignable = is_trivially_assignable<add_lvalue_reference_t<Tp>, add_lvalue_reference_t<const Tp>>;

	template<class Tp>
	using is_trivially_move_assignable = is_trivially_assignable<add_lvalue_reference_t<Tp>, Tp>;

	template<class Tp>
	using is_trivially_destructible = std::is_trivially_destructible<Tp>;

	template<class Tp, class... Args>
	using is_nothrow_constructible = std::is_nothrow_constructible<Tp, Args...>;

	template<class Tp>
	using is_nothrow_default_constructible = is_nothrow_constructible<Tp>;

	template<class Tp>
	using is_nothrow_copy_constructible = is_nothrow_constructible<Tp, add_lvalue_reference_t<const Tp>>;

	template<class Tp>
	using is_nothrow_move_constructible = is_nothrow_constructible<Tp, Tp>;

	template<class To, class From>
	using is_nothrow_assignable = std::is_nothrow_assignable<To, From>;

	template<class Tp>
	using is_nothrow_copy_assignable = is_nothrow_assignable<add_lvalue_reference_t<Tp>, add_lvalue_reference_t<const Tp>>;

	template<class Tp>
	using is_nothrow_move_assignable = is_nothrow_assignable<add_lvalue_reference_t<Tp>, Tp>;

	template<class Tp>
	using is_nothrow_destructible = std::is_nothrow_destructible<Tp>;

	template<class Tp, bool = is_integral_v<Tp>>
	struct _is_signed {
		using _Rp = remove_cv_t<Tp>;
		static constexpr bool _signed = static_cast<_Rp>(-1) < static_cast<_Rp>(0);
		static constexpr bool _unsigned = !_signed;
	};

	template<class Tp>
	struct _is_signed<Tp, false> {
		static constexpr bool _signed = is_floating_point_v<Tp>;
		static constexpr bool _unsigned = false;
	};

	template<class Tp>
	struct is_signed : bool_constant<_is_signed<Tp>::_signed> {};

	template<class Tp>
	struct is_unsigned : bool_constant<_is_signed<Tp>::_unsigned> {};

	template<class Tp>
	constexpr bool is_signed_v = _is_signed<Tp>::_signed;

	template<class Tp>
	constexpr bool is_unsigned_v = _is_signed<Tp>::_unsigned;

	template<bool>
	struct _Select {
		template<class Tp1, class Tp2>
		using _Apply = Tp1;
	};

	template<>
	struct _Select<false> {
		template<class Tp1, class Tp2>
		using _Apply = Tp2;
	};

	template<size_t>
	struct _Make_signed2;

	template<>
	struct _Make_signed2<1> {
		template<class>
		using _Apply = signed char;
	};

	template<>
	struct _Make_signed2<2> {
		template<class>
		using _Apply = short;
	};

	template<>
	struct _Make_signed2<4> {
		template<class Tp>
		using _Apply = typename _Select<is_same_v<Tp, long> || is_same_v<Tp, unsigned long>>::template _Apply<long, int>;
	};

	template<>
	struct _Make_signed2<8> {
		template<class>
		using _Apply = long long;
	};

	template<class Tp>
	using _Make_signed1 = typename _Make_signed2<sizeof(Tp)>::template _Apply<Tp>;

	template<class Tp>
	struct make_signed {
		using type = typename remove_cv_t<Tp>::template _Apply<_Make_signed1<Tp>>;
	};

	template<class Tp>
	using make_signed_t = typename make_signed<Tp>::type;

	template<size_t>
	struct _Make_unsigned2;

	template<>
	struct _Make_unsigned2<1> {
		template<class>
		using _Apply = unsigned char;
	};

	template<>
	struct _Make_unsigned2<2> {
		template<class>
		using _Apply = unsigned short;
	};

	template<>
	struct _Make_unsigned2<4> {
		template<class Tp>
		using _Apply = typename _Select<is_same_v<Tp, long> || is_same_v<Tp, unsigned long>>::template _Apply<unsigned long, unsigned int>;
	};

	template<>
	struct _Make_unsigned2<8> {
		template<class>
		using _Apply = unsigned long long;
	};

	template<class Tp>
	using _Make_unsigned1 = typename _Make_unsigned2<sizeof(Tp)>::template _Apply<Tp>;

	template<class Tp>
	struct make_unsigned {
		using type = typename remove_cv_t<Tp>::template _Apply<_Make_unsigned1<Tp>>;
	};

	template<class Tp>
	using make_unsigned_t = typename make_unsigned<Tp>::type;

	template<class Tp>
	struct alignment_of : integral_constant<size_t, alignof(Tp)> {};

	template<class Tp>
	constexpr size_t alignment_of_v = alignof(Tp);

	template<class Tp, size_t len>
	union _Align_type {
		Tp val;
		char pad[len];
	};

	template<size_t len, size_t align, class Tp, bool>
	struct _Aligned;

	template<size_t len, size_t align, class Tp>
	struct _Aligned<len, align, Tp, true> {
		using type = _Align_type<Tp, len>;
	};

	template<size_t len, size_t align>
	struct _Aligned<len, align, double, false> {
		using type = _Align_type<max_align_t, len>;
	};

	template<size_t len, size_t align>
	struct _Aligned<len, align, int, false> {
		using Next = double;
		static constexpr bool fit = align <= alignof(Next);
		using type = typename _Aligned<len, align, Next, fit>::type;
	};

	template<size_t len, size_t align>
	struct _Aligned<len, align, short, false> {
		using Next = int;
		static constexpr bool fit = align <= alignof(Next);
		using type = typename _Aligned<len, align, Next, fit>::type;
	};

	template<size_t len, size_t align>
	struct _Aligned<len, align, char, false> {
		using Next = short;
		static constexpr bool fit = align <= alignof(Next);
		using type = typename _Aligned<len, align, Next, fit>::type;
	};

	template<size_t len, size_t align>
	struct _Aligned_storage {
		using Next = char;
		static constexpr bool fit = align <= alignof(Next);
		using type = typename _Aligned<len, align, Next, fit>::type;
	};

	template<size_t len, size_t align = alignof(max_align_t)>
	struct aligned_storage {
		using type = typename _Aligned_storage<len, align>::type;
	};

	template<size_t len, size_t align = alignof(max_align_t)>
	using aligned_storage_t = typename _Aligned_storage<len, align>::type;

	template<size_t...>
	struct _Max_val;

	template<>
	struct _Max_val<> : integral_constant<size_t, 0> {};

	template<size_t val>
	struct _Max_val<val> : integral_constant<size_t, val> {};

	template<size_t first, size_t second, size_t... rest>
	struct _Max_val<first, second, rest...> : _Max_val<(first < second) ? second : first, rest...>::type{};

	template<size_t len, class... Types>
	struct aligned_union {
		static constexpr size_t _max_len = _Max_val<len, sizeof(Types)...>::value;
		static constexpr size_t _max_align = _Max_val<alignof(Types)...>::value;
		using type = typename _Aligned_storage<_max_len, _max_align>::type;
	};

	template<size_t len, class... Types>
	using aligned_union_t = typename aligned_union<len, Types...>::type;

	template<class Tp>
	using underlying_type = std::underlying_type<Tp>;

	template<class Tp>
	constexpr size_t rank_v = 0;

	template<class Tp>
	constexpr size_t rank_v<Tp[]> = rank_v<Tp> +1;

	template<class Tp, size_t Size>
	constexpr size_t rank_v<Tp[Size]> = rank_v<Tp> +1;

	template<class Tp>
	struct rank : integral_constant<size_t, rank_v<Tp>> {};

	template<class Tp, unsigned int Index = 0>
	constexpr size_t extent_v = 0;

	template<class Tp, size_t Size>
	constexpr size_t extent_v<Tp[Size], 0> = Size;

	template<class Tp, unsigned int Index, size_t Size>
	constexpr size_t extent_v<Tp[Size], Index> = extent_v<Tp, Index - 1>;

	template<class Tp, unsigned int Index>
	constexpr size_t extent_v<Tp[], Index> = extent_v<Tp, Index - 1>;

	template<class Base, class Derived>
	using is_base_of = std::is_base_of<Base, Derived>;

	template<class Tp>
	struct decay {
		using _Tp1 = remove_reference_t<Tp>;
		using _Tp2 = typename _Select<is_function_v<_Tp1>>::template _Apply<add_pointer_t<_Tp1>, remove_cv_t<_Tp1>>;
		using type = typename _Select<is_array_v<_Tp1>>::template _Apply<add_pointer_t<remove_extent_t<_Tp1>>, _Tp2>;
	};

	template<class Tp>
	using decay_t = typename decay<Tp>::type;

	template<class Tp1, class Tp2>
	using _Conditional_type = decltype(false ? declval<Tp1>() : declval<Tp2>());

	template<class Tp1, class Tp2, class = void>
	struct _Common_type_impl {};

	template<class Tp1, class Tp2>
	struct _Common_type_impl<Tp1, Tp2, void_t<_Conditional_type<Tp1, Tp2>>> {
		using type = decay_t<_Conditional_type<Tp1, Tp2>>;
	};

	template<class... Tp>
	struct common_type;

	template<class... Tp>
	using common_type_t = typename common_type<Tp...>::type;

	template<>
	struct common_type<> {};

	template<class Tp>
	struct common_type<Tp> : common_type<Tp, Tp> {};

	template<class Tp1, class Tp2, class Decayed1 = decay_t<Tp1>, class Decayed2 = decay_t<Tp2>>
	struct _Common_type1 : common_type<Decayed1, Decayed2> {};

	template<class Tp1, class Tp2>
	struct _Common_type1<Tp1, Tp2, Tp1, Tp2> : _Common_type_impl<Tp1, Tp2> {};

	template<class Tp1, class Tp2>
	struct common_type<Tp1, Tp2> : _Common_type1<Tp1, Tp2> {};

	template<class Void, class Tp1, class Tp2, class... Rest>
	struct _Common_type2 {};

	template<class Tp1, class Tp2, class... Rest>
	struct _Common_type2<void_t<common_type_t<Tp1, Tp2>>, Tp1, Tp2, Rest...> :
		common_type<common_type_t<Tp1, Tp2>, Rest...> {};

	template<class Tp1, class Tp2, class... Rest>
	struct common_type<Tp1, Tp2, Rest...> : _Common_type2<void, Tp1, Tp2, Rest...> {};

	// 实现 reference_wrapper<>;
	template<class Tp>
	class reference_wrapper;

	enum class _Invoker_strategy {
		_Functor,		// 有参仿函数或有参函数指针的调用；
		_Pmf_object,	// 对象实例调用成员函数指针；
		_Pmf_refwrap,	// 引用包装器调用成员函数指针；
		_Pmf_pointer,	// 对象指针调用成员函数指针；

		_Pmd_object,	// 对象实例调用成员数据指针；
		_Pmd_refwrap,	// 引用包装器调用成员数据指针；
		_Pmd_pointer	// 对象指针调用成员数据指针；
	};

	struct _Invoker_Functor {
		static constexpr _Invoker_strategy strategy = _Invoker_strategy::_Functor;
		template<class Callable, class... Types>
		static constexpr auto Call(Callable&& obj, Types&&... args)
			noexcept(noexcept(static_cast<Callable&&>(obj)(static_cast<Types>(args)...)))
			-> decltype(static_cast<Callable&&>(obj)(static_cast<Types>(args)...)) {
			return static_cast<Callable&&>(obj)(static_cast<Types>(args)...);
		}
	};

	struct _Invoker_Pmf_object {
		static constexpr _Invoker_strategy strategy = _Invoker_strategy::_Pmf_object;
		template<class Decayed, class Tp, class... Types>
		static constexpr auto Call(Decayed&& pmf, Tp&& obj, Types&&... args)
			noexcept(noexcept((static_cast<Tp&&>(obj).*pmf)(static_cast<Types>(args)...)))
			-> decltype((static_cast<Tp&&>(obj).*pmf)(static_cast<Types>(args)...)) {
			return (static_cast<Tp&&>(obj).*pmf)(static_cast<Types>(args)...);
		}
	};

	struct _Invoker_Pmf_refwrap {
		static constexpr _Invoker_strategy strategy = _Invoker_strategy::_Pmf_refwrap;
		template<class Decayed, class Refwrap, class... Types>
		static constexpr auto Call(Decayed&& pmf, Refwrap&& rw, Types&&... args)
			noexcept(noexcept((rw.get().*pmf)(static_cast<Types>(args)...)))
			-> decltype((rw.get().*pmf)(static_cast<Types>(args)...)) {
			return (rw.get().*pmf)(static_cast<Types>(args)...);
		}
	};

	struct _Invoker_Pmf_pointer {
		static constexpr _Invoker_strategy strategy = _Invoker_strategy::_Pmf_pointer;
		template<class Decayed, class Ptr, class... Types>
		static constexpr auto Call(Decayed&& pmf, Ptr&& ptr, Types&&... args)
			noexcept(noexcept(((*static_cast<Ptr&&>(ptr)).*pmf)(static_cast<Types&&>(args)...)))
			->decltype(((*static_cast<Ptr&&>(ptr)).*pmf)(static_cast<Types&&>(args)...)) {
			return ((*static_cast<Ptr&&>(ptr)).*pmf)(static_cast<Types&&>(args)...);
		}
	};

	struct _Invoker_Pmd_object {
		static constexpr _Invoker_strategy strategy = _Invoker_strategy::_Pmd_object;
		template<class Decayed, class Tp>
		static constexpr auto Call(Decayed&& pmd, Tp&& obj) noexcept
			-> decltype(static_cast<Tp&&>(obj).*pmd) {
			return static_cast<Tp&&>(obj).*pmd;
		}
	};

	struct _Invoker_Pmd_refwrap {
		static constexpr _Invoker_strategy strategy = _Invoker_strategy::_Pmd_refwrap;
		template<class Decayed, class Refwrap>
		static constexpr auto Call(Decayed&& pmd, Refwrap&& rw) noexcept
			->decltype(rw.get().*pmd) {
			return rw.get().*pmd;
		}
	};

	struct _Invoker_Pmd_pointer {
		static constexpr _Invoker_strategy strategy = _Invoker_strategy::_Pmd_pointer;
		template<class Decayed, class Ptr>
		static constexpr auto Call(Decayed&& pmd, Ptr&& ptr) noexcept
			->decltype((*static_cast<Ptr&&>(ptr)).*pmd) {
			return (*static_cast<Ptr&&>(ptr)).*pmd;
		}
	};

	template<class Tp>
	using _Remove_cvref_t = remove_cv_t<remove_reference_t<Tp>>;

	template<class Tp, template<class...> class Template>
	constexpr bool _is_specialization_v = false;

	template<template<class...> class Template, class... Types>
	constexpr bool _is_specialization_v<Template<Types...>, Template> = true;

	template<class Callable, class Tp, class _Removed_cvref = _Remove_cvref_t<Callable>,
		bool _is_pmf = is_member_function_pointer_v<_Removed_cvref>,
		bool _is_pmd = is_member_object_pointer_v<_Removed_cvref>>
		struct _Invoker;

	template<class Callable, class Tp, class _Removed_cvref>
	struct _Invoker<Callable, Tp, _Removed_cvref, true, false>
		: conditional_t<is_base_of_v<typename std::_Is_memfunptr<_Removed_cvref>::_Class_type, remove_reference_t<Tp>>, _Invoker_Pmf_object,
		conditional_t<_Is_specialization_v<_Removed_cvref, reference_wrapper>, _Invoker_Pmf_refwrap,
		_Invoker_Pmf_pointer>> {};

	template<class Callable, class Tp, class _Removed_cvref>
	struct _Invoker<Callable, Tp, _Removed_cvref, false, true>
		: conditional_t<is_base_of_v<typename std::_Is_member_object_pointer<_Removed_cvref>::_Class_type, remove_reference_t<Tp>>, _Invoker_Pmd_object,
		conditional_t<_Is_specialization_v<_Removed_cvref, reference_wrapper>, _Invoker_Pmd_refwrap,
		_Invoker_Pmd_pointer>> {};

	template<class Callable, class Tp, class _Removed_cvref>
	struct _Invoker<Callable, Tp, _Removed_cvref, false, false> : _Invoker_Functor {};

	template<class Callable>	// 无参仿函数和无参函数指针调用
	inline auto invoke(Callable&& obj) noexcept(noexcept(static_cast<Callable&&>(obj)()))
		-> decltype(static_cast<Callable&&>(obj)()) {
		return static_cast<Callable&&>(obj)();
	}

	template<class Callable, class Tp, class... Types>
	inline auto invoke(Callable&& ptr, Tp&& obj, Types&&... args)
		noexcept(noexcept(_Invoker<Callable, Tp>::Call(static_cast<Callable&&>(ptr), static_cast<Tp&&>(obj), static_cast<Types>(args)...)))
		-> decltype(_Invoker<Callable, Tp>::Call(static_cast<Callable&&>(ptr), static_cast<Tp&&>(obj), static_cast<Types>(args)...)) {
		if constexpr (_Invoker<Callable, Tp>::strategy == _Invoker_strategy::_Pmf_object) {
			return (static_cast<Tp&&>(obj).*ptr)(static_cast<Types>(args)...);
		}
		else if constexpr (_Invoker<Callable, Tp>::strategy == _Invoker_strategy::_Pmf_refwrap) {
			return (obj.get().*ptr)(static_cast<Types>(args)...);
		}
		else if constexpr (_Invoker<Callable, Tp>::strategy == _Invoker_strategy::_Pmf_pointer) {
			return ((*static_cast<Tp&&>(obj)).*ptr)(static_cast<Types>(args)...);
		}
		else if constexpr (_Invoker<Callable, Tp>::strategy == _Invoker_strategy::_Pmd_object) {
			return static_cast<Tp&&>(obj).*ptr;
		}
		else if constexpr (_Invoker<Callable, Tp>::strategy == _Invoker_strategy::_Pmd_refwrap) {
			return obj.get().*ptr;
		}
		else if constexpr (_Invoker<Callable, Tp>::strategy == _Invoker_strategy::_Pmd_pointer) {
			return (*static_cast<Tp&&>(obj)).*ptr;
		}
		else {
			return static_cast<Callable&&>(ptr)(static_cast<Tp&&>(obj), static_cast<Types>(args)...);
		}
	}

	template<class Tp>
	Tp _Fake_copy_init(Tp) noexcept; // not defined，利用参数传递时的拷贝行为

	// _Fake_copy_init<T>(E):
	// (1) has type T [decay_t<decltype((E))> if T is deduced],
	// (2) is well-formed if and only if E is implicitly convertible to T and T is destructible, and
	// (3) is non-throwing if and only if both conversion from decltype((E)) to T and destruction of T are non-throwing.

	template<class Tp>
	Tp _Returns_exactly() noexcept; // not defined，返回Tp类型的值，不同于declval()的对于非引用类型返回Tp&&

	template<class From, class To, bool = is_convertible_v<From, To>, bool = is_void_v<To>>
	constexpr bool _is_nothrow_convertible_v = noexcept(_Fake_copy_init<To>(declval<From>()));

	template<class From, class To, bool IsVoid>
	constexpr bool _is_nothrow_convertible_v<From, To, false, IsVoid> = false;

	template<class From, class To>
	constexpr bool _is_nothrow_convertible_v<From, To, true, true> = true;

	template<class From, class To>
	struct _is_nothrow_convertible : bool_constant<_is_nothrow_convertible_v<From, To>> {};

	template<class Tp, class = void>
	struct _Weak_result_type {};

	template<class Tp>
	struct _Weak_result_type<Tp, void_t<typename Tp::result_type>> {
		using result_type_name = typename Tp::result_type;
	};

	template<class Tp, class = void>
	struct _Weak_argument_type : _Weak_result_type<Tp> {};

	template<class Tp>
	struct _Weak_argument_type<Tp, void_t<typename Tp::argument_type>> {
		using argument_type_name = typename Tp::argument_type;
	};

	template<class Tp, class = void>
	struct _Weak_binary_rags : _Weak_argument_type<Tp> {};

	template<class Tp>
	struct _Weak_binary_rags<Tp, void_t<typename Tp::first_argument_type,
		typename Tp::second_argument_type>> : _Weak_argument_type<Tp> {
		using first_argument_type = typename Tp::first_argument_type;
		using second_argument_type = typename Tp::second_argument_type;
	};

	template<class Tp>
	using _Weak_type = conditional_t < is_function_v<remove_pointer_t<Tp>>, std::_Function_args<remove_pointer_t<Tp>>, conditional_t<is_member_function_pointer_v<Tp>,
		std::_Is_memfunptr<remove_cv_t<Tp>>, _Weak_binary_rags<Tp>>>;

	template<class Tp>
	struct _Identity {
		using type = Tp;
	};

	template<class Tp>
	using _Identity_t = typename _Identity<Tp>::type;

	// not define, 利用函数传参用于判断是否可以转换为Tp&（reference_wrapper定义了到Tp&的隐式转换）
	template<class Tp>
	void _Refwrap_ctor_fun(_Identity_t<Tp&>) noexcept;

	template<class Tp>
	void _Refwrap_ctor_fun(_Identity_t<Tp&&>) = delete;

	template<class Tp, class Utp, class = void>
	struct _Refwrap_has_ctor_from : false_type {};

	template<class Tp, class Utp>
	struct _Refwrap_has_ctor_from<Tp, Utp, void_t<decltype(_Refwrap_ctor_fun<Tp>(declval<Utp>()))>> : true_type {};

	template<class Tp>
	class reference_wrapper : _Weak_type<Tp> {

		static_assert(is_object_v<Tp> || is_function_v<Tp>,
			"reference_wrapper<Tp>: Tp needs an object type or a function type");

	private:
		Tp* ptr_{};

	public:
		using type = Tp;

		template<class Utp, enable_if_t<conjunction_v<negation<is_same<_Remove_cvref_t<Tp>, reference_wrapper>>,
			_Refwrap_has_ctor_from<Tp, Utp>>, int> = 0 >
		reference_wrapper(Utp&& val) noexcept(noexcept(_Refwrap_ctor_fun<Tp>(declval<Utp>()))) {
			Tp& _ref = static_cast<Utp&&>(val);  // 若为右值则会为其分配内存
			ptr_ = std::addressof(_ref);
		}

		operator Tp& () const noexcept {
			return *ptr_;
		}

		Tp& get() const noexcept {
			return *ptr_;
		}

		// Tp为可调用类型
		template<class... Types>
		auto operator()(Types&&...args) const
			noexcept(noexcept(mstd::invoke(*ptr_, static_cast<Types&&>(args)...)))
			->decltype(mstd::invoke(*ptr_, static_cast<Types&&>(args)...)) {
			return mstd::invoke(*ptr_, static_cast<Types&&>(args)...);
		}
	};

	template<class Tp>
	inline reference_wrapper<Tp> ref(Tp& val) noexcept {
		return reference_wrapper<Tp>(val);
	}

	template<class Tp>
	void ref(const Tp&&) = delete; // 常量右值不可被包装器包装

	template<class Tp>
	inline reference_wrapper<Tp> ref(reference_wrapper<Tp>& val) noexcept {
		return val;
	}

	template<class Tp>
	inline reference_wrapper<const Tp> cref(const Tp& val) noexcept {
		return reference_wrapper<const Tp>(val);
	}

	template<class Tp>
	inline reference_wrapper<const Tp> cref(reference_wrapper<Tp>& val) noexcept {
		return val;
	}

}


namespace gnu_type_traits {

	/// 定义 true_type / false_type
	/// integral_constant 是 true_type/false_type 的基类，rank/extent 的基类
	template<typename _IntglConst, _IntglConst __value>
	struct integral_constant {
		static constexpr _IntglConst value = __value;
		typedef _IntglConst value_type;
		typedef integral_constant<_IntglConst, __value> type;
		constexpr operator value_type() const { return value; }
	};

	// 类的静态变量外部初始化
	template<typename _IntglConst, _IntglConst __value>
	constexpr _IntglConst integral_constant<_IntglConst, __value>::value;

	typedef integral_constant<bool, true> true_type;
	typedef integral_constant<bool, false> false_type;

	/// 辅助工具 __and_ / __or_ / __not_ / __failure_type / __success_type
	// __and_ / __or_ / __not_ 模板参数及模板类自身是或继承自 true_type / false_type 类型

	template<bool, typename, typename>
	struct conditional; // 前置声明 for __and_ / __or_

	/// __and_
	template<typename...>
	struct __and_;
	template<>
	struct __and_<> : public true_type {};
	template<typename _T1>
	struct __and_<_T1> : public _T1 {};
	template<typename _T1, typename _T2>
	struct __and_<_T1, _T2> : public conditional<_T1::value, _T2, _T1>::type {};
	template<typename _T1, typename _T2, typename _T3, typename... _Tn>
	struct __and_<_T1, _T2, _T3, _Tn...>
		: public conditional<_T1::value, __and_<_T2, _T3, _Tn...>, _T1>::type {};

	/// __or_
	template<typename...>
	struct __or_;
	template<>
	struct __or_<> : public false_type {};
	template<typename _T1>
	struct __or_<_T1> : public _T1 {};
	template<typename _T1, typename _T2>
	struct __or_<_T1, _T2> : public conditional<_T1::value, _T1, _T2>::type {};
	template<typename _T1, typename _T2, typename _T3, typename... _Tn>
	struct __or_<_T1, _T2, _T3, _Tn...>
		: public conditional<_T1::value, _T1, __or_<_T2, _T3, _Tn...>>::type {};

	/// __not_
	template<typename _T>
	struct __not_ : public integral_constant<bool, !_T::value> {};

	template<typename>
	struct remove_cv; // 前置声明 for is_void/is_integral/is_pointer.../, 先去掉 const/volatile

	template<typename>
	struct __is_void_helper : false_type {};
	template<>
	struct __is_void_helper<void> : true_type {};
	/// is_void : 是否是 void类型；
	template<typename _T>
	struct is_void
		: public __is_void_helper<typename remove_cv<_T>::type>::type {};

	template<typename>
	struct __is_integral_helper : public false_type {};
	template<>
	struct __is_integral_helper<bool> : public true_type {};
	template<>
	struct __is_integral_helper<char> : public true_type {};
	template<>
	struct __is_integral_helper<signed char> : public true_type {};
	template<>
	struct __is_integral_helper<unsigned char> : public true_type {};
	template<>
	struct __is_integral_helper<wchar_t> : public true_type {};
	template<>
	struct __is_integral_helper<char16_t> : public true_type {};
	template<>
	struct __is_integral_helper<char32_t> : public true_type {};
	template<>
	struct __is_integral_helper<short> : public true_type {};
	template<>
	struct __is_integral_helper<unsigned short> : public true_type {};
	template<>
	struct __is_integral_helper<int> : public true_type {};
	template<>
	struct __is_integral_helper<unsigned int> : public true_type {};
	template<>
	struct __is_integral_helper<long> : public true_type {};
	template<>
	struct __is_integral_helper<unsigned long> : public true_type {};
	template<>
	struct __is_integral_helper<long long> : public true_type {};
	template<>
	struct __is_integral_helper<unsigned long long> : public true_type {};
	/// is_integral : 是否是 整数
	/// bool/char/char16_t/char32_t/wchar_t/[unsigned] short/int/long/long long；
	template<typename _T>
	struct is_integral
		: public __is_integral_helper<typename remove_cv<_T>::type> {};

	template<typename>
	struct __is_floating_point_helper : public false_type {};
	template<>
	struct __is_floating_point_helper<float> : public true_type {};
	template<>
	struct __is_floating_point_helper<double> : public true_type {};
	template<>
	struct __is_floating_point_helper<long double> : public true_type {};
	/// is_floating_point : 是否是 浮点数：float/double/long double；
	template<typename _T>
	struct is_floating_point
		: public __is_floating_point_helper<typename remove_cv<_T>::type> {};

	/// is_arithmetic : 是否是 算术类型：整数/浮点数；
	template<typename _T>
	struct is_arithmetic
		: public __or_<is_integral<_T>, is_floating_point<_T>>::type {};

	template<typename _T, bool = is_arithmetic<_T>::value>
	struct __is_signed_helper : false_type {};
	template<typename _T>
	struct __is_signed_helper<_T, true>
		: public integral_constant<bool, _T(-1) < _T(0) > {};
	/// is_signed : 是否是 有符号算术类型；
	template<typename _T>
	struct is_signed : public __is_signed_helper<_T>::type {};

	/// is_unsigned : 是否是 无符号算术类型；
	template<typename _T>
	struct is_unsigned
		: public __and_<is_arithmetic<_T>, __not_<is_signed<_T>>>::type {};

	/// is_const : 是否是 const；
	template<typename _T>
	struct is_const : public false_type {};
	template<typename _T>
	struct is_const<const _T> : public true_type {};

	/// is_volatile : 是否是 volatile；
	template<typename _T>
	struct is_volatile : public false_type {};
	template<typename _T>
	struct is_volatile<volatile _T> : public true_type {};

	/// is_array : 是否是 内置数组（非std::array）；
	template<typename>
	struct is_array : false_type {};
	template<typename _T>
	struct is_array<_T[]> : public true_type {};
	template<typename _T, std::size_t _Size>
	struct is_array<_T[_Size]> : public true_type {};

	/// is_enum : 是否是 枚举（enumeration）；
	template<typename Tp>
	struct is_enum : public integral_constant<bool, __is_enum(Tp)> {};

	/// is_union : 是否是 联合体（union）；
	template<typename Tp>
	struct is_union : public integral_constant<bool, __is_union(Tp)> {};

	/// is_class : 是否是 class/struct 类型（非union）
	template<typename Tp>
	struct is_class : public integral_constant<bool, __is_class(Tp)> {};

	/// is_function : 是否是 函数类型；
	template<typename>
	struct is_function : false_type {};
	// 一般的函数类型偏特化
	template<typename _Ret, typename... Args> // 不可变参数函数类型
	struct is_function<_Ret(Args...)> : true_type {};
	// Partial specialization of function type : ReturnType(Type1,Type2,Args...)  -> 类似 printf 函数
	template<typename _Ret, typename... Args>  // 可变参数函数类型
	struct is_function<_Ret(Args......)> : true_type {};
	// 特别的函数类型偏特化
	// 常量+volatile成员函数类型偏特化
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) const> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) volatile> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) const volatile> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) const> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) volatile> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) const volatile> : true_type {};
	// 引用成员函数类型偏特化
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...)&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) const&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) volatile&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) const volatile&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......)&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) const&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) volatile&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) const volatile&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...)&&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) const&&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) volatile&&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args...) const volatile&&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......)&&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) const&&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) volatile&&> : true_type {};
	template<typename _Ret, typename... Args>
	struct is_function<_Ret(Args......) const volatile&&> : true_type {};
	// C++17 出现 noexcept 的 is_function 的偏特化版本

	/// is_lvalue_reference : 是否是 左值引用；
	template<typename _T>
	struct is_lvalue_reference : public false_type {};
	template<typename _T>
	struct is_lvalue_reference<_T&> : public true_type {};

	/// is_rvalue_reference : 是否是 右值引用；
	template<typename _T>
	struct is_rvalue_reference : public false_type {};
	template<typename _T>
	struct is_rvalue_reference<_T&&> : public true_type {};

	/// is_reference : 是否是 引用：左值或右值引用；
	template<typename _T>
	struct is_reference
		: public __or_<is_lvalue_reference<_T>, is_rvalue_reference<_T>>::type {};

	template<typename>
	struct __is_pointer_helper : false_type {};
	template<typename _T>
	struct __is_pointer_helper<_T*> : true_type {};
	/// is_pointer : 是否是 pointer 类型
	///（包括 function pointer 但不包括 pointer to nostatic member）；
	template<typename _T>
	struct is_pointer
		: __is_pointer_helper<typename remove_cv<_T>::type>::type {}; // 先移除指针本身的const/volatile属性

	template<typename>
	struct __is_member_pointer_helper : false_type {};
	template<typename _T, typename _C>
	struct __is_member_pointer_helper<_T _C::*> : true_type {};
	/// is_member_pointer : 是否是 指向 nostatic 成员的指针；
	template<typename _T>
	struct is_member_pointer
		: __is_member_pointer_helper<typename remove_cv<_T>::type>::type {}; // 先移除指针本身的const/volatile属性

	template<typename>
	struct __is_member_object_pointer_helper : public false_type {};
	template<typename Tp, typename _Cp>
	struct __is_member_object_pointer_helper<Tp _Cp::*>
		: public integral_constant<bool, !is_function<Tp>::value> {};  // is_function<_Tp> 判断成员指针所指向的类型，_Tp 是否是 函数类型
	/// is_member_object_pointer : 是否是 指向 nostatic 数据成员的指针；
	template<typename Tp>
	struct is_member_object_pointer
		: public __is_member_object_pointer_helper
		<typename remove_cv<Tp>::type>::type {};

	template<typename>
	struct __is_member_function_pointer_helper : public false_type {};
	template<typename Tp, typename _Cp>
	struct __is_member_function_pointer_helper<Tp _Cp::*>
		: public integral_constant<bool, is_function<Tp>::value> {};
	/// is_member_function_pointer : 是否是 指向 nostatic 成员函数的指针；
	template<typename Tp>
	struct is_member_function_pointer
		: public __is_member_function_pointer_helper
		<typename remove_cv<Tp>::type>::type {};

	template<typename>
	struct __is_null_pointer_helper : public false_type {};
	template<>
	struct __is_null_pointer_helper<std::nullptr_t> : public true_type {};
	/// is_null_pointer : 是否是 空指针；
	template<typename _T>
	struct is_null_pointer
		: __is_null_pointer_helper<typename remove_cv<_T>::type>::type {};

	/// is_null_pointer_t : 是否是 nullptr_t；
	template<typename _T>
	struct is_null_pointer_t
		: public is_null_pointer<_T>::type {};

	/// is_fundamental : 是否是 基础类型：void/算术类型/nullptr_t；
	template<typename _T>
	struct is_fundamental
		: public __or_< is_void<_T>, is_arithmetic<_T>,
		is_null_pointer<_T>>::type{};

	/// is_scalar : 是否是 标量：算术类型/枚举/pointer/member pointer/nullptr_t；
	template<typename _T>
	struct is_scalar
		: public __or_<is_arithmetic<_T>, is_enum<_T>, is_pointer<_T>,
		is_member_pointer<_T>, is_null_pointer<_T>>::type{};

	/// is_object : 是否是 任何类型（不包括 void/function/reference）；
	template<typename _T>
	struct is_object
		: public __not_<__or_<is_void<_T>, is_function<_T>, is_reference<_T>>>::type {};

	/// is_compound : 是否是 复杂类型（非基础类型）
	/// 包括：数组/enumeration/union/class/function/reference/pointer
	template<typename _T>
	struct is_compound
		: public integral_constant<bool, !is_fundamental<_T>::value> {};
	//template<typename _T>
	//struct is_compound : public __or_<is_array<_T>, is_enum<_T>, is_union<_T>,
	//	is_class<_T>, is_function<_T>, is_reference<_T>, is_pointer<_T>>::type{};

	/// is_trivial : scalar/trivial class/由 scalar 和 trivial class 构成的数组
	template<typename _T>
	struct is_trivial
		: public integral_constant<bool, __is_trivial(_T)> {};

	/// is_trivially_copyable
	/// 包括：scalar/trivial copyable class/由 scalar 和 trivial copyable class 构成的数组
	template<typename _T>
	struct is_trivially_copyable
		: public integral_constant<bool, __is_trivially_copyable(_T)> {};

	/// is_standard_layout
	/// 包括：scalar/standard layout class/由 scalar 和 standard layout class 构成的数组
	template<typename _T>
	struct is_standard_layout
		: public integral_constant<bool, __is_standard_layout(_T)> {};

	/// is_pod : plain old data type，可以使用 memcpy/memmove
	template<typename Tp>
	struct is_pod : public integral_constant<bool, __is_pod(Tp)> {};

	/// is_literal_type : scalar/reference/class/由 scalar、reference和class 构成的数组；
	template<typename _T>
	struct is_literal_type : public integral_constant<bool, __is_literal_type(_T)> {};



	/// is_empty : class 不带任何成员/virtual 成员函数/virtual base class；
	template<typename _T>
	struct is_empty : public integral_constant<bool, __is_empty(_T)> {};

	/// is_polymorphic : class 带有一个（derived）virtual 成员函数；
	template<typename _T>
	struct is_polymorphic
		: public integral_constant<bool, __is_polymorphic(_T)> {};

	/// is_abstract : abstract class（至少有一个 pure virtual 函数）；
	template<typename _T>
	struct is_abstract
		: public integral_constant<bool, __is_abstract(_T)> {};

	/// has_virtual_destructor : class 带有 虚析构函数；
	template<typename _T>
	struct has_virtual_destructor
		: public integral_constant<bool, __has_virtual_destructor(_T)> {};

	template<typename _T>
	struct add_rvalue_reference;

	template<typename _T>
	typename add_rvalue_reference<_T>::type declval() noexcept;

	template<typename, unsigned = 0>
	struct extent;

	template<typename _T>
	struct remove_all_extents;

	template<typename Tp>
	struct __is_array_known_bounds
		: public integral_constant<bool, (extent<Tp>::value > 0)>
	{ };

	template<typename Tp>
	struct __is_array_unknown_bounds
		: public __and_<is_array<Tp>, __not_<extent<Tp>>>::type
	{ };

	// In N3290 is_destructible does not say anything about function
	// types and abstract types, see LWG 2049. This implementation
	// describes function types as non-destructible and all complete
	// object types as destructible, iff the explicit destructor
	// call expression is wellformed.
	struct __do_is_destructible_impl
	{
		template<typename Tp, typename = decltype(declval<Tp&>().~_Tp())>
		static true_type __test(int);

		template<typename>
		static false_type __test(...);
	};

	template<typename Tp>
	struct __is_destructible_impl
		: public __do_is_destructible_impl
	{
		typedef decltype(__test<Tp>(0)) type;
	};

	template<typename Tp,
		bool = __or_<is_void<Tp>,
		__is_array_unknown_bounds<Tp>,
		is_function<Tp>>::value,
		bool = __or_<is_reference<Tp>, is_scalar<Tp>>::value>
		struct __is_destructible_safe;

	template<typename Tp>
	struct __is_destructible_safe<Tp, false, false>
		: public __is_destructible_impl<typename
		remove_all_extents<Tp>::type>::type
	{ };

	template<typename Tp>
	struct __is_destructible_safe<Tp, true, false>
		: public false_type { };

	template<typename Tp>
	struct __is_destructible_safe<Tp, false, true>
		: public true_type { };

	/// is_destructiable : 是否能够完成 析构（析构函数不能是 deleted/protected/private）；
	template<typename Tp>
	struct is_destructible
		: public __is_destructible_safe<Tp>::type
	{ };


	struct __do_is_default_constructible_impl
	{
		template<typename Tp, typename = decltype(Tp())>
		static true_type __test(int);

		template<typename>
		static false_type __test(...);
	};

	template<typename Tp>
	struct __is_default_constructible_impl
		: public __do_is_default_constructible_impl
	{
		typedef decltype(__test<Tp>(0)) type;
	};

	template<typename Tp>
	struct __is_default_constructible_atom
		: public __and_<__not_<is_void<Tp>>,
		__is_default_constructible_impl<Tp>>::type
	{ };

	template<typename Tp, bool = is_array<Tp>::value>
	struct __is_default_constructible_safe;

	// The following technique is a workaround for a current core language
	// restriction, which does not allow for array types to occur in 
	// functional casts of the form T().  Complete arrays can be default-
	// constructed, if the element type is default-constructible, but 
	// arrays with unknown bounds are not.
	template<typename Tp>
	struct __is_default_constructible_safe<Tp, true>
		: public __and_<__is_array_known_bounds<Tp>,
		__is_default_constructible_atom<typename
		remove_all_extents<Tp>::type>>::type
	{ };

	template<typename Tp>
	struct __is_default_constructible_safe<Tp, false>
		: public __is_default_constructible_atom<Tp>::type
	{ };

	/// is_default_constructible : 是否能够完成 默认构造；
	template<typename Tp>
	struct is_default_constructible
		: public __is_default_constructible_safe<Tp>::type
	{ };




	/// is_copy_constructible : 是够能够完成 拷贝构造；


	/// is_move_constructible : 是否能够完成 移动构造；


	/// is_copy_assignable : 是否能够完成 拷贝赋值；


	/// is_move_assignable : 是否能够完成 移动赋值；


	template<typename _T1, typename _T2>
	struct is_same : false_type {};
	template<typename _T>
	struct is_same<_T, _T> : true_type {};

	/// is_base_of <T1,T2> : T1 是否是 T2 的 base class；
	template<typename _Base, typename _Derived>
	struct is_base_of
		: public integral_constant<bool, __is_base_of(_Base, _Derived)> {};

	template<typename _From, typename _To,
		bool = __or_<is_void<_From>, is_function<_To>, is_array<_To>>::value>
	struct __is_convertible_helper { typedef typename is_void<_To>::type type; };

	template<typename _From, typename _To>
	class __is_convertible_helper<_From, _To, false> {
		template<typename _T>
		static void __test_aux(_T); // 判断是否函数参数类型可以转化为_T的类型

		template<typename _From, typename _To,
			typename = decltype(__test_aux<_To>(declval<_From>()))>
		static true_type __test(int);
		template<typename _From, typename _To>
		static false_type __test(...);
	public:
		typedef decltype(__test<_From, _To>(0)) type;
	};

	/// is_convertible <T1,T2> : T1 是否可转换成 T2；
	template<typename _From, typename _To>
	struct is_convertible : __is_convertible_helper<_From, _To>::type {};


	// is_nothrow_destructible requires that is_destructible is
   // satisfied as well.  We realize that by mimicing the
   // implementation of is_destructible but refer to noexcept(expr)
   // instead of decltype(expr).
	struct __do_is_nt_destructible_impl
	{
		template<typename Tp>
		static integral_constant<bool, noexcept(declval<Tp&>().~Tp())>
			__test(int);

		template<typename>
		static false_type __test(...);
	};

	template<typename Tp>
	struct __is_nt_destructible_impl
		: public __do_is_nt_destructible_impl
	{
		typedef decltype(__test<Tp>(0)) type;
	};

	template<typename Tp,
		bool = __or_<is_void<Tp>,
		__is_array_unknown_bounds<Tp>,
		is_function<Tp>>::value,
		bool = __or_<is_reference<Tp>, is_scalar<Tp>>::value>
		struct __is_nt_destructible_safe;

	template<typename Tp>
	struct __is_nt_destructible_safe<Tp, false, false>
		: public __is_nt_destructible_impl<typename
		remove_all_extents<Tp>::type>::type
	{ };

	template<typename Tp>
	struct __is_nt_destructible_safe<Tp, true, false>
		: public false_type { };

	template<typename Tp>
	struct __is_nt_destructible_safe<Tp, false, true>
		: public true_type { };

	/// is_nothrow_destructible
	template<typename Tp>
	struct is_nothrow_destructible
		: public __is_nt_destructible_safe<Tp>::type
	{ };

	/// remove_const : 去掉 const 的类型；
	template<typename _T>
	struct remove_const { typedef _T type; };
	template<typename _T>
	struct remove_const<const _T> { typedef _T type; };

	/// remove_volatile : 去掉 volatile 的类型；
	template<typename _T>
	struct remove_volatile { typedef _T type; };
	template<typename _T>
	struct remove_volatile<volatile _T> { typedef _T type; };

	/// remove_cv : 去掉 const 和 volatile 的类型；
	template<typename _T>
	struct remove_cv {
		typedef typename remove_const<typename remove_volatile<_T>::type>::type type;
	};

	/// add_const : 带 const 的类型；
	template<typename _T>
	struct add_const { typedef _T const type; };

	/// add_volatile : 带 volatile 的类型；
	template<typename _T>
	struct add_volatile { typedef _T volatile type; };

	/// add_cv : 带 const 和 volatile 的类型；
	template<typename _T>
	struct add_cv {
		typedef typename add_const<typename add_volatile<_T>::type>::type type;
	};

	// gcc 4.9
	// Utility for constructing identically cv-qualified types.
	template<typename _Unqualified, bool _IsConst, bool _IsVol>
	struct __cv_selector;

	template<typename _Unqualified>
	struct __cv_selector<_Unqualified, false, false> {
		typedef _Unqualified __type;
	};
	template<typename _Unqualified>
	struct __cv_selector<_Unqualified, false, true> {
		typedef volatile _Unqualified __type;
	};
	template<typename _Unqualified>
	struct __cv_selector<_Unqualified, true, false> {
		typedef const _Unqualified __type;
	};
	template<typename _Unqualified>
	struct __cv_selector<_Unqualified, true, true> {
		typedef const volatile _Unqualified __type;
	};
	template<typename _Qualified, typename _Unqualified,
		bool _IsConst = is_const<_Qualified>::value,
		bool _IsVol = is_volatile<_Qualified>::value>
	class __match_cv_qualifiers {
		typedef __cv_selector<_Unqualified, _IsConst, _IsVol> __match;
	public:
		typedef typename __match::__type __type;
	};

	// Utility for finding the unsigned versions of signed integral types.
	template<typename Tp>
	struct __make_unsigned { typedef Tp __type; };
	template<>
	struct __make_unsigned<char> { typedef unsigned char __type; };
	template<>
	struct __make_unsigned<signed char> { typedef unsigned char __type; };
	template<>
	struct __make_unsigned<short> { typedef unsigned short __type; };
	template<>
	struct __make_unsigned<int> { typedef unsigned int __type; };
	template<>
	struct __make_unsigned<long> { typedef unsigned long __type; };
	template<>
	struct __make_unsigned<long long> { typedef unsigned long long __type; };
	// Select between integral and enum: not possible to be both.
	template<typename Tp,
		bool _IsInt = is_integral<Tp>::value,
		bool _IsEnum = is_enum<Tp>::value>
	class __make_unsigned_selector;

	template<typename Tp>
	class __make_unsigned_selector<Tp, true, false> {
		typedef __make_unsigned<typename remove_cv<Tp>::type> __unsignedt;
		typedef typename __unsignedt::__type __unsigned_type;
		typedef __match_cv_qualifiers<Tp, __unsigned_type> __cv_unsigned;

	public:
		typedef typename __cv_unsigned::__type __type;
	};
	template<typename Tp>
	class __make_unsigned_selector<Tp, false, true> {
		// With -fshort-enums, an enum may be as small as a char.
		typedef unsigned char __smallest;
		static const bool __b0 = sizeof(Tp) <= sizeof(__smallest);
		static const bool __b1 = sizeof(Tp) <= sizeof(unsigned short);
		static const bool __b2 = sizeof(Tp) <= sizeof(unsigned int);
		typedef conditional<__b2, unsigned int, unsigned long> __cond2;
		typedef typename __cond2::type __cond2_type;
		typedef conditional<__b1, unsigned short, __cond2_type> __cond1;
		typedef typename __cond1::type __cond1_type;
	public:
		typedef typename conditional<__b0, __smallest, __cond1_type>::type __type;
	};

	/// make_unsigned : 对应 unsigned noreference 的类型；
	template<typename Tp>  // 使用 gcc 4.9 版定义方法
	struct make_unsigned { typedef typename __make_unsigned_selector<Tp>::__type type; };
	// Integral, but don't define.
	template<>
	struct make_unsigned<bool>;

	// Utility for finding the signed versions of unsigned integral types.
	template<typename Tp>
	struct __make_signed { typedef Tp __type; };
	template<>
	struct __make_signed<char> { typedef signed char __type; };
	template<>
	struct __make_signed<unsigned char> { typedef signed char __type; };
	template<>
	struct __make_signed<unsigned short> { typedef signed short __type; };
	template<>
	struct __make_signed<unsigned int> { typedef signed int __type; };
	template<>
	struct __make_signed<unsigned long> { typedef signed long __type; };
	template<>
	struct __make_signed<unsigned long long> { typedef signed long long __type; };

	// Select between integral and enum: not possible to be both.
	template<typename Tp,
		bool _IsInt = is_integral<Tp>::value,
		bool _IsEnum = is_enum<Tp>::value>
	class __make_signed_selector;

	template<typename Tp>
	class __make_signed_selector<Tp, true, false> {
		typedef __make_signed<typename remove_cv<Tp>::type> __signedt;
		typedef typename __signedt::__type __signed_type;
		typedef __match_cv_qualifiers<Tp, __signed_type> __cv_signed;
	public:
		typedef typename __cv_signed::__type __type;
	};

	template<typename Tp>
	class __make_signed_selector<Tp, false, true> {
		// With -fshort-enums, an enum may be as small as a char.
		typedef signed char __smallest;
		static const bool __b0 = sizeof(Tp) <= sizeof(__smallest);
		static const bool __b1 = sizeof(Tp) <= sizeof(signed short);
		static const bool __b2 = sizeof(Tp) <= sizeof(signed int);
		typedef conditional<__b2, signed int, signed long> __cond2;
		typedef typename __cond2::type __cond2_type;
		typedef conditional<__b1, signed short, __cond2_type> __cond1;
		typedef typename __cond1::type __cond1_type;
	public:
		typedef typename conditional<__b0, __smallest, __cond1_type>::type __type;
	};

	/// make_signed : 对应 signed noreference 的类型；
	template<typename Tp>  // 使用 gcc 4.9 版定义方法
	struct make_signed {
		typedef typename __make_signed_selector<Tp>::__type type;
	};
	// Integral, but don't define.
	template<>
	struct make_signed<bool>;

	/// remove_reference : 对应 noreference 类型；
	template<typename _T>
	struct remove_reference { typedef _T type; };
	template<typename _T>
	struct remove_reference<_T&> { typedef _T type; };
	template<typename _T>
	struct remove_reference<_T&&> { typedef _T type; };

	/// 是否是 可引用，可引用类型包括：
	template<typename _T>
	struct __is_referenceable : public __or_<is_object<_T>, is_reference<_T>>::type {};
	// 函数类型偏特化
	template<typename _Ret, typename...Args>
	struct __is_referenceable<_Ret(Args...)> : public true_type {};
	template<typename _Ret, typename...Args>
	struct __is_referenceable<_Ret(Args......)> : public true_type {};

	template<typename _T, bool = __is_referenceable<_T>::value>
	struct __add_lvalue_reference_helper { typedef _T type; };
	template<typename _T>
	struct __add_lvalue_reference_helper<_T, true> { typedef _T& type; };

	/// add_lvalue_reference : 对应 左值引用（右值变左值）；
	template<typename _T>
	struct add_lvalue_reference : public __add_lvalue_reference_helper<_T> {};

	template<typename _T, bool = __is_referenceable<_T>::value>
	struct __add_rvalue_reference_helper { typedef _T type; };
	template<typename _T>
	struct __add_rvalue_reference_helper<_T, true> { typedef _T&& type; };

	/// add_rvalue_reference : 对应 右值引用（左值不变）；
	template<typename _T>
	struct add_rvalue_reference : public __add_rvalue_reference_helper<_T> {};

	template<typename _T, typename _U>
	struct __remove_pointer_helper { typedef _T type; };
	template<typename _T, typename _U>
	struct __remove_pointer_helper<_T, _U*> { typedef _U type; };

	/// remove_pointer : pointer 所指向的类型（若是非pointer,则是原类型）（对成员指针不起作用）；
	/// 不能去掉成员指针的 指针属性
	template<typename _T>
	struct remove_pointer : __remove_pointer_helper<_T, typename remove_cv<_T>::type> {};

	template<typename _T, bool = __or_<__is_referenceable<_T>, is_void<_T>>::value>
	struct __add_pointer_helper { typedef _T type; };
	template<typename _T>
	struct __add_pointer_helper<_T, true> { typedef typename remove_reference<_T>::type* type; };
	/// add_pointer : 一个 pointer 类型，指向对应 noreference 类型（对类中成员不起作用）；
	/// 向可引用类型和void类型添加指针
	template<typename _T>
	struct add_pointer : __add_pointer_helper<_T> {};

	/// rank <T> : array 类型的维度（或是0）
	template<typename _T>
	struct rank : integral_constant<std::size_t, 0> {};
	template<typename _T>
	struct rank<_T[]> : integral_constant<std::size_t, 1 + rank<_T>::value> {};
	template<typename _T, std::size_t _Size>
	struct rank<_T[_Size]> : integral_constant<std::size_t, 1 + rank<_T>::value> {};

	/// extent <T,I=0> : 维度I（或0）的尺度（extent）；
	template<typename _T, unsigned _I>
	struct extent : public integral_constant<std::size_t, 0> {};
	template<typename _T, unsigned _I, std::size_t _Size>
	struct extent<_T[_Size], _I> : public integral_constant < std::size_t,
		_I == 0 ? _Size : extent<_T, _I - 1>::value> {};
	template<typename _T, unsigned _I>
	struct extent<_T[], _I> : public integral_constant < std::size_t,
		_I == 0 ? 0 : extent<_T, _I - 1>::value> {};

	/// remove_extent <T> : 获得 array 的元素类型（若T不是array则获得类型T）；
	template<typename _T>
	struct remove_extent { typedef _T type; };
	template<typename _T>
	struct remove_extent<_T[]> { typedef _T type; };
	template<typename _T, std::size_t _Size>
	struct remove_extent<_T[_Size]> { typedef _T type; };

	/// remove_all_extents <T> : 获得多维 array 的元素类型（若T不是多维array则获得类型T）；
	template<typename _T>
	struct remove_all_extents { typedef _T type; };
	template<typename _T>
	struct remove_all_extents<_T[]> { typedef typename remove_all_extents<_T>::type type; };
	template<typename _T, std::size_t _Size>
	struct remove_all_extents<_T[_Size]> { typedef typename remove_all_extents<_T>::type type; };

	/// underlying_type <T> : 枚举类型的低层类型；
	template<typename Tp>
	struct underlying_type { typedef __underlying_type(Tp) type; };

	template<typename _T, bool = is_array<_T>::value, bool = is_function<_T>::value>
	struct __decay_helper {};
	template<typename _T>
	struct __decay_helper<_T, false, false> { typedef typename remove_cv<_T>::type type; };
	template<typename _T>
	struct __decay_helper<_T, true, false> { typedef typename remove_extent<_T>::type* type; };
	template<typename _T>
	struct __decay_helper<_T, false, true> { typedef typename add_pointer<_T>::type type; };

	/// decay <T> : 将 T 转化为 “实值类型”（"by-value" type）；
	/// 普通类型将会去掉 引用/const/volatile, 函数类型 和 内置数组类型 去掉引用会转化为相应指针类型；
	template<typename _T>
	struct decay : __decay_helper<typename remove_reference<_T>::type> {};

	/// enable_if <B,T=void> : 只有当 bool B 为 true 时会产出 type T；
	template<bool, typename _T = void>
	struct enable_if {};
	template<typename _T>
	struct enable_if<true, _T> { typedef _T type; };

	/// conditional <B,T,F> : 当 bool B 为 true 会产出 type T，否则产出 type F；
	template<bool, typename _IfTrue, typename _IfFalse>
	struct conditional { typedef _IfTrue type; };
	template<typename _IfTrue, typename _IfFalse>
	struct conditional<false, _IfTrue, _IfFalse> { typedef _IfFalse type; };

	// 辅助模板类
	template<typename _T>
	struct __success_type { typedef _T type; };
	struct __failure_type {};

	template<typename Tp>
	struct __declval_protector
	{
		static const bool __stop = false;
		static typename add_rvalue_reference<Tp>::type __delegate(); // 声明一个返回右值引用的静态成员函数
	};

	template<typename Tp>
	inline typename add_rvalue_reference<Tp>::type  // 保持原类型 _Tp
		declval() noexcept   // 不能被调用  使用：decltype(declval<_Tp>()) 返回 _Tp 类型
	{
		static_assert(__declval_protector<Tp>::__stop,
			"declval() must not be used!");
		return __declval_protector<Tp>::__delegate();  // 只进行函数参数和返回值类型检查
	}

	template<typename...>
	struct common_type;

	struct __do_common_type_impl
	{
		template<typename Tp, typename _Up>
		static __success_type<typename decay<decltype
		(true ? declval<Tp>()
			: declval<_Up>())>::type> _S_test(int); // _Tp 与 _Up 有共通类型时编译通过 返回 __success_type 
		// __success_type::type 为 共通类型（“实值”）
		template<typename, typename>
		static __failure_type _S_test(...);
	};

	template<typename Tp, typename _Up>
	struct __common_type_impl
		: private __do_common_type_impl
	{
		typedef decltype(_S_test<Tp, _Up>(0)) type; // type: __success_type/__failure_type
	};

	struct __do_member_type_wrapper
	{
		template<typename Tp>
		static __success_type<typename Tp::type> _S_test(int); // _Tp 是 __success_type 编译通过

		template<typename>
		static __failure_type _S_test(...);
	};

	template<typename Tp>
	struct __member_type_wrapper
		: private __do_member_type_wrapper
	{
		typedef decltype(_S_test<Tp>(0)) type; // type: __success_type/__failure_type
	};

	template<typename _CTp, typename... _Args>
	struct __expanded_common_type_wrapper
	{
		typedef common_type<typename _CTp::type, _Args...> type;
	};

	template<typename... _Args>
	struct __expanded_common_type_wrapper<__failure_type, _Args...>
	{
		typedef __failure_type type;
	};

	/// common_type <T,...> : 所有被传入类型的共通类型；
	template<typename Tp>
	struct common_type<Tp>
	{
		typedef typename decay<Tp>::type type;
	};

	template<typename Tp, typename _Up>
	struct common_type<Tp, _Up>
		: public __common_type_impl<Tp, _Up>::type
	{ };

	template<typename Tp, typename _Up, typename... _Vp>
	struct common_type<Tp, _Up, _Vp...>
		: public __expanded_common_type_wrapper<typename __member_type_wrapper<
		common_type<Tp, _Up>>::type, _Vp...>::type
	{ };


	/// result_of <F,Arg> : 将 Arg 作为实参调用 F 后，其所得结果的类型；
	template<typename _Signature>
	class result_of;

	// [func.require] paragraph 1 bullet 1:
	struct __result_of_memfun_ref_impl
	{
		template<typename _Fp, typename _Tp1, typename... _Args>  // obj.fun(arg...)
		static __success_type<decltype(
			(declval<_Tp1>().*declval<_Fp>())(declval<_Args>()...)
			)> _S_test(int);

		template<typename...>
		static __failure_type _S_test(...);
	};

	template<typename _MemPtr, typename _Arg, typename... _Args>
	struct __result_of_memfun_ref
		: private __result_of_memfun_ref_impl
	{
		typedef decltype(_S_test<_MemPtr, _Arg, _Args...>(0)) type;
	};

	// [func.require] paragraph 1 bullet 2:
	struct __result_of_memfun_deref_impl
	{
		template<typename _Fp, typename _Tp1, typename... _Args>
		static __success_type<decltype(
			((*declval<_Tp1>()).*declval<_Fp>())(declval<_Args>()...) // (*objptr).fun(arg...)
			)> _S_test(int);

		template<typename...>
		static __failure_type _S_test(...);
	};

	template<typename _MemPtr, typename _Arg, typename... _Args>
	struct __result_of_memfun_deref
		: private __result_of_memfun_deref_impl
	{
		typedef decltype(_S_test<_MemPtr, _Arg, _Args...>(0)) type;
	};

	// [func.require] paragraph 1 bullet 3:
	struct __result_of_memobj_ref_impl
	{
		template<typename _Fp, typename _Tp1>
		static __success_type<decltype(
			declval<_Tp1>().*declval<_Fp>() // obj.data
			)> _S_test(int);

		template<typename, typename>
		static __failure_type _S_test(...);
	};

	template<typename _MemPtr, typename _Arg>
	struct __result_of_memobj_ref
		: private __result_of_memobj_ref_impl
	{
		typedef decltype(_S_test<_MemPtr, _Arg>(0)) type;
	};

	// [func.require] paragraph 1 bullet 4:
	struct __result_of_memobj_deref_impl
	{
		template<typename _Fp, typename _Tp1>
		static __success_type<decltype(
			(*declval<_Tp1>()).*declval<_Fp>() // (*objptr).data
			)> _S_test(int);

		template<typename, typename>
		static __failure_type _S_test(...);
	};

	template<typename _MemPtr, typename _Arg>
	struct __result_of_memobj_deref
		: private __result_of_memobj_deref_impl
	{
		typedef decltype(_S_test<_MemPtr, _Arg>(0)) type;
	};

	template<typename _MemPtr, typename _Arg>
	struct __result_of_memobj;

	template<typename _Res, typename _Class, typename _Arg>
	struct __result_of_memobj<_Res _Class::*, _Arg>
	{//  _Res _Class::* 成员指针类型， _Res 指针所指向的类型，_Class 成员指针所属类（对象类型或对象基类类型） _Arg 对象类型
		typedef typename remove_cv<typename remove_reference<
			_Arg>::type>::type _Argval;
		typedef _Res _Class::* _MemPtr;
		typedef typename conditional<__or_<is_same<_Argval, _Class>,
			is_base_of<_Class, _Argval>>::value,  // value 的值为 false 则说明 对象的类型是 指针，调用 deref 的版本
			__result_of_memobj_ref<_MemPtr, _Arg>,
			__result_of_memobj_deref<_MemPtr, _Arg>
			>::type::type type;
	};

	template<typename _MemPtr, typename _Arg, typename... _Args>
	struct __result_of_memfun;

	template<typename _Res, typename _Class, typename _Arg, typename... _Args>
	struct __result_of_memfun<_Res _Class::*, _Arg, _Args...>
	{
		typedef typename remove_cv<typename remove_reference<
			_Arg>::type>::type _Argval;
		typedef _Res _Class::* _MemPtr;
		typedef typename conditional<__or_<is_same<_Argval, _Class>,
			is_base_of<_Class, _Argval>>::value,
			__result_of_memfun_ref<_MemPtr, _Arg, _Args...>,
			__result_of_memfun_deref<_MemPtr, _Arg, _Args...>
			>::type::type type;
	};

	template<bool, bool, typename _Functor, typename... _ArgTypes>
	struct __result_of_impl
	{
		typedef __failure_type type;
	};

	template<typename _MemPtr, typename _Arg>  // 对象成员返回 obj.data 的类型
	struct __result_of_impl<true, false, _MemPtr, _Arg>
		: public __result_of_memobj<typename decay<_MemPtr>::type, _Arg>
	{ };

	template<typename _MemPtr, typename _Arg, typename... _Args>  // 对象方法调用 obj.fun(args...)
	struct __result_of_impl<false, true, _MemPtr, _Arg, _Args...>
		: public __result_of_memfun<typename decay<_MemPtr>::type, _Arg, _Args...>
	{ };

	// [func.require] paragraph 1 bullet 5:
	struct __result_of_other_impl
	{
		template<typename _Fn, typename... _Args>
		static __success_type<decltype(
			declval<_Fn>()(declval<_Args>()...)
			)> _S_test(int);

		template<typename...>
		static __failure_type _S_test(...);
	};

	template<typename _Functor, typename... _ArgTypes>  // 函数对象和普通函数调用 fun(args...)
	struct __result_of_impl<false, false, _Functor, _ArgTypes...>
		: private __result_of_other_impl
	{
		typedef decltype(_S_test<_Functor, _ArgTypes...>(0)) type;
	};

	template<typename _Functor, typename... _ArgTypes>
	struct result_of<_Functor(_ArgTypes...)>
		: public __result_of_impl<
		is_member_object_pointer<typename remove_reference<_Functor>::type>::value,
		is_member_function_pointer<typename remove_reference<_Functor>::type>::value,
		_Functor, _ArgTypes...
		>::type
	{ };




	/// aligned_of <T> : 等价于 alignof(T)；


	/// aligned_sotrage <Len> : Len byte 加上默认对齐规则所可容纳的类型；
	/// aligned_storage <Len,Align> : Len byte 且以 size_t Align 的约数进行对齐，所可容纳的类型；
	/// aligned_union <Len,Types...> : Len byte 且以 a union of Types... 进行对齐，所可容纳的类型；


	template<typename Tp>
	class reference_wrapper;

	// Helper which adds a reference to a type when given a reference_wrapper
	template<typename Tp>
	struct __strip_reference_wrapper
	{
		typedef Tp __type;
	};

	template<typename Tp>
	struct __strip_reference_wrapper<reference_wrapper<Tp> >
	{
		typedef Tp& __type;
	};

	template<typename Tp>
	struct __decay_and_strip
	{
		typedef typename __strip_reference_wrapper<
			typename decay<Tp>::type>::__type __type;
	};


	/// 模板别名
	// 
	/// remove_pointer_t
	template<typename _T>
	using remove_pointer_t = typename remove_pointer<_T>::type;

	/// add_pointer_t
	template<typename _T>
	using add_pointer_t = typename add_pointer<_T>::type;


	// Note：
	// true_type/false_type 的实现
	// 类型的表示、函数类型、成员指针
	// 模板重定义、递归和偏特化
	// make_signed/make_unsigned 的实现技巧
	// declval函数的实现、decltype()
	// __test(int)/__test(...) 模板函数的匹配
	// common_type / result_of 的实现
	// is_nothrow_destructible 的实现
	// <functional> reference wrapper for make_pair

	//struct B
	//{
	//	int func(int) { return 0; }
	//	int a{ };
	//};
	// std::cout << m_type_traits::is_same<int, m_type_traits::result_of<decltype(&B::func)(B, int)>::type>::value << std::endl;  //1
	// std::cout << m_type_traits::is_same<int, m_type_traits::result_of<decltype(&B::a)(B)>::type>::value << std::endl;  //0
	// std::cout << std::is_same<int, std::result_of<decltype(&B::a)(B)>::type>::value << std::endl;  //0


} //! m_type_traits


#endif //! _M_TYPE_TRAITS_H_

