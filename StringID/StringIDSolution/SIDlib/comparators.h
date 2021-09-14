#ifndef COMPARATORS_H

#include<type_traits>

#ifndef CMPCONSTEXPR
	#if __cpp_constexpr >= 200704L                   // Check for a specific version of a feature
	#  define CMPCONSTEXPR constexpr
	#else
	#  define CMPCONSTEXPR inline
	#endif
#else
	#  define CMPCONSTEXPR inline
#endif // !CMPCONSTEXPR



#ifndef CompIndirect_DEFAULT_LOGIC_OP
#define CompIndirect_DEFAULT_LOGIC_OP false
#endif // !CompIndirect_DEFAULT_LOGIC_OP

template <bool LOGICAND = CompIndirect_DEFAULT_LOGIC_OP>
class CompNull
{
public:
	CMPCONSTEXPR operator bool() const noexcept { return true; }
	static CMPCONSTEXPR bool logicAnd() noexcept {return LOGICAND;}
};

CMPCONSTEXPR const CompNull<> CMPN = {};
CMPCONSTEXPR const CompNull<true> CMPNL = {};
CMPCONSTEXPR const CompNull<false> CMPNB = {};

template <typename T1, bool LOGICAND = CompIndirect_DEFAULT_LOGIC_OP>
class CompValue
{
public:
	const T1& value;

	CMPCONSTEXPR CompValue(const T1& value) :
		value(value)
	{

	}

	template<bool LOGICAND2>
	CMPCONSTEXPR CompValue(const CompValue<T1, LOGICAND2>& other) : value(other.value) {}

	CompValue &operator=(const CompValue&) = delete;


	CMPCONSTEXPR operator bool() const noexcept { return bool(value); }
	static CMPCONSTEXPR bool logicAnd() noexcept { return LOGICAND; }
};

template<typename T1, bool LOGICAND = CompIndirect_DEFAULT_LOGIC_OP>
CMPCONSTEXPR CompValue<T1, LOGICAND> CMP(const T1& value)
{
	return CompValue<T1, LOGICAND>(value);
}
template<typename T1>
CMPCONSTEXPR CompValue<T1, true> CMPL(const T1& value)
{
	return CompValue<T1, true>(value);
}
template<typename T1, bool LOGICAND = CompIndirect_DEFAULT_LOGIC_OP>
CMPCONSTEXPR CompValue<T1, false> CMPB(const T1& value)
{
	return CompValue<T1, false>(value);
}




template <typename T1, typename T2, bool LOGICAND>
class CompIndirect
{
public:
	const T1& left;
	const T2& right;
	bool result;

	CMPCONSTEXPR CompIndirect(const T1& left, const T2& right, bool result = true) noexcept :
		left(left), right(right), result(result)
	{

	}

	CompIndirect& operator=(const CompIndirect&) = delete;
	
	template<bool LOGICAND2>
	CMPCONSTEXPR CompIndirect(const CompIndirect<T1, T2, LOGICAND2>&other) : 
		left(other.left), right(other.right), result(other.result) {}

	CMPCONSTEXPR operator bool() const noexcept { return result; }
	static CMPCONSTEXPR bool logicAnd() noexcept { return LOGICAND; }
};



template <typename T>
struct isCompNull
{
	using value = std::false_type;
};

template <bool a>
struct isCompNull<CompNull<a>>
{
	using value = std::true_type;
};




template <typename T>
struct isCompValue
{
	using value = std::false_type;
};

template <typename T, bool land>
struct isCompValue<CompValue<T, land>>
{
	using value = std::true_type;
};


template <typename T>
struct isCompIndirect
{
	using value = std::false_type;
};

template <typename T1, typename T2, bool land>
struct isCompIndirect<CompIndirect<T1, T2, land>>
{
	using value = std::true_type;
};

#define ENABLEIFNOTCOMP(T) typename = std::enable_if_t<!isCompNull<T>::value::value && !isCompValue<T>::value::value && !isCompIndirect<T>::value::value>



template<bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompNull<LOGICAND1> operator<(const CompNull<LOGICAND1>& /*a*/, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompNull<LOGICAND1>();
}

