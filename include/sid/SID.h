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


	//LUT 38<->byte
#ifndef SID_LUT38
#define SID_LUT38
	constexpr ::std::array<uint8_t, 256> getCharTo38() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;
		code++;

		for (size_t i = 0; i <= '9' - '0'; i++)
			tab[i + '0'] = code++;

		for (size_t i = 0; i <= 'z' - 'a'; i++)
			tab[i + 'A'] = tab[i + 'a'] = code++;

		tab['_'] = tab[' '] = tab['.'] = code++;
		
		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo38 = getCharTo38();

	constexpr ::std::array<char, 38> get38ToChar() noexcept
	{
		::std::array<char, 38> tab = {};
		uint8_t code = 0;

		tab[code++] = '\0';

		for (size_t i = '0'; i <= '9'; i++)
			tab[code++] = i;

		for (size_t i = 'A'; i <= 'Z'; i++)
			tab[code++] = i;
		
		tab[code++] = '_';

		return tab;
	}
	constexpr inline ::std::array<char, 38> lut38ToChar = get38ToChar();
#endif // !SID_LUT38

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

	//LUT 128<->byte
#ifndef SID_LUT128
#define SID_LUT128
	constexpr ::std::array<uint8_t, 256> getCharTo128() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint8_t code = 0;
		
		for (size_t i = 0; i < 128; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo128 = getCharTo128();

	constexpr ::std::array<char, 128> get128ToChar() noexcept
	{
		::std::array<char, 128> tab = {};
		uint8_t code = 0;
		
		for (size_t i = 0; i < 128; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<char, 128> lut128ToChar = get128ToChar();
#endif // !SID_LUT128

	//LUT 256<->byte
#ifndef SID_LUT256
#define SID_LUT256
	constexpr ::std::array<uint8_t, 256> getCharTo256() noexcept
	{
		::std::array<uint8_t, 256> tab = {};
		uint16_t code = 0;

		for (size_t i = 0; i < 256; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<uint8_t, 256> lutCharTo256 = getCharTo256();

	constexpr ::std::array<char, 256> get256ToChar() noexcept
	{
		::std::array<char, 256> tab = {};
		uint8_t code = 0;

		for (size_t i = 0; i < 256; i++)
			tab[code++] = i;

		return tab;
	}
	constexpr inline ::std::array<char, 256> lut256ToChar = get256ToChar();
#endif // !SID_LUT256









	namespace PackType {
		enum PT
		{
			numbers = 0x10000,
			texts = 0x20000,
			hashes = 0x40000,
			extra = 0x100000,
			dicts = 0x200000,
			
			
			error = -1,
			
			null = 0,
			
			number = numbers | 1,
			
			hash = hashes | 1,

			alnum = texts | 1,
			base64,
			ascii,
			byte,
			
			dict = dicts | 1
		};
	}


	namespace ip	//Internal Params
	{
		struct EncodingInfo
		{
			uint64_t frst;	// first code
			uint64_t last;	// last code
			uint64_t cdpt;	// code point
			uint64_t lngh;	// length - number of characters
			char name[4];	// name
			uint64_t maxValue;	//max value
		};
		
		static constexpr EncodingInfo nullInfo = 
		{
			0,											// first code
			0,											// last code
			1,											// code point
			1,											// max length
			{ 'N', 'U', 'L', 'L' },						// name
			0											//max value
		};
		
		static constexpr EncodingInfo hashInfo = {
			nullInfo.last + 1,							// first code
			nullInfo.last + 1 + 1152921504606846975,	// last code
			16,											// code point
			15,											// max length
			{ 'H', 'A', 'S', 'H'},						// name
			1152921504606846975
		};

		static constexpr EncodingInfo alnumInfo = {
			hashInfo.last + 1,							// first code
			hashInfo.last + 1 + 9065737908494995455,	// last code
			38,											// code point
			12,											// max length
			{ 'A', 'L', 'N', 'U' },						// name
			9065737908494995455							// maxValue
		};

		static constexpr EncodingInfo base64Info = {
			alnumInfo.last + 1,							// first code
			alnumInfo.last + 1 + 4297625829703557648,	// last code
			73,											// code point
			10,											// max length
			{ 'B', '6', '4', '_' },						// name
			4297625829703557648							// maxValue
		};

		static constexpr EncodingInfo asciiInfo = {
			base64Info.last + 1,						// first code
			base64Info.last + 1 + 72057594037927935,	// last code
			128,										// code point
			8,											// max length
			{ 'A', 'S', 'C', 'I' },						// name
			72057594037927935							// maxValue
		};

		static constexpr EncodingInfo byteInfo = {
			asciiInfo.last + 1,							// first code
			asciiInfo.last + 1 + 72057594037927935,		// last code
			256,										// code point
			7,											// max length
			{ 'B', 'Y', 'T', 'E' },						// name
			72057594037927935							// maxValue
		};

		static constexpr EncodingInfo numberInfo = {
			byteInfo.last + 1,							// first code
			byteInfo.last + 1 + 1152921504606846975,	// last code
			16,											// code point
			15,											// max length
			{ 'N', 'M', 'B', 'R' },						// name
			1152921504606846975							// maxValue
		};

		static constexpr EncodingInfo dictInfo = {
			numberInfo.last + 1,						// first code
			numberInfo.last + 1 + 1152921504606846975,	// last code
			32768,										// code point
			4,											// max length
			{ 'D', 'I', 'C', 'T' },						// name
			1152921504606846975							// maxValue
		};

		static constexpr EncodingInfo customInfo = {
			dictInfo.last + 1,							// first code
			dictInfo.last + 1 + 1480500633614601709,	// last code
			0,											// code point
			0,											// max length
			{ 'C', 'U', 'S', 'T' },						// name
			1480500633614601709							// maxValue
		};
		
		static_assert(customInfo.last == 0xFFFF'FFFF'FFFF'FFFF);
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

	template <uint64_t FIRSTCODE, uint64_t MAX_VALUE>
	constexpr SID packHashTempl(uint64_t hash) noexcept {
		
		hash = (hash % MAX_VALUE) + FIRSTCODE;

		hash = shuffle(hash);

		return { hash };
	}

	template <uint64_t FIRSTCODE, uint64_t MAX_VALUE>
	constexpr SID packHashTempl(const char* data, size_t size) noexcept {
		if (data == nullptr) size = 0;
		return packHashTempl<FIRSTCODE, MAX_VALUE>(hashFunc(data, size));
	}

	template <uint64_t FIRSTCODE, uint64_t MAX_VALUE>
	constexpr SID packNumberTempl(uint64_t number) noexcept {

		if (number <= MAX_VALUE)
		{
			number = number + FIRSTCODE;

			number = shuffle(number);

			return { number };
		}
		else
		{
//#if SID_ASSERT_FALLTHROUGH_HASHING!=0
//			assert(false);
//#endif
			return packHashTempl<ip::hashInfo.frst, ip::hashInfo.maxValue>(number);
		}
	}
	
	template <uint64_t FIRSTCODE, uint64_t MAX_VALUE>
	constexpr SID packDictTempl(uint64_t encodedWords) noexcept {

		if (encodedWords <= MAX_VALUE)
		{
			encodedWords = encodedWords + FIRSTCODE;

			encodedWords = shuffle(encodedWords);

			return { encodedWords };
		}
		else
		{
//#if SID_ASSERT_FALLTHROUGH_HASHING!=0
//			assert(false);
//#endif
			return packHashTempl<ip::hashInfo.frst, ip::hashInfo.maxValue>(encodedWords);
		}
	}
	
	template <
		uint64_t FIRSTCODE, 
		uint64_t MAX_VALUE,
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

			hash = hash + FIRSTCODE;

			hash = shuffle(hash);

			return { hash };
		}
		else
		{
//#if SID_ASSERT_FALLTHROUGH_HASHING!=0
//			assert(false);
//#endif
			return packHashTempl<ip::hashInfo.frst, ip::hashInfo.maxValue>(data, size);
		}
	}
	
	template <
		uint64_t FIRSTCODE, 
		uint64_t LASTCODE,
		uint64_t CODE_POINTS,
		uint64_t LENGTH,
		typename LUT_TYPE>
	constexpr bool unpackTempl(uint64_t hsh, ::std::array<char, 20>& text, const LUT_TYPE& LUT, const char NAME[4]) noexcept
	{
		if ((hsh >= FIRSTCODE) && (hsh <= LASTCODE))
		{
			hsh = hsh - FIRSTCODE;
			for (intptr_t i = 19; i > 19 - intptr_t(LENGTH); --i)
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
		uint64_t FIRSTCODE, 
		uint64_t LASTCODE,
		PackType::PT TYPE>
	constexpr bool unpackNumberTempl(uint64_t hsh, UnpackedNumber& result) noexcept
	{
		if ((hsh >= FIRSTCODE) && (hsh <= LASTCODE))
		{
			hsh = hsh - FIRSTCODE;
			result = { hsh, TYPE };
			return  true;
		}
		return false;
	}

	template <
		uint64_t FIRSTCODE, 
		uint64_t LASTCODE,
		uint64_t CODE_POINTS,
		uint64_t LENGTH,
		PackType::PT TYPE,
		typename LUT_TYPE>
		constexpr bool unpackTextTempl(uint64_t hsh, UnpackedText& result, const LUT_TYPE& LUT) noexcept
	{
		if ((hsh >= FIRSTCODE) && (hsh <= LASTCODE))
		{
			hsh = hsh - FIRSTCODE;

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
		return packHashTempl<ip::hashInfo.frst, ip::hashInfo.maxValue>(hash);
	}

	//hash string to 64bit
	constexpr SID packHash(const char* data, size_t size) noexcept {
		return packHashTempl<ip::hashInfo.frst, ip::hashInfo.maxValue>(data, size);
	}

	//put 60bit value or hash is bigger
	constexpr SID packNumber(uint64_t number) noexcept {
		return packNumberTempl<ip::numberInfo.frst, ip::numberInfo.maxValue>(number);
	}
	
	//put 60bit value or hash is bigger
	constexpr SID packDict(uint64_t encodedWords) noexcept {
		return packDictTempl<ip::dictInfo.frst, ip::dictInfo.maxValue>(encodedWords);
	}
	
	//pack or hash string to 64bit
	constexpr SID packAlnum(const char* data, size_t size) noexcept {
		return packStringTempl<ip::alnumInfo.frst, ip::alnumInfo.maxValue, ip::alnumInfo.cdpt, ip::alnumInfo.lngh>(data, size, lutCharTo38);
	}


	//pack or hash string to 64bit
	constexpr SID packB64(const char* data, size_t size) noexcept {
		return packStringTempl<ip::base64Info.frst, ip::base64Info.maxValue, ip::base64Info.cdpt, ip::base64Info.lngh>(data, size, lutCharTo73);
	}


	//pack or hash string to 64bit
	constexpr SID packAscii(const char* data, size_t size) noexcept {
		return packStringTempl<ip::asciiInfo.frst, ip::asciiInfo.maxValue, ip::asciiInfo.cdpt, ip::asciiInfo.lngh>(data, size, lutCharTo128);
	}


	//pack or hash string to 64bit
	constexpr SID packByte(const char* data, size_t size) noexcept {
		return packStringTempl<ip::byteInfo.frst, ip::byteInfo.maxValue, ip::byteInfo.cdpt, ip::byteInfo.lngh>(data, size, lutCharTo256);
	}








	//unpack or print hash as hex 64bit to 15 char string 
	constexpr ::std::array<char, 20> unpack(const SID& sid) noexcept 
	{
		::std::array<char, 20> text = {};

		uint64_t hsh = sid.val;

		hsh = unshuffle(hsh);

		if (unpackTempl<ip::alnumInfo.frst, ip::alnumInfo.last, ip::alnumInfo.cdpt, ip::alnumInfo.lngh>(hsh, text, lut38ToChar, ip::alnumInfo.name)) return text;
		if (unpackTempl<ip::base64Info.frst, ip::base64Info.last, ip::base64Info.cdpt, ip::base64Info.lngh>(hsh, text, lut73ToChar, ip::base64Info.name)) return text;
		if (unpackTempl<ip::asciiInfo.frst, ip::asciiInfo.last, ip::asciiInfo.cdpt, ip::asciiInfo.lngh>(hsh, text, lut128ToChar, ip::asciiInfo.name)) return text;
		if (unpackTempl<ip::byteInfo.frst, ip::byteInfo.last, ip::byteInfo.cdpt, ip::byteInfo.lngh>(hsh, text, lut256ToChar, ip::byteInfo.name)) return text;
		if (unpackTempl<ip::numberInfo.frst, ip::numberInfo.last, ip::numberInfo.cdpt, ip::numberInfo.lngh>(hsh, text, "0123456789ABCDEF", ip::numberInfo.name)) return text;
		if (unpackTempl<ip::hashInfo.frst, ip::hashInfo.last, ip::hashInfo.cdpt, ip::hashInfo.lngh>(hsh, text, "0123456789ABCDEF", ip::hashInfo.name)) return text;

		return text;
	}

	constexpr UnpackedNumber unpackNumber(const SID& sid)
	{
		uint64_t hsh = sid.val;

		hsh = unshuffle(hsh);
		UnpackedNumber result = {};

		if (unpackNumberTempl<ip::numberInfo.frst, ip::numberInfo.last, PackType::PT::number>(hsh, result)) return result;

		return { 0, PackType::error };
	}
	
	constexpr UnpackedNumber unpackDict(const SID& sid)
	{
		uint64_t hsh = sid.val;

		hsh = unshuffle(hsh);
		UnpackedNumber result = {};

		if (unpackNumberTempl<ip::dictInfo.frst, ip::dictInfo.last, PackType::PT::dict>(hsh, result)) return result;

		return { 0, PackType::error };
	}

	constexpr UnpackedText unpackText(const SID& sid) {
		uint64_t hsh = sid.val;

		hsh = unshuffle(hsh);

		UnpackedText text = {};

		if (unpackTextTempl<ip::alnumInfo.frst, ip::alnumInfo.last, ip::alnumInfo.cdpt, ip::alnumInfo.lngh, PackType::PT::alnum>(hsh, text, lut38ToChar)) return text;
		if (unpackTextTempl<ip::base64Info.frst, ip::base64Info.last, ip::base64Info.cdpt, ip::base64Info.lngh, PackType::PT::base64>(hsh, text, lut73ToChar)) return text;
		if (unpackTextTempl<ip::asciiInfo.frst, ip::asciiInfo.last, ip::asciiInfo.cdpt, ip::asciiInfo.lngh, PackType::PT::ascii>(hsh, text, lut128ToChar)) return text;
		if (unpackTextTempl<ip::byteInfo.frst, ip::byteInfo.last, ip::byteInfo.cdpt, ip::byteInfo.lngh, PackType::PT::byte>(hsh, text, lut256ToChar)) return text;

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
	constexpr sid::SID operator"" _SIDNUM(uint64_t number) noexcept
	{
		return sid::packNumber(number);
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


constexpr auto binteste = 0b101000111111001111111111000000000001111111111111111000111001_SIDNUM;
constexpr auto bintestd = unpack(binteste);


constexpr uint64_t binnum = 0b101000111111001111111111000000000001111111111111111000111001;
constexpr auto bintest2e = 0b101000111111001111111111000000000001111111111111111000111001_SIDNUM;
constexpr auto bintest2d = unpack(bintest2e);
constexpr auto bintest2dn = unpackNumber(bintest2e);

constexpr auto decteste = 12345678901111119_SIDNUM;
constexpr auto dectestd = unpack(decteste);

constexpr auto dectest2e = 12345678911111119_SIDNUM;
constexpr auto dectest2d = unpack(dectest2e);
constexpr auto dectest2dn = unpackNumber(dectest2e);


constexpr auto hexteste = 0x0123789ABCdefa_SIDNUM;
constexpr auto hextestd = unpack(hexteste);

constexpr auto hextest2e = 0x0123789ABCdefa_SIDNUM;
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
	(0b0000_SIDNUM).val % 256,
	(0b0001_SIDNUM).val % 256,
	(0b0010_SIDNUM).val % 256,
	(0b0011_SIDNUM).val % 256,
	(0b0100_SIDNUM).val % 256,
	(0b0101_SIDNUM).val % 256,
	(0b0110_SIDNUM).val % 256,
	(0b0111_SIDNUM).val % 256,
	(0b1000_SIDNUM).val % 256,
	(0b1001_SIDNUM).val % 256,
	(0b1010_SIDNUM).val % 256,
	(0b1011_SIDNUM).val % 256,
	(0b1100_SIDNUM).val % 256,
	(0b1101_SIDNUM).val % 256,
	(0b1110_SIDNUM).val % 256,
	(0b1111_SIDNUM).val % 256
};



	constexpr auto bintest8e = 0b000001111000011011110_SIDNUM;
	constexpr auto bintest8d = unpack(bintest8e);

	constexpr auto bintest9e = 61662_SIDNUM;
	constexpr auto bintest9d = unpack(bintest9e);


	constexpr bool fdshfkjasdhfkjdsh = (bintest9e == bintest8e);


	constexpr auto  gf1 = sid::unpackNumber(0b010001100010010010001010101000101_SIDNUM);
	//constexpr auto  gf2 = sid::unpackNumber("010001100010010010001010101000101"_SIDBIN);






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
