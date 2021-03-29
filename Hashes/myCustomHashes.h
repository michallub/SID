#ifndef __myCustomHashes_H__ 
#define __myCustomHashes_H__ 

#include "crc32.h"
#include "crc64.h"
#include <cassert>

template<typename UINTTYPE, size_t ELEMENTCOUNT>
struct HashType
{
	using UIntType = UINTTYPE;
	static constexpr size_t N = ELEMENTCOUNT;
	
	
	UINTTYPE value[N];
};

template<typename UINTTYPE, size_t ELEMENTCOUNT>
std::ostream& operator<<(std::ostream& os, const HashType<UINTTYPE, ELEMENTCOUNT> &hash)
{
	const char * hex = "0123456789ABCDEF";
	for(const auto& e: hash.value)
	{
		for(int i = sizeof(e)-1; i >= 0; i--)
		{
			os << hex[(e >> (i*8+4))&0xF];
			os << hex[(e >> (i*8))&0xF];
		}
	}
	return os;
}

using HashType32 = HashType<uint32_t, 1>;
using HashType64 = HashType<uint64_t, 1>;
using HashType96 = HashType<uint32_t, 3>;
using HashType128 = HashType<uint64_t, 2>;
using HashType256 = HashType<uint64_t, 4>;

HashType32 hash32(const void *data, const size_t size) noexcept {
	assert(data);
	return {crc32(data, size)};
}

HashType64 hash64(const void *data, const size_t size) noexcept {
	assert(data);
	return {crc64(data, size)};
}

template <bool recursion = true>
HashType96 hash96(const void *data, const size_t size) noexcept {
	assert(data);
	const uint8_t* textdata = reinterpret_cast<const uint8_t*>(data);
	
	uint32_t crc32L = 0xFFFFFFFFu;
	uint32_t crc32M = 0xFFFFFFFFu;
	uint32_t crc32H = 0xFFFFFFFFu;
	
	uint64_t crc0 = 0xFFFF'FFFFull;
	uint64_t crc1 = 0x0000'0000ull;
	uint64_t crc2 = 0xFFFF'FFFFull;
	
	for(size_t i = 0; i < size; ++i)
	{
		uint8_t byte0=0, byte1=1, byte2=2;
		{
			uint8_t byte = byte0 = textdata[i];
			uint8_t LUTindex = (crc32L ^ byte) & 0xFFu;
			crc32L = (crc32L >> 8u) ^ CRC32Table[LUTindex];
		}
		++i;
		if(i < size)
		{
			uint8_t byte = byte1 = textdata[i];
			uint8_t LUTindex = (crc32M ^ byte) & 0xFFu;
			crc32M = (crc32M >> 8u) ^ CRC32Table[LUTindex];
		}
		++i;
		if(i < size)
		{
			uint8_t byte = byte2 = textdata[i];
			uint8_t LUTindex = (crc32H ^ byte) & 0xFFu;
			crc32H = (crc32H >> 8u) ^ CRC32Table[LUTindex];
		}
		
		if constexpr(recursion == true)
		{
			uint8_t byte;
			uint8_t LUTindex;
			
			byte = byte2^byte0;
			LUTindex = (crc0 ^ byte) & 0xFFu;
			crc0 = ((crc0 >> 8u) ^ CRC32Table[LUTindex]);
			
			byte = byte2^byte1;
			LUTindex = (crc1 ^ byte) & 0xFFu;
			crc1 = ((crc1 >> 8u) ^ CRC32Table[LUTindex]);
			
			byte = byte1^byte0;
			LUTindex = (crc2 ^ byte) & 0xFFu;
			crc2 = ((crc2 >> 8u) ^ CRC32Table[LUTindex]);
		}
	}
	
	crc32L = crc32L ^ 0xFFFFFFFFu;
	crc32M = crc32M ^ 0xFFFFFFFFu;
	crc32H = crc32H ^ 0xFFFFFFFFu;
	
	if constexpr(recursion == true)
	{
		HashType96 hash[2] = {{crc32L, crc32M, crc32H},{crc0, crc1, crc2}};
		return hash96<false>((uint8_t*)&hash,sizeof(hash));
	}
	else
		return {crc32L, crc32M, crc32H};
}

