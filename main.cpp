#include <list>
#include <array>
#include <vector>
#include <deque>
#include <queue>	// priority_queue
#include <map>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <forward_list>

#include <utility>
#include <memory>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>

using namespace std;

template<typename Tp>
void print_container(const Tp& cont) {
	for (auto const& elem : cont) {
		cout << elem << " ";
	}
	cout << endl;
}

struct Str {
	string* ptr;
	Str() : ptr(new string("mstring")) {
		cout << *ptr << " default constructor is called" << endl;
	}
	Str(const char* str) : ptr(new string(str)) {
		cout << *ptr << " const char* constructor is called" << endl;
	}
	Str(const Str& str) : ptr(new string(*str.ptr)) {
		cout << *ptr << " const MString& constructor is called" << endl;
	}
	Str(Str&& right) noexcept {
		ptr = right.ptr;
		right.ptr = nullptr;
		cout << *ptr << " move constructor is called" << endl;
	}
	Str& operator=(const Str& str) {
		if (this != &str) {
			Str temp(str);
			std::swap(temp.ptr, ptr);
			cout << *ptr << " copy assignment operator is called" << endl;
		}
		return *this;
	}
	Str& operator=(Str&& right) noexcept {
		if (this != &right) {
			std::swap(right.ptr, ptr);
			right.~Str();
			right.ptr = nullptr;
			cout << *ptr << " move assignment operator is called" << endl;
		}
		return *this;
	}
	~Str() {
		if (ptr) {
			cout << *ptr << " destructor is called" << endl;
			delete ptr;
			ptr = nullptr;
		}
	}
};

inline bool operator<(const Str& left, const Str& right) noexcept {
	return *left.ptr < *right.ptr;
}

inline bool operator==(const Str& left, const Str& right) noexcept {
	return *left.ptr == *right.ptr;
}

ostream& operator<<(ostream& os, const Str& str) {
	if (str.ptr) os << *(str.ptr);
	return os;
}

#include "m_vector.h"
#include "m_algorithm.h"

