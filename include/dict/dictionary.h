#ifndef DICTDICTIONARY_H
#define DICTDICTIONARY_H

#ifndef GEN_DICT_CONSTEXPR
#include "genDictConstexpr.h"
#endif

#ifndef GEN_DICT_DYNAMIC
#include "genDictDynamic.h"
#endif

constexpr size_t CDict(const char * word, size_t size = size_t(-1)) noexcept
{
	return word ? DictionaryConstexpr.findIndex(word) : DictionaryConstexpr.findIndex("");
}

size_t DDict(const char * word, size_t size = size_t(-1)) noexcept
{
	return word ? DictionaryDynamic.findIndex(word) : DictionaryDynamic.findIndex("");
}

constexpr const char* CDict(unsigned long long int index) noexcept
{
	return DictionaryConstexpr.findWord(index);
}

const char* DDict(unsigned long long int index) noexcept
{
	return DictionaryDynamic.findWord(index);
}









constexpr size_t operator"" _CDict(const char * word, size_t size) noexcept
{
	return CDict(word, size);
}

size_t operator"" _DDict(const char * word, size_t size) noexcept
{
	return DDict(word, size);
}

constexpr const char* operator"" _CDict(unsigned long long int index) noexcept
{
	return CDict(index);
}

const char* operator"" _DDict(unsigned long long int index) noexcept
{
	return DDict(index);
}



#endif
