#pragma once

/*
Functions
These functions create objects of wrapper classes based on its arguments:

bind		Bind function arguments (function template )
cref		Construct reference_wrapper to const (function template )
mem_fn		Convert member function to function object (function template )
not1		Return negation of unary function object (function template )
not2		Return negation of binary function object (function template )
ref			Construct reference_wrapper (function template )


Classes
Wrapper classes:
Wrapper classes are classes that hold an object and have an interface similar to that object,
but adding or changing some of its features:

binary_negate		Negate binary function object class (class template )
function			Function wrapper (class template )
reference_wrapper	Reference wrapper (class template )
unary_negate		Negate unary function object class (class template )


Operator classes:
Operator classes are classes that define functional objects that call operators:

bit_and			Bitwise AND function object class (class template )
bit_or			Bitwise OR function object class (class template )
bit_xor			Bitwise XOR function object class (class template )
divides			Division function object class (class template )
equal_to		Function object class for equality comparison (class template )
greater			Function object class for greater-than inequality comparison (class template )
greater_equal	Function object class for greater-than-or-equal-to comparison (class template )
less			Function object class for less-than inequality comparison (class template )
less_equal		Function object class for less-than-or-equal-to comparison (class template )
logical_and		Logical AND function object class (class template )
logical_not		Logical NOT function object class (class template )
logical_or		Logical OR function object class (class template )
minus			Subtraction function object class (class template )
modulus			Modulus function object class (class template )
multiplies		Multiplication function object class (class template )
negate			Negative function object class (class template )
not_equal_to	Function object class for non-equality comparison (class template )
plus			Addition function object class (class template )


Other classes
bad_function_call	Exception thrown on bad call (class )
hash				Default hash function object class (class template )
is_bind_expression	Is bind expression (class template )
is_placeholder		Is placeholder (class template )

Namespaces
placeholders		Bind argument placeholders (namespace )
*/

namespace mstd {

	template<typename Arg, typename Result>
	struct unary_function {
		typedef Arg arguement_type;
		typedef Result result_type;
	};

	template<typename Arg1, typename Arg2, typename Result >
	struct binary_function {
		typedef Arg1 first_arguement_type;
		typedef Arg2 second_arguement_type;
		typedef Result result_type;
	};

	// Operator classes :
	// Operator classes are classes that define functional objects that call operators :
	// 
	// 算术类仿函数
	template<typename Tp = void>
	struct plus;

	template<typename Tp>
	struct plus : public binary_function<Tp, Tp, Tp> {
		Tp operator()(const Tp& left, const Tp& right) const {
			return left + right;
		}
	};

	template<>
	struct plus<void> {  // plus<>{} 默认模板参数 调用operator()会发生参数推导，没有binary_function里面的typedef成员
		template<typename Tp, typename Up>
		auto operator()(Tp&& left, Up&& right) const
			noexcept(noexcept(std::forward<Tp>(left) + std::forward<Up>(right)))
			->decltype(std::forward<Tp>(left) + std::forward<Up>(right))
		{
			return left + right;
		}
	};

	template<typename Tp>
	struct minus : public binary_function<Tp, Tp, Tp> {
		Tp operator()(const Tp& left, const Tp& right) const {
			return left - right;
		}
	};

	template<typename Tp>
	struct multiplies : public binary_function<Tp, Tp, Tp> {
		Tp operator()(const Tp& left, const Tp& right) const {
			return left * right;
		}
	};

	template<typename Tp>
	struct divides : public binary_function<Tp, Tp, Tp> {
		Tp operator()(const Tp& left, const Tp& right) const {
			return left / right;
		}
	};

	template<typename Tp>
	struct modulus : public binary_function<Tp, Tp, Tp> {
		Tp operator()(const Tp& left, const Tp& right) const {
			return left % right;
		}
	};

	template<typename Tp>
	struct negate : public unary_function<Tp, Tp> {
		Tp operator()(const Tp& other) const {
			return -other;
		}
	};