void test_vector() {
#define stl mstd

	stl::vector<Str> v;
	stl::vector<Str> v1(3);
	stl::vector<Str> v2(4, "abcd");
	stl::vector<Str> v3{ "abc","bcd","cdf" };
	print_container(v1);
	print_container(v2);
	print_container(v3);
	v = stl::vector<Str>(std::move(v2));
	print_container(v);
	v = { "a","b","c" };
	print_container(v);
	cout << *v.begin() << endl;
	try {
		cout << v.at(8) << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	v.assign(v3.begin(), v3.end());
	print_container(v);
	v.assign(3, "a");
	print_container(v);
	v.assign({ "b","b","b" });
	print_container(v);
	v.emplace_back("c");
	print_container(v);
	v.erase(v.begin());
	print_container(v);
	v.erase(v.begin(), v.begin() + 1);
	print_container(v);
	v.push_back(*v3.begin());
	v.push_back(Str{ "fkslsd" });
	print_container(v);
	v.pop_back();
	print_container(v);
	cout << v.size() << endl;
	v.emplace(v.begin(), Str{ "fkslsd" });
	cout << v.size() << endl;
	print_container(v);
	v.emplace(v.begin() + 1, "123");
	print_container(v);
	v.insert(v.begin(), "ttt");
	print_container(v);
	auto iter = v.insert(v.begin(), Str{ "yyy" });
	iter = v.insert(iter, "ggg");
	print_container(v);
	iter = v.insert(iter, { "uu","ii","oo" });
	print_container(v);
	v.insert(iter, 2, "11");
	print_container(v);
	v.resize(20);
	print_container(v);
	v.resize(3);
	print_container(v);
	v.clear();
	print_container(v);
	//try {
	//	v.insert(v.begin() + 2, v3.begin(), v3.end());
	//}
	//catch (exception e) {
	//	cout << e.what() << endl;
	//}
	v.emplace_back("111");
	v.insert(v.begin(), v3.begin(), v3.end());
	print_container(v);
	cout << v[3] << endl;
	stl::swap(v, v3);
	print_container(v);

	stl::vector<int> vi1{ 1,2,3,4 };
	stl::vector<int> vi2{ 2,3, };
	cout << (vi1 < vi2) << endl;
	cout << (vi1 > vi2) << endl;

	stl::vector<int> vi;
	for (size_t i = 0; i < 10; ++i) {
		vi.push_back(i);
	}

	for (auto iter = vi.crbegin(); iter != vi.crend(); ++iter) {
		cout << *iter << endl;
	}
	cout << vi.size() << " " << vi.capacity() << endl;
	vi.shrink_to_fit();
	cout << vi.size() << " " << vi.capacity() << endl;

#undef stl
}

#include "m_list.h"

void test_list() {
#define stl mstd
	stl::list<Str> mstr1{};
	stl::list<Str> mstr2(2);
	stl::list<Str> mstr3(3, "hi");
	stl::vector<Str> vstr{ "hello","good" };
	stl::list<Str> mstr4(vstr.begin(), vstr.end());
	mstr1 = mstr2;
	print_container(mstr1);
	print_container(mstr2);
	print_container(mstr3);
	print_container(mstr4);
	mstr1 = mstd::move(mstr3);
	print_container(mstr1);
	stl::list<Str> mstr5 = stl::list<Str>({ "tt","aa","bb" });
	print_container(mstr5);
	mstr5.push_back("vv");
	print_container(mstr5);
	mstr5.push_front("ss");
	print_container(mstr5);
	mstr5.pop_back();
	print_container(mstr5);
	mstr5.pop_front();
	print_container(mstr5);
	mstr5.emplace_front("ff");
	print_container(mstr5);
	mstr5.emplace_back("ll");
	print_container(mstr5);
	auto iter = mstr5.begin();
	mstr5.emplace(iter, "22");
	print_container(mstr5);
	mstr5.insert(++iter, "11");
	print_container(mstr5);
	iter = --mstr5.end();
	mstr5.erase(--iter);
	print_container(mstr5);
	iter = ++++mstr5.begin();
	mstr5.erase(mstr5.begin(), iter);
	print_container(mstr5);
	mstr5.insert(mstr5.begin(), "000");
	mstr5.insert(mstr5.begin(), vstr.begin(), vstr.end());
	print_container(mstr5);
	mstr5.resize(10);
	print_container(mstr5);
	mstr5.resize(3);
	print_container(mstr5);
	mstr5.clear();
	print_container(mstr5);
	swap(mstr5, mstr3);
	print_container(mstr5);
	mstr5.assign(4, "skt");
	print_container(mstr5);
	mstr5.splice(++mstr5.begin(), stl::list<Str>(3, "sp"));
	print_container(mstr5);
	mstr5.unique();
	print_container(mstr5);
	mstr5.remove("skt");
	print_container(mstr5);
	mstr5.insert(mstr5.end(), { "11","22","33" });
	print_container(mstr5);
	mstr5.sort();
	mstr4.sort();
	mstr5.merge(mstr4);  // 需要先sort();
	print_container(mstr5);
	cout << mstr5.size() << endl;
	cout << (mstr5 < mstr4) << endl;
#undef stl
}


void test_alignof() {
	cout << alignof(std::max_align_t) << endl;

	struct s1
	{
		char s;
		int i;
	};

	struct s2
	{
		int i;
		double d;
	};

	cout << "-------basic type" << endl;
	cout << "sizeof(char)	" << sizeof(char) << endl;
	cout << "sizeof(int)	" << sizeof(int) << endl;
	cout << "sizeof(double)	" << sizeof(double) << endl;

	cout << endl;
	cout << "-------struct" << endl;
	cout << "sizeof(s1)	" << sizeof(s1) << endl;
	cout << "sizeof(s2)	" << sizeof(s2) << endl;

	struct s3
	{
		char s;
		double d;
		int i;
	};

	struct s4
	{
		char s;
		int i;
		double d;
	};

	struct s11
	{
		alignas(16) char s;
		int i;
	};

	struct s12
	{
		alignas(16) char s;
		int i;
	};


	// alignof
	cout << "-------------------alignof---------------------" << endl;
	// 基本对齐值
	cout << "alignof(std::max_align_t)	" << alignof(std::max_align_t) << endl;
	cout << endl;
	cout << "-------basic type" << endl;
	cout << "sizeof(s3)			" << sizeof(s3) << endl;
	cout << "sizeof(s4)			" << sizeof(s4) << endl;
	cout << "alignof(char)		" << alignof(char) << endl;
	cout << "alignof(int)		" << alignof(int) << endl;
	cout << "alignof(double)	" << alignof(double) << endl;

	cout << endl;
	cout << "-------struct" << endl;
	cout << "alignof(s1)		" << alignof(s1) << endl;
	cout << "alignof(s2)		" << alignof(s2) << endl;
	cout << "alignof(s3)		" << alignof(s3) << endl;

	cout << endl;
	cout << endl;

	// alignas
	cout << "-------------------alignas---------------------" << endl;
	cout << "alignof(s1)		" << alignof(s1) << endl;
	cout << "alignof(s11)		" << alignof(s11) << endl;
	cout << "alignof(s12)		" << alignof(s12) << endl;

	cout << "sizeof(s1)    	" << sizeof(s1) << endl;
	cout << "sizeof(s11)	" << sizeof(s11) << endl;
	cout << "sizeof(s12)	" << sizeof(s12) << endl;
}


template<class Tp = int>
struct cmp_less {
	constexpr Tp operator()(const Tp& left, const Tp& right) const {
		return left < right;
	}
};

template<>
struct cmp_less<void> {
	template<class Tp1, class Tp2>
	constexpr auto operator()(Tp1&& left, Tp2&& right) const
		-> decltype(std::forward<Tp1>(left) < std::forward<Tp2>(right)) {
		return std::forward<Tp1>(left) < std::forward<Tp2>(right);
	}
};

template<>
struct cmp_less<int> {
	constexpr int operator()(int left, int right) const {
		return left < right;
	}
};

template<class Tp>
ostream& print(ostream& os, const Tp& obj) {
	os << obj << endl;
	return os;
}

#include "m_type_traits.h"



struct Test {


	Test() = default;


	static void fun1() {}

	operator int() { return 0; }

	//Test& operator=(const Test&) { return *this; }

	bool fun2() const noexcept { return true; }


};

#include "m_iterator.h"

void test_mstd_verify() {
	int a{ 1 };
	int b{ 2 };
	_MSTD_VERIFY(a == b, "value is not equal");
}


struct A {
	A() = default;
	A(const A& a) noexcept(false) {

	}
	Test a_{};
};


template<class Tp>
struct test_type {
	using type = Tp;
};

template<class Tp>
struct test_type<Tp&&> {
	using type = Tp;
};

template<class Tp>
struct test_type<Tp&> {
	using type = Tp;
};

int main() {
	//test_vector();
	//mstd::vector<MString>::iterator it;
	//cout << std::is_same_v<MString, std::remove_reference_t<decltype(*it)>> << endl;
	//cout << std::is_same_v<std::true_type, std::is_trivially_destructible<decltype(*it)>::type> << endl;
	//mstd::vector<int> v1{ 1,2,3,4 };
	//mstd::vector<int> v2{ 2,3 };
	//v1.resize(6);
	//mstd::copy_backward(v1.begin(), v1.begin() + 4, v1.end());
	//print_container(v1);
	//cout << std::is_pod_v<const int> << endl;
	//cout << std::is_pod_v<volatile const int> << endl;
	//cout << std::is_pod_v<const int*> << endl;
	//cout << std::is_pod_v<const int&> << endl;
	/*vector<int> v;
	v.insert(v.end(), 2);
	print_container(v);*/
	//std::vector<int> vi{ 1,2,4 };
	//std::vector<int>::const_iterator iter;
	//iter = vi.begin();
	//cout << *iter << endl;

	/*int* a = (int*)mstd::malloc_allocator<0>::allocate(4);
	mstd::construct(a);
	cout << *a << endl;
	mstd::malloc_allocator<0>::deallocate(a, 4);*/

	//test_list();

	//int* ptr = new int{};
	//cout << ptr << endl;
	//cout << &(ptr[-1]) << endl;

	//cout << sizeof(void*) << endl;

	/*int a1 = 2;
	int a2 = 3;
	double b = 2.5;
	int* c = nullptr;
	cout << cmp_less<>{}(a1, b) << endl;*/

	//ofstream f("output");
	//print(cout, 42);
	//print(f, 10);

	//cout << std::is_same_v<const int* const, 
	//	typename mstd::remove_reference<const int* const&>::type> << endl;

	//auto f1 = &Test::fun1;
	//Test t{};
	//auto f2 = &Test::fun2;
	//auto f3 = &Test::operator=;
	//auto f4 = &Test::operator int;

	//cout << std::is_member_function_pointer_v<decltype(f1)> << endl;
	//cout << std::is_function_v<decltype(f1)> << endl;
	//cout << std::is_pointer_v<decltype(f1)> << endl;

	//cout << std::is_member_function_pointer_v<decltype(f2)> << endl;
	//cout << std::is_member_function_pointer_v<decltype(f3)> << endl;
	//cout << std::is_member_function_pointer_v<decltype(f4)> << endl;

	//cout << mstd::is_pod<Test>::value << endl;

	//cout << std::is_trivial<Test>::value << endl;

	//cout << std::is_trivially_copyable<Test>::value << endl;
	//cout << std::is_constructible<Test, int>::value << endl;
	//cout << std::is_trivially_constructible<Test>::value << endl;
	//cout << std::is_trivially_constructible<Test, const Test&>::value << endl;
	//cout << std::is_trivially_constructible<Test, Test&&>::value << endl;

	//cout << std::is_trivial<Test>::value << endl;

	//cout << std::is_nothrow_default_constructible<Test>::value << endl;

	//cout << std::is_same_v<int&, std::add_rvalue_reference_t<int&>> << endl;

	//std::vector<int> v1{ 1,2,3,4 };
	//std::vector<int> v2{ 5,6,7 };
	//for_each(v1.begin(), v2.end(), [](const int elem) {cout << elem << endl; });

	//test_mstd_verify();

	int a{ 1 };

	const int b = std::move(a);
	cout << b << endl;
	cout << a << endl;



}