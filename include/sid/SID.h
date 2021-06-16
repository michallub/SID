#ifndef SID_H
#define SID_H


#include <array>

namespace sid {


	//treat too long data as error in debug compilation
#ifndef SID_ASSERT_FALLTHROUGH_HASHING
#define SID_ASSERT_FALLTHROUGH_HASHING 1
#endif



#ifndef SID_CUSTOM_HASH_FUNCTION


#ifndef SID_HASH_FUNCTION_SIPHASH
#ifndef SID_HASH_FUNCTION_CRC64

#define SID_HASH_FUNCTION_SIPHASH
//#define SID_HASH_FUNCTION_CRC64

#endif // !SID_HASH_FUNCTION_CRC64
#endif // !SID_HASH_FUNCTION_SIPHASH






#ifdef  SID_HASH_FUNCTION_CRC64

}
#include "crc64.h"
namespace sid {
	
	constexpr uint64_t hashFunc(const char* data, size_t size)
	{
		return crc64(data, size);
	}
#endif

	
#ifdef SID_HASH_FUNCTION_SIPHASH
}
#include "SipHash.h"
namespace sid {

	constexpr inline Hash8x16 SipHashStandardKey = {
		'S', 't', 'd', 
		'S', 'i', 'p', 
		'H', 'a', 's', 'h', 
		'K', 'E', 'Y', 
		'1', '2', '3' 
	};

	constexpr uint64_t hashFunc(const char* data, size_t size)
	{
		return SipHash64(data, size, SipHashStandardKey).val[0];
	}

#endif

	


#endif // !SID_CUSTOM_HASH_FUNCTION

	

	


	



#ifndef ENCRYPT_SID
#define ENCRYPT_SID 1
#endif // !1

#ifndef ENCRYPT_SID_KEY
#define ENCRYPT_SID_KEY 123456789333666999
#endif // !1


#if ENCRYPT_SID!=0
}
#include "DES.h"
namespace sid {
	constexpr uint64_t  sidencryptkey = ENCRYPT_SID_KEY;
	constexpr auto sidencryptsubkeys = DES::KeyGenerator(sidencryptkey);
#endif


	// prime * inverse_prime = 1		(mod 2^64)
	constexpr uint64_t prime64 = 2744922570705554189ull;
	constexpr uint64_t invprime64 = 2356850971703802821ull;


	// encode SID package for better hashing
	inline constexpr uint64_t shuffle(uint64_t in) noexcept
	{
		in = in * prime64;

#if ENCRYPT_SID!=0
		in = DES::DES_Enc_Block(in, sidencryptsubkeys);
#endif

		return in;
	}


	// decode SID package from encrypted
	inline constexpr uint64_t unshuffle(uint64_t in) noexcept
	{

#if ENCRYPT_SID!=0
		in = DES::DES_Dec_Block(in, sidencryptsubkeys);
#endif

		in = in * invprime64;

		return in;
	}

	struct SID
	{
		uint64_t val;

		constexpr bool operator==(const SID& b) const noexcept
		{
			return val == b.val;
		}
		constexpr bool operator!=(const SID& b) const noexcept
		{
			return val != b.val;
		}
		constexpr bool operator<(const SID& b) const noexcept
		{
			return val < b.val;
		}
		constexpr bool operator<=(const SID& b) const noexcept
		{
			return val <= b.val;
		}
		constexpr bool operator>(const SID& b) const noexcept
		{
			return val > b.val;
		}
		constexpr bool operator>=(const SID& b) const noexcept
		{
			return val >= b.val;
		}

		constexpr operator uint64_t() const noexcept { return val; }
	};


	//LUT 37<->byte
#ifndef SID_LUT37
#define SID_LUT37
	constexpr ::std::array<uint8_t, 256> getCharTo37() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;
		code++;

		for (size_t i = 0; i <= '9' - '0'; i++)
			tab[i + '0'] = code++;

		for (size_t i = 0; i <= 'z' - 'a'; i++)
			tab[i + 'A'] = tab[i + 'a'] = code++;

		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo37 = getCharTo37();