	template<typename Tp>
	struct bit_and : public binary_function<Tp, Tp, Tp> {
		Tp operator()(const Tp& left, const Tp& right) const {
			return left & right;
		}
	};

	template<typename Tp>
	struct bit_or : public binary_function<Tp, Tp, Tp> {
		Tp operator()(const Tp& left, const Tp& right) const {
			return left | right;
		}
	};

	template<typename Tp>
	struct bit_not : public unary_function<Tp, Tp> {
		Tp operator()(const Tp& other) const {
			return ~other;
		}
	};

	// 关系类仿函数
	template<typename Tp>
	struct equal_to : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left == right;
		}
	};

	template<typename Tp>
	struct not_equal_to : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left != right;
		}
	};

	template<typename Tp>
	struct greater : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left > right;
		}
	};

	template<typename Tp>
	struct less : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left < right;
		}
	};

	template<typename Tp>
	struct greater_equal : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left >= right;
		}
	};

	template<typename Tp>
	struct less_equal : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left <= right;
		}
	};

	// 逻辑运算类仿函数
	template<typename Tp>
	struct logical_and : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left && right;
		}
	};

	template<typename Tp>
	struct logical_or : public binary_function<Tp, Tp, bool> {
		bool operator()(const Tp& left, const Tp& right) const {
			return left || right;
		}
	};

	template<typename Tp>
	struct logical_not : public unary_function<Tp, bool> {
		bool operator()(const Tp& left) const {
			return !left;
		}
	};

	// Classes
	// Wrapper classes :
	template<typename Predicate>
	class unary_negate
		: public unary_function<typename Predicate::argument_type, bool>
	{
	protected:
		Predicate m_pred;
	public:
		explicit unary_negate(const Predicate& pred) : m_pred(pred) {}
		bool operator()(const typename Predicate::argument_type& left) const {
			return !m_pred(left);
		}
	};

	template<typename Predicate>
	inline unary_negate<Predicate> not1(const Predicate& pred) {
		return unary_negate<Predicate>(pred);
	}

	template<typename Predicate>
	class binary_negate
		: public binary_function<typename Predicate::first_argument_type,
		typename Predicate::second_argument_type, bool>
	{
	protected:
		Predicate m_pred;
	public:
		explicit binary_negate(const Predicate& pred) : m_pred(pred) {}
		bool operator()(const typename Predicate::first_argument_type& left,
			const typename Predicate::second_argument_type& right) const {
			return !m_pred(left, right);
		}
	};

	template<typename Predicate>
	inline binary_negate<Predicate> not2(const Predicate& pred) {
		return binary_negate<Predicate>(pred);
	}

	/*

	template<typename Ret, typename Tp>
	class mem_fun_t : public unary_function<Tp*, Ret> {
	private:
		Ret(Tp::* m_f)();
	public:
		explicit mem_fun_t(Ret(Tp::* pf)()) : m_f(pf) {}
		Ret operator()(Tp* p) const {
			return p->*m_f();
		}
	};

	template<typename Ret, typename Tp>
	class const_mem_fun_t : public unary_function<const Tp*, Ret> {
	private:
		Ret(Tp::* m_f)() const;
	public:
		explicit const_mem_fun_t(Ret(Tp::* pf)() const) : m_f(pf) {}
		Ret operator()(const Tp* p) const {
			return p->*m_f();
		}
	};

	template<typename Ret, typename Tp>
	class mem_fun_ref_t : public binary_function<Tp, Ret> {
	private:
		Ret(Tp::* m_f)();
	public:
		explicit mem_fun_t(Ret(Tp::* pf)()) : m_f(pf) {}
		Ret operator()(Tp& r) const {
			return r.*m_f();
		}
	};

	template<typename Ret, typename Tp>
	class const_mem_fun_ref_t : public binary_function<Tp, Ret> {
	private:
		Ret(Tp::* m_f)() const;
	public:
		explicit mem_fun_t(Ret(Tp::* pf)() const) : m_f(pf) {}
		Ret operator()(const Tp& r) const {
			return r.*m_f();
		}
	};

	template<typename Ret, typename Tp, typename Arg>
	class mem_fun1_t : public binary_function<Tp*, Arg, Ret> {
	private:
		Ret(Tp::* m_f)(Arg);
	public:
		explicit mem_fun1_t(Ret(Tp::* pf)(Arg)) : m_f(pf) {}
		Ret operator()(Tp* p, Arg left) const {
			return p->*m_f(x);
		}
	};

	template<typename Ret, typename Tp, typename Arg>
	class const_mem_fun1_t : public binary_function<const Tp*, Arg, Ret> {
	private:
		Ret(Tp::* m_f)(Arg) const;
	public:
		explicit mem_fun1_t(Ret(Tp::* pf)(Arg) const) : m_f(pf) {}
		Ret operator()(const Tp* p, Arg x) const {
			return p->*m_f(x);
		}
	};

	template<typename Ret, typename Tp, typename Arg>
	class mem_fun1_ref_t : public binary_function<Tp, Arg, Ret> {
	private:
		Ret(Tp::* m_f)(Arg);
	public:
		explicit mem_fun1_t(Ret(Tp::* pf)(Arg)) : m_f(pf) {}
		Ret operator()(Tp& r, Arg x) const {
			return r.*m_f(x);
		}
	};

	template<typename Ret, typename Tp, typename Arg>
	class const_mem_fun1_ref_t : public binary_function<Tp, Arg, Ret> {
	private:
		Ret(Tp::* m_f)(Arg) const;
	public:
		explicit mem_fun1_t(Ret(Tp::* pf)(Arg) const) : m_f(pf) {}
		Ret operator()(const Tp& r, Arg x) const {
			return r.*m_f(x);
		}
	};

	template<typename Ret, typename Tp>
	inline mem_fun_t<Ret, Tp> mem_fun(Ret(Tp::* p_f)()) {
		return mem_fun_t<Ret, Tp>(p_f);
	}

	template<typename Ret, typename Tp>
	inline const_mem_fun_t<Ret, Tp> mem_fun(Ret(Tp::* p_f)() const) {
		return const_mem_fun_t<Ret, Tp>(p_f);
	}

	template<typename Ret, typename Tp>
	inline mem_fun_ref_t<Ret, Tp> mem_fun_ref(Ret(Tp::* p_f)()) {
		return mem_fun_t<Ret, Tp>(p_f);
	}

	template<typename Ret, typename Tp>
	inline const_mem_fun_ref_t<Ret, Tp> mem_fun_ref(Ret(Tp::* p_f)() const) {
		return const_mem_fun_t<Ret, Tp>(p_f);
	}

	template<typename Ret, typename Tp, typename Arg>
	inline mem_fun1_t<Ret, Tp, Arg> mem_fun(Ret(Tp::* p_f)(Arg)) {
		return mem_fun1_t<Ret, Tp, Arg>(p_f);
	}

	template<typename Ret, typename Tp, typename Arg>
	inline const_mem_fun1_t<Ret, Tp, Arg> mem_fun(Ret(Tp::* p_f)(Arg) const) {
		return const_mem_fun1_t<Ret, Tp, Arg>(p_f);
	}

	template<typename Ret, typename Tp, typename Arg>
	inline mem_fun1_ref_t<Ret, Tp, Arg> mem_fun(Ret(Tp::* p_f)(Arg)) {
		return mem_fun1_ref_t<Ret, Tp, Arg>(p_f);
	}

	template<typename Ret, typename Tp, typename Arg>
	inline const_mem_fun1_ref_t<Ret, Tp, Arg> mem_fun(Ret(Tp::* p_f)(Arg) const) {
		return const_mem_fun1_ref_t<Ret, Tp, Arg>(p_f);
	}

	*/



} //! namespace mstd
