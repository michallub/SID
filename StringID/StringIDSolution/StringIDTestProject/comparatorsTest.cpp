#include "pch.h"

#include "../../StringIDSolution/SIDlib/comparators.h"
#include <vector>
#include <utility>
#include <typeinfo>
#include <random>

template<typename Type1>
void testComparatorsHelperCompNull(const Type1& a)
{
	EXPECT_EQ(typeid(CMPNL), typeid(CompNull<true>));
	EXPECT_EQ(typeid(CMPNB), typeid(CompNull<false>));
	EXPECT_EQ(typeid(CMPN), typeid(CompNull<CompIndirect_DEFAULT_LOGIC_OP>));

	EXPECT_EQ(typeid(CMPNL > a), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(CMPNB > a), typeid(CompValue<Type1, false>));
	EXPECT_EQ(typeid(a > CMPNL), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(a > CMPNB), typeid(CompValue<Type1, false>));

	EXPECT_EQ(typeid(CMPNL >= a), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(CMPNB >= a), typeid(CompValue<Type1, false>));
	EXPECT_EQ(typeid(a >= CMPNL), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(a >= CMPNB), typeid(CompValue<Type1, false>));
	
	EXPECT_EQ(typeid(CMPNL < a), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(CMPNB < a), typeid(CompValue<Type1, false>));
	EXPECT_EQ(typeid(a < CMPNL), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(a < CMPNB), typeid(CompValue<Type1, false>));

	EXPECT_EQ(typeid(CMPNL <= a), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(CMPNB <= a), typeid(CompValue<Type1, false>));
	EXPECT_EQ(typeid(a <= CMPNL), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(a <= CMPNB), typeid(CompValue<Type1, false>));

	EXPECT_EQ(typeid(CMPNL == a), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(CMPNB == a), typeid(CompValue<Type1, false>));
	EXPECT_EQ(typeid(a == CMPNL), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(a == CMPNB), typeid(CompValue<Type1, false>));

	EXPECT_EQ(typeid(CMPNL != a), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(CMPNB != a), typeid(CompValue<Type1, false>));
	EXPECT_EQ(typeid(a != CMPNL), typeid(CompValue<Type1, true>));
	EXPECT_EQ(typeid(a != CMPNB), typeid(CompValue<Type1, false>));

	EXPECT_EQ(bool(CMPNL > a == a), bool(a == a));
	EXPECT_EQ(bool(CMPNB > a == a), bool(a == a));
	EXPECT_EQ(bool(a > CMPNL == a), bool(a == a));
	EXPECT_EQ(bool(a > CMPNB == a), bool(a == a));

	EXPECT_EQ(bool(CMPNL >= a == a), bool(a == a));
	EXPECT_EQ(bool(CMPNB >= a == a), bool(a == a));
	EXPECT_EQ(bool(a >= CMPNL == a), bool(a == a));
	EXPECT_EQ(bool(a >= CMPNB == a), bool(a == a));

	EXPECT_EQ(bool(CMPNL < a == a), bool(a == a));
	EXPECT_EQ(bool(CMPNB < a == a), bool(a == a));
	EXPECT_EQ(bool(a < CMPNL == a), bool(a == a));
	EXPECT_EQ(bool(a < CMPNB == a), bool(a == a));

	EXPECT_EQ(bool(CMPNL <= a == a), bool(a == a));
	EXPECT_EQ(bool(CMPNB <= a == a), bool(a == a));
	EXPECT_EQ(bool(a <= CMPNL == a), bool(a == a));
	EXPECT_EQ(bool(a <= CMPNB == a), bool(a == a));

	EXPECT_EQ(bool(CMPNL == a == a), bool(a == a));
	EXPECT_EQ(bool(CMPNB == a == a), bool(a == a));
	EXPECT_EQ(bool(a == CMPNL == a), bool(a == a));
	EXPECT_EQ(bool(a == CMPNB == a), bool(a == a));
	
	EXPECT_EQ(bool(CMPNL != a == a), bool(a == a));
	EXPECT_EQ(bool(CMPNB != a == a), bool(a == a));
	EXPECT_EQ(bool(a != CMPNL == a), bool(a == a));
	EXPECT_EQ(bool(a != CMPNB == a), bool(a == a));
}

