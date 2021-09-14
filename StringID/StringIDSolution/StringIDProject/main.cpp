#include <iostream>

#include "../SIDlib/comparators.h"










template<bool heavycmp>
struct TestType
{
	uint64_t value;

	constexpr int64_t compare(const TestType<heavycmp>& b) const
	{
		if constexpr (!heavycmp)
		{
			return int64_t(value - (int64_t)b.value);
		}
		else
		{
			int count = 0;
			uint64_t av = value;
			uint64_t bv = b.value;

			for (int i = 0; i < 1024; ++i)
			{
				if (av < bv) count--;
				else if (av > bv) count++;
				av = av * 257 + 1;
				bv = bv * 257 + 1;
			}
			return count;
		}
	}
};



template<bool heavycmp>
constexpr bool operator<(const TestType<heavycmp>& a, const TestType<heavycmp>& b)
{
	return a.compare(b) < 0;
}



template<bool heavycmp>
constexpr bool operator>(const TestType<heavycmp>& a, const TestType<heavycmp>& b)
{
	return a.compare(b) > 0;
}

template<bool heavycmp>
constexpr bool operator<=(const TestType<heavycmp>& a, const TestType<heavycmp>& b)
{
	return a.compare(b) <= 0;
}

template<bool heavycmp>
constexpr bool operator>=(const TestType<heavycmp>& a, const TestType<heavycmp>& b)
{
	return a.compare(b) >= 0;
}

template<bool heavycmp>
constexpr bool operator==(const TestType<heavycmp>& a, const TestType<heavycmp>& b)
{
	return a.compare(b) == 0;
}

template<bool heavycmp>
constexpr bool operator!=(const TestType<heavycmp>& a, const TestType<heavycmp>& b)
{
	return a.compare(b) != 0;
}

#include <random>
#include <chrono>

template <bool logicAnd, bool mycmp, typename Type >
double PerformanceTest(uint64_t iterations, size_t iter, size_t &sum, std::mt19937_64 &rng)
{
	auto t0 = std::chrono::steady_clock::now();
	constexpr bool  logicand = logicAnd;
	constexpr bool  myCmp = mycmp;

	Type x = { 0 };
	Type y = { 65535 };
	uint64_t num = 1;
	for (volatile size_t i = 0; i < iterations; i++)
	{
		num = rng();// (num ^ 0x1234567890ABCDEF)* ((i * iter) & 0x1);// rng1();
		Type a = { (num >> 0) & 0xFFFF };
		Type b = { (num >> 16) & 0xFFFF };
		Type c = { (num >> 32) & 0xFFFF };
		Type d = { (num >> 48) & 0xFFFF };

		if constexpr (logicAnd && mycmp)
			sum += bool(CMPL(x) < a < b < c < d < y);
		else if constexpr (!logicAnd && mycmp)
			sum += bool(CMPB(x) < a < b < c < d < y);
		else if constexpr (logicAnd && !mycmp)
			sum += bool((x < a) && (a < b) && (b < c) && (c < d) && (d < y));
		else if constexpr (!logicAnd && !mycmp)
			sum += bool((x < a) & (a < b) & (b < c) & (c < d) & (d < y));
		else
			static_assert(false, "?");
	}
	auto t1 = std::chrono::steady_clock::now();

	using namespace std::chrono_literals;
	auto d1 = std::chrono::duration<double>(t1 - t0);
	return d1.count();
}









