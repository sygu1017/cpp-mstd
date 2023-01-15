#pragma once  // �����������ļ������ж������ļ��Ƿ���ͬ
#ifndef _M_TYPE_TRAITS_H_
#define _M_TYPE_TRAITS_H_

#include <cstddef> 

/*
* �������ڲ���intrinsic��
* C++11�У���Ҫ����������ʵ���жϣ�
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

///  �ж�������� type predicate    
//
// is_void : �Ƿ���void���ͣ�
// is_integral : ���� bool/char/char16_t/char32_t/wchar_t/[unsigned] short/int/long/long long��
// is_floating_point : ��������floating point����float/double/long double��
// is_arithmetic : �������ͣ�arithmetic��������/��������
// is_signed/is_unsigned : �Ƿ����з��Ż��޷������ͣ�
// is_const : �Ƿ��� const��
// is_volatile : �Ƿ��� volatile��
// is_array : �������飨��std::array����
// is_enum : ö�٣�enumeration����
// is_union : �����壨union����
// is_class : class/struct ���ͣ���union��
// is_function : �Ƿ��� �������ͣ�
//
// is_reference : ���ã���ֵ����ֵ���ã�
// is_lvalue_reference : ��ֵ���ã�
// is_rvalue_reference : ��ֵ���ã�
// is_pointer : pointer ���ͣ�!is_member_pointer����
// is_member_pointer : ָ�� nostatic ��Ա��ָ�루is_member_function_pointer+is_member_object_pointer����
// is_member_object_pointer : ָ�� nostatic ���ݳ�Ա��ָ�룻
// is_member_function_pointer : ָ�� nostatic ��Ա������ָ��(������ͨ��Ա��������������ת���͵��ò�������Ա����)��
//
// is_fundamental : �������ͣ���������/void/nullptr_t��
// is_scalar : ��������������/ö��/pointer/member pointer/nullptr_t��
// is_object : �������ͣ������� void/function/reference����!is_fundamental����
// is_compound : ����/enumeration/union/class/function/reference/pointer��
// is_trivial : scalar/trivial class/�� scalar �� trivial class ���ɵ����飻
// is_trivially_copyable : scalar/trivial copyable class/��scalar��trivial copyable class���ɵ����飬safe memcpy()�ȣ�
// is_standard_layout : scalar/standard layout class/�� scalar �� standard layout class ���ɵ����飻
// is_pod : plain old data type������ʹ�� memcpy() memmove()��
// is_literal_type : scalar/reference/class/�� scalar��reference��class ���ɵ����飻


///  �ж� class ����ʵ��ϸ��
//
// is_empty : class �����κγ�Ա/virtual ��Ա����/virtual base class��
// is_polymorphic : class ����һ����derived��virtual ��Ա������
// is_abstract : abstract class��������һ�� pure virtual ��������
// has_virtual_destructor : class ���� ������������
//
// is_default_constructible : �Ƿ��ܹ���� Ĭ�Ϲ��죻
// is_copy_constructible : �ǹ��ܹ���� �������죻
// is_move_constructible : �Ƿ��ܹ���� �ƶ����죻
// is_copy_assignable : �Ƿ��ܹ���� ������ֵ��
// is_move_assignable : �Ƿ��ܹ���� �ƶ���ֵ��
// is_destructiable : �Ƿ��ܹ���� �������������������� deleted/protected/private����
//
// is_trivially_default_constructible : �Ƿ��ܹ���� trivial Ĭ�Ϲ��죻
// is_trivially_copy_constructible : �ǹ��ܹ���� trivial �������죻
// is_trivially_move_constructible : �Ƿ��ܹ���� trivial �ƶ����죻
// is_trivially_copy_assignable : �Ƿ��ܹ���� trivial ������ֵ��
// is_trivially_move_assignable : �Ƿ��ܹ���� trivial �ƶ���ֵ��
// is_trivially_destructiable : �Ƿ��ܹ���� trivial ����;
//
// is_nothrow_default_constructible : �Ƿ��ܹ���� nothrow Ĭ�Ϲ��죻
// is_nothrow_copy_constructible : �ǹ��ܹ���� nothrow �������죻
// is_nothrow_move_constructible : �Ƿ��ܹ���� nothrow �ƶ����죻
// is_nothrow_copy_assignable : �Ƿ��ܹ���� nothrow ������ֵ��
// is_nothrow_move_assignable : �Ƿ��ܹ���� nothrow �ƶ���ֵ��
// is_nothrow_destructiable : �Ƿ��ܹ���� nothrow ����;


///  ��������֮���ϵ
//
// is_same <T1,T2> : T1��T2���� �Ƿ���ͬ������ const/volatile����
// is_base_of <T1,T2> : T1 �Ƿ��� T2 �� base class��
// is_convertible <T1,T2> : T1 �Ƿ��ת���� T2��
//
// is_constructible <T,Args...> : T �Ƿ���� Args ��ʼ����
// is_trivially_constructible <T,Args...> : T �Ƿ���� Args trivial ��ʼ����
// is_nothrow_constructible <T,Args...> : T �Ƿ���� Args nothrow ��ʼ����
//
// is_assignale <T1,T2> : T2 �Ƿ�� ��ֵ�� T1��
// is_trivially_assignable <T1,T2> : T2 �Ƿ�� trivial ��ֵ�� T1��
// is_nothrow_assignable <T1,T2> : T2 �Ƿ�� nothrow ��ֵ�� T1��
//
// uses_allocator <T,Alloc> : Alloc �Ƿ��ת��Ϊ T::Allocator��


///  ���� Type Trait
//
// rank <T> : array ���͵�ά�ȣ�����0��
// extent <T,I=0> : ά��I����0���ĳ߶ȣ�extent����
// remove_extent <T> : ��� array ��Ԫ�����ͣ���T����array��������T����
// remove_all_extents <T> : ��ö�ά array ��Ԫ�����ͣ���T���Ƕ�άarray��������T����
//
// underlying_type <T> : ö�����͵ĵͲ����ͣ�
// decay <T> : �� T ת��Ϊ ��ʵֵ���͡���"by-value" type����ȥ��ref cv���η����򽫺�������������ת��Ϊָ�����ͣ�
// enable_if <B,T=void> : ֻ�е� bool B Ϊ true ʱ����� type T��
// conditional <B,T,F> : �� bool B Ϊ true ����� type T��������� type F��
// 
// common_type <T,...> : ���б��������͵Ĺ�ͨ���ͣ������й�ͨ���ͣ���
// result_of <F,Arg> : �� Arg ��Ϊʵ�ε��� F �������ý�������ͣ�
//
// aligned_of <T> : �ȼ��� alignof(T)��
// aligned_sotrage <Len> : Len byte ����Ĭ�϶�������������ɵ����ͣ�
// aligned_storage <Len,Align> : Len byte ���� size_t Align ��Լ�����ж��룬�������ɵ����ͣ�
// aligned_union <Len,Types...> : Len byte ���� a union of Types... ���ж��룬�������ɵ����ͣ�


///  ���������޸�
//
// remove_const : ���� const �����ͣ�
// remove_volatile : ���� volatile �����ͣ�
// remove_cv : ���� const �� volatile �����ͣ�
//
// add_const : �� const �����ͣ�
// add_volatile : �� volatile �����ͣ�
// add_cv : �� const �� volatile �����ͣ�
//
// make_signed : ��Ӧ signed noreference �����ͣ�
// make_unsigned : ��Ӧ unsigned noreference �����ͣ�
//
// remove_reference : ��Ӧ noreference ���ͣ�
// add_lvalue_reference : ��Ӧ ��ֵ���ã���ֵ����ֵ����
// add_rvalue_reference : ��Ӧ ��ֵ���ã���ֵ���䣩��
//
// remove_pointer : �õ�pointer ��ָ������ͣ����Ƿ�pointer,����ԭ���ͣ���
// add_pointer : �õ�һ�� pointer ���ͣ�ָ���Ӧ noreference ���ͣ�


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

	// ����������ʵ��
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

	// ʵ�� reference_wrapper<>;
	template<class Tp>
	class reference_wrapper;

	enum class _Invoker_strategy {
		_Functor,		// �вηº������вκ���ָ��ĵ��ã�
		_Pmf_object,	// ����ʵ�����ó�Ա����ָ�룻
		_Pmf_refwrap,	// ���ð�װ�����ó�Ա����ָ�룻
		_Pmf_pointer,	// ����ָ����ó�Ա����ָ�룻

		_Pmd_object,	// ����ʵ�����ó�Ա����ָ�룻
		_Pmd_refwrap,	// ���ð�װ�����ó�Ա����ָ�룻
		_Pmd_pointer	// ����ָ����ó�Ա����ָ�룻
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

	template<class Callable>	// �޲ηº������޲κ���ָ�����
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
	Tp _Fake_copy_init(Tp) noexcept; // not defined�����ò�������ʱ�Ŀ�����Ϊ

	// _Fake_copy_init<T>(E):
	// (1) has type T [decay_t<decltype((E))> if T is deduced],
	// (2) is well-formed if and only if E is implicitly convertible to T and T is destructible, and
	// (3) is non-throwing if and only if both conversion from decltype((E)) to T and destruction of T are non-throwing.

	template<class Tp>
	Tp _Returns_exactly() noexcept; // not defined������Tp���͵�ֵ����ͬ��declval()�Ķ��ڷ��������ͷ���Tp&&

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

	// not define, ���ú������������ж��Ƿ����ת��ΪTp&��reference_wrapper�����˵�Tp&����ʽת����
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
			Tp& _ref = static_cast<Utp&&>(val);  // ��Ϊ��ֵ���Ϊ������ڴ�
			ptr_ = std::addressof(_ref);
		}

		operator Tp& () const noexcept {
			return *ptr_;
		}

		Tp& get() const noexcept {
			return *ptr_;
		}

		// TpΪ�ɵ�������
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
	void ref(const Tp&&) = delete; // ������ֵ���ɱ���װ����װ

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

	/// ���� true_type / false_type
	/// integral_constant �� true_type/false_type �Ļ��࣬rank/extent �Ļ���
	template<typename _IntglConst, _IntglConst __value>
	struct integral_constant {
		static constexpr _IntglConst value = __value;
		typedef _IntglConst value_type;
		typedef integral_constant<_IntglConst, __value> type;
		constexpr operator value_type() const { return value; }
	};

	// ��ľ�̬�����ⲿ��ʼ��
	template<typename _IntglConst, _IntglConst __value>
	constexpr _IntglConst integral_constant<_IntglConst, __value>::value;

	typedef integral_constant<bool, true> true_type;
	typedef integral_constant<bool, false> false_type;

	/// �������� __and_ / __or_ / __not_ / __failure_type / __success_type
	// __and_ / __or_ / __not_ ģ�������ģ���������ǻ�̳��� true_type / false_type ����

	template<bool, typename, typename>
	struct conditional; // ǰ������ for __and_ / __or_

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
	struct remove_cv; // ǰ������ for is_void/is_integral/is_pointer.../, ��ȥ�� const/volatile

	template<typename>
	struct __is_void_helper : false_type {};
	template<>
	struct __is_void_helper<void> : true_type {};
	/// is_void : �Ƿ��� void���ͣ�
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
	/// is_integral : �Ƿ��� ����
	/// bool/char/char16_t/char32_t/wchar_t/[unsigned] short/int/long/long long��
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
	/// is_floating_point : �Ƿ��� ��������float/double/long double��
	template<typename _T>
	struct is_floating_point
		: public __is_floating_point_helper<typename remove_cv<_T>::type> {};

	/// is_arithmetic : �Ƿ��� �������ͣ�����/��������
	template<typename _T>
	struct is_arithmetic
		: public __or_<is_integral<_T>, is_floating_point<_T>>::type {};

	template<typename _T, bool = is_arithmetic<_T>::value>
	struct __is_signed_helper : false_type {};
	template<typename _T>
	struct __is_signed_helper<_T, true>
		: public integral_constant<bool, _T(-1) < _T(0) > {};
	/// is_signed : �Ƿ��� �з����������ͣ�
	template<typename _T>
	struct is_signed : public __is_signed_helper<_T>::type {};

	/// is_unsigned : �Ƿ��� �޷����������ͣ�
	template<typename _T>
	struct is_unsigned
		: public __and_<is_arithmetic<_T>, __not_<is_signed<_T>>>::type {};

	/// is_const : �Ƿ��� const��
	template<typename _T>
	struct is_const : public false_type {};
	template<typename _T>
	struct is_const<const _T> : public true_type {};

	/// is_volatile : �Ƿ��� volatile��
	template<typename _T>
	struct is_volatile : public false_type {};
	template<typename _T>
	struct is_volatile<volatile _T> : public true_type {};

	/// is_array : �Ƿ��� �������飨��std::array����
	template<typename>
	struct is_array : false_type {};
	template<typename _T>
	struct is_array<_T[]> : public true_type {};
	template<typename _T, std::size_t _Size>
	struct is_array<_T[_Size]> : public true_type {};

	/// is_enum : �Ƿ��� ö�٣�enumeration����
	template<typename Tp>
	struct is_enum : public integral_constant<bool, __is_enum(Tp)> {};

	/// is_union : �Ƿ��� �����壨union����
	template<typename Tp>
	struct is_union : public integral_constant<bool, __is_union(Tp)> {};

	/// is_class : �Ƿ��� class/struct ���ͣ���union��
	template<typename Tp>
	struct is_class : public integral_constant<bool, __is_class(Tp)> {};

	/// is_function : �Ƿ��� �������ͣ�
	template<typename>
	struct is_function : false_type {};
	// һ��ĺ�������ƫ�ػ�
	template<typename _Ret, typename... Args> // ���ɱ������������
	struct is_function<_Ret(Args...)> : true_type {};
	// Partial specialization of function type : ReturnType(Type1,Type2,Args...)  -> ���� printf ����
	template<typename _Ret, typename... Args>  // �ɱ������������
	struct is_function<_Ret(Args......)> : true_type {};
	// �ر�ĺ�������ƫ�ػ�
	// ����+volatile��Ա��������ƫ�ػ�
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
	// ���ó�Ա��������ƫ�ػ�
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
	// C++17 ���� noexcept �� is_function ��ƫ�ػ��汾

	/// is_lvalue_reference : �Ƿ��� ��ֵ���ã�
	template<typename _T>
	struct is_lvalue_reference : public false_type {};
	template<typename _T>
	struct is_lvalue_reference<_T&> : public true_type {};

	/// is_rvalue_reference : �Ƿ��� ��ֵ���ã�
	template<typename _T>
	struct is_rvalue_reference : public false_type {};
	template<typename _T>
	struct is_rvalue_reference<_T&&> : public true_type {};

	/// is_reference : �Ƿ��� ���ã���ֵ����ֵ���ã�
	template<typename _T>
	struct is_reference
		: public __or_<is_lvalue_reference<_T>, is_rvalue_reference<_T>>::type {};

	template<typename>
	struct __is_pointer_helper : false_type {};
	template<typename _T>
	struct __is_pointer_helper<_T*> : true_type {};
	/// is_pointer : �Ƿ��� pointer ����
	///������ function pointer �������� pointer to nostatic member����
	template<typename _T>
	struct is_pointer
		: __is_pointer_helper<typename remove_cv<_T>::type>::type {}; // ���Ƴ�ָ�뱾���const/volatile����

	template<typename>
	struct __is_member_pointer_helper : false_type {};
	template<typename _T, typename _C>
	struct __is_member_pointer_helper<_T _C::*> : true_type {};
	/// is_member_pointer : �Ƿ��� ָ�� nostatic ��Ա��ָ�룻
	template<typename _T>
	struct is_member_pointer
		: __is_member_pointer_helper<typename remove_cv<_T>::type>::type {}; // ���Ƴ�ָ�뱾���const/volatile����

	template<typename>
	struct __is_member_object_pointer_helper : public false_type {};
	template<typename Tp, typename _Cp>
	struct __is_member_object_pointer_helper<Tp _Cp::*>
		: public integral_constant<bool, !is_function<Tp>::value> {};  // is_function<_Tp> �жϳ�Աָ����ָ������ͣ�_Tp �Ƿ��� ��������
	/// is_member_object_pointer : �Ƿ��� ָ�� nostatic ���ݳ�Ա��ָ�룻
	template<typename Tp>
	struct is_member_object_pointer
		: public __is_member_object_pointer_helper
		<typename remove_cv<Tp>::type>::type {};

	template<typename>
	struct __is_member_function_pointer_helper : public false_type {};
	template<typename Tp, typename _Cp>
	struct __is_member_function_pointer_helper<Tp _Cp::*>
		: public integral_constant<bool, is_function<Tp>::value> {};
	/// is_member_function_pointer : �Ƿ��� ָ�� nostatic ��Ա������ָ�룻
	template<typename Tp>
	struct is_member_function_pointer
		: public __is_member_function_pointer_helper
		<typename remove_cv<Tp>::type>::type {};

	template<typename>
	struct __is_null_pointer_helper : public false_type {};
	template<>
	struct __is_null_pointer_helper<std::nullptr_t> : public true_type {};
	/// is_null_pointer : �Ƿ��� ��ָ�룻
	template<typename _T>
	struct is_null_pointer
		: __is_null_pointer_helper<typename remove_cv<_T>::type>::type {};

	/// is_null_pointer_t : �Ƿ��� nullptr_t��
	template<typename _T>
	struct is_null_pointer_t
		: public is_null_pointer<_T>::type {};

	/// is_fundamental : �Ƿ��� �������ͣ�void/��������/nullptr_t��
	template<typename _T>
	struct is_fundamental
		: public __or_< is_void<_T>, is_arithmetic<_T>,
		is_null_pointer<_T>>::type{};

	/// is_scalar : �Ƿ��� ��������������/ö��/pointer/member pointer/nullptr_t��
	template<typename _T>
	struct is_scalar
		: public __or_<is_arithmetic<_T>, is_enum<_T>, is_pointer<_T>,
		is_member_pointer<_T>, is_null_pointer<_T>>::type{};

	/// is_object : �Ƿ��� �κ����ͣ������� void/function/reference����
	template<typename _T>
	struct is_object
		: public __not_<__or_<is_void<_T>, is_function<_T>, is_reference<_T>>>::type {};

	/// is_compound : �Ƿ��� �������ͣ��ǻ������ͣ�
	/// ����������/enumeration/union/class/function/reference/pointer
	template<typename _T>
	struct is_compound
		: public integral_constant<bool, !is_fundamental<_T>::value> {};
	//template<typename _T>
	//struct is_compound : public __or_<is_array<_T>, is_enum<_T>, is_union<_T>,
	//	is_class<_T>, is_function<_T>, is_reference<_T>, is_pointer<_T>>::type{};

	/// is_trivial : scalar/trivial class/�� scalar �� trivial class ���ɵ�����
	template<typename _T>
	struct is_trivial
		: public integral_constant<bool, __is_trivial(_T)> {};

	/// is_trivially_copyable
	/// ������scalar/trivial copyable class/�� scalar �� trivial copyable class ���ɵ�����
	template<typename _T>
	struct is_trivially_copyable
		: public integral_constant<bool, __is_trivially_copyable(_T)> {};

	/// is_standard_layout
	/// ������scalar/standard layout class/�� scalar �� standard layout class ���ɵ�����
	template<typename _T>
	struct is_standard_layout
		: public integral_constant<bool, __is_standard_layout(_T)> {};

	/// is_pod : plain old data type������ʹ�� memcpy/memmove
	template<typename Tp>
	struct is_pod : public integral_constant<bool, __is_pod(Tp)> {};

	/// is_literal_type : scalar/reference/class/�� scalar��reference��class ���ɵ����飻
	template<typename _T>
	struct is_literal_type : public integral_constant<bool, __is_literal_type(_T)> {};



	/// is_empty : class �����κγ�Ա/virtual ��Ա����/virtual base class��
	template<typename _T>
	struct is_empty : public integral_constant<bool, __is_empty(_T)> {};

	/// is_polymorphic : class ����һ����derived��virtual ��Ա������
	template<typename _T>
	struct is_polymorphic
		: public integral_constant<bool, __is_polymorphic(_T)> {};

	/// is_abstract : abstract class��������һ�� pure virtual ��������
	template<typename _T>
	struct is_abstract
		: public integral_constant<bool, __is_abstract(_T)> {};

	/// has_virtual_destructor : class ���� ������������
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

	/// is_destructiable : �Ƿ��ܹ���� �������������������� deleted/protected/private����
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

	/// is_default_constructible : �Ƿ��ܹ���� Ĭ�Ϲ��죻
	template<typename Tp>
	struct is_default_constructible
		: public __is_default_constructible_safe<Tp>::type
	{ };




	/// is_copy_constructible : �ǹ��ܹ���� �������죻


	/// is_move_constructible : �Ƿ��ܹ���� �ƶ����죻


	/// is_copy_assignable : �Ƿ��ܹ���� ������ֵ��


	/// is_move_assignable : �Ƿ��ܹ���� �ƶ���ֵ��


	template<typename _T1, typename _T2>
	struct is_same : false_type {};
	template<typename _T>
	struct is_same<_T, _T> : true_type {};

	/// is_base_of <T1,T2> : T1 �Ƿ��� T2 �� base class��
	template<typename _Base, typename _Derived>
	struct is_base_of
		: public integral_constant<bool, __is_base_of(_Base, _Derived)> {};

	template<typename _From, typename _To,
		bool = __or_<is_void<_From>, is_function<_To>, is_array<_To>>::value>
	struct __is_convertible_helper { typedef typename is_void<_To>::type type; };

	template<typename _From, typename _To>
	class __is_convertible_helper<_From, _To, false> {
		template<typename _T>
		static void __test_aux(_T); // �ж��Ƿ����������Ϳ���ת��Ϊ_T������

		template<typename _From, typename _To,
			typename = decltype(__test_aux<_To>(declval<_From>()))>
		static true_type __test(int);
		template<typename _From, typename _To>
		static false_type __test(...);
	public:
		typedef decltype(__test<_From, _To>(0)) type;
	};

	/// is_convertible <T1,T2> : T1 �Ƿ��ת���� T2��
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

	/// remove_const : ȥ�� const �����ͣ�
	template<typename _T>
	struct remove_const { typedef _T type; };
	template<typename _T>
	struct remove_const<const _T> { typedef _T type; };

	/// remove_volatile : ȥ�� volatile �����ͣ�
	template<typename _T>
	struct remove_volatile { typedef _T type; };
	template<typename _T>
	struct remove_volatile<volatile _T> { typedef _T type; };

	/// remove_cv : ȥ�� const �� volatile �����ͣ�
	template<typename _T>
	struct remove_cv {
		typedef typename remove_const<typename remove_volatile<_T>::type>::type type;
	};

	/// add_const : �� const �����ͣ�
	template<typename _T>
	struct add_const { typedef _T const type; };

	/// add_volatile : �� volatile �����ͣ�
	template<typename _T>
	struct add_volatile { typedef _T volatile type; };

	/// add_cv : �� const �� volatile �����ͣ�
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

	/// make_unsigned : ��Ӧ unsigned noreference �����ͣ�
	template<typename Tp>  // ʹ�� gcc 4.9 �涨�巽��
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

	/// make_signed : ��Ӧ signed noreference �����ͣ�
	template<typename Tp>  // ʹ�� gcc 4.9 �涨�巽��
	struct make_signed {
		typedef typename __make_signed_selector<Tp>::__type type;
	};
	// Integral, but don't define.
	template<>
	struct make_signed<bool>;

	/// remove_reference : ��Ӧ noreference ���ͣ�
	template<typename _T>
	struct remove_reference { typedef _T type; };
	template<typename _T>
	struct remove_reference<_T&> { typedef _T type; };
	template<typename _T>
	struct remove_reference<_T&&> { typedef _T type; };

	/// �Ƿ��� �����ã����������Ͱ�����
	template<typename _T>
	struct __is_referenceable : public __or_<is_object<_T>, is_reference<_T>>::type {};
	// ��������ƫ�ػ�
	template<typename _Ret, typename...Args>
	struct __is_referenceable<_Ret(Args...)> : public true_type {};
	template<typename _Ret, typename...Args>
	struct __is_referenceable<_Ret(Args......)> : public true_type {};

	template<typename _T, bool = __is_referenceable<_T>::value>
	struct __add_lvalue_reference_helper { typedef _T type; };
	template<typename _T>
	struct __add_lvalue_reference_helper<_T, true> { typedef _T& type; };

	/// add_lvalue_reference : ��Ӧ ��ֵ���ã���ֵ����ֵ����
	template<typename _T>
	struct add_lvalue_reference : public __add_lvalue_reference_helper<_T> {};

	template<typename _T, bool = __is_referenceable<_T>::value>
	struct __add_rvalue_reference_helper { typedef _T type; };
	template<typename _T>
	struct __add_rvalue_reference_helper<_T, true> { typedef _T&& type; };

	/// add_rvalue_reference : ��Ӧ ��ֵ���ã���ֵ���䣩��
	template<typename _T>
	struct add_rvalue_reference : public __add_rvalue_reference_helper<_T> {};

	template<typename _T, typename _U>
	struct __remove_pointer_helper { typedef _T type; };
	template<typename _T, typename _U>
	struct __remove_pointer_helper<_T, _U*> { typedef _U type; };

	/// remove_pointer : pointer ��ָ������ͣ����Ƿ�pointer,����ԭ���ͣ����Գ�Աָ�벻�����ã���
	/// ����ȥ����Աָ��� ָ������
	template<typename _T>
	struct remove_pointer : __remove_pointer_helper<_T, typename remove_cv<_T>::type> {};

	template<typename _T, bool = __or_<__is_referenceable<_T>, is_void<_T>>::value>
	struct __add_pointer_helper { typedef _T type; };
	template<typename _T>
	struct __add_pointer_helper<_T, true> { typedef typename remove_reference<_T>::type* type; };
	/// add_pointer : һ�� pointer ���ͣ�ָ���Ӧ noreference ���ͣ������г�Ա�������ã���
	/// ����������ͺ�void�������ָ��
	template<typename _T>
	struct add_pointer : __add_pointer_helper<_T> {};

	/// rank <T> : array ���͵�ά�ȣ�����0��
	template<typename _T>
	struct rank : integral_constant<std::size_t, 0> {};
	template<typename _T>
	struct rank<_T[]> : integral_constant<std::size_t, 1 + rank<_T>::value> {};
	template<typename _T, std::size_t _Size>
	struct rank<_T[_Size]> : integral_constant<std::size_t, 1 + rank<_T>::value> {};

	/// extent <T,I=0> : ά��I����0���ĳ߶ȣ�extent����
	template<typename _T, unsigned _I>
	struct extent : public integral_constant<std::size_t, 0> {};
	template<typename _T, unsigned _I, std::size_t _Size>
	struct extent<_T[_Size], _I> : public integral_constant < std::size_t,
		_I == 0 ? _Size : extent<_T, _I - 1>::value> {};
	template<typename _T, unsigned _I>
	struct extent<_T[], _I> : public integral_constant < std::size_t,
		_I == 0 ? 0 : extent<_T, _I - 1>::value> {};

	/// remove_extent <T> : ��� array ��Ԫ�����ͣ���T����array��������T����
	template<typename _T>
	struct remove_extent { typedef _T type; };
	template<typename _T>
	struct remove_extent<_T[]> { typedef _T type; };
	template<typename _T, std::size_t _Size>
	struct remove_extent<_T[_Size]> { typedef _T type; };

	/// remove_all_extents <T> : ��ö�ά array ��Ԫ�����ͣ���T���Ƕ�άarray��������T����
	template<typename _T>
	struct remove_all_extents { typedef _T type; };
	template<typename _T>
	struct remove_all_extents<_T[]> { typedef typename remove_all_extents<_T>::type type; };
	template<typename _T, std::size_t _Size>
	struct remove_all_extents<_T[_Size]> { typedef typename remove_all_extents<_T>::type type; };

	/// underlying_type <T> : ö�����͵ĵͲ����ͣ�
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

	/// decay <T> : �� T ת��Ϊ ��ʵֵ���͡���"by-value" type����
	/// ��ͨ���ͽ���ȥ�� ����/const/volatile, �������� �� ������������ ȥ�����û�ת��Ϊ��Ӧָ�����ͣ�
	template<typename _T>
	struct decay : __decay_helper<typename remove_reference<_T>::type> {};

	/// enable_if <B,T=void> : ֻ�е� bool B Ϊ true ʱ����� type T��
	template<bool, typename _T = void>
	struct enable_if {};
	template<typename _T>
	struct enable_if<true, _T> { typedef _T type; };

	/// conditional <B,T,F> : �� bool B Ϊ true ����� type T��������� type F��
	template<bool, typename _IfTrue, typename _IfFalse>
	struct conditional { typedef _IfTrue type; };
	template<typename _IfTrue, typename _IfFalse>
	struct conditional<false, _IfTrue, _IfFalse> { typedef _IfFalse type; };

	// ����ģ����
	template<typename _T>
	struct __success_type { typedef _T type; };
	struct __failure_type {};

	template<typename Tp>
	struct __declval_protector
	{
		static const bool __stop = false;
		static typename add_rvalue_reference<Tp>::type __delegate(); // ����һ��������ֵ���õľ�̬��Ա����
	};

	template<typename Tp>
	inline typename add_rvalue_reference<Tp>::type  // ����ԭ���� _Tp
		declval() noexcept   // ���ܱ�����  ʹ�ã�decltype(declval<_Tp>()) ���� _Tp ����
	{
		static_assert(__declval_protector<Tp>::__stop,
			"declval() must not be used!");
		return __declval_protector<Tp>::__delegate();  // ֻ���к��������ͷ���ֵ���ͼ��
	}

	template<typename...>
	struct common_type;

	struct __do_common_type_impl
	{
		template<typename Tp, typename _Up>
		static __success_type<typename decay<decltype
		(true ? declval<Tp>()
			: declval<_Up>())>::type> _S_test(int); // _Tp �� _Up �й�ͨ����ʱ����ͨ�� ���� __success_type 
		// __success_type::type Ϊ ��ͨ���ͣ���ʵֵ����
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
		static __success_type<typename Tp::type> _S_test(int); // _Tp �� __success_type ����ͨ��

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

	/// common_type <T,...> : ���б��������͵Ĺ�ͨ���ͣ�
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


	/// result_of <F,Arg> : �� Arg ��Ϊʵ�ε��� F �������ý�������ͣ�
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
	{//  _Res _Class::* ��Աָ�����ͣ� _Res ָ����ָ������ͣ�_Class ��Աָ�������ࣨ�������ͻ����������ͣ� _Arg ��������
		typedef typename remove_cv<typename remove_reference<
			_Arg>::type>::type _Argval;
		typedef _Res _Class::* _MemPtr;
		typedef typename conditional<__or_<is_same<_Argval, _Class>,
			is_base_of<_Class, _Argval>>::value,  // value ��ֵΪ false ��˵�� ����������� ָ�룬���� deref �İ汾
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

	template<typename _MemPtr, typename _Arg>  // �����Ա���� obj.data ������
	struct __result_of_impl<true, false, _MemPtr, _Arg>
		: public __result_of_memobj<typename decay<_MemPtr>::type, _Arg>
	{ };

	template<typename _MemPtr, typename _Arg, typename... _Args>  // ���󷽷����� obj.fun(args...)
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

	template<typename _Functor, typename... _ArgTypes>  // �����������ͨ�������� fun(args...)
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




	/// aligned_of <T> : �ȼ��� alignof(T)��


	/// aligned_sotrage <Len> : Len byte ����Ĭ�϶�������������ɵ����ͣ�
	/// aligned_storage <Len,Align> : Len byte ���� size_t Align ��Լ�����ж��룬�������ɵ����ͣ�
	/// aligned_union <Len,Types...> : Len byte ���� a union of Types... ���ж��룬�������ɵ����ͣ�


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


	/// ģ�����
	// 
	/// remove_pointer_t
	template<typename _T>
	using remove_pointer_t = typename remove_pointer<_T>::type;

	/// add_pointer_t
	template<typename _T>
	using add_pointer_t = typename add_pointer<_T>::type;


	// Note��
	// true_type/false_type ��ʵ��
	// ���͵ı�ʾ���������͡���Աָ��
	// ģ���ض��塢�ݹ��ƫ�ػ�
	// make_signed/make_unsigned ��ʵ�ּ���
	// declval������ʵ�֡�decltype()
	// __test(int)/__test(...) ģ�庯����ƥ��
	// common_type / result_of ��ʵ��
	// is_nothrow_destructible ��ʵ��
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