template<typename Type1A, typename Type2A, typename Type1B, typename Type2B>
void testAllCompOps(const Type1A& a1, const Type2A& a2, const Type1B& b1, const Type2B& b2)
{
	EXPECT_EQ(bool(a1 <  a2), bool(b1 < b2));
	EXPECT_EQ(bool(a1 <= a2), bool(b1 <= b2));
	EXPECT_EQ(bool(a1 > a2), bool(b1 > b2));
	EXPECT_EQ(bool(a1 >= a2), bool(b1 >= b2));
	EXPECT_EQ(bool(a1 == a2), bool(b1 == b2));
	EXPECT_EQ(bool(a1 != a2), bool(b1 != b2));
}

template<typename Type1A, typename Type2A>
void testAllCompOps(const Type1A& a1, const Type2A& a2, bool result)
{
	EXPECT_EQ(bool(a1 < a2), result);
	EXPECT_EQ(bool(a1 <= a2), result);
	EXPECT_EQ(bool(a1 > a2), result);
	EXPECT_EQ(bool(a1 >= a2), result);
	EXPECT_EQ(bool(a1 == a2), result);
	EXPECT_EQ(bool(a1 != a2), result);
}

template <typename Type1, typename Type2, bool logicand>
void compareCompIndirects(const CompIndirect<Type1, Type2, logicand> &a, const CompIndirect<Type1, Type2, logicand> &b)
{
	EXPECT_EQ(a.left, b.left);
	EXPECT_EQ(a.right, b.right);
	EXPECT_EQ(a.result, b.result);
	EXPECT_EQ(bool(a), bool(b));
	//EXPECT_EQ(a.logicAnd(), b.logicAnd());
}

template <typename Type, bool logicand>
void compareCompValues(const CompValue<Type, logicand> &a, const CompValue<Type, logicand> &b)
{
	EXPECT_EQ(a.value, b.value);
	//EXPECT_EQ(a.logicAnd(), b.logicAnd());
}

template <typename A, typename B, typename AB, typename BA>
void compareCompIndirectCompNullAllOps(const A& a, const B& b, const AB& ab, const BA& ba)
{
	compareCompIndirects((a < b), ab);
	compareCompIndirects((a <= b), ab);
	compareCompIndirects((a > b), ab);
	compareCompIndirects((a >= b), ab);
	compareCompIndirects((a == b), ab);
	compareCompIndirects((a != b), ab);
	
	compareCompIndirects((b < a), ba);
	compareCompIndirects((b <= a), ba);
	compareCompIndirects((b > a), ba);
	compareCompIndirects((b >= a), ba);
	compareCompIndirects((b == a), ba);
	compareCompIndirects((b != a), ba);
}
template <typename A, typename B, typename AB, typename BA>
void compareCompValueCompNullAllOps(const A& a, const B& b, const AB& ab, const BA& ba)
{
	compareCompValues((a < b), ab);
	compareCompValues((a <= b), ab);
	compareCompValues((a > b), ab);
	compareCompValues((a >= b), ab);
	compareCompValues((a == b), ab);
	compareCompValues((a != b), ab);

	compareCompValues((b < a), ba);
	compareCompValues((b <= a), ba);
	compareCompValues((b > a), ba);
	compareCompValues((b >= a), ba);
	compareCompValues((b == a), ba);
	compareCompValues((b != a), ba);
}