int main()
{
	std::cout << "hello 1\n";
	//int a, b, c, d;
	//std::cin >> a >> b >> c >> d;
	//if (0 < CMP(a) < CMP(b) < CMP(c) < CMP(d) < 15)
	//	std::clog << "YES\n";
	//else
	//	std::clog << "NO\n";

	//system("pause");
	std::mt19937_64 rng(123);
	for (size_t i = 0; i < 10000000; i++)
	{
		uint64_t a = rng() % 16;
		uint64_t b = rng() % 16;
		uint64_t c = rng() % 16;
		uint64_t d = rng() % 16;
		
		bool mycmp = bool(CMP(0) <= a > b < c >= d !=15);
		bool stdcmp = bool((0 <= a) & (a > b) & (b < c) & (c >= d) & (d != 15));
		if (mycmp != stdcmp)
		{
			std::clog << "errA(" << i << "): " << a << " " << b << " " << c << " " << d << "\n";
		}
	}

	size_t sum1 = 0;
	size_t sum2 = 0;
	size_t sum3 = 0;
	size_t sum4 = 0;
	size_t sum5 = 0;
	size_t sum6 = 0;
	size_t sum7 = 0;
	size_t sum8 = 0;

	TestType<true> x = { 0 };
	TestType<true> y = { 65535 };

	TestType<true> n0 = { 0 };
	TestType<true> n1 = { 1 };

	using namespace std::chrono_literals;
	uint64_t seed = 123;
	std::cin >> seed;
	std::mt19937_64 rng1 = std::mt19937_64(seed);
	std::mt19937_64 rng2 = std::mt19937_64(seed);
	std::mt19937_64 rng3 = std::mt19937_64(seed);
	std::mt19937_64 rng4 = std::mt19937_64(seed);
	std::mt19937_64 rng5 = std::mt19937_64(seed);
	std::mt19937_64 rng6 = std::mt19937_64(seed);
	std::mt19937_64 rng7 = std::mt19937_64(seed);
	std::mt19937_64 rng8 = std::mt19937_64(seed);
	std::cout << "mycmp:logic:heavy\t";
	std::cout << "std:logic:heavy\t";
	std::cout << "mycmp:binary:heavy\t";
	std::cout << "std:binary:heavy\t";
	std::cout << "mycmp:logic:light\t";
	std::cout << "std:logic:light\t";
	std::cout << "mycmp:binary:light\t";
	std::cout << "std:binary:light\n";
	size_t firstiter = 1;
	//std::cin >> firstiter;
	for (size_t iter = firstiter; iter < 90000000000; iter+=1324512314)
	{
		auto t0 = std::chrono::steady_clock::now();
		double d1 = PerformanceTest<true, true, TestType<true>>(50000, iter, sum1, rng1);
		auto t1 = std::chrono::steady_clock::now();
		double d2 = PerformanceTest<true, false, TestType<true>>(50000, iter, sum2, rng2);
		auto t2 = std::chrono::steady_clock::now();
		double d3 = PerformanceTest<false, true, TestType<true>>(10000, iter, sum3, rng3);
		auto t3 = std::chrono::steady_clock::now();
		double d4 = PerformanceTest<false, false, TestType<true>>(10000, iter, sum4, rng4);
		auto t4 = std::chrono::steady_clock::now();
		double d5 = PerformanceTest<true, true, TestType<false>>(20000000, iter, sum5, rng5);
		auto t5 = std::chrono::steady_clock::now();
		double d6 = PerformanceTest<true, false, TestType<false>>(20000000, iter, sum6, rng6);
		auto t6 = std::chrono::steady_clock::now();
		double d7 = PerformanceTest<false, true, TestType<false>>(25000000, iter, sum7, rng7);
		auto t7 = std::chrono::steady_clock::now();
		double d8 = PerformanceTest<false, false, TestType<false>>(25000000, iter, sum8, rng8);
		auto t8 = std::chrono::steady_clock::now();

		

		//auto d1 = std::chrono::duration<double>(t1 - t0);
		//auto d2 = std::chrono::duration<double>(t2 - t1);
		//auto d3 = std::chrono::duration<double>(t3 - t2);
		//auto d4 = std::chrono::duration<double>(t4 - t3);
		//auto d5 = std::chrono::duration<double>(t5 - t4);
		//auto d6 = std::chrono::duration<double>(t6 - t5);
		//auto d7 = std::chrono::duration<double>(t7 - t6);
		//auto d8 = std::chrono::duration<double>(t8 - t7);

		std::cout
			<< (d1 < d2) << (d3 < d4) << (d5 < d6) << (d7 < d8) << "\t\t\t\t"
			<< d1 << "\t"
			<< d2 << "\t\t"
			<< d3 << "\t"
			<< d4 << "\t\t"
			<< d5 << "\t"
			<< d6 << "\t\t"
			<< d7 << "\t"
			<< d8 << "\t\t\t\t" 
			<< sum1 << "\t"
			<< sum2 << "\t"
			<< sum3 << "\t"
			<< sum4 << "\t"
			<< sum5 << "\t"
			<< sum6 << "\t"
			<< sum7 << "\t"
			<< sum8 << "\t"
			<<  "\n";
	}
}