template<typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator<(const CompNull<LOGICAND1>& /*a*/, const T3& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompValue<T1, LOGICAND2> operator<(const T1& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND2>(a);
}

template<typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator<(const CompNull<LOGICAND1>& /*a*/, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T1, LOGICAND1> operator<(const CompValue<T1, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND1>(a);
}

template<typename T3, typename T4, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T3, T4, LOGICAND1> operator<(const CompNull<LOGICAND1>& /*a*/, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T3, T4, LOGICAND1>(b);
}

template<typename T1, typename T2, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, LOGICAND1> operator<(const CompIndirect<T1, T2, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompIndirect<T1, T2, LOGICAND1>(a);
}

template <typename T1, typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator<(const CompValue<T1, LOGICAND1>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b.value, a.value < b.value);
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, true> operator<(const CompIndirect<T1, T2, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, true>(a.left, b.right, (a.result & b.result) && (a.right < b.left));
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, false> operator<(const CompIndirect<T1, T2, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, false>(a.left, b.right, (a.result & b.result) & (a.right < b.left));
}

template <typename T1, typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator<(const CompValue<T1, LOGICAND1>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b, a.value < b);
}

template <typename T1, typename T3, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND2> operator<(const T1& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND2>(a, b.value, a < b.value);
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator<(const T1& a, const CompIndirect<T3, T4, true>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a, b.right, (b.result) && (a < b.left));
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator<(const T1& a, const CompIndirect<T3, T4, false>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a, b.right, (b.result) & (a < b.left));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator<(const CompIndirect<T1, T2, true>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b, (a.result) && (a.right < b));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator<(const CompIndirect<T1, T2, false>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b, (a.result) & (a.right < b));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator<(const CompValue<T1, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a.value, b.right, (b.result) && (a.value < b.left));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator<(const CompValue<T1, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a.value, b.right, (b.result) & (a.value < b.left));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator<(const CompIndirect<T1, T2, true>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b.value, (a.result) && (a.right < b.value));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator<(const CompIndirect<T1, T2, false>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b.value, (a.result) & (a.right < b.value));
}







template<bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompNull<LOGICAND1> operator<=(const CompNull<LOGICAND1>& /*a*/, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompNull<LOGICAND1>();
}

template<typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator<=(const CompNull<LOGICAND1>& /*a*/, const T3& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompValue<T1, LOGICAND2> operator<=(const T1& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND2>(a);
}

template<typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator<=(const CompNull<LOGICAND1>& /*a*/, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T1, LOGICAND1> operator<=(const CompValue<T1, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND1>(a);
}

template<typename T3, typename T4, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T3, T4, LOGICAND1> operator<=(const CompNull<LOGICAND1>& /*a*/, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T3, T4, LOGICAND1>(b);
}

template<typename T1, typename T2, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, LOGICAND1> operator<=(const CompIndirect<T1, T2, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompIndirect<T1, T2, LOGICAND1>(a);
}

template <typename T1, typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator<=(const CompValue<T1, LOGICAND1>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b.value, a.value <= b.value);
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, true> operator<=(const CompIndirect<T1, T2, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, true>(a.left, b.right, (a.result & b.result) && (a.right <= b.left));
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, false> operator<=(const CompIndirect<T1, T2, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, false>(a.left, b.right, (a.result & b.result) & (a.right <= b.left));
}

template <typename T1, typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator<=(const CompValue<T1, LOGICAND1>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b, a.value <= b);
}

template <typename T1, typename T3, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND2> operator<=(const T1& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND2>(a, b.value, a <= b.value);
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator<=(const T1& a, const CompIndirect<T3, T4, true>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a, b.right, (b.result) && (a <= b.left));
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator<=(const T1& a, const CompIndirect<T3, T4, false>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a, b.right, (b.result) & (a <= b.left));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator<=(const CompIndirect<T1, T2, true>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b, (a.result) && (a.right <= b));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator<=(const CompIndirect<T1, T2, false>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b, (a.result) & (a.right <= b));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator<=(const CompValue<T1, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a.value, b.right, (b.result) && (a.value <= b.left));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator<=(const CompValue<T1, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a.value, b.right, (b.result) & (a.value <= b.left));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator<=(const CompIndirect<T1, T2, true>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b.value, (a.result) && (a.right <= b.value));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator<=(const CompIndirect<T1, T2, false>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b.value, (a.result) & (a.right <= b.value));
}



template<bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompNull<LOGICAND1> operator>(const CompNull<LOGICAND1>& /*a*/, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompNull<LOGICAND1>();
}

template<typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator>(const CompNull<LOGICAND1>& /*a*/, const T3& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompValue<T1, LOGICAND2> operator>(const T1& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND2>(a);
}

template<typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator>(const CompNull<LOGICAND1>& /*a*/, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T1, LOGICAND1> operator>(const CompValue<T1, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND1>(a);
}

template<typename T3, typename T4, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T3, T4, LOGICAND1> operator>(const CompNull<LOGICAND1>& /*a*/, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T3, T4, LOGICAND1>(b);
}

template<typename T1, typename T2, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, LOGICAND1> operator>(const CompIndirect<T1, T2, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompIndirect<T1, T2, LOGICAND1>(a);
}

template <typename T1, typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator>(const CompValue<T1, LOGICAND1>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b.value, a.value > b.value);
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, true> operator>(const CompIndirect<T1, T2, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, true>(a.left, b.right, (a.result & b.result) && (a.right > b.left));
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, false> operator>(const CompIndirect<T1, T2, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, false>(a.left, b.right, (a.result & b.result) & (a.right > b.left));
}

template <typename T1, typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator>(const CompValue<T1, LOGICAND1>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b, a.value > b);
}

template <typename T1, typename T3, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND2> operator>(const T1& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND2>(a, b.value, a > b.value);
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator>(const T1& a, const CompIndirect<T3, T4, true>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a, b.right, (b.result) && (a > b.left));
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator>(const T1& a, const CompIndirect<T3, T4, false>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a, b.right, (b.result) & (a > b.left));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator>(const CompIndirect<T1, T2, true>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b, (a.result) && (a.right > b));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator>(const CompIndirect<T1, T2, false>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b, (a.result) & (a.right > b));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator>(const CompValue<T1, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a.value, b.right, (b.result) && (a.value > b.left));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator>(const CompValue<T1, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a.value, b.right, (b.result) & (a.value > b.left));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator>(const CompIndirect<T1, T2, true>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b.value, (a.result) && (a.right > b.value));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator>(const CompIndirect<T1, T2, false>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b.value, (a.result) & (a.right > b.value));
}





template<bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompNull<LOGICAND1> operator>=(const CompNull<LOGICAND1>& /*a*/, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompNull<LOGICAND1>();
}

template<typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator>=(const CompNull<LOGICAND1>& /*a*/, const T3& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompValue<T1, LOGICAND2> operator>=(const T1& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND2>(a);
}

template<typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator>=(const CompNull<LOGICAND1>& /*a*/, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T1, LOGICAND1> operator>=(const CompValue<T1, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND1>(a);
}

template<typename T3, typename T4, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T3, T4, LOGICAND1> operator>=(const CompNull<LOGICAND1>& /*a*/, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T3, T4, LOGICAND1>(b);
}

template<typename T1, typename T2, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, LOGICAND1> operator>=(const CompIndirect<T1, T2, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompIndirect<T1, T2, LOGICAND1>(a);
}

template <typename T1, typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator>=(const CompValue<T1, LOGICAND1>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b.value, a.value >= b.value);
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, true> operator>=(const CompIndirect<T1, T2, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, true>(a.left, b.right, (a.result & b.result) && (a.right >= b.left));
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, false> operator>=(const CompIndirect<T1, T2, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, false>(a.left, b.right, (a.result & b.result) & (a.right >= b.left));
}

template <typename T1, typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator>=(const CompValue<T1, LOGICAND1>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b, a.value >= b);
}

template <typename T1, typename T3, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND2> operator>=(const T1& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND2>(a, b.value, a >= b.value);
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator>=(const T1& a, const CompIndirect<T3, T4, true>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a, b.right, (b.result) && (a >= b.left));
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator>=(const T1& a, const CompIndirect<T3, T4, false>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a, b.right, (b.result) & (a >= b.left));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator>=(const CompIndirect<T1, T2, true>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b, (a.result) && (a.right >= b));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator>=(const CompIndirect<T1, T2, false>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b, (a.result) & (a.right >= b));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator>=(const CompValue<T1, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a.value, b.right, (b.result) && (a.value >= b.left));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator>=(const CompValue<T1, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a.value, b.right, (b.result) & (a.value >= b.left));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator>=(const CompIndirect<T1, T2, true>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b.value, (a.result) && (a.right >= b.value));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator>=(const CompIndirect<T1, T2, false>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b.value, (a.result) & (a.right >= b.value));
}





template<bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompNull<LOGICAND1> operator==(const CompNull<LOGICAND1>& /*a*/, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompNull<LOGICAND1>();
}

template<typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator==(const CompNull<LOGICAND1>& /*a*/, const T3& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompValue<T1, LOGICAND2> operator==(const T1& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND2>(a);
}

template<typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator==(const CompNull<LOGICAND1>& /*a*/, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T1, LOGICAND1> operator==(const CompValue<T1, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND1>(a);
}

template<typename T3, typename T4, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T3, T4, LOGICAND1> operator==(const CompNull<LOGICAND1>& /*a*/, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T3, T4, LOGICAND1>(b);
}

template<typename T1, typename T2, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, LOGICAND1> operator==(const CompIndirect<T1, T2, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompIndirect<T1, T2, LOGICAND1>(a);
}

template <typename T1, typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator==(const CompValue<T1, LOGICAND1>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b.value, a.value == b.value);
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, true> operator==(const CompIndirect<T1, T2, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, true>(a.left, b.right, (a.result & b.result) && (a.right == b.left));
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, false> operator==(const CompIndirect<T1, T2, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, false>(a.left, b.right, (a.result & b.result) & (a.right == b.left));
}

template <typename T1, typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator==(const CompValue<T1, LOGICAND1>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b, a.value == b);
}

template <typename T1, typename T3, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND2> operator==(const T1& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND2>(a, b.value, a == b.value);
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator==(const T1& a, const CompIndirect<T3, T4, true>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a, b.right, (b.result) && (a == b.left));
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator==(const T1& a, const CompIndirect<T3, T4, false>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a, b.right, (b.result) & (a == b.left));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator==(const CompIndirect<T1, T2, true>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b, (a.result) && (a.right == b));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator==(const CompIndirect<T1, T2, false>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b, (a.result) & (a.right == b));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator==(const CompValue<T1, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a.value, b.right, (b.result) && (a.value == b.left));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator==(const CompValue<T1, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a.value, b.right, (b.result) & (a.value == b.left));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator==(const CompIndirect<T1, T2, true>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b.value, (a.result) && (a.right == b.value));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator==(const CompIndirect<T1, T2, false>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b.value, (a.result) & (a.right == b.value));
}




template<bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompNull<LOGICAND1> operator!=(const CompNull<LOGICAND1>& /*a*/, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompNull<LOGICAND1>();
}

template<typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator!=(const CompNull<LOGICAND1>& /*a*/, const T3& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompValue<T1, LOGICAND2> operator!=(const T1& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND2>(a);
}

template<typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T3, LOGICAND1> operator!=(const CompNull<LOGICAND1>& /*a*/, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompValue<T3, LOGICAND1>(b);
}

template<typename T1, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompValue<T1, LOGICAND1> operator!=(const CompValue<T1, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompValue<T1, LOGICAND1>(a);
}

template<typename T3, typename T4, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T3, T4, LOGICAND1> operator!=(const CompNull<LOGICAND1>& /*a*/, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T3, T4, LOGICAND1>(b);
}

template<typename T1, typename T2, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, LOGICAND1> operator!=(const CompIndirect<T1, T2, LOGICAND1>& a, const CompNull<LOGICAND2>& /*b*/) noexcept
{
	return CompIndirect<T1, T2, LOGICAND1>(a);
}

template <typename T1, typename T3, bool LOGICAND1, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator!=(const CompValue<T1, LOGICAND1>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b.value, a.value != b.value);
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, true> operator!=(const CompIndirect<T1, T2, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, true>(a.left, b.right, (a.result & b.result) && (a.right != b.left));
}

template <typename T1, typename T2, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T2, false> operator!=(const CompIndirect<T1, T2, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T2, false>(a.left, b.right, (a.result & b.result) & (a.right != b.left));
}

template <typename T1, typename T3, bool LOGICAND1, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND1> operator!=(const CompValue<T1, LOGICAND1>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND1>(a.value, b, a.value != b);
}

template <typename T1, typename T3, bool LOGICAND2, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T3, LOGICAND2> operator!=(const T1& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, LOGICAND2>(a, b.value, a != b.value);
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator!=(const T1& a, const CompIndirect<T3, T4, true>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a, b.right, (b.result) && (a != b.left));
}

template <typename T1, typename T3, typename T4, ENABLEIFNOTCOMP(T1)>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator!=(const T1& a, const CompIndirect<T3, T4, false>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a, b.right, (b.result) & (a != b.left));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator!=(const CompIndirect<T1, T2, true>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b, (a.result) && (a.right != b));
}

template <typename T1, typename T2, typename T3, ENABLEIFNOTCOMP(T3)>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator!=(const CompIndirect<T1, T2, false>& a, const T3& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b, (a.result) & (a.right != b));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, true> operator!=(const CompValue<T1, true>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, true>(a.value, b.right, (b.result) && (a.value != b.left));
}

template <typename T1, typename T3, typename T4, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T4, false> operator!=(const CompValue<T1, false>& a, const CompIndirect<T3, T4, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T4, false>(a.value, b.right, (b.result) & (a.value != b.left));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, true> operator!=(const CompIndirect<T1, T2, true>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, true>(a.left, b.value, (a.result) && (a.right != b.value));
}

template <typename T1, typename T2, typename T3, bool LOGICAND2>
CMPCONSTEXPR CompIndirect<T1, T3, false> operator!=(const CompIndirect<T1, T2, false>& a, const CompValue<T3, LOGICAND2>& b) noexcept
{
	return CompIndirect<T1, T3, false>(a.left, b.value, (a.result) & (a.right != b.value));
}

#endif // !COMPARATORS_H