template<typename Type1, typename Type2>
void testComparatorsHelper(const std::vector<std::pair<Type1, Type2>>& testvalues)
{
	for (const auto& e : testvalues)
	{
		const Type1& a = e.first;
		const Type2& b = e.second;

		testComparatorsHelperCompNull(a);
		testComparatorsHelperCompNull(b);

		EXPECT_EQ(typeid(CMPL(a)), typeid(CompValue<Type1, true>));
		EXPECT_EQ(typeid(CMPL(b)), typeid(CompValue<Type2, true>));
		EXPECT_EQ(typeid(CMPB(a)), typeid(CompValue<Type1, false>));
		EXPECT_EQ(typeid(CMPB(b)), typeid(CompValue<Type2, false>));
		EXPECT_EQ(typeid(CMP(a)), typeid(CompValue<Type1, CompIndirect_DEFAULT_LOGIC_OP>));
		EXPECT_EQ(typeid(CMP(b)), typeid(CompValue<Type2, CompIndirect_DEFAULT_LOGIC_OP>));

		EXPECT_TRUE(CMPL(a).logicAnd());
		EXPECT_TRUE(CMPL(b).logicAnd());
		EXPECT_FALSE(CMPB(a).logicAnd());
		EXPECT_FALSE(CMPB(b).logicAnd());
		EXPECT_EQ(CMP(a).logicAnd(), CompIndirect_DEFAULT_LOGIC_OP);
		EXPECT_EQ(CMP(b).logicAnd(), CompIndirect_DEFAULT_LOGIC_OP);

		EXPECT_TRUE(CMPL(a) == a);
		EXPECT_TRUE(CMPL(b) == b);
		EXPECT_TRUE(CMPB(a) == a);
		EXPECT_TRUE(CMPB(b) == b);
		EXPECT_TRUE(CMP(a) == a);
		EXPECT_TRUE(CMP(b) == b);


		testAllCompOps(CMPL(a), b, a, b);
		testAllCompOps(a, CMPL(b), a, b);
		testAllCompOps(CMPL(a), CMPL(b), a, b);

		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, true)  , b, a, b);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, true) , b, a, b);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, false) , b, false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, false), b, false);

		testAllCompOps(a, CompIndirect<Type2, Type1, true>(b, a, true), a, b);
		testAllCompOps(a, CompIndirect<Type2, Type1, false>(b, a, true), a, b);
		testAllCompOps(a, CompIndirect<Type2, Type1, true>(b, a, false), false);
		testAllCompOps(a, CompIndirect<Type2, Type1, false>(b, a, false), false);

		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, true),   CMPL(b), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, true),  CMPL(b), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, false),  CMPL(b), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, false), CMPL(b), false);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, true),   CMPB(b), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, true),  CMPB(b), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, false),  CMPB(b), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, false), CMPB(b), false);

		testAllCompOps(CMPL(a), CompIndirect<Type2, Type1, true>(b, a, true), a, b);
		testAllCompOps(CMPL(a), CompIndirect<Type2, Type1, false>(b, a, true), a, b);
		testAllCompOps(CMPL(a), CompIndirect<Type2, Type1, true>(b, a, false), false);
		testAllCompOps(CMPL(a), CompIndirect<Type2, Type1, false>(b, a, false), false);
		testAllCompOps(CMPB(a), CompIndirect<Type2, Type1, true>(b, a, true), a, b);
		testAllCompOps(CMPB(a), CompIndirect<Type2, Type1, false>(b, a, true), a, b);
		testAllCompOps(CMPB(a), CompIndirect<Type2, Type1, true>(b, a, false), false);
		testAllCompOps(CMPB(a), CompIndirect<Type2, Type1, false>(b, a, false), false);

		compareCompValueCompNullAllOps(CompValue<Type1, true>(a), CMPNL, CompValue<Type1, true>(a), CompValue<Type1, true>(a));
		compareCompValueCompNullAllOps(CompValue<Type1, true>(a), CMPNB, CompValue<Type1, true>(a), CompValue<Type1, false>(a));
		compareCompValueCompNullAllOps(CompValue<Type1, false>(a), CMPNL, CompValue<Type1, false>(a), CompValue<Type1, true>(a));
		compareCompValueCompNullAllOps(CompValue<Type1, false>(a), CMPNB, CompValue<Type1, false>(a), CompValue<Type1, false>(a));
		compareCompValueCompNullAllOps(CompValue<Type1, true>(b), CMPNL, CompValue<Type1, true>(b), CompValue<Type1, true>(b));
		compareCompValueCompNullAllOps(CompValue<Type1, true>(b), CMPNB, CompValue<Type1, true>(b), CompValue<Type1, false>(b));
		compareCompValueCompNullAllOps(CompValue<Type1, false>(b), CMPNL, CompValue<Type1, false>(b), CompValue<Type1, true>(b));
		compareCompValueCompNullAllOps(CompValue<Type1, false>(b), CMPNB, CompValue<Type1, false>(b), CompValue<Type1, false>(b));

		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, true>(b, a, true), CMPNL, CompIndirect<Type2, Type1, true>(b, a, true), CompIndirect<Type2, Type1, true>(b, a, true));
		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, true>(b, a, true), CMPNB, CompIndirect<Type2, Type1, true>(b, a, true), CompIndirect<Type2, Type1, false>(b, a, true));
		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, true>(b, a, false), CMPNL, CompIndirect<Type2, Type1, true>(b, a, false), CompIndirect<Type2, Type1, true>(b, a, false));
		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, true>(b, a, false), CMPNB, CompIndirect<Type2, Type1, true>(b, a, false), CompIndirect<Type2, Type1, false>(b, a, false));
		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, false>(b, a, true), CMPNL, CompIndirect<Type2, Type1, false>(b, a, true), CompIndirect<Type2, Type1, true>(b, a, true));
		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, false>(b, a, true), CMPNB, CompIndirect<Type2, Type1, false>(b, a, true), CompIndirect<Type2, Type1, false>(b, a, true));
		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, false>(b, a, false), CMPNL, CompIndirect<Type2, Type1, false>(b, a, false), CompIndirect<Type2, Type1, true>(b, a, false));
		compareCompIndirectCompNullAllOps(CompIndirect<Type2, Type1, false>(b, a, false), CMPNB, CompIndirect<Type2, Type1, false>(b, a, false), CompIndirect<Type2, Type1, false>(b, a, false));


		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, true),   CompIndirect<Type2, Type1, true>(b, a, true), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, true),   CompIndirect<Type2, Type1, true>(b, a, false), false);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, true),   CompIndirect<Type2, Type1, false>(b, a, true), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, true),   CompIndirect<Type2, Type1, false>(b, a, false), false);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, false),  CompIndirect<Type2, Type1, true>(b, a, true), false);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, false),  CompIndirect<Type2, Type1, true>(b, a, false), false);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, false),  CompIndirect<Type2, Type1, false>(b, a, true), false);
		testAllCompOps(CompIndirect<Type2, Type1, true>(b, a, false),  CompIndirect<Type2, Type1, false>(b, a, false), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, true),  CompIndirect<Type2, Type1, true>(b, a, true), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, true),  CompIndirect<Type2, Type1, true>(b, a, false), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, true),  CompIndirect<Type2, Type1, false>(b, a, true), a, b);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, true),  CompIndirect<Type2, Type1, false>(b, a, false), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, false), CompIndirect<Type2, Type1, true>(b, a, true), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, false), CompIndirect<Type2, Type1, true>(b, a, false), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, false), CompIndirect<Type2, Type1, false>(b, a, true), false);
		testAllCompOps(CompIndirect<Type2, Type1, false>(b, a, false), CompIndirect<Type2, Type1, false>(b, a, false), false);
	}
}