	constexpr ::std::array<char, 37> get37ToChar() noexcept
	{
		::std::array<char, 37> tab = {};
		uint8_t code = 0;

		tab[code++] = '\0';

		for (size_t i = '0'; i <= '9'; i++)
			tab[code++] = i;

		for (size_t i = 'A'; i <= 'Z'; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<char, 37> lut37ToChar = get37ToChar();
#endif // !SID_LUT37

	//LUT 73<->byte
#ifndef SID_LUT73
#define SID_LUT73
	constexpr ::std::array<uint8_t, 256> getCharTo73() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;
		tab['\0'] = code++;		//0 and any

		for (size_t i = 0; i <= '9' - '0'; i++)		//0-9 -> 1-10
			tab[i + '0'] = code++;

		for (size_t i = 0; i <= 'Z' - 'A'; i++)		//A-Z -> 11-36
			tab[i + 'A'] = code++;

		for (size_t i = 0; i <= 'z' - 'a'; i++)		//a-z -> 37-62
			tab[i + 'a'] = code++;

		tab['+'] = code++;		//63
		tab['/'] = tab['\\'] = code++;		//64
		tab[':'] = tab[';'] = code++;		//65
		tab['('] = tab['['] = tab['{'] = tab['<'] = code++;		//66
		tab[')'] = tab[']'] = tab['}'] = tab['>'] = code++;		//67
		tab['_'] = tab['-'] = tab['~'] = tab[' '] = code++;		//68
		tab['*'] = code++;		//69
		tab['&'] = code++;		//70
		tab['@'] = code++;		//71
		tab['='] = code++;		//72

		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo73 = getCharTo73();

	constexpr ::std::array<char, 73> get73ToChar() noexcept
	{
		::std::array<char, 73> tab = {};
		uint8_t code = 0;

		tab[code++] = '\0';

		for (size_t i = '0'; i <= '9'; i++)
			tab[code++] = i;

		for (size_t i = 'A'; i <= 'Z'; i++)
			tab[code++] = i;

		for (size_t i = 'a'; i <= 'z'; i++)
			tab[code++] = i;

		tab[code++] = '+';
		tab[code++] = '/';
		tab[code++] = ':';
		tab[code++] = '(';
		tab[code++] = ')';
		tab[code++] = '_';
		tab[code++] = '*';
		tab[code++] = '&';
		tab[code++] = '@';
		tab[code++] = '=';

		return tab;
	}
	constexpr inline ::std::array<char, 73> lut73ToChar = get73ToChar();
#endif // !SID_LUT73

	//LUT 131<->byte
#ifndef SID_LUT131
#define SID_LUT131
	constexpr ::std::array<uint8_t, 256> getCharTo131() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;

		for (size_t i = 0; i < 131; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo131 = getCharTo131();

	constexpr ::std::array<char, 131> get131ToChar() noexcept
	{
		::std::array<char, 131> tab = {};
		uint8_t code = 0;

		for (size_t i = 0; i < 131; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<char, 131> lut131ToChar = get131ToChar();
#endif // !SID_LUT131

	//LUT 257<->byte
#ifndef SID_LUT257
#define SID_LUT257
	constexpr ::std::array<uint16_t, 256> getCharTo257() noexcept
	{
		::std::array<uint16_t, 256> tab = {};
		uint16_t code = 0;

		for (size_t i = 0; i < 256; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<uint16_t, 256> lutCharTo257 = getCharTo257();

	constexpr ::std::array<char, 257> get257ToChar() noexcept
	{
		::std::array<char, 257> tab = {};
		uint8_t code = 0;

		for (size_t i = 0; i < 256; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<char, 257> lut257ToChar = get257ToChar();
#endif // !SID_LUT257

	//LUT 2<->byte
#ifndef SID_LUT2
#define SID_LUT2
	constexpr ::std::array<uint8_t, 256> getCharTo2() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;
		tab['0'] = code++;
		tab['1'] = code++;
		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo2 = getCharTo2();

	constexpr ::std::array<char, 2> get2ToChar() noexcept
	{
		::std::array<char, 2> tab = {};
		uint8_t code = 0;

		tab[code++] = '0';
		tab[code++] = '1';

		return tab;
	}
	constexpr inline ::std::array<char, 2> lut2ToChar = get2ToChar();
#endif // !SID_LUT2

	//LUT 11<->byte
#ifndef SID_LUT11
#define SID_LUT11
	constexpr ::std::array<uint8_t, 256> getCharTo11() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;
		code++;

		for (size_t i = 0; i <= '9' - '0'; i++)
			tab[i + '0'] = code++;

		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo11 = getCharTo11();

	constexpr ::std::array<char, 11> get11ToChar() noexcept
	{
		::std::array<char, 11> tab = {};
		uint8_t code = 0;

		tab[code++] = '\0';

		for (size_t i = '0'; i <= '9'; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<char, 11> lut11ToChar = get11ToChar();
#endif // !SID_LUT11

	//LUT 17<->byte
#ifndef SID_LUT17
#define SID_LUT17
	constexpr ::std::array<uint8_t, 256> getCharTo17() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;
		code++;

		for (size_t i = 0; i <= '9' - '0'; i++)
			tab[i + '0'] = code++;

		for (size_t i = 0; i <= 'F' - 'A'; i++)		//A-Z -> 11-36
			tab[i + 'A'] = tab[i + 'a'] = code++;

		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo17 = getCharTo17();

	constexpr ::std::array<char, 17> get17ToChar() noexcept
	{
		::std::array<char, 17> tab = {};
		uint8_t code = 0;

		tab[code++] = '\0';

		for (size_t i = '0'; i <= '9'; i++)
			tab[code++] = i;

		for (size_t i = 'A'; i <= 'F'; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<char, 17> lut17ToChar = get17ToChar();
#endif // !SID_LUT17



#ifndef EXTRA_NUM_0
#define EXTRA_NUM_0 extra_num_0
#endif
#ifndef EXTRA_NUM_1
#define EXTRA_NUM_1 extra_num_1
#endif
#ifndef EXTRA_NUM_2
#define EXTRA_NUM_2 extra_num_2
#endif
#ifndef EXTRA_NUM_3
#define EXTRA_NUM_3 extra_num_3
#endif
#ifndef EXTRA_NUM_4
#define EXTRA_NUM_4 extra_num_4
#endif
#ifndef EXTRA_NUM_5
#define EXTRA_NUM_5 extra_num_5
#endif
#ifndef EXTRA_NUM_6
#define EXTRA_NUM_6 extra_num_6
#endif
#ifndef EXTRA_NUM_7
#define EXTRA_NUM_7 extra_num_7
#endif
#ifndef EXTRA_NUM_8
#define EXTRA_NUM_8 extra_num_8
#endif
#ifndef EXTRA_NUM_9
#define EXTRA_NUM_9 extra_num_9
#endif
#ifndef EXTRA_NUM_A
#define EXTRA_NUM_A extra_num_A
#endif
#ifndef EXTRA_NUM_B
#define EXTRA_NUM_B extra_num_B
#endif
#ifndef EXTRA_NUM_C
#define EXTRA_NUM_C extra_num_C
#endif
#ifndef EXTRA_NUM_D
#define EXTRA_NUM_D extra_num_D
#endif
#ifndef EXTRA_NUM_E
#define EXTRA_NUM_E extra_num_E
#endif
#ifndef EXTRA_NUM_F
#define EXTRA_NUM_F extra_num_F
#endif

#ifndef EXTRA_TXT_0
#define EXTRA_TXT_0 extra_txt_0
#endif
#ifndef EXTRA_TXT_1
#define EXTRA_TXT_1 extra_txt_1
#endif
#ifndef EXTRA_TXT_2
#define EXTRA_TXT_2 extra_txt_2
#endif
#ifndef EXTRA_TXT_3
#define EXTRA_TXT_3 extra_txt_3
#endif
#ifndef EXTRA_TXT_4
#define EXTRA_TXT_4 extra_txt_4
#endif
#ifndef EXTRA_TXT_5
#define EXTRA_TXT_5 extra_txt_5
#endif
#ifndef EXTRA_TXT_6
#define EXTRA_TXT_6 extra_txt_6
#endif
#ifndef EXTRA_TXT_7
#define EXTRA_TXT_7 extra_txt_7
#endif
#ifndef EXTRA_TXT_8
#define EXTRA_TXT_8 extra_txt_8
#endif
#ifndef EXTRA_TXT_9
#define EXTRA_TXT_9 extra_txt_9
#endif
#ifndef EXTRA_TXT_A
#define EXTRA_TXT_A extra_txt_A
#endif
#ifndef EXTRA_TXT_B
#define EXTRA_TXT_B extra_txt_B
#endif
#ifndef EXTRA_TXT_C
#define EXTRA_TXT_C extra_txt_C
#endif
#ifndef EXTRA_TXT_D
#define EXTRA_TXT_D extra_txt_D
#endif
#ifndef EXTRA_TXT_E
#define EXTRA_TXT_E extra_txt_E
#endif
#ifndef EXTRA_TXT_F
#define EXTRA_TXT_F extra_txt_F
#endif

#ifndef EXTRA_HSH_0
#define EXTRA_HSH_0 extra_hsh_0
#endif
#ifndef EXTRA_HSH_1
#define EXTRA_HSH_1 extra_hsh_1
#endif
#ifndef EXTRA_HSH_2
#define EXTRA_HSH_2 extra_hsh_2
#endif
#ifndef EXTRA_HSH_3
#define EXTRA_HSH_3 extra_hsh_3
#endif
#ifndef EXTRA_HSH_4
#define EXTRA_HSH_4 extra_hsh_4
#endif
#ifndef EXTRA_HSH_5
#define EXTRA_HSH_5 extra_hsh_5
#endif
#ifndef EXTRA_HSH_6
#define EXTRA_HSH_6 extra_hsh_6
#endif
#ifndef EXTRA_HSH_7
#define EXTRA_HSH_7 extra_hsh_7
#endif
#ifndef EXTRA_HSH_8
#define EXTRA_HSH_8 extra_hsh_8
#endif
#ifndef EXTRA_HSH_9
#define EXTRA_HSH_9 extra_hsh_9
#endif
#ifndef EXTRA_HSH_A
#define EXTRA_HSH_A extra_hsh_A
#endif
#ifndef EXTRA_HSH_B
#define EXTRA_HSH_B extra_hsh_B
#endif
#ifndef EXTRA_HSH_C
#define EXTRA_HSH_C extra_hsh_C
#endif
#ifndef EXTRA_HSH_D
#define EXTRA_HSH_D extra_hsh_D
#endif
#ifndef EXTRA_HSH_E
#define EXTRA_HSH_E extra_hsh_E
#endif
#ifndef EXTRA_HSH_F
#define EXTRA_HSH_F extra_hsh_F
#endif



	namespace PackType {
		enum PT
		{
			numbers = 0x10000,
			texts = 0x20000,
			hashes = 0x40000,

			extra = 0x100000,

			extra_num = extra | numbers,
			extra_txt = extra | texts,
			extra_hsh = extra | hashes,



			error = 0,

			hash = hashes | 1,

			bin = numbers | 1,
			dec,
			hex,

			alnum = texts | 1,
			base64,
			ascii,
			byte,

			EXTRA_NUM_0 = extra_num | 1,
			EXTRA_NUM_1,
			EXTRA_NUM_2,
			EXTRA_NUM_3,
			EXTRA_NUM_4,
			EXTRA_NUM_5,
			EXTRA_NUM_6,
			EXTRA_NUM_7,
			EXTRA_NUM_8,
			EXTRA_NUM_9,
			EXTRA_NUM_A,
			EXTRA_NUM_B,
			EXTRA_NUM_C,
			EXTRA_NUM_D,
			EXTRA_NUM_E,
			EXTRA_NUM_F,

			EXTRA_TXT_0 = extra_txt | 1,
			EXTRA_TXT_1,
			EXTRA_TXT_2,
			EXTRA_TXT_3,
			EXTRA_TXT_4,
			EXTRA_TXT_5,
			EXTRA_TXT_6,
			EXTRA_TXT_7,
			EXTRA_TXT_8,
			EXTRA_TXT_9,
			EXTRA_TXT_A,
			EXTRA_TXT_B,
			EXTRA_TXT_C,
			EXTRA_TXT_D,
			EXTRA_TXT_E,
			EXTRA_TXT_F,

			EXTRA_HSH_0 = extra_hsh | 1,
			EXTRA_HSH_1,
			EXTRA_HSH_2,
			EXTRA_HSH_3,
			EXTRA_HSH_4,
			EXTRA_HSH_5,
			EXTRA_HSH_6,
			EXTRA_HSH_7,
			EXTRA_HSH_8,
			EXTRA_HSH_9,
			EXTRA_HSH_A,
			EXTRA_HSH_B,
			EXTRA_HSH_C,
			EXTRA_HSH_D,
			EXTRA_HSH_E,
			EXTRA_HSH_F
		};
	}


	namespace ip	//Internal Params
	{
		struct EncodingInfo
		{
			uint64_t code;
			uint64_t mask;
			uint64_t cdpt;
			uint64_t lngh;
			char name[4];
			uint64_t maxValue;
		};

		static constexpr EncodingInfo hashInfo = {
			uint64_t(0b11110000) << 56,		// code
			uint64_t(0b11110000) << 56,		// mask
			16,								// code point
			15,								// max length
			{ 'H', 'A', 'S', 'H'},			// name
			72057594037927935
		};

		static constexpr EncodingInfo alnumInfo = {
			uint64_t(0b00000000) << 56,		// code
			uint64_t(0b10000000) << 56,		// mask
			37,								// code point
			12,								// max length
			{ 'A', 'L', 'N', 'U' },			// name
			6582952005840035280				// maxValue
		};

		static constexpr EncodingInfo base64Info = {
			uint64_t(0b10000000) << 56,		// code
			uint64_t(0b11000000) << 56,		// mask
			73,								// code point
			10,								// max length
			{ 'B', '6', '4', '_' },			// name
			58871586708267912				// maxValue
		};

		static constexpr EncodingInfo asciiInfo = {
			uint64_t(0b11101100) << 56,		// code
			uint64_t(0b11111110) << 56,		// mask
			131,								// code point
			8,								// max length
			{ 'A', 'S', 'C', 'I' },			// name
			86730203469006240				// maxValue
		};

		static constexpr EncodingInfo byteInfo = {
			uint64_t(0b11101110) << 56,		// code
			uint64_t(0b11111110) << 56,		// mask
			257,								// code point
			7,								// max length
			{ 'B', 'Y', 'T', 'E' },			// name
			74051159531521793				// maxValue
		};

		static constexpr EncodingInfo binInfo = {
			uint64_t(0b11000000) << 56,		// code
			uint64_t(0b11110000) << 56,		// mask
			2,								// code point
			60,								// max length
			{ 'B', 'I', 'N', '_' },			// name
			1152921504606846975				// maxValue
		};

		static constexpr EncodingInfo decInfo = {
			uint64_t(0b11100000) << 56,		// code
			uint64_t(0b11111000) << 56,		// mask
			11,								// code point
			17,							// max length
			{ 'D', 'E', 'C', '_' },			// name
			99999999999999999				// maxValue
		};

		static constexpr EncodingInfo hexInfo = {
			uint64_t(0b11101000) << 56,		// code
			uint64_t(0b11111100) << 56,		// mask
			17,								// code point
			14,								// max length
			{ 'H', 'E', 'X', '_' },			// name
			72057594037927935				// maxValue
		};




#ifndef SID_EXTRA_NUM_COUNT
#define SID_EXTRA_NUM_COUNT 6
#endif // !SID_EXTRA_NUM_COUNT

#ifndef SID_EXTRA_TXT_COUNT
#define SID_EXTRA_TXT_COUNT 6
#endif // !SID_EXTRA_TXT_COUNT

#ifndef SID_EXTRA_HSH_COUNT
#define SID_EXTRA_HSH_COUNT 4
#endif // !SID_EXTRA_HSH_COUNT

#define SID_FIRST_EXTRA_NUM 0
#define SID_FIRST_EXTRA_TXT SID_FIRST_EXTRA_NUM+SID_EXTRA_NUM_COUNT
#define SID_FIRST_EXTRA_HSH SID_FIRST_EXTRA_TXT+SID_EXTRA_TXT_COUNT
#define SID_FIRST_EXTRA_END SID_FIRST_EXTRA_HSH+SID_EXTRA_HSH_COUNT
		static_assert(SID_FIRST_EXTRA_END <= 16);

		static inline constexpr uint64_t extraCodes[16] = {
			uint64_t(0b11010000) << 56,
			uint64_t(0b11010001) << 56,
			uint64_t(0b11010010) << 56,
			uint64_t(0b11010011) << 56,
			uint64_t(0b11010100) << 56,
			uint64_t(0b11010101) << 56,
			uint64_t(0b11010110) << 56,
			uint64_t(0b11010111) << 56,
			uint64_t(0b11011000) << 56,
			uint64_t(0b11011001) << 56,
			uint64_t(0b11011010) << 56,
			uint64_t(0b11011011) << 56,
			uint64_t(0b11011100) << 56,
			uint64_t(0b11011101) << 56,
			uint64_t(0b11011110) << 56,
			uint64_t(0b11011111) << 56
		};



#ifndef SID_EXTRA_NUM_NAME_0
#define SID_EXTRA_NUM_NAME_0 "XN_0"
#endif
#ifndef SID_EXTRA_NUM_NAME_1
#define SID_EXTRA_NUM_NAME_1 "XN_1"
#endif
#ifndef SID_EXTRA_NUM_NAME_2
#define SID_EXTRA_NUM_NAME_2 "XN_2"
#endif
#ifndef SID_EXTRA_NUM_NAME_3
#define SID_EXTRA_NUM_NAME_3 "XN_3"
#endif
#ifndef SID_EXTRA_NUM_NAME_4
#define SID_EXTRA_NUM_NAME_4 "XN_4"
#endif
#ifndef SID_EXTRA_NUM_NAME_5
#define SID_EXTRA_NUM_NAME_5 "XN_5"
#endif
#ifndef SID_EXTRA_NUM_NAME_6
#define SID_EXTRA_NUM_NAME_6 "XN_6"
#endif
#ifndef SID_EXTRA_NUM_NAME_7
#define SID_EXTRA_NUM_NAME_7 "XN_7"
#endif
#ifndef SID_EXTRA_NUM_NAME_8
#define SID_EXTRA_NUM_NAME_8 "XN_8"
#endif
#ifndef SID_EXTRA_NUM_NAME_9
#define SID_EXTRA_NUM_NAME_9 "XN_9"
#endif
#ifndef SID_EXTRA_NUM_NAME_A
#define SID_EXTRA_NUM_NAME_A "XN_A"
#endif
#ifndef SID_EXTRA_NUM_NAME_B
#define SID_EXTRA_NUM_NAME_B "XN_B"
#endif
#ifndef SID_EXTRA_NUM_NAME_C
#define SID_EXTRA_NUM_NAME_C "XN_C"
#endif
#ifndef SID_EXTRA_NUM_NAME_D
#define SID_EXTRA_NUM_NAME_D "XN_D"
#endif
#ifndef SID_EXTRA_NUM_NAME_E
#define SID_EXTRA_NUM_NAME_E "XN_E"
#endif
#ifndef SID_EXTRA_NUM_NAME_F
#define SID_EXTRA_NUM_NAME_F "XN_F"
#endif

#ifndef SID_EXTRA_TXT_NAME_0
#define SID_EXTRA_TXT_NAME_0 "XT_0"
#endif
#ifndef SID_EXTRA_TXT_NAME_1
#define SID_EXTRA_TXT_NAME_1 "XT_1"
#endif
#ifndef SID_EXTRA_TXT_NAME_2
#define SID_EXTRA_TXT_NAME_2 "XT_2"
#endif
#ifndef SID_EXTRA_TXT_NAME_3
#define SID_EXTRA_TXT_NAME_3 "XT_3"
#endif
#ifndef SID_EXTRA_TXT_NAME_4
#define SID_EXTRA_TXT_NAME_4 "XT_4"
#endif
#ifndef SID_EXTRA_TXT_NAME_5
#define SID_EXTRA_TXT_NAME_5 "XT_5"
#endif
#ifndef SID_EXTRA_TXT_NAME_6
#define SID_EXTRA_TXT_NAME_6 "XT_6"
#endif
#ifndef SID_EXTRA_TXT_NAME_7
#define SID_EXTRA_TXT_NAME_7 "XT_7"
#endif
#ifndef SID_EXTRA_TXT_NAME_8
#define SID_EXTRA_TXT_NAME_8 "XT_8"
#endif
#ifndef SID_EXTRA_TXT_NAME_9
#define SID_EXTRA_TXT_NAME_9 "XT_9"
#endif
#ifndef SID_EXTRA_TXT_NAME_A
#define SID_EXTRA_TXT_NAME_A "XT_A"
#endif
#ifndef SID_EXTRA_TXT_NAME_B
#define SID_EXTRA_TXT_NAME_B "XT_B"
#endif
#ifndef SID_EXTRA_TXT_NAME_C
#define SID_EXTRA_TXT_NAME_C "XT_C"
#endif
#ifndef SID_EXTRA_TXT_NAME_D
#define SID_EXTRA_TXT_NAME_D "XT_D"
#endif
#ifndef SID_EXTRA_TXT_NAME_E
#define SID_EXTRA_TXT_NAME_E "XT_E"
#endif
#ifndef SID_EXTRA_TXT_NAME_F
#define SID_EXTRA_TXT_NAME_F "XT_F"
#endif

#ifndef SID_EXTRA_HSH_NAME_0
#define SID_EXTRA_HSH_NAME_0 "XH_0"
#endif
#ifndef SID_EXTRA_HSH_NAME_1
#define SID_EXTRA_HSH_NAME_1 "XH_1"
#endif
#ifndef SID_EXTRA_HSH_NAME_2
#define SID_EXTRA_HSH_NAME_2 "XH_2"
#endif
#ifndef SID_EXTRA_HSH_NAME_3
#define SID_EXTRA_HSH_NAME_3 "XH_3"
#endif
#ifndef SID_EXTRA_HSH_NAME_4
#define SID_EXTRA_HSH_NAME_4 "XH_4"
#endif
#ifndef SID_EXTRA_HSH_NAME_5
#define SID_EXTRA_HSH_NAME_5 "XH_5"
#endif
#ifndef SID_EXTRA_HSH_NAME_6
#define SID_EXTRA_HSH_NAME_6 "XH_6"
#endif
#ifndef SID_EXTRA_HSH_NAME_7
#define SID_EXTRA_HSH_NAME_7 "XH_7"
#endif
#ifndef SID_EXTRA_HSH_NAME_8
#define SID_EXTRA_HSH_NAME_8 "XH_8"
#endif
#ifndef SID_EXTRA_HSH_NAME_9
#define SID_EXTRA_HSH_NAME_9 "XH_9"
#endif
#ifndef SID_EXTRA_HSH_NAME_A
#define SID_EXTRA_HSH_NAME_A "XH_A"
#endif
#ifndef SID_EXTRA_HSH_NAME_B
#define SID_EXTRA_HSH_NAME_B "XH_B"
#endif
#ifndef SID_EXTRA_HSH_NAME_C
#define SID_EXTRA_HSH_NAME_C "XH_C"
#endif
#ifndef SID_EXTRA_HSH_NAME_D
#define SID_EXTRA_HSH_NAME_D "XH_D"
#endif
#ifndef SID_EXTRA_HSH_NAME_E
#define SID_EXTRA_HSH_NAME_E "XH_E"
#endif
#ifndef SID_EXTRA_HSH_NAME_F
#define SID_EXTRA_HSH_NAME_F "XH_F"
#endif


		static inline constexpr const char *extraNumNames[16] = {
			SID_EXTRA_NUM_NAME_0,
			SID_EXTRA_NUM_NAME_1,
			SID_EXTRA_NUM_NAME_2,
			SID_EXTRA_NUM_NAME_3,
			SID_EXTRA_NUM_NAME_4,
			SID_EXTRA_NUM_NAME_5,
			SID_EXTRA_NUM_NAME_6,
			SID_EXTRA_NUM_NAME_7,
			SID_EXTRA_NUM_NAME_8,
			SID_EXTRA_NUM_NAME_9,
			SID_EXTRA_NUM_NAME_A,
			SID_EXTRA_NUM_NAME_B,
			SID_EXTRA_NUM_NAME_C,
			SID_EXTRA_NUM_NAME_D,
			SID_EXTRA_NUM_NAME_E,
			SID_EXTRA_NUM_NAME_F
		};

		static inline constexpr const char* extraTxtNames[16] = {
			SID_EXTRA_TXT_NAME_0,
			SID_EXTRA_TXT_NAME_1,
			SID_EXTRA_TXT_NAME_2,
			SID_EXTRA_TXT_NAME_3,
			SID_EXTRA_TXT_NAME_4,
			SID_EXTRA_TXT_NAME_5,
			SID_EXTRA_TXT_NAME_6,
			SID_EXTRA_TXT_NAME_7,
			SID_EXTRA_TXT_NAME_8,
			SID_EXTRA_TXT_NAME_9,
			SID_EXTRA_TXT_NAME_A,
			SID_EXTRA_TXT_NAME_B,
			SID_EXTRA_TXT_NAME_C,
			SID_EXTRA_TXT_NAME_D,
			SID_EXTRA_TXT_NAME_E,
			SID_EXTRA_TXT_NAME_F
		};

		static inline constexpr const char* extraHshNames[16] = {
			SID_EXTRA_HSH_NAME_0,
			SID_EXTRA_HSH_NAME_1,
			SID_EXTRA_HSH_NAME_2,
			SID_EXTRA_HSH_NAME_3,
			SID_EXTRA_HSH_NAME_4,
			SID_EXTRA_HSH_NAME_5,
			SID_EXTRA_HSH_NAME_6,
			SID_EXTRA_HSH_NAME_7,
			SID_EXTRA_HSH_NAME_8,
			SID_EXTRA_HSH_NAME_9,
			SID_EXTRA_HSH_NAME_A,
			SID_EXTRA_HSH_NAME_B,
			SID_EXTRA_HSH_NAME_C,
			SID_EXTRA_HSH_NAME_D,
			SID_EXTRA_HSH_NAME_E,
			SID_EXTRA_HSH_NAME_F
		};

		constexpr std::array<EncodingInfo, SID_EXTRA_NUM_COUNT> getExtraNumInfo()
		{
			std::array<EncodingInfo, SID_EXTRA_NUM_COUNT> result = {};

			int codeindex = SID_FIRST_EXTRA_NUM;
			int extraindex = 0;
			for (auto& e : result)
			{
				assert(extraNumNames[extraindex] != nullptr);

				e.code = extraCodes[codeindex++];
				e.mask = uint64_t(0b11111111) << 56;
				e.lngh = 56;
				e.cdpt = 2;
				e.name[0] = extraNumNames[extraindex][0];
				e.name[1] = extraNumNames[extraindex][1];
				e.name[2] = extraNumNames[extraindex][2];
				e.name[3] = extraNumNames[extraindex][3];
				e.maxValue = 72057594037927935;

				extraindex++;
			}
			return result;
		}

		constexpr std::array<EncodingInfo, SID_EXTRA_TXT_COUNT> getExtraTxtInfo()
		{
			std::array<EncodingInfo, SID_EXTRA_TXT_COUNT> result = {};

			int codeindex = SID_FIRST_EXTRA_TXT;
			int extraindex = 0;
			for (auto& e : result)
			{
				assert(extraNumNames[extraindex] != nullptr);
				e.code = extraCodes[codeindex++];
				e.mask = uint64_t(0b11111111) << 56;
				e.lngh = 9;
				e.cdpt = 73;
				e.name[0] = extraTxtNames[extraindex][0];
				e.name[1] = extraTxtNames[extraindex][1];
				e.name[2] = extraTxtNames[extraindex][2];
				e.name[3] = extraTxtNames[extraindex][3];
				e.maxValue = 58871586708267912;

				extraindex++;
			}
			return result;
		}
		
		constexpr std::array<EncodingInfo, SID_EXTRA_HSH_COUNT> getExtraHshInfo()
		{
			std::array<EncodingInfo, SID_EXTRA_HSH_COUNT> result = {};

			int codeindex = SID_FIRST_EXTRA_HSH;
			int extraindex = 0;
			for (auto& e : result)
			{
				assert(extraNumNames[extraindex] != nullptr);
				e.code = extraCodes[codeindex++];
				e.mask = uint64_t(0b11111111) << 56;
				e.lngh = 14;
				e.cdpt = 16;
				e.name[0] = extraHshNames[extraindex][0];
				e.name[1] = extraHshNames[extraindex][1];
				e.name[2] = extraHshNames[extraindex][2];
				e.name[3] = extraHshNames[extraindex][3];
				e.maxValue = 72057594037927935;

				extraindex++;
			}
			return result;
		}


		static inline constexpr auto extra_num = getExtraNumInfo();
		static inline constexpr auto extra_txt = getExtraTxtInfo();
		static inline constexpr auto extra_hsh = getExtraHshInfo();

	}


	struct UnpackedNumber
	{
		uint64_t number;
		PackType::PT type;
	};

	struct UnpackedText
	{
		char str[16];
		PackType::PT type;
	};

	template <uint64_t CODE, uint64_t MASK>
	constexpr SID packHashTempl(uint64_t hash) noexcept {
		
		hash = ((hash & ~MASK) | CODE);

		hash = shuffle(hash);

		return { hash };
	}

	template <uint64_t CODE, uint64_t MASK>
	constexpr SID packHashTempl(const char* data, size_t size) noexcept {
		if (data == nullptr) size = 0;

		uint64_t hash = ((hashFunc(data, size) & ~MASK) | CODE);

		hash = shuffle(hash);

		return { hash };
	}

	template <uint64_t CODE, uint64_t MASK, uint64_t MAX_VALUE>
	constexpr SID packBinTempl(uint64_t number) noexcept {

		if (number <= MAX_VALUE)
		{
			number = ((number & ~MASK) | CODE);

			number = shuffle(number);

			return { number };
		}
		else
		{
//#if SID_ASSERT_FALLTHROUGH_HASHING!=0
//			assert(false);
//#endif
			return packHashTempl<ip::hashInfo.code, ip::hashInfo.mask>(number);
		}
	}

	template <
		uint64_t CODE, 
		uint64_t MASK,
		uint64_t CODE_POINTS, 
		uint64_t LENGTH, 
		typename LUT_TYPE>
	constexpr SID packStringTempl(const char* data, size_t size, const LUT_TYPE &LUT) noexcept {
		if (data == nullptr) size = 0;

		if (size <= LENGTH)
		{
			uint64_t hash = 0;
			for (size_t i = size; i < LENGTH; ++i)
				hash = hash * CODE_POINTS + 0;
			for (size_t i = 0; i < size; ++i)
				hash = hash * CODE_POINTS + LUT[uint8_t(data[i])];

			hash = ((hash & ~MASK) | CODE);

			hash = shuffle(hash);

			return { hash };
		}
		else
		{
//#if SID_ASSERT_FALLTHROUGH_HASHING!=0
//			assert(false);
//#endif
			return packHashTempl<ip::hashInfo.code, ip::hashInfo.mask>(data, size);
		}
	}

	template <
		uint64_t CODE, 
		uint64_t MASK,
		uint64_t CODE_POINTS,
		uint64_t MAX_VALUE,
		uint64_t BASE>
	constexpr SID packNumberTempl(uint64_t number) noexcept {

		if (number <= MAX_VALUE)
		{
			uint64_t digit = 1;
			uint64_t hash = 0;
			while (number)
			{
				hash = hash + (number % BASE + 1) * digit ;
				number = number / BASE;
				digit = digit * CODE_POINTS;
			}

			hash = ((hash & ~MASK) | CODE);

			hash = shuffle(hash);

			return { hash };
		}
		else
		{
//#if SID_ASSERT_FALLTHROUGH_HASHING!=0
//			assert(false);
//#endif
			return packHashTempl<ip::hashInfo.code, ip::hashInfo.mask>(number);
		}
	}

	template <
		uint64_t CODE,
		uint64_t MASK,
		uint64_t CODE_POINTS,
		uint64_t LENGTH,
		typename LUT_TYPE>
	constexpr bool unpackTempl(uint64_t hsh, ::std::array<char, 64>& text, const LUT_TYPE& LUT, const char NAME[4]) noexcept
	{
		if ((hsh & MASK) == CODE)
		{
			hsh &= ~(MASK);
			for (intptr_t i = 63; i > 63 - intptr_t(LENGTH); --i)
			{
				uint8_t code = uint8_t(hsh % CODE_POINTS);
				hsh = hsh / CODE_POINTS;
				text[i] = LUT[code];
			}
			text[0] = NAME[0];
			text[1] = NAME[1];
			text[2] = NAME[2];
			text[3] = NAME[3];
			return true;
		}
		return false;
	}

	template <
		uint64_t CODE,
		uint64_t MASK,
		PackType::PT TYPE>
		constexpr bool unpackNumberBinaryTempl(uint64_t hsh, UnpackedNumber &result) noexcept
	{
		if ((hsh & MASK) == CODE)
		{
			hsh &= ~(MASK);
			result = { hsh, TYPE };
			return  true;
		}
		return false;
	}

	template <
		uint64_t CODE,
		uint64_t MASK,
		uint64_t CODE_POINTS,
		PackType::PT TYPE,
		uint64_t BASE>
		constexpr bool unpackNumberTempl(uint64_t hsh, UnpackedNumber& result) noexcept
	{
		if ((hsh & MASK) == CODE)
		{
			hsh &= ~(MASK);

			uint64_t digit = 1;
			uint64_t number = 0;
			while (hsh)
			{
				uint64_t hashmod = hsh % CODE_POINTS;
				hsh = hsh / CODE_POINTS;
				if (hashmod == 0)
				{ 
					result = { 0, PackType::error };
					return true;
				}
				number = number + digit * (hashmod - 1);
				digit = digit * BASE;
			}

			result = { number, TYPE };
			return  true;
		}
		return false;
	}

	template <
		uint64_t CODE,
		uint64_t MASK,
		uint64_t CODE_POINTS,
		uint64_t LENGTH,
		PackType::PT TYPE,
		typename LUT_TYPE>
		constexpr bool unpackTextTempl(uint64_t hsh, UnpackedText& result, const LUT_TYPE& LUT) noexcept
	{
		if ((hsh & MASK) == CODE)
		{
			hsh &= ~(MASK);

			result.type = TYPE;

			for (intptr_t i = LENGTH - 1; i >= 0; --i)
			{
				uint8_t code = uint8_t(hsh % CODE_POINTS);
				hsh = hsh / CODE_POINTS;
				result.str[i] = LUT[code];
			}
			return true;
		}
		return false;
	}



















	//put 60bit value (4 MSBs are truncated)
	constexpr SID packHash(uint64_t hash) noexcept {
		return packHashTempl<ip::hashInfo.code, ip::hashInfo.mask>(hash);
	}

	//pack or hash string to 64bit
	constexpr SID packHash(const char* data, size_t size) noexcept {
		return packHashTempl<ip::hashInfo.code, ip::hashInfo.mask>(data, size);
	}

	//put 60bit value or hash is bigger
	constexpr SID packBin(uint64_t number) noexcept {
		return packBinTempl<ip::binInfo.code, ip::binInfo.mask, ip::binInfo.maxValue>(number);
	}

	//pack or hash string to 64bit
	constexpr SID packBin(const char* data, size_t size) noexcept {
		return packStringTempl<ip::binInfo.code, ip::binInfo.mask, ip::binInfo.cdpt, ip::binInfo.lngh>(data, size, lutCharTo2);
	}

	

	//put 17 decimal digit value
	constexpr SID packDec(uint64_t number) noexcept {
		return packNumberTempl<ip::decInfo.code, ip::decInfo.mask, ip::decInfo.cdpt,ip::decInfo.maxValue, 10>(number);
	}

	//pack or hash string to 64bit
	constexpr SID packDec(const char* data, size_t size) noexcept {
		return packStringTempl<ip::decInfo.code, ip::decInfo.mask, ip::decInfo.cdpt, ip::decInfo.lngh>(data, size, lutCharTo11);
	}



	//put 14 hex digit value
	constexpr SID packHex(uint64_t number) noexcept {
		return packNumberTempl<ip::hexInfo.code, ip::hexInfo.mask, ip::hexInfo.cdpt, ip::hexInfo.maxValue, 16>(number);
	}

	//pack or hash string to 64bit
	constexpr SID packHex(const char* data, size_t size) noexcept {
		return packStringTempl<ip::hexInfo.code, ip::hexInfo.mask, ip::hexInfo.cdpt, ip::hexInfo.lngh>(data, size, lutCharTo17);
	}


	//pack or hash string to 64bit
	constexpr SID packAlnum(const char* data, size_t size) noexcept {
		return packStringTempl<ip::alnumInfo.code, ip::alnumInfo.mask, ip::alnumInfo.cdpt, ip::alnumInfo.lngh>(data, size, lutCharTo37);
	}


	//pack or hash string to 64bit
	constexpr SID packB64(const char* data, size_t size) noexcept {
		return packStringTempl<ip::base64Info.code, ip::base64Info.mask, ip::base64Info.cdpt, ip::base64Info.lngh>(data, size, lutCharTo73);
	}


	//pack or hash string to 64bit
	constexpr SID packAscii(const char* data, size_t size) noexcept {
		return packStringTempl<ip::asciiInfo.code, ip::asciiInfo.mask, ip::asciiInfo.cdpt, ip::asciiInfo.lngh>(data, size, lutCharTo131);
	}


	//pack or hash string to 64bit
	constexpr SID packByte(const char* data, size_t size) noexcept {
		return packStringTempl<ip::byteInfo.code, ip::byteInfo.mask, ip::byteInfo.cdpt, ip::byteInfo.lngh>(data, size, lutCharTo257);
	}




	//put 56bit value or hash is bigger
	template<size_t NumIndex>
	constexpr SID packExtraNum(uint64_t number) noexcept {
		return packBinTempl<ip::extra_num[NumIndex].code, ip::extra_num[NumIndex].mask, ip::extra_num[NumIndex].maxValue>(number);
	}

	template<size_t NumIndex>
	constexpr SID packExtraNum(const char* data, size_t size) noexcept {
		return packStringTempl<ip::extra_num[NumIndex].code, ip::extra_num[NumIndex].mask, ip::extra_num[NumIndex].cdpt, ip::extra_num[NumIndex].lngh>(data, size, lutCharTo2);
	}



	//pack or hash string to 64bit
	template<size_t NumIndex>
	constexpr SID packExtraText(const char* data, size_t size) noexcept {
		return packStringTempl<ip::extra_txt[NumIndex].code, ip::extra_txt[NumIndex].mask, ip::extra_txt[NumIndex].cdpt, ip::extra_txt[NumIndex].lngh>(data, size, lutCharTo73);
	}



	//put 56bit value (4 MSBs are truncated)
	template<size_t NumIndex>
	constexpr SID packExtraHash(uint64_t hash) noexcept {
		return packHashTempl<ip::extra_hsh[NumIndex].code, ip::extra_hsh[NumIndex].mask>(hash);
	}

	//pack or hash string to 64bit
	template<size_t NumIndex>
	constexpr SID packExtraHash(const char* data, size_t size) noexcept {
		return packHashTempl<ip::extra_hsh[NumIndex].code, ip::extra_hsh[NumIndex].mask>(data, size);
	}
















	//unpack or print hash as hex 64bit to 15 char string 
	constexpr ::std::array<char, 64> unpack(const SID& sid) noexcept 
	{
		::std::array<char, 64> text = {};

		uint64_t hsh = sid.val;

		hsh = unshuffle(hsh);


		if (unpackTempl<ip::alnumInfo.code, ip::alnumInfo.mask, ip::alnumInfo.cdpt, ip::alnumInfo.lngh>(hsh, text, lut37ToChar, ip::alnumInfo.name)) return text;
		if (unpackTempl<ip::base64Info.code, ip::base64Info.mask, ip::base64Info.cdpt, ip::base64Info.lngh>(hsh, text, lut73ToChar, ip::base64Info.name)) return text;
		if (unpackTempl<ip::binInfo.code, ip::binInfo.mask, ip::binInfo.cdpt, ip::binInfo.lngh>(hsh, text, lut2ToChar, ip::binInfo.name)) return text;
		if (unpackTempl<ip::decInfo.code, ip::decInfo.mask, ip::decInfo.cdpt, ip::decInfo.lngh>(hsh, text, lut11ToChar, ip::decInfo.name)) return text;
		if (unpackTempl<ip::hexInfo.code, ip::hexInfo.mask, ip::hexInfo.cdpt, ip::hexInfo.lngh>(hsh, text, lut17ToChar, ip::hexInfo.name)) return text;
		if (unpackTempl<ip::asciiInfo.code, ip::asciiInfo.mask, ip::asciiInfo.cdpt, ip::asciiInfo.lngh>(hsh, text, lut131ToChar, ip::asciiInfo.name)) return text;
		if (unpackTempl<ip::byteInfo.code, ip::byteInfo.mask, ip::byteInfo.cdpt, ip::byteInfo.lngh>(hsh, text, lut257ToChar, ip::byteInfo.name)) return text;
		if (unpackTempl<ip::hashInfo.code, ip::hashInfo.mask, ip::hashInfo.cdpt, ip::hashInfo.lngh>(hsh, text, "0123456789ABCDEF", ip::hashInfo.name)) return text;






#define UNPACK_EXTRA_NUM(NUMINDEX) \
		if (unpackTempl<ip::extra_num[NUMINDEX].code, ip::extra_num[NUMINDEX].mask, ip::extra_num[NUMINDEX].cdpt, ip::extra_num[NUMINDEX].lngh>(hsh, text, lut2ToChar, ip::extra_num[NUMINDEX].name)) return text;

#if SID_EXTRA_NUM_COUNT>0
		UNPACK_EXTRA_NUM(0)
#endif

#if SID_EXTRA_NUM_COUNT>1
			UNPACK_EXTRA_NUM(1)
#endif

#if SID_EXTRA_NUM_COUNT>2
			UNPACK_EXTRA_NUM(2)
#endif

#if SID_EXTRA_NUM_COUNT>3
			UNPACK_EXTRA_NUM(3)
#endif

#if SID_EXTRA_NUM_COUNT>4
			UNPACK_EXTRA_NUM(4)
#endif

#if SID_EXTRA_NUM_COUNT>5
			UNPACK_EXTRA_NUM(5)
#endif

#if SID_EXTRA_NUM_COUNT>6
			UNPACK_EXTRA_NUM(6)
#endif

#if SID_EXTRA_NUM_COUNT>7
			UNPACK_EXTRA_NUM(7)
#endif

#if SID_EXTRA_NUM_COUNT>8
			UNPACK_EXTRA_NUM(8)
#endif

#if SID_EXTRA_NUM_COUNT>9
			UNPACK_EXTRA_NUM(9)
#endif

#if SID_EXTRA_NUM_COUNT>10
			UNPACK_EXTRA_NUM(10)
#endif

#if SID_EXTRA_NUM_COUNT>11
			UNPACK_EXTRA_NUM(11)
#endif

#if SID_EXTRA_NUM_COUNT>12
			UNPACK_EXTRA_NUM(12)
#endif

#if SID_EXTRA_NUM_COUNT>13
			UNPACK_EXTRA_NUM(13)
#endif

#if SID_EXTRA_NUM_COUNT>14
			UNPACK_EXTRA_NUM(14)
#endif

#if SID_EXTRA_NUM_COUNT>15
			UNPACK_EXTRA_NUM(15)
#endif

#undef UNPACK_EXTRA_NUM

			


			
#define UNPACK_EXTRA_TXT(TXTINDEX) \
			if (unpackTempl<ip::extra_txt[TXTINDEX].code, ip::extra_txt[TXTINDEX].mask, ip::extra_txt[TXTINDEX].cdpt, ip::extra_txt[TXTINDEX].lngh>(hsh, text, lut73ToChar, ip::extra_txt[TXTINDEX].name)) return text;

#if SID_EXTRA_TXT_COUNT>0
			UNPACK_EXTRA_TXT(0)
#endif

#if SID_EXTRA_TXT_COUNT>1
			UNPACK_EXTRA_TXT(1)
#endif

#if SID_EXTRA_TXT_COUNT>2
			UNPACK_EXTRA_TXT(2)
#endif

#if SID_EXTRA_TXT_COUNT>3
			UNPACK_EXTRA_TXT(3)
#endif

#if SID_EXTRA_TXT_COUNT>4
			UNPACK_EXTRA_TXT(4)
#endif

#if SID_EXTRA_TXT_COUNT>5
			UNPACK_EXTRA_TXT(5)
#endif

#if SID_EXTRA_TXT_COUNT>6
			UNPACK_EXTRA_TXT(6)
#endif

#if SID_EXTRA_TXT_COUNT>7
			UNPACK_EXTRA_TXT(7)
#endif

#if SID_EXTRA_TXT_COUNT>8
			UNPACK_EXTRA_TXT(8)
#endif

#if SID_EXTRA_TXT_COUNT>9
			UNPACK_EXTRA_TXT(9)
#endif

#if SID_EXTRA_TXT_COUNT>10
			UNPACK_EXTRA_TXT(10)
#endif

#if SID_EXTRA_TXT_COUNT>11
			UNPACK_EXTRA_TXT(11)
#endif

#if SID_EXTRA_TXT_COUNT>12
			UNPACK_EXTRA_TXT(12)
#endif

#if SID_EXTRA_TXT_COUNT>13
			UNPACK_EXTRA_TXT(13)
#endif

#if SID_EXTRA_TXT_COUNT>14
			UNPACK_EXTRA_TXT(14)
#endif

#if SID_EXTRA_TXT_COUNT>15
			UNPACK_EXTRA_TXT(15)
#endif

#undef UNPACK_EXTRA_TXT








#define UNPACK_EXTRA_HSH(HSHINDEX) \
			if (unpackTempl<ip::extra_hsh[HSHINDEX].code, ip::extra_hsh[HSHINDEX].mask, ip::extra_hsh[HSHINDEX].cdpt, ip::extra_hsh[HSHINDEX].lngh>(hsh, text, "0123456789ABCDEF", ip::extra_hsh[HSHINDEX].name)) return text;

#if SID_EXTRA_HSH_COUNT>0
				UNPACK_EXTRA_HSH(0)
#endif

#if SID_EXTRA_HSH_COUNT>1
				UNPACK_EXTRA_HSH(1)
#endif

#if SID_EXTRA_HSH_COUNT>2
				UNPACK_EXTRA_HSH(2)
#endif

#if SID_EXTRA_HSH_COUNT>3
				UNPACK_EXTRA_HSH(3)
#endif

#if SID_EXTRA_HSH_COUNT>4
				UNPACK_EXTRA_HSH(4)
#endif

#if SID_EXTRA_HSH_COUNT>5
				UNPACK_EXTRA_HSH(5)
#endif

#if SID_EXTRA_HSH_COUNT>6
				UNPACK_EXTRA_HSH(6)
#endif

#if SID_EXTRA_HSH_COUNT>7
				UNPACK_EXTRA_HSH(7)
#endif

#if SID_EXTRA_HSH_COUNT>8
				UNPACK_EXTRA_HSH(8)
#endif

#if SID_EXTRA_HSH_COUNT>9
				UNPACK_EXTRA_HSH(9)
#endif

#if SID_EXTRA_HSH_COUNT>10
				UNPACK_EXTRA_HSH(10)
#endif

#if SID_EXTRA_HSH_COUNT>11
				UNPACK_EXTRA_HSH(11)
#endif

#if SID_EXTRA_HSH_COUNT>12
				UNPACK_EXTRA_HSH(12)
#endif

#if SID_EXTRA_HSH_COUNT>13
				UNPACK_EXTRA_HSH(13)
#endif

#if SID_EXTRA_HSH_COUNT>14
				UNPACK_EXTRA_HSH(14)
#endif

#if SID_EXTRA_HSH_COUNT>15
				UNPACK_EXTRA_HSH(15)
#endif

#undef UNPACK_EXTRA_HSH



		return text;
	}

	constexpr UnpackedNumber unpackNumber(const SID& sid)
	{
		uint64_t hsh = sid.val;

		hsh = unshuffle(hsh);
		UnpackedNumber result = {};

		if (unpackNumberBinaryTempl<ip::binInfo.code, ip::binInfo.mask, PackType::PT::bin>(hsh, result)) return result;
		if (unpackNumberTempl<ip::decInfo.code, ip::decInfo.mask, ip::decInfo.cdpt, PackType::PT::dec, 10>(hsh, result)) return result;
		if (unpackNumberTempl<ip::hexInfo.code, ip::hexInfo.mask, ip::hexInfo.cdpt, PackType::PT::hex, 16>(hsh, result)) return result;




#define UNPACK_EXTRA_NUM(NUMINDEX, PACKTYPE) \
		if (unpackNumberBinaryTempl<ip::extra_num[NUMINDEX].code, ip::extra_num[NUMINDEX].mask, PackType::PT::PACKTYPE>(hsh, result)) return result;

#if SID_EXTRA_NUM_COUNT>0
		UNPACK_EXTRA_NUM(0, EXTRA_NUM_0)
#endif

#if SID_EXTRA_NUM_COUNT>1
			UNPACK_EXTRA_NUM(1, EXTRA_NUM_1)
#endif

#if SID_EXTRA_NUM_COUNT>2
			UNPACK_EXTRA_NUM(2, EXTRA_NUM_2)
#endif

#if SID_EXTRA_NUM_COUNT>3
			UNPACK_EXTRA_NUM(3, EXTRA_NUM_3)
#endif

#if SID_EXTRA_NUM_COUNT>4
			UNPACK_EXTRA_NUM(4, EXTRA_NUM_4)
#endif

#if SID_EXTRA_NUM_COUNT>5
			UNPACK_EXTRA_NUM(5, EXTRA_NUM_5)
#endif

#if SID_EXTRA_NUM_COUNT>6
			UNPACK_EXTRA_NUM(6, EXTRA_NUM_6)
#endif

#if SID_EXTRA_NUM_COUNT>7
			UNPACK_EXTRA_NUM(7, EXTRA_NUM_7)
#endif

#if SID_EXTRA_NUM_COUNT>8
			UNPACK_EXTRA_NUM(8, EXTRA_NUM_8)
#endif

#if SID_EXTRA_NUM_COUNT>9
			UNPACK_EXTRA_NUM(9, EXTRA_NUM_9)
#endif

#if SID_EXTRA_NUM_COUNT>10
			UNPACK_EXTRA_NUM(10, EXTRA_NUM_A)
#endif

#if SID_EXTRA_NUM_COUNT>11
			UNPACK_EXTRA_NUM(11, EXTRA_NUM_B)
#endif

#if SID_EXTRA_NUM_COUNT>12
			UNPACK_EXTRA_NUM(12, EXTRA_NUM_C)
#endif

#if SID_EXTRA_NUM_COUNT>13
			UNPACK_EXTRA_NUM(13, EXTRA_NUM_D)
#endif

#if SID_EXTRA_NUM_COUNT>14
			UNPACK_EXTRA_NUM(14, EXTRA_NUM_E)
#endif

#if SID_EXTRA_NUM_COUNT>15
			UNPACK_EXTRA_NUM(15, EXTRA_NUM_F)
#endif

#undef UNPACK_EXTRA_NUM



		return { 0, PackType::error };
	}

	constexpr UnpackedText unpackText(const SID& sid) {
		uint64_t hsh = sid.val;

		hsh = unshuffle(hsh);

		UnpackedText text = {};

		if (unpackTextTempl<ip::alnumInfo.code, ip::alnumInfo.mask, ip::alnumInfo.cdpt, ip::alnumInfo.lngh, PackType::PT::alnum>(hsh, text, lut37ToChar)) return text;
		if (unpackTextTempl<ip::base64Info.code, ip::base64Info.mask, ip::base64Info.cdpt, ip::base64Info.lngh, PackType::PT::base64>(hsh, text, lut73ToChar)) return text;
		if (unpackTextTempl<ip::asciiInfo.code, ip::asciiInfo.mask, ip::asciiInfo.cdpt, ip::asciiInfo.lngh, PackType::PT::ascii>(hsh, text, lut131ToChar)) return text;
		if (unpackTextTempl<ip::byteInfo.code, ip::byteInfo.mask, ip::byteInfo.cdpt, ip::byteInfo.lngh, PackType::PT::byte>(hsh, text, lut257ToChar)) return text;




#define UNPACK_EXTRA_TXT(TXTINDEX, PACKTYPE) \
		if (unpackTextTempl<ip::extra_txt[TXTINDEX].code, ip::extra_txt[TXTINDEX].mask, ip::extra_txt[TXTINDEX].cdpt, ip::extra_txt[TXTINDEX].lngh, PackType::PT::PACKTYPE>(hsh, text, lut73ToChar)) return text;

#if SID_EXTRA_TXT_COUNT>0
		UNPACK_EXTRA_TXT(0, EXTRA_TXT_0)
#endif

#if SID_EXTRA_TXT_COUNT>1
			UNPACK_EXTRA_TXT(1, EXTRA_TXT_1)
#endif

#if SID_EXTRA_TXT_COUNT>2
			UNPACK_EXTRA_TXT(2, EXTRA_TXT_2)
#endif

#if SID_EXTRA_TXT_COUNT>3
			UNPACK_EXTRA_TXT(3, EXTRA_TXT_3)
#endif

#if SID_EXTRA_TXT_COUNT>4
			UNPACK_EXTRA_TXT(4, EXTRA_TXT_4)
#endif

#if SID_EXTRA_TXT_COUNT>5
			UNPACK_EXTRA_TXT(5, EXTRA_TXT_5)
#endif

#if SID_EXTRA_TXT_COUNT>6
			UNPACK_EXTRA_TXT(6, EXTRA_TXT_6)
#endif

#if SID_EXTRA_TXT_COUNT>7
			UNPACK_EXTRA_TXT(7, EXTRA_TXT_7)
#endif

#if SID_EXTRA_TXT_COUNT>8
			UNPACK_EXTRA_TXT(8, EXTRA_TXT_8)
#endif

#if SID_EXTRA_TXT_COUNT>9
			UNPACK_EXTRA_TXT(9, EXTRA_TXT_9)
#endif

#if SID_EXTRA_TXT_COUNT>10
			UNPACK_EXTRA_TXT(10, EXTRA_TXT_A)
#endif

#if SID_EXTRA_TXT_COUNT>11
			UNPACK_EXTRA_TXT(11, EXTRA_TXT_B)
#endif

#if SID_EXTRA_TXT_COUNT>12
			UNPACK_EXTRA_TXT(12, EXTRA_TXT_C)
#endif

#if SID_EXTRA_TXT_COUNT>13
			UNPACK_EXTRA_TXT(13, EXTRA_TXT_D)
#endif

#if SID_EXTRA_TXT_COUNT>14
			UNPACK_EXTRA_TXT(14, EXTRA_TXT_E)
#endif

#if SID_EXTRA_TXT_COUNT>15
			UNPACK_EXTRA_TXT(15, EXTRA_TXT_F)
#endif

#undef UNPACK_EXTRA_TXT



		return { 0, PackType::error };
	}






} // !namespace sid



//std::hash<SID> specjalization
namespace std
{
	template<> struct hash<::sid::SID>
	{
		constexpr std::size_t operator()(::sid::SID const& s) const noexcept
		{
			return s.val;
		}
	};
}

	//SID = hash & 0x0fff'ffff'ffff'ffff	//lower 60 bits of hash
	constexpr sid::SID operator"" _SIDHASH(uint64_t hash) noexcept
	{
		return sid::packHash(hash);
	}

	//SID = hash of data (crc64 or siphash)
	constexpr sid::SID operator"" _SIDHASH(const char* data, size_t size) noexcept
	{
		return sid::packHash(data, size);
	}


	//SID = 
	constexpr sid::SID operator"" _SIDBIN(uint64_t number) noexcept
	{
		return sid::packBin(number);
	}

	//SID = packed up to 60 {0, 1} or hash if longer 
	constexpr sid::SID operator"" _SIDBIN(const char* data, size_t size) noexcept
	{
		return sid::packBin(data, size);
	}


	//SID = 
	constexpr sid::SID operator"" _SIDDEC(uint64_t number) noexcept
	{
		return sid::packDec(number);
	}

	//SID = packed up to 17 { \0, 0-9 } or hash if longer 
	constexpr sid::SID operator"" _SIDDEC(const char* data, size_t size) noexcept
	{
		return sid::packDec(data, size);
	}


	//SID =
	constexpr sid::SID operator"" _SIDHEX(uint64_t number) noexcept
	{
		return sid::packHex(number);
	}

	//SID = packed up to 14 { \0, 0-9, a-f, A-F } or hash if longer 
	constexpr sid::SID operator"" _SIDHEX(const char* data, size_t size) noexcept
	{
		return sid::packHex(data, size);
	}


	//SID = packed up to 12 alphanumeric (ignore case) or hash if longer 
	constexpr sid::SID operator"" _SIDALNUM(const char* data, size_t size) noexcept
	{
		return sid::packAlnum(data, size);
	}

	//SID = packed up to 10 character from base64 or hash if longer 
	constexpr sid::SID operator"" _SIDBASE64(const char* data, size_t size) noexcept
	{
		return sid::packB64(data, size);
	}

	//SID = packed up to 8 ascii or hash if longer 
	constexpr sid::SID operator"" _SIDASCII(const char* data, size_t size) noexcept
	{
		return sid::packAscii(data, size);
	}

	//SID = packed up to 7 byte or hash if longer 
	constexpr sid::SID operator"" _SIDBYTE(const char* data, size_t size) noexcept
	{
		return sid::packByte(data, size);
	}




	//configurable name of extra literals

#ifndef SID_EXTRA_NUM_LITERAL_NAME_0
#define SID_EXTRA_NUM_LITERAL_NAME_0 _SIDXN0
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_1
#define SID_EXTRA_NUM_LITERAL_NAME_1 _SIDXN1
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_2
#define SID_EXTRA_NUM_LITERAL_NAME_2 _SIDXN2
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_3
#define SID_EXTRA_NUM_LITERAL_NAME_3 _SIDXN3
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_4
#define SID_EXTRA_NUM_LITERAL_NAME_4 _SIDXN4
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_5
#define SID_EXTRA_NUM_LITERAL_NAME_5 _SIDXN5
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_6
#define SID_EXTRA_NUM_LITERAL_NAME_6 _SIDXN6
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_7
#define SID_EXTRA_NUM_LITERAL_NAME_7 _SIDXN7
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_8
#define SID_EXTRA_NUM_LITERAL_NAME_8 _SIDXN8
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_9
#define SID_EXTRA_NUM_LITERAL_NAME_9 _SIDXN9
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_10
#define SID_EXTRA_NUM_LITERAL_NAME_10 _SIDXN10
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_11
#define SID_EXTRA_NUM_LITERAL_NAME_11 _SIDXN11
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_12
#define SID_EXTRA_NUM_LITERAL_NAME_12 _SIDXN12
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_13
#define SID_EXTRA_NUM_LITERAL_NAME_13 _SIDXN13
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_14
#define SID_EXTRA_NUM_LITERAL_NAME_14 _SIDXN14
#endif

#ifndef SID_EXTRA_NUM_LITERAL_NAME_15
#define SID_EXTRA_NUM_LITERAL_NAME_15 _SIDXN15
#endif






#ifndef SID_EXTRA_TXT_LITERAL_NAME_0
#define SID_EXTRA_TXT_LITERAL_NAME_0 _SIDXT0
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_1
#define SID_EXTRA_TXT_LITERAL_NAME_1 _SIDXT1
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_2
#define SID_EXTRA_TXT_LITERAL_NAME_2 _SIDXT2
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_3
#define SID_EXTRA_TXT_LITERAL_NAME_3 _SIDXT3
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_4
#define SID_EXTRA_TXT_LITERAL_NAME_4 _SIDXT4
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_5
#define SID_EXTRA_TXT_LITERAL_NAME_5 _SIDXT5
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_6
#define SID_EXTRA_TXT_LITERAL_NAME_6 _SIDXT6
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_7
#define SID_EXTRA_TXT_LITERAL_NAME_7 _SIDXT7
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_8
#define SID_EXTRA_TXT_LITERAL_NAME_8 _SIDXT8
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_9
#define SID_EXTRA_TXT_LITERAL_NAME_9 _SIDXT9
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_10
#define SID_EXTRA_TXT_LITERAL_NAME_10 _SIDXT10
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_11
#define SID_EXTRA_TXT_LITERAL_NAME_11 _SIDXT11
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_12
#define SID_EXTRA_TXT_LITERAL_NAME_12 _SIDXT12
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_13
#define SID_EXTRA_TXT_LITERAL_NAME_13 _SIDXT13
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_14
#define SID_EXTRA_TXT_LITERAL_NAME_14 _SIDXT14
#endif

#ifndef SID_EXTRA_TXT_LITERAL_NAME_15
#define SID_EXTRA_TXT_LITERAL_NAME_15 _SIDXT15
#endif







#ifndef SID_EXTRA_HSH_LITERAL_NAME_0
#define SID_EXTRA_HSH_LITERAL_NAME_0 _SIDXH0
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_1
#define SID_EXTRA_HSH_LITERAL_NAME_1 _SIDXH1
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_2
#define SID_EXTRA_HSH_LITERAL_NAME_2 _SIDXH2
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_3
#define SID_EXTRA_HSH_LITERAL_NAME_3 _SIDXH3
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_4
#define SID_EXTRA_HSH_LITERAL_NAME_4 _SIDXH4
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_5
#define SID_EXTRA_HSH_LITERAL_NAME_5 _SIDXH5
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_6
#define SID_EXTRA_HSH_LITERAL_NAME_6 _SIDXH6
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_7
#define SID_EXTRA_HSH_LITERAL_NAME_7 _SIDXH7
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_8
#define SID_EXTRA_HSH_LITERAL_NAME_8 _SIDXH8
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_9
#define SID_EXTRA_HSH_LITERAL_NAME_9 _SIDXH9
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_10
#define SID_EXTRA_HSH_LITERAL_NAME_10 _SIDXH10
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_11
#define SID_EXTRA_HSH_LITERAL_NAME_11 _SIDXH11
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_12
#define SID_EXTRA_HSH_LITERAL_NAME_12 _SIDXH12
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_13
#define SID_EXTRA_HSH_LITERAL_NAME_13 _SIDXH13
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_14
#define SID_EXTRA_HSH_LITERAL_NAME_14 _SIDXH14
#endif

#ifndef SID_EXTRA_HSH_LITERAL_NAME_15
#define SID_EXTRA_HSH_LITERAL_NAME_15 _SIDXH15
#endif




	//define user defined literals for extra number (as int or string)

#define SIDLITERALNUM(NUMINDEX, LITERALNAME) \
	constexpr sid::SID operator"" LITERALNAME(uint64_t number) noexcept\
	{\
		return sid::packExtraNum<NUMINDEX>(number);\
	}\
	constexpr sid::SID operator"" LITERALNAME(const char* data, size_t size) noexcept\
	{\
		return sid::packExtraNum<NUMINDEX>(data, size);\
	}

#if SID_EXTRA_NUM_COUNT>0
	SIDLITERALNUM(0, SID_EXTRA_NUM_LITERAL_NAME_0)
#endif

#if SID_EXTRA_NUM_COUNT>1
		SIDLITERALNUM(1, SID_EXTRA_NUM_LITERAL_NAME_1)
#endif

#if SID_EXTRA_NUM_COUNT>2
		SIDLITERALNUM(2, SID_EXTRA_NUM_LITERAL_NAME_2)
#endif

#if SID_EXTRA_NUM_COUNT>3
		SIDLITERALNUM(3, SID_EXTRA_NUM_LITERAL_NAME_3)
#endif

#if SID_EXTRA_NUM_COUNT>4
		SIDLITERALNUM(4, SID_EXTRA_NUM_LITERAL_NAME_4)
#endif

#if SID_EXTRA_NUM_COUNT>5
		SIDLITERALNUM(5, SID_EXTRA_NUM_LITERAL_NAME_5)
#endif

#if SID_EXTRA_NUM_COUNT>6
		SIDLITERALNUM(6, SID_EXTRA_NUM_LITERAL_NAME_6)
#endif

#if SID_EXTRA_NUM_COUNT>7
		SIDLITERALNUM(7, SID_EXTRA_NUM_LITERAL_NAME_7)
#endif

#if SID_EXTRA_NUM_COUNT>8
		SIDLITERALNUM(8, SID_EXTRA_NUM_LITERAL_NAME_8)
#endif

#if SID_EXTRA_NUM_COUNT>9
		SIDLITERALNUM(9, SID_EXTRA_NUM_LITERAL_NAME_9)
#endif

#if SID_EXTRA_NUM_COUNT>10
		SIDLITERALNUM(10, SID_EXTRA_NUM_LITERAL_NAME_10)
#endif

#if SID_EXTRA_NUM_COUNT>11
		SIDLITERALNUM(11, SID_EXTRA_NUM_LITERAL_NAME_11)
#endif

#if SID_EXTRA_NUM_COUNT>12
		SIDLITERALNUM(12, SID_EXTRA_NUM_LITERAL_NAME_12)
#endif

#if SID_EXTRA_NUM_COUNT>13
		SIDLITERALNUM(13, SID_EXTRA_NUM_LITERAL_NAME_13)
#endif

#if SID_EXTRA_NUM_COUNT>14
		SIDLITERALNUM(14, SID_EXTRA_NUM_LITERAL_NAME_14)
#endif

#if SID_EXTRA_NUM_COUNT>15
		SIDLITERALNUM(15, SID_EXTRA_NUM_LITERAL_NAME_15)
#endif

#undef SIDLITERALNUM


		

		// define user defined literals for extra text

#define SIDLITERALTEX(TXTINDEX, LITERALNAME) \
	constexpr sid::SID operator"" LITERALNAME(const char* data, size_t size) noexcept\
	{\
		return sid::packExtraText<TXTINDEX>(data, size);\
	}

#if SID_EXTRA_TXT_COUNT>0
		SIDLITERALTEX(0, SID_EXTRA_TXT_LITERAL_NAME_0)
#endif

#if SID_EXTRA_TXT_COUNT>1
		SIDLITERALTEX(1, SID_EXTRA_TXT_LITERAL_NAME_1)
#endif

#if SID_EXTRA_TXT_COUNT>2
		SIDLITERALTEX(2, SID_EXTRA_TXT_LITERAL_NAME_2)
#endif

#if SID_EXTRA_TXT_COUNT>3
		SIDLITERALTEX(3, SID_EXTRA_TXT_LITERAL_NAME_3)
#endif

#if SID_EXTRA_TXT_COUNT>4
		SIDLITERALTEX(4, SID_EXTRA_TXT_LITERAL_NAME_4)
#endif

#if SID_EXTRA_TXT_COUNT>5
		SIDLITERALTEX(5, SID_EXTRA_TXT_LITERAL_NAME_5)
#endif

#if SID_EXTRA_TXT_COUNT>6
		SIDLITERALTEX(6, SID_EXTRA_TXT_LITERAL_NAME_6)
#endif

#if SID_EXTRA_TXT_COUNT>7
		SIDLITERALTEX(7, SID_EXTRA_TXT_LITERAL_NAME_7)
#endif

#if SID_EXTRA_TXT_COUNT>8
		SIDLITERALTEX(8, SID_EXTRA_TXT_LITERAL_NAME_8)
#endif

#if SID_EXTRA_TXT_COUNT>9
		SIDLITERALTEX(9, SID_EXTRA_TXT_LITERAL_NAME_9)
#endif

#if SID_EXTRA_TXT_COUNT>10
		SIDLITERALTEX(10, SID_EXTRA_TXT_LITERAL_NAME_10)
#endif

#if SID_EXTRA_TXT_COUNT>11
		SIDLITERALTEX(11, SID_EXTRA_TXT_LITERAL_NAME_11)
#endif

#if SID_EXTRA_TXT_COUNT>12
		SIDLITERALTEX(12, SID_EXTRA_TXT_LITERAL_NAME_12)
#endif

#if SID_EXTRA_TXT_COUNT>13
		SIDLITERALTEX(13, SID_EXTRA_TXT_LITERAL_NAME_13)
#endif

#if SID_EXTRA_TXT_COUNT>14
		SIDLITERALTEX(14, SID_EXTRA_TXT_LITERAL_NAME_14)
#endif

#if SID_EXTRA_TXT_COUNT>15
		SIDLITERALTEX(15, SID_EXTRA_TXT_LITERAL_NAME_15)
#endif

#undef SIDLITERALTEX



			//define user defined literals for extra hash (as int or string)

#define SIDLITERALHSH(HSHINDEX, LITERALNAME) \
	constexpr sid::SID operator"" LITERALNAME(uint64_t number) noexcept\
	{\
		return sid::packExtraHash<HSHINDEX>(number);\
	}\
	constexpr sid::SID operator"" LITERALNAME(const char* data, size_t size) noexcept\
	{\
		return sid::packExtraHash<HSHINDEX>(data, size);\
	}

#if SID_EXTRA_HSH_COUNT>0
			SIDLITERALHSH(0, SID_EXTRA_HSH_LITERAL_NAME_0)
#endif

#if SID_EXTRA_HSH_COUNT>1
			SIDLITERALHSH(1, SID_EXTRA_HSH_LITERAL_NAME_1)
#endif

#if SID_EXTRA_HSH_COUNT>2
			SIDLITERALHSH(2, SID_EXTRA_HSH_LITERAL_NAME_2)
#endif

#if SID_EXTRA_HSH_COUNT>3
			SIDLITERALHSH(3, SID_EXTRA_HSH_LITERAL_NAME_3)
#endif

#if SID_EXTRA_HSH_COUNT>4
			SIDLITERALHSH(4, SID_EXTRA_HSH_LITERAL_NAME_4)
#endif

#if SID_EXTRA_HSH_COUNT>5
			SIDLITERALHSH(5, SID_EXTRA_HSH_LITERAL_NAME_5)
#endif

#if SID_EXTRA_HSH_COUNT>6
			SIDLITERALHSH(6, SID_EXTRA_HSH_LITERAL_NAME_6)
#endif

#if SID_EXTRA_HSH_COUNT>7
			SIDLITERALHSH(7, SID_EXTRA_HSH_LITERAL_NAME_7)
#endif

#if SID_EXTRA_HSH_COUNT>8
			SIDLITERALHSH(8, SID_EXTRA_HSH_LITERAL_NAME_8)
#endif

#if SID_EXTRA_HSH_COUNT>9
			SIDLITERALHSH(9, SID_EXTRA_HSH_LITERAL_NAME_9)
#endif

#if SID_EXTRA_HSH_COUNT>10
			SIDLITERALHSH(10, SID_EXTRA_HSH_LITERAL_NAME_10)
#endif

#if SID_EXTRA_HSH_COUNT>11
			SIDLITERALHSH(11, SID_EXTRA_HSH_LITERAL_NAME_11)
#endif

#if SID_EXTRA_HSH_COUNT>12
			SIDLITERALHSH(12, SID_EXTRA_HSH_LITERAL_NAME_12)
#endif

#if SID_EXTRA_HSH_COUNT>13
			SIDLITERALHSH(13, SID_EXTRA_HSH_LITERAL_NAME_13)
#endif

#if SID_EXTRA_HSH_COUNT>14
			SIDLITERALHSH(14, SID_EXTRA_HSH_LITERAL_NAME_14)
#endif

#if SID_EXTRA_HSH_COUNT>15
			SIDLITERALHSH(15, SID_EXTRA_HSH_LITERAL_NAME_15)
#endif

#undef SIDLITERALHSH















#undef SID_FIRST_EXTRA_NUM
#undef SID_FIRST_EXTRA_TXT
#undef SID_FIRST_EXTRA_HSH
#undef SID_FIRST_EXTRA_END


















// ----- tests -----


constexpr auto hashhashteste = 0x0123'4567'8987'6542_SIDHASH;
constexpr auto hashhashtestd = sid::unpack(hashhashteste);

constexpr auto hashteste = ""_SIDHASH;
constexpr auto hashtestd = sid::unpack(hashteste);

constexpr auto hashtest1e = "\0\0\0"_SIDHASH;
constexpr auto hashtest1d = sid::unpack(hashtest1e);

constexpr auto hashtest2e = "\1\0\0"_SIDHASH;
constexpr auto hashtest2d = sid::unpack(hashtest2e);

constexpr auto hashtest3e = "abcdefghijklmnoprstuwxyzabcdefghijklmnoprstuwxyzabcdefghijklmnoprstuwxyzabcdefghijklmnoprstuwxyz"_SIDHASH;
constexpr auto hashtest3d = sid::unpack(hashtest3e);


constexpr auto binteste = "1.10.0111111..1111111111000000000001111111111111111000111001"_SIDBIN;
constexpr auto bintestd = unpack(binteste);


constexpr uint64_t binnum = 0b101000111111001111111111000000000001111111111111111000111001;
constexpr auto bintest2e = 0b101000111111001111111111000000000001111111111111111000111001_SIDBIN;
constexpr auto bintest2d = unpack(bintest2e);
constexpr auto bintest2dn = unpackNumber(bintest2e);

constexpr auto decteste = "12345678901111119"_SIDDEC;
constexpr auto dectestd = unpack(decteste);

constexpr auto dectest2e = 12345678911111119_SIDDEC;
constexpr auto dectest2d = unpack(dectest2e);
constexpr auto dectest2dn = unpackNumber(dectest2e);


constexpr auto hexteste = "0123789ABCdefa"_SIDHEX;
constexpr auto hextestd = unpack(hexteste);

constexpr auto hextest2e = 0x0123789ABCdefa_SIDHEX;
constexpr auto hextest2d = unpack(hextest2e);

constexpr auto hextest2dn = unpackNumber(hextest2e);



constexpr auto alnumteste = "Updt<(Player"_SIDALNUM;
constexpr auto alnumtestd = unpack(alnumteste);
constexpr auto alnumtestdt = unpackText(alnumteste);

constexpr auto base64teste = "Up\nt<(Play"_SIDBASE64;
constexpr auto base64testd = unpack(base64teste);
constexpr auto base64testdt = unpackText(base64teste);

constexpr auto asciiteste = "Up\nt<(\x83l"_SIDASCII;
constexpr auto asciitestd = unpack(asciiteste);
constexpr auto asciitestdt = unpackText(asciiteste);

constexpr auto byteteste = "U\nt<(\x83l"_SIDBYTE;
constexpr auto bytetestd = unpack(byteteste);
constexpr auto bytetestdt = unpackText(byteteste);


const uint64_t tab[] = {
	"0000"_SIDBIN.val % 256,
	"0001"_SIDBIN.val % 256,
	"0010"_SIDBIN.val % 256,
	"0011"_SIDBIN.val % 256,
	"0100"_SIDBIN.val % 256,
	"0101"_SIDBIN.val % 256,
	"0110"_SIDBIN.val % 256,
	"0111"_SIDBIN.val % 256,
	"1000"_SIDBIN.val % 256,
	"1001"_SIDBIN.val % 256,
	"1010"_SIDBIN.val % 256,
	"1011"_SIDBIN.val % 256,
	"1100"_SIDBIN.val % 256,
	"1101"_SIDBIN.val % 256,
	"1110"_SIDBIN.val % 256,
	"1111"_SIDBIN.val % 256
};



	constexpr auto bintest8e = "000001111000011011110"_SIDBIN;
	constexpr auto bintest8d = unpack(bintest8e);

	constexpr auto bintest9e = 61662_SIDBIN;
	constexpr auto bintest9d = unpack(bintest9e);


	constexpr bool fdshfkjasdhfkjdsh = (bintest9e == bintest8e);


	constexpr auto  gf1 = sid::unpackNumber(0b010001100010010010001010101000101_SIDBIN);
	constexpr auto  gf2 = sid::unpackNumber("010001100010010010001010101000101"_SIDBIN);


	constexpr auto testxn0Ae = 61662_SIDXN0;
	constexpr auto testxn0Ad = unpack(testxn0Ae);

	constexpr auto testxn0Be = "00011110100011111111100111001001011001010100100001010100"_SIDXN0;
	constexpr auto testxn0Bd = unpack(testxn0Be);



	constexpr auto testxt0Ae = "H1lloWorl"_SIDXT2;
	constexpr auto testxt0Ad = unpack(testxt0Ae);

	constexpr auto testxh0Ae = "HelloWorl"_SIDXH2;
	constexpr auto testxh0Ad = unpack(testxh0Ae);

	constexpr auto testxh0Be = 0x61662_SIDXH3;
	constexpr auto testxh0Bd = unpack(testxh0Be);




	////LUT 197<->byte
	//constexpr ::std::array<uint8_t, 256> getCharTo197() noexcept
	//{
	//	::std::array<uint8_t, 256> tab = {};
	//	uint8_t code = 0;
	//
	//	for (size_t i = 0; i < 197; i++)
	//		tab[i] = i;
	//
	//	return tab;
	//}
	//constexpr inline ::std::array<uint8_t, 256> lutCharTo197 = getCharTo197();
	//
	//constexpr ::std::array<char, 197> get197ToChar() noexcept
	//{
	//	::std::array<char, 197> tab = {};
	//	uint8_t code = 0;
	//
	//	for (size_t i = 0; i < 197; i++)
	//		tab[code++] = i;
	//
	//	return tab;
	//}
	//constexpr inline ::std::array<char, 197> lut197ToChar = get197ToChar();
	//
	//
	////LUT 379<->byte
	//constexpr ::std::array<uint16_t, 256> getCharTo379() noexcept
	//{
	//	::std::array<uint16_t, 256> tab = {};
	//	uint16_t code = 0;
	//
	//	for (size_t i = 0; i < 256; i++)
	//		tab[i] = i;
	//
	//	return tab;
	//}
	//constexpr inline ::std::array<uint16_t, 256> lutCharTo379 = getCharTo379();
	//
	//constexpr ::std::array<char, 379> get379ToChar() noexcept
	//{
	//	::std::array<char, 379> tab = {};
	//	uint8_t code = 0;
	//
	//	for (size_t i = 0; i < 256; i++)
	//		tab[code++] = i;
	//
	//	return tab;
	//}
	//constexpr inline ::std::array<char, 379> lut379ToChar = get379ToChar();
	////pack or hash string to 64bit
	//constexpr SID packSID(const char* data, size_t size) noexcept {
	//	if (data == nullptr) size = 0;
	//
	//	switch (size)
	//	{
	//	case 12:
	//	case 11:
	//	case 0:
	//	{
	//		uint64_t hash = 0;
	//		for (size_t i = 0; i < size; ++i)
	//			hash = hash * 37 + lutCharTo37[data[i]];
	//		for (size_t i = size; i < 12; ++i)
	//			hash = hash * 37 + 0;
	//		return { hash * prime64 };
	//	}
	//	case 10:
	//	case 9:
	//	{
	//		uint64_t hash = 0;
	//		for (size_t i = 0; i < size; ++i)
	//			hash = hash * 73 + lutCharTo73[data[i]];
	//		for (size_t i = size; i < 10; ++i)
	//			hash = hash * 73 + 0;
	//		return { (hash | (uint64_t(1) << 63)) * prime64 };
	//	}
	//	case 8:
	//	{
	//		uint64_t hash = 0;
	//		for (size_t i = 0; i < size; ++i)
	//			hash = hash * 197 + lutCharTo197[data[i]];
	//		return { (hash | (uint64_t(0b11) << 62)) * prime64 };
	//	}
	//	case 7:
	//	case 6:
	//	case 5:
	//	case 4:
	//	case 3:
	//	case 2:
	//	case 1:
	//	{
	//		uint64_t hash = 0;
	//		for (size_t i = 0; i < size; ++i)
	//			hash = hash * 379 + lutCharTo379[data[i]];
	//		for (size_t i = size; i < 7; ++i)
	//			hash = hash * 379 + 0;
	//		return { (hash | (uint64_t(0b111) << 61)) * prime64 };
	//	}
	//	default:
	//	{
	//		constexpr auto lastbit = (uint64_t(0b1111) << 60);
	//		return { (crc64(data, size) | lastbit) * prime64 };
	//	}
	//	}
	//}
	//
	////unpack or print hash as hex 64bit to 15 char string 
	//constexpr ::std::array<char, 16> unpackSID(const SID& sid) noexcept {
	//	::std::array<char, 16> text = {};
	//	uint64_t hsh = sid.val * invprime64;
	//
	//	if ((hsh & (uint64_t(1) << 63)) == 0)
	//	{
	//		//37
	//		for (intptr_t i = 12; i >= 0; --i)
	//		{
	//			uint8_t code = uint8_t(hsh % 37);
	//			hsh = hsh / 37;
	//			text[i] = lut37ToChar[code];
	//		}
	//	}
	//	else if ((hsh & (uint64_t(1) << 62)) == 0)
	//	{
	//		//73
	//		hsh &= ~(uint64_t(0b1) << 63);
	//		for (intptr_t i = 10; i >= 0; --i)
	//		{
	//			uint8_t code = uint8_t(hsh % 73);
	//			hsh = hsh / 73;
	//			text[i] = lut73ToChar[code];
	//		}
	//	}
	//	else if ((hsh & (uint64_t(1) << 61)) == 0)
	//	{
	//		//197
	//		hsh &= ~(uint64_t(0b11) << 62);
	//		for (intptr_t i = 8; i >= 0; --i)
	//		{
	//			uint8_t code = uint8_t(hsh % 197);
	//			hsh = hsh / 197;
	//			text[i] = lut197ToChar[code];
	//		}
	//	}
	//	else if ((hsh & (uint64_t(1) << 60)) == 0)
	//	{
	//		//379
	//		hsh &= ~(uint64_t(0b111) << 61);
	//		for (intptr_t i = 7; i >= 0; --i)
	//		{
	//			uint8_t code = uint8_t(hsh % 379);
	//			hsh = hsh / 379;
	//			text[i] = lut379ToChar[code];
	//		}
	//	}
	//	else
	//	{
	//		//crc
	//		hsh &= ~(uint64_t(0b1111) << 60);
	//		for (intptr_t i = 14; i >= 0; --i)
	//		{
	//			uint8_t code = uint8_t(hsh % 16);
	//			hsh = hsh / 16;
	//			text[i] = "0123456789ABCDEF"[code];
	//		}
	//	}
	//	return text;
	//}
	//
	//constexpr SID operator"" _SID(const char* data, size_t size) noexcept
	//{
	//	return packSID(data, size);
	//}
	//
	//
	//constexpr auto dsadsad = "P1Velo3f"_SID;
	//constexpr auto asdjasdl = unpackSID(dsadsad);
	//
	//
	//constexpr auto dsadsad1 = "aa"_SID.val %128;
	//constexpr auto dsadsad2 = "ac"_SID.val %128;
	//constexpr auto dsadsad3 = "ca"_SID.val %128;
	//constexpr auto dsadsad4 = "cc"_SID.val %128;



/*
///////////////////// TESTS /////////////////////

constexpr uint64_t axdsfdfsfh = "P3Update1234"_SID;
constexpr auto fsdfjyhsdk = unpackSID({ axdsfdfsfh });


constexpr auto sadkjfhjhaks3 = unpackSID("UpdateObject"_SID);
constexpr auto hsdiudsyfiu = "UpdateP8732948739248723947328259635"_SID;
constexpr auto sadkjfhjhaks1 = unpackSID(packSID("00000000000000000000000000000000", 12));
constexpr auto sadkjfhjhaks2 = unpackSID(packSID("00000000000000000000000000000000", 22));
constexpr auto uryuivx1 = packSID("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16);
constexpr auto uryuivx2 = packSID("HELLOWORLDDDDDD^^^^^", 11);

const auto dsfjhsad1 = uryuivx1.val % 1024;
const auto dsfjhsad2 = uryuivx2.val % 1024;

constexpr auto dfs = (("abcAAAAAAA#"_SID) == ("ABCAAAAAAA\0.."_SID));

const uint64_t sdfhsadjfhsak = 123 % 0;
constexpr uint64_t pe[] = {
	""_SID.val % 1024,
	" "_SID.val % 1024,
	"0"_SID.val % 1024,
	"1"_SID.val % 1024,
	"2"_SID.val % 1024,
	"3"_SID.val % 1024,
	"4"_SID.val % 1024,
	"5"_SID.val % 1024,
	"6"_SID.val % 1024,
	"7"_SID.val % 1024,
	"8"_SID.val % 1024,
	"9"_SID.val % 1024,
	"A"_SID.val % 1024,
	"B"_SID.val % 1024,
	"hello"_SID.val % 1024,
	"heloo"_SID.val % 1024,
	"world"_SID.val % 1024,
	"word"_SID.val % 1024,
	"foo"_SID.val % 1024,
	"bar"_SID.val % 1024,
	"update"_SID.val % 1024,
	"draw"_SID.val % 1024,
	"position"_SID.val % 1024,
	"velocity"_SID.val % 1024,
	"orientation"_SID.val % 1024,
	"health"_SID.val % 1024,
	"weapon"_SID.val % 1024,
	"direction"_SID.val % 1024,
	"radius"_SID.val % 1024,
	"angle"_SID.val % 1024,
	"mass"_SID.val % 1024,
	"force"_SID.val % 1024,
	"acceleration"_SID.val % 1024,
	"mesh"_SID.val % 1024,
	"texture"_SID.val % 1024,
	"audio"_SID.val % 1024,
	"music"_SID.val % 1024,
	"asset"_SID.val % 1024,
	"geometry"_SID.val % 1024,
	"shit"_SID.val % 1024,
	"shit2"_SID.val % 1024,
	"wheel"_SID.val % 1024,
	"source"_SID.val % 1024
};
*/


/*
template <size_t Size>
struct SIDN
{
	::std::array<uint64_t, Size> val;

	constexpr bool operator==(const SIDN& b) const noexcept
	{
		for (size_t i = 0; i < val.size(); ++i)
		{
			if (val[i] != b.val[i]) return false;
		}
		return true;
	}
	constexpr bool operator!=(const SIDN& b) const noexcept
	{
		return !(*this == b);
	}
	constexpr bool operator<(const SIDN& b) const noexcept
	{
		for (size_t i = 0; i < val.size(); ++i)
		{
			if (val[i] < b.val[i]) return true;
			if (b.val[i] < val[i]) return false;
		}
		return false;
	}
	constexpr bool operator<=(const SIDN& b) const noexcept
	{
		for (size_t i = 0; i < val.size(); ++i)
		{
			if (val[i] < b.val[i]) return true;
			if (b.val[i] < val[i]) return false;
		}
		return true;
	}
	constexpr bool operator>(const SIDN& b) const noexcept
	{
		return !(*this <= b);
	}
	constexpr bool operator>=(const SIDN& b) const noexcept
	{
		return !(*this < b);
	}

	constexpr ::std::array<uint8_t, Size * 8> asU8() const noexcept
	{
		::std::array<uint8_t, Size * 8> u = {};
		for (size_t i = 0; i < Size; ++i)
		{
			for (size_t j = 0; j < 8; ++j)
			{
				u[i * 8 + j] = (val[i] >> j*8)&0xFF;
			}
		}
		return u;
	}
	constexpr const ::std::array<char, Size * 12+2> unpack() const noexcept {
		::std::array<char, Size * 12+2> text = {};
		uint64_t hsh = val[0]* 2356850971703802821;
		for (intptr_t i = Size * 12; i >= 0; --i)
		{
			uint8_t code =  uint8_t(hsh % 37);
			hsh = hsh / 37;
			if (code == 0)
				text[i] = '_';			//0 -> _
			else if (code <= 10)
				text[i] = code + ('0' - 1);	//1-10 -> 0-9
			else// if (code <= 36)
				text[i] = code + ('A' - 11);//11-36 -> A-Z
			//else
			//	text[i] = '_';
		}
		return text;
	}

	constexpr operator uint64_t() const noexcept { return val[0]; }
};

using SID64 = SIDN<1>;
using SID128 = SIDN<2>;
using SID192 = SIDN<3>;
using SID256 = SIDN<4>;

union u64x1Tou8x8
{
	uint64_t u64[1];
	uint8_t u8[8];
};
union u64x2Tou8x16
{
	uint64_t u64[2];
	uint8_t u8[16];
};
union u64x3Tou8x24
{
	uint64_t u64[3];
	uint8_t u8[24];
};
union u64x4Tou8x32
{
	uint64_t u64[4];
	uint8_t u8[32];
};
#ifndef NDEBUG
	#define SIDTEXTENCODE 1
#else
	#define SIDTEXTENCODE 1
#endif
constexpr SID64 operator"" _SID64(const char* data, size_t size)
{
	const auto crc1 = crc64(data, size, 1);
#if SIDTEXTENCODE==0
	return  { crc1 };
#else
	constexpr size_t bytes = sizeof(SID64);
	uint8_t text[bytes] = {};
	const uint8_t crc[bytes] = {crc1, crc1 >> 8 , crc1 >> 16, crc1 >> 24, crc1 >> 32, crc1 >> 40, crc1 >> 48, crc1 >> 56 };
	constexpr size_t i0 = 1, i1 = 3, i2 = 4;
	for (size_t i = 0; i < i0; i++)
	{
		text[i] = (i < size ? data[i] : crc[i]);
	}
	for (size_t i = i0; i < i0+ i1; i++)
	{
		text[i] = crc[i];
	}
	for (size_t i = 0; i < i2; i++)
	{
		text[bytes - 1 - i] = (i < size ? data[size-1 - i] : crc[i0 + i1 + i]);
	}
	return  {
		uint64_t(text[0])  |
		(uint64_t(text[1])<< 8) |
		(uint64_t(text[2])<<16) |
		(uint64_t(text[3])<<24) |
		(uint64_t(text[4])<<32) |
		(uint64_t(text[5])<<40) |
		(uint64_t(text[6])<<48) |
		(uint64_t(text[7])<<56)
	};
#endif
}
constexpr SID128 operator"" _SID128(const char* data, size_t size)
{
	const auto crc1 = crc64(data, size, 1);
	const auto crc2 = crc64WE(data, size, 2);
#if SIDTEXTENCODE==0
	return  { crc1, crc2 };
#else
	constexpr size_t bytes = sizeof(SID128);
	uint8_t text[bytes] = {};
	const uint8_t crc[bytes] = {
		crc1, crc1 >> 8 , crc1 >> 16, crc1 >> 24, crc1 >> 32, crc1 >> 40, crc1 >> 48, crc1 >> 56,
		crc2, crc2 >> 8 , crc2 >> 16, crc2 >> 24, crc2 >> 32, crc2 >> 40, crc2 >> 48, crc2 >> 56,
	};
	constexpr size_t i0 = 2, i1 = 6, i2 = 8;
	for (size_t i = 0; i < i0; i++)
	{
		text[i] = (i < size ? data[i] : crc[i]);
	}
	for (size_t i = i0; i < i0 + i1; i++)
	{
		text[i] = crc[i];
	}
	for (size_t i = 0; i < i2; i++)
	{
		text[bytes - 1 - i] = (i < size ? data[size - 1 - i] : crc[i0 + i1 + i]);
	}
	return  {
		(uint64_t(text[0])) |
		(uint64_t(text[1]) << 8) |
		(uint64_t(text[2]) << 16) |
		(uint64_t(text[3]) << 24) |
		(uint64_t(text[4]) << 32) |
		(uint64_t(text[5]) << 40) |
		(uint64_t(text[6]) << 48) |
		(uint64_t(text[7]) << 56),
		(uint64_t(text[8])) |
		(uint64_t(text[9]) << 8) |
		(uint64_t(text[10]) << 16) |
		(uint64_t(text[11]) << 24) |
		(uint64_t(text[12]) << 32) |
		(uint64_t(text[13]) << 40) |
		(uint64_t(text[14]) << 48) |
		(uint64_t(text[15]) << 56)
	};
#endif
	//return  { crc64(data, size, 1), crc64WE(data, size, 2) };
}
constexpr SID192 operator"" _SID192(const char* data, size_t size)
{
	const auto crc1 = crc64(data, size, 1);
	const auto crc2 = crc64WE(data, size, 2);
	const auto crc3 = crc64(data + 1, size - 1, 2);
#if SIDTEXTENCODE==0
	return  { crc1, crc2, crc3 };
#else
	constexpr size_t bytes = sizeof(SID192);
	uint8_t text[bytes] = {};
	const uint8_t crc[bytes] = {
		crc1, crc1 >> 8 , crc1 >> 16, crc1 >> 24, crc1 >> 32, crc1 >> 40, crc1 >> 48, crc1 >> 56,
		crc2, crc2 >> 8 , crc2 >> 16, crc2 >> 24, crc2 >> 32, crc2 >> 40, crc2 >> 48, crc2 >> 56,
		crc3, crc3 >> 8 , crc3 >> 16, crc3 >> 24, crc3 >> 32, crc3 >> 40, crc3 >> 48, crc3 >> 56
	};
	constexpr size_t i0 = 3, i1 = 9, i2 = 12;
	for (size_t i = 0; i < i0; i++)
	{
		text[i] = (i < size ? data[i] : crc[i]);
	}
	for (size_t i = i0; i < i0 + i1; i++)
	{
		text[i] = crc[i];
	}
	for (size_t i = 0; i < i2; i++)
	{
		text[bytes - 1 - i] = (i < size ? data[size - 1 - i] : crc[i0 + i1 + i]);
	}
	return  {
		(uint64_t(text[0])) |
		(uint64_t(text[1]) << 8) |
		(uint64_t(text[2]) << 16) |
		(uint64_t(text[3]) << 24) |
		(uint64_t(text[4]) << 32) |
		(uint64_t(text[5]) << 40) |
		(uint64_t(text[6]) << 48) |
		(uint64_t(text[7]) << 56),
		(uint64_t(text[8])) |
		(uint64_t(text[9]) << 8) |
		(uint64_t(text[10]) << 16) |
		(uint64_t(text[11]) << 24) |
		(uint64_t(text[12]) << 32) |
		(uint64_t(text[13]) << 40) |
		(uint64_t(text[14]) << 48) |
		(uint64_t(text[15]) << 56),
		(uint64_t(text[16])) |
		(uint64_t(text[17]) << 8) |
		(uint64_t(text[18]) << 16) |
		(uint64_t(text[19]) << 24) |
		(uint64_t(text[20]) << 32) |
		(uint64_t(text[21]) << 40) |
		(uint64_t(text[22]) << 48) |
		(uint64_t(text[23]) << 56)
	};
#endif
	//return  { crc64(data, size, 1), crc64WE(data, size, 2), crc64(data + 1, size - 1, 2) };
}
constexpr SID256 operator"" _SID256(const char* data, size_t size)
{
	const auto crc1 = crc64(data, size, 1);
	const auto crc2 = crc64WE(data, size, 2);
	const auto crc3 = crc64(data + 1, size - 1, 2);
	const auto crc4 = crc64WE(data + 1, size - 1, 1);
#if SIDTEXTENCODE==0
	return  { crc1, crc2, crc3, crc4 };
#else
	constexpr size_t bytes = sizeof(SID256);
	uint8_t text[bytes] = {};
	const uint8_t crc[bytes] = {
		crc1, crc1 >> 8 , crc1 >> 16, crc1 >> 24, crc1 >> 32, crc1 >> 40, crc1 >> 48, crc1 >> 56,
		crc2, crc2 >> 8 , crc2 >> 16, crc2 >> 24, crc2 >> 32, crc2 >> 40, crc2 >> 48, crc2 >> 56,
		crc3, crc3 >> 8 , crc3 >> 16, crc3 >> 24, crc3 >> 32, crc3 >> 40, crc3 >> 48, crc3 >> 56,
		crc4, crc4 >> 8 , crc4 >> 16, crc4 >> 24, crc4 >> 32, crc4 >> 40, crc4 >> 48, crc4 >> 56
	};
	constexpr size_t i0 = 4, i1 = 12, i2 = 16;
	for (size_t i = 0; i < i0; i++)
	{
		text[i] = (i < size ? data[i] : crc[i]);
	}
	for (size_t i = i0; i < i0 + i1; i++)
	{
		text[i] = crc[i];
	}
	for (size_t i = 0; i < i2; i++)
	{
		text[bytes - 1 - i] = (i < size ? data[size - 1 - i] : crc[i0 + i1 + i]);
	}
	return  {
		(uint64_t(text[0])) |
		(uint64_t(text[1]) << 8) |
		(uint64_t(text[2]) << 16) |
		(uint64_t(text[3]) << 24) |
		(uint64_t(text[4]) << 32) |
		(uint64_t(text[5]) << 40) |
		(uint64_t(text[6]) << 48) |
		(uint64_t(text[7]) << 56),
		(uint64_t(text[8])) |
		(uint64_t(text[9]) << 8) |
		(uint64_t(text[10]) << 16) |
		(uint64_t(text[11]) << 24) |
		(uint64_t(text[12]) << 32) |
		(uint64_t(text[13]) << 40) |
		(uint64_t(text[14]) << 48) |
		(uint64_t(text[15]) << 56),
		(uint64_t(text[16])) |
		(uint64_t(text[17]) << 8) |
		(uint64_t(text[18]) << 16) |
		(uint64_t(text[19]) << 24) |
		(uint64_t(text[20]) << 32) |
		(uint64_t(text[21]) << 40) |
		(uint64_t(text[22]) << 48) |
		(uint64_t(text[23]) << 56),
		(uint64_t(text[24])) |
		(uint64_t(text[25]) << 8) |
		(uint64_t(text[26]) << 16) |
		(uint64_t(text[27]) << 24) |
		(uint64_t(text[28]) << 32) |
		(uint64_t(text[29]) << 40) |
		(uint64_t(text[30]) << 48) |
		(uint64_t(text[31]) << 56)
	};
#endif
	//return  { crc64(data, size, 1), crc64WE(data, size, 2), crc64(data + 1, size - 1, 2),crc64WE(data+1, size-1, 1) };
}

constexpr SID64 operator"" _SIDP64(const char* data, size_t size) noexcept
{
	if (size <= 12)
	{
		uint64_t hash = 0;
		for (size_t i = 0; i < size; ++i)
		{
			const uint8_t byte = data[i];
			if ((byte >= '0') && (byte <= '9'))
				hash = hash * 37 + (byte - '0') + 1;	//0-9 -> 1-10
			else if ((byte >= 'a') && (byte <= 'z'))
				hash = hash * 37 + (byte - 'a') + 11;	//a-z -> 11-36
			else if ((byte >= 'A') && (byte <= 'Z'))
				hash = hash * 37 + (byte - 'A') + 11;	//A-Z -> 11-36
			//else if (byte == '_')
			//	hash = hash * 37 + 37;
			else
				hash = hash * 37 + 0;;
			//else
			//	hash = hash * 38 + byte%38;
		}
		for (size_t i = size; i < 12; ++i)
			hash = hash * 37 + 0;
		return {hash * 2744922570705554189ull };
	}
	else
	{
		constexpr auto lastbit = (uint64_t(1) << 63);
		return { (crc64(data, size) | lastbit)* 2744922570705554189ull };
	}
}


namespace std
{
	template<size_t Size> struct hash<SIDN<Size>>
	{
		constexpr std::size_t operator()(SIDN<Size> const& s) const noexcept
		{
			return s.val[0];
		}
	};
}


constexpr auto sdahkasdjf64 = "shitake"_SID64;
constexpr auto sdahkasdjf64P = "4hel_o world"_SIDP64;
constexpr auto unpacketd64 = sdahkasdjf64P.unpack();


constexpr uint64_t pe[] = {
	""_SIDP64.val[0] % 1024,
	" "_SIDP64.val[0] % 1024,
	"0"_SIDP64.val[0] % 1024,
	"1"_SIDP64.val[0] % 1024,
	"2"_SIDP64.val[0] % 1024,
	"3"_SIDP64.val[0] % 1024,
	"4"_SIDP64.val[0] % 1024,
	"5"_SIDP64.val[0] % 1024,
	"6"_SIDP64.val[0] % 1024,
	"7"_SIDP64.val[0] % 1024,
	"8"_SIDP64.val[0] % 1024,
	"9"_SIDP64.val[0] % 1024,
	"A"_SIDP64.val[0] % 1024,
	"B"_SIDP64.val[0] % 1024,
	"hello"_SIDP64.val[0] % 1024,
	"heloo"_SIDP64.val[0] % 1024,
	"world"_SIDP64.val[0] % 1024,
	"word"_SIDP64.val[0] % 1024,
	"foo"_SIDP64.val[0] % 1024,
	"bar"_SIDP64.val[0] % 1024,
	"update"_SIDP64.val[0] % 1024,
	"draw"_SIDP64.val[0] % 1024,
	"position"_SIDP64.val[0] % 1024,
	"velocity"_SIDP64.val[0] % 1024,
	"orientation"_SIDP64.val[0] % 1024,
	"health"_SIDP64.val[0] % 1024,
	"weapon"_SIDP64.val[0] % 1024,
	"direction"_SIDP64.val[0] % 1024,
	"radius"_SIDP64.val[0] % 1024,
	"angle"_SIDP64.val[0] % 1024,
	"mass"_SIDP64.val[0] % 1024,
	"force"_SIDP64.val[0] % 1024,
	"acceleration"_SIDP64.val[0] % 1024,
	"mesh"_SIDP64.val[0] % 1024,
	"texture"_SIDP64.val[0] % 1024,
	"audio"_SIDP64.val[0] % 1024,
	"music"_SIDP64.val[0] % 1024,
	"asset"_SIDP64.val[0] % 1024,
	"geometry"_SIDP64.val[0] % 1024,
	"shit"_SIDP64.val[0] % 1024,
	"shit2"_SIDP64.val[0] % 1024,
	"wheel"_SIDP64.val[0] % 1024,
	"source"_SIDP64.val[0] % 1024
};

constexpr auto adadsfs = "geometry"_SIDP64.val[0] % 4096;
constexpr auto fgjhdfg = "shit"_SIDP64.val[0] % 4096;

constexpr auto yudoiyc = "acceleration"_SIDP64;
constexpr auto unpacketd64 = yudoiyc.unpack();



constexpr auto sdahkasdjf64 = "111011"_SIDP64;
constexpr auto sdahkasdjf128 = "shitake"_SID128;
constexpr auto sdahkasdjf192 = "shitake"_SID192;
constexpr auto sdahkasdjf256 = "shitake"_SID256;
constexpr bool asjd = sdahkasdjf256 >= sdahkasdjf256;
constexpr auto asjddsfg64 = sdahkasdjf64.asU8();
constexpr auto asjddsfg128 = sdahkasdjf128.asU8();
constexpr auto asjddsfg192 = sdahkasdjf192.asU8();
constexpr auto asjddsfg256 = sdahkasdjf256.asU8();

constexpr auto dshfkjiuoqwer = std::hash<SID256>()(sdahkasdjf256);



constexpr int testfunc(SID64 sid) noexcept
{
	switch (sid)
	{
	case "abc"_SIDP64:
		return 3;
	case "xyz"_SIDP64:
		return 8;
	case " abc"_SIDP64:
		return 13;
	default:
		return 0;
	}
}
constexpr auto asdgodsyuo = testfunc("xyz"_SIDP64);

*/


#endif // 
