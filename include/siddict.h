#ifndef DICTSIDDICT_H
#define DICTSIDDICT_H

#include "sid/sid.h"
#include "dict/dictionary.h"
#include <cassert>

//encode 4 word on 
constexpr uint64_t encodeWords4C(uint64_t dictsize, const char* a, const char* b, const char* c, const char* d) noexcept 
{
	if(dictsize>65536)
		return size_t(-1);
	
	uint64_t ai = CDict(a);
	uint64_t bi = CDict(b);
	uint64_t ci = CDict(c);
	uint64_t di = CDict(d);
	
	if(ai >= dictsize || bi >= dictsize || ci >= dictsize || di >= dictsize) 
		return size_t(-1);
	
	return (((ai)*dictsize + bi)*dictsize + ci)*dictsize + di;
}

uint64_t encodeWords4D(uint64_t dictsize, const char* a, const char* b, const char* c, const char* d) noexcept 
{
	if(dictsize>65536)
		return size_t(-1);
	
	uint64_t ai = DDict(a);
	uint64_t bi = DDict(b);
	uint64_t ci = DDict(c);
	uint64_t di = DDict(d);
	
	if(ai >= dictsize || bi >= dictsize || ci >= dictsize || di >= dictsize) 
		return size_t(-1);
	
	return (((ai)*dictsize + bi)*dictsize + ci)*dictsize + di;
}

constexpr std::array<const char*, 4> decodeWords4C(uint64_t dictsize, uint64_t abcd) noexcept 
{
	uint64_t di = abcd % dictsize;
	abcd = abcd / dictsize;
	uint64_t ci = abcd % dictsize;
	abcd = abcd / dictsize;
	uint64_t bi = abcd % dictsize;
	abcd = abcd / dictsize;
	uint64_t ai = abcd % dictsize;
	
	return {
		CDict(ai),
		CDict(bi),
		CDict(ci),
		CDict(di)
	};
}

std::array<const char*, 4> decodeWords4D(uint64_t dictsize, uint64_t abcd) noexcept 
{
	uint64_t di = abcd % dictsize;
	abcd = abcd / dictsize;
	uint64_t ci = abcd % dictsize;
	abcd = abcd / dictsize;
	uint64_t bi = abcd % dictsize;
	abcd = abcd / dictsize;
	uint64_t ai = abcd % dictsize;
	
	return {
		DDict(ai),
		DDict(bi),
		DDict(ci),
		DDict(di)
	};
}

//encode 4 words on 56 bits as one of extraNum, compile time version
constexpr sid::SID SIDDICT4C(const char* a, const char* b, const char* c, const char* d) noexcept
{
	return sid::packDict(encodeWords4C(sid::ip::dictInfo.maxValue, a,b,c,d));
}

//encode 4 words on 56 bits as one of extraNum, run time version
sid::SID SIDDICT4D(const char* a, const char* b, const char* c, const char* d) noexcept
{
	return sid::packDict(encodeWords4D(sid::ip::dictInfo.maxValue, a,b,c,d));
}

//decode 4 words on 56 bits as one of extraNum, compile time version
constexpr std::array<const char*, 4> unpackWords4C(sid::SID s)
{
	auto upn = sid::unpackDict(s);
	if(upn.type == (sid::PackType::dict))
		return decodeWords4C(sid::ip::dictInfo.maxValue, upn.number);
	else
		return {nullptr, nullptr, nullptr, nullptr};
}

//decode 4 words on 56 bits as one of extraNum, run time version
std::array<const char*, 4> unpackWords4D(sid::SID s)
{
	auto upn = sid::unpackDict(s);
	//printf("%lld, %lld, %lld", upn.number, upn.type, (sid::PackType::extra_num|0x1) + extraNumIndex);
	assert(upn.type == (sid::PackType::dict));
	return decodeWords4D(sid::ip::dictInfo.maxValue, upn.number);
}






















#endif