TEST(ComparatorsTestCase, CompNullCompNullTest) {
	EXPECT_EQ(typeid(CMPNL), typeid(CompNull<true>));
	EXPECT_EQ(typeid(CMPNB), typeid(CompNull<false>));
	EXPECT_EQ(typeid(CMPN), typeid(CompNull<CompIndirect_DEFAULT_LOGIC_OP>));

	EXPECT_EQ(typeid(CMPNL < CMPNL), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNL < CMPNB), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNB < CMPNL), typeid(CMPNB));
	EXPECT_EQ(typeid(CMPNB < CMPNB), typeid(CMPNB));

	EXPECT_EQ(typeid(CMPNL <= CMPNL), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNL <= CMPNB), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNB <= CMPNL), typeid(CMPNB));
	EXPECT_EQ(typeid(CMPNB <= CMPNB), typeid(CMPNB));

	EXPECT_EQ(typeid(CMPNL > CMPNL), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNL > CMPNB), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNB > CMPNL), typeid(CMPNB));
	EXPECT_EQ(typeid(CMPNB > CMPNB), typeid(CMPNB));

	EXPECT_EQ(typeid(CMPNL >= CMPNL), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNL >= CMPNB), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNB >= CMPNL), typeid(CMPNB));
	EXPECT_EQ(typeid(CMPNB >= CMPNB), typeid(CMPNB));
	
	EXPECT_EQ(typeid(CMPNL == CMPNL), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNL == CMPNB), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNB == CMPNL), typeid(CMPNB));
	EXPECT_EQ(typeid(CMPNB == CMPNB), typeid(CMPNB));

	EXPECT_EQ(typeid(CMPNL != CMPNL), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNL != CMPNB), typeid(CMPNL));
	EXPECT_EQ(typeid(CMPNB != CMPNL), typeid(CMPNB));
	EXPECT_EQ(typeid(CMPNB != CMPNB), typeid(CMPNB));
}