template <bool recursion = true>
HashType128 hash128(const void *data, const size_t size) noexcept {
	assert(data);
	const uint8_t* textdata = reinterpret_cast<const uint8_t*>(data);
	
	uint64_t crc64L = 0xFFFF'FFFF'FFFF'FFFFull;
	uint64_t crc64H = 0xFFFF'FFFF'FFFF'FFFFull;
	
	uint64_t crc0 = 0xFFFF'FFFF'FFFF'FFFFull;
	uint64_t crc1 = 0x0000'0000'0000'0000ull;
	
	for(size_t i = 0; i < size; ++i)
	{
		uint8_t byte0=0, byte1=1;
		{
			uint8_t byte = byte0 = textdata[i];
			uint8_t LUTindex = (crc64L ^ byte) & 0xFFu;
			crc64L = (crc64L >> 8u) ^ CRC64Table[LUTindex];
		}
		++i;
		if(i < size)
		{
			uint8_t byte = byte1 = textdata[i];
			uint8_t LUTindex = (crc64H ^ byte) & 0xFFu;
			crc64H = (crc64H >> 8u) ^ CRC64Table[LUTindex];
		}
		
		if constexpr(recursion == true)
		{
			uint8_t byte;
			uint8_t LUTindex;
			
			byte = byte0;
			LUTindex = (crc0 ^ byte) & 0xFFu;
			crc0 = ((crc0 >> 8u) ^ CRC64Table[LUTindex]);
			
			byte = byte1;
			LUTindex = (crc1 ^ byte) & 0xFFu;
			crc1 = ((crc1 >> 8u) ^ CRC64Table[LUTindex]);
			
			crc0 ^= crc1 ^= crc0 ^= crc1;
		}
	}
	
	crc64L = crc64L ^ 0xFFFF'FFFF'FFFF'FFFFull;
	crc64H = crc64H ^ 0xFFFF'FFFF'FFFF'FFFFull;
	
	if constexpr(recursion == true)
	{
		HashType128 hash[2] = {{crc64L, crc64H},{crc0, crc1}};
		return hash128<false>((uint8_t*)&hash,sizeof(hash));
	}
	else
		return {crc64L, crc64H};
}

template <bool recursion = true>
HashType256 hash256(const void* data, const size_t size) noexcept {
	assert(data);
	uint64_t crc0 = 0xFFFF'FFFF'FFFF'FFFFull;
	uint64_t crc1 = 0x0000'0000'0000'0000ull;
	uint64_t crc2 = 0xFFFF'FFFF'FFFF'FFFFull;
	uint64_t crc3 = 0x0000'0000'0000'0000ull;
	
	uint64_t crc4 = 0xFFFF'FFFF'FFFF'FFFFull;
	uint64_t crc5 = 0x0000'0000'0000'0000ull;
	uint64_t crc6 = 0xFFFF'FFFF'FFFF'FFFFull;
	uint64_t crc7 = 0x0000'0000'0000'0000ull;
	
	const uint8_t* textdata = reinterpret_cast<const uint8_t*>(data);
	
	for(size_t i = 0; i < size; ++i)
	{
		uint8_t byte0=0, byte1=1, byte2=2, byte3=3;
		{
			uint8_t byte = byte0 = textdata[i];
			uint8_t LUTindex = (crc0 ^ byte) & 0xFFu;
			crc0 = ((crc0 >> 8u) ^ CRC64Table[LUTindex]);
		}
		++i;
		{
			uint8_t byte = byte1 = textdata[i];
			uint8_t LUTindex = (crc1 ^ byte) & 0xFFu;
			crc1 = ((crc1 >> 8u) ^ CRC64Table[LUTindex]);
		}
		++i;
		{
			uint8_t byte = byte2 = textdata[i];
			uint8_t LUTindex = (crc2 ^ byte) & 0xFFu;
			crc2 = ((crc2 >> 8u) ^ CRC64Table[LUTindex]);
		}
		++i;
		{
			uint8_t byte = byte3 = textdata[i];
			uint8_t LUTindex = (crc3 ^ byte) & 0xFFu;
			crc3 = ((crc3 >> 8u) ^ CRC64Table[LUTindex]);
		}
		
		if constexpr(recursion == true)
		{
			uint8_t byte;
			uint8_t LUTindex;
			
			byte = byte3^byte0;
			LUTindex = (crc4 ^ byte) & 0xFFu;
			crc4 = ((crc4 >> 8u) ^ CRC64Table[LUTindex]);
			
			byte = byte2^byte1;
			LUTindex = (crc5 ^ byte) & 0xFFu;
			crc5 = ((crc5 >> 8u) ^ CRC64Table[LUTindex]);
			
			byte = byte2^byte3;
			LUTindex = (crc6 ^ byte) & 0xFFu;
			crc6 = ((crc6 >> 8u) ^ CRC64Table[LUTindex]);
			
			byte = byte1^byte0;
			LUTindex = (crc7 ^ byte) & 0xFFu;
			crc7 = ((crc7 >> 8u) ^ CRC64Table[LUTindex]);
		}
	}
	
	crc0 = crc0 ^ 0xFFFF'FFFF'FFFF'FFFFull;
	crc2 = crc2 ^ 0xFFFF'FFFF'FFFF'FFFFull;
	
	if constexpr(recursion == true)
	{
		HashType256 hash[2] = {{crc0, crc1, crc2, crc3},{crc4, crc5, crc6, crc7}};
		return hash256<false>((uint8_t*)&hash,sizeof(hash));
	}
	else
		return {crc0, crc1, crc2, crc3};
}




#endif