TEST(ComparatorsTestCase, IntIntTest) {
	std::vector<std::pair<int, int>> testvalues = 
	{
		{1, 1},
		{2, 1},
		{1, 2},
		{2, 2}
	};
	testComparatorsHelper(testvalues);
}

//TEST(ComparatorsTestCase, IntFloatTest) {
//	std::vector<std::pair<int, float>> testvalues =
//	{
//		{1, 1.f},
//		{2, 1.f},
//		{1, 2.f},
//		{2, 2.f}
//	};
//	testComparatorsHelper(testvalues);
//}


TEST(ComparatorsTestCase, IntRandomTest) {
	std::mt19937_64 rng(123);
	
	for (int i = 0; i < 1024; i++)
	{
		auto a = rng()%16;
		auto b = rng()%2+7;
		auto c = rng()%2+7;
		auto d = rng()%16;
		auto e = rng()%16;
		auto f = rng()%16;
		auto g = rng()%16;

		bool rl1 = (a >= CMPNB < b == c != d <= CMPB(e) > f >= g);
		bool rb1 = (CMPNB > a < b == c != CMPB(d) <= e > f >= g);
		bool rl2 = ((a < b) && (b == c) && (c != d) && (d <= e) && (e > f) && (f >= g));
		bool rb2 = ((a < b) & (b == c) & (c != d) & (d <= e) & (e > f) & (f >= g));
		EXPECT_EQ(rl1, rl2);
		EXPECT_EQ(rb1, rb2);
	}
}

constexpr uint64_t constexprrng(uint64_t seed)
{
	constexpr uint64_t prime = 6107546065689387577ull;
	for (int i = 0; i < 16; i++)
	{
		seed = seed * prime + 2;
		seed = (seed >> 13) | (seed << 51);
	}
	return seed;
}

constexpr bool constexprTestHelper() noexcept
{
	for (int i = 0; i < 1024; i++)
	{
		auto rval = constexprrng(i);
		auto a = rval % 16; rval /= 16;
		auto b = rval % 2 + 7; rval /= 2;
		auto c = rval % 2 + 7; rval /= 2;
		auto d = rval % 16; rval /= 16;
		auto e = rval % 16; rval /= 16;
		auto f = rval % 16; rval /= 16;
		auto g = rval % 16; rval /= 16;

		bool rl1 = (a >= CMPNB < b == c != d <= CMPB(e) > f >= g);
		bool rb1 = (CMPNB > a < b == c != CMPB(d) <= e > f >= g);
		bool rl2 = ((a < b) && (b == c) && (c != d) && (d <= e) && (e > f) && (f >= g));
		bool rb2 = ((a < b) & (b == c) & (c != d) & (d <= e) & (e > f) & (f >= g));
		if (rl1 != rl2)
			return false;
		if (rb1 != rb2)
			return false;
	}
	return true;
}

TEST(ComparatorsTestCase, ConstexprTest) {
{
		constexpr bool result = constexprTestHelper();
		EXPECT_TRUE(result);
}

