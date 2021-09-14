#include <fstream>
#include <iostream>
#include <vector>
#include <map>


#include "../../include/dict/dictutils.h"

#ifndef WORDLISTCONSTEXPR
#include "WordListConstexpr.h"
#endif

#ifndef WORDLISTDYNAMIC
#include "WordListDynamic.h"
#endif

#ifndef GEN_WORD_LIST
#if __has_include("genWordList.h")
#include "genWordList.h"
#endif
#endif




uint64_t roundUp2(uint64_t v){
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	return v;
}

const size_t dictsizes[] = {
	16,
	24,
	32,
	48,
	64,
	80,
	96,
	112,
	128,
	160,
	192,
	224,
	256,
	320,
	384,
	448,
	512,
	640,
	768,
	896,
	1024,
	1280,
	1536,
	1792,
	2048,
	2560,
	3072,
	3584,
	4096,
	5120,
	6144,
	7168,
	8192,
	10240,
	12288,
	14336,
	16384,
	20480,
	24576,
	28672,
	32768,
	40960,
	49152,
	57344,
	65536,
	81920,
	98304,
	114688,
	131072,
	163840,
	196608,
	229376,
	262144,
	327680,
	393216,
	458752,
	524288,
	655360,
	786432,
	917504,
	1048576,
	1310720,
	1572864,
	1835008,
	2097152,
	2621440,
	3145728,
	3670016,
	4194304,
	5242880,
	6291456,
	7340032,
	8388608,
	10485760,
	12582912,
	14680064,
	16777216,
	20971520,
	25165824,
	29360128,
	33554432,
	41943040,
	50331648,
	58720256,
	67108864,
	83886080,
	100663296,
	117440512,
	134217728,
	167772160,
	201326592,
	234881024,
	268435456,
	335544320,
	402653184,
	469762048,
	536870912,
	671088640,
	805306368,
	939524096,
	1073741824,
	1342177280,
	1610612736,
	1879048192,
	2147483648
};
	
	
constexpr size_t roundSize(size_t size) noexcept
{
	size *= 1.25;
	
	for (auto e:dictsizes)
		if(e >=size)
			return e;
	return 0;
}


std::string stringCoder(const char* in){
	std::string out;
	
	const char *hex = "0123456789ABCDEF";
	
	while(*in != '\0')
	{
		//std::cerr << "*in: " << *in << "          \r"; 
		const char ch = *in++;
		if(ch>='a' && ch <= 'z') out += std::string() + ch;
		else if(ch>='A' && ch <= 'Z') out += std::string() + ch;
		else if(ch>='0' && ch <= '9') out += std::string() + ch;
		else if(ch == '\'')	out += std::string() + '\\' + '\'';
		else if(ch == '\"')	out += std::string() + '\\' + '\"';
		else if(ch == '\?')	out += std::string() + '\\' + '\?';
		else if(ch == '\\')	out += std::string() + '\\' + '\\';
		else if(ch == '\a')	out += std::string() + '\\' + '\a';
		else if(ch == '\b')	out += std::string() + '\\' + '\b';
		else if(ch == '\f')	out += std::string() + '\\' + '\f';
		else if(ch == '\n')	out += std::string() + '\\' + '\n';
		else if(ch == '\r')	out += std::string() + '\\' + '\r';
		else if(ch == '\t')	out += std::string() + '\\' + '\t';
		else if(ch == '\v')	out += std::string() + '\\' + '\v';
		else if(ch>=' ' && ch <= '~') out += std::string() + ch;
		else out += std::string() + '\\' + 'x' + hex[((ch) >>4)&0xF] + hex[(ch)&0xF];
	}
	
	return out;
}




void loadOldWordIndex(
	std::map<const char*, size_t> &wordindexmap,	
	std::map<size_t, const char*> &indexwordmap)
{
	#ifdef GEN_WORD_LIST
	for(const auto &e: DictionaryWordList.wtoi)
	{
		if(e.word != nullptr)
		{
			wordindexmap[e.word] = e.index;
			indexwordmap[e.index] = e.word;
		}
	}
	#endif
}

void loadWordLists(
	std::vector<Dict::Word> &constdict, 
	std::vector<Dict::Word> &dynamicdict,
	std::map<const char*, size_t> &newwordindexmap)
{
	// fill constdict dynamicdict newwordindexmap with const dictionary words
	for(const auto &e: unsorted_dictionary_constexpr){
		constdict.push_back({e, size_t(-1)});
		dynamicdict.push_back({e, size_t(-1)});
		newwordindexmap.insert({e, size_t(-1)});
	}
	// fill dynamicdict newwordindexmap with dynamic dictionary words
	for(const auto &e: unsorted_dictionary_dynamic){
		dynamicdict.push_back({e, size_t(-1)});
		newwordindexmap.insert({e, size_t(-1)});
	}
}

void removeObsoletteWords(
	std::map<const char*, size_t> &wordindexmap,
	std::map<size_t, const char*> &indexwordmap,
	std::map<const char*, size_t> &newwordindexmap)
{
	for(auto it = wordindexmap.begin(); it != wordindexmap.end(); ){
		auto it2 = newwordindexmap.find(it->first);
		if(it2 == newwordindexmap.end())
		{
			auto itrem = it++;
			//int cnt1 = 
			indexwordmap.erase(itrem->second);
			itrem = wordindexmap.erase(itrem);
			//std::clog << "cnt1:" << cnt1 << " cnt2:" << itrem->first << std::endl;
		}
		else
			it++;
	}
}

void inheritOrGenerateIndexesForConstWords(
	size_t &nextindex,
	std::vector<Dict::Word> &constdict, 
	std::map<const char*, size_t> &wordindexmap,
	std::map<size_t, const char*> &indexwordmap,
	std::map<const char*, size_t> &newwordindexmap)
{
	for(auto &e: constdict){
		auto it = wordindexmap.find(e.word);
		if(it != wordindexmap.end())
		{
			//inherit old index
			e.index = it->second;
			newwordindexmap[e.word] = it->second;
		}
		else
		{
			//generate new index
			auto it2  = indexwordmap.find(nextindex);
			while((it2 != indexwordmap.end()) && (it->first != nullptr))
			{
				nextindex++;
				it2  = indexwordmap.find(nextindex);
			}
			e.index = nextindex;
			newwordindexmap[e.word] = nextindex;
			
			wordindexmap[e.word] = nextindex;
			indexwordmap[nextindex] = e.word;
		}
	}
}

void inheritOrGenerateIndexesForDynamicWords(
	size_t &nextindex,
	std::vector<Dict::Word> &dynamicdict, 
	std::map<const char*, size_t> &wordindexmap,
	std::map<size_t, const char*> &indexwordmap,
	std::map<const char*, size_t> &newwordindexmap)
{
	for(auto &e: dynamicdict){
		auto it = wordindexmap.find(e.word);
		if(it != wordindexmap.end())
		{
			//inherit old index
			e.index = it->second;
			newwordindexmap[e.word] = it->second;
		}
		else
		{
			//generate new index
			auto it2  = indexwordmap.find(nextindex);
			while((it2 != indexwordmap.end()) && (it->first != nullptr))
			{
				nextindex++;
				it2  = indexwordmap.find(nextindex);
			}
			e.index = nextindex;
			newwordindexmap[e.word] = nextindex;
			
			wordindexmap[e.word] = nextindex;
			indexwordmap[nextindex] = e.word;
		}
	}
}

void buildHashMap(
	std::vector<Dict::Word> &words, 
	std::vector<Dict::Word> &WTOI,
	std::vector<Dict::Word> &ITOW
	)
{
	size_t missesWTOI = 0;
	size_t missesITOW = 0;
	for(auto e: words)
	{
		InsertWordIndex(WTOI.data(), WTOI.size(), e, missesWTOI);
		InsertIndexWord(ITOW.data(), ITOW.size(), e, missesITOW);
	}
	std::clog << "\tmisses WTOI:\t" << missesWTOI << std::endl;
	std::clog << "\tmisses ITOW:\t" << missesITOW << std::endl;
}

int createGenDictConstexpr_h(
	size_t dictionaryConstSize,
	std::vector<Dict::Word> &constWTOI,
	std::vector<Dict::Word> &constITOW)
{
	std::ofstream out("genDictConstexpr.h");
	if(!out){
		std::cerr << "ERROR: cant't open genDictConstexpr.h for writing";
		return -1;
	}
	
	out << "#ifndef GEN_DICT_CONSTEXPR\n";
	out << "#define GEN_DICT_CONSTEXPR\n";
	out << "\n";
	out << "#ifndef DICTUTILS_H\n";
	out << "#include \"../../include/dict/dictutils.h\"\n";
	out << "#endif\n";
	out << "\n";
	out << "inline constexpr Dict::Dictionary<" << dictionaryConstSize << "> DictionaryConstexpr = \n";
	out << "{\n";
	out << "\t{\n";
	size_t restword = constWTOI.size();
	for(const auto &e:constWTOI)
	{
		restword--;
		if(e.word)
			out << "\t\t{\"" + stringCoder(e.word) + "\", " <<  e.index << " }" << (restword ? "," : "") << "\n";
		else
			out << "\t\t{}" << (restword ? "," : "") << "\n";
	}
	
	out << "\t},\n";
	
	out << "\t{\n";
	restword = constITOW.size();
	for(const auto &e:constITOW)
	{
		restword--;
		if(e.word)
			out << "\t\t{\"" + stringCoder(e.word) + "\", " <<  e.index << " }" << (restword ? "," : "") << "\n";
		else
			out << "\t\t{}" << (restword ? "," : "") << "\n";
	}
	
	out << "\t}\n";
	
	out << "};\n";
	out << "#endif\n";
	
	out.close();
	
	return 0;
}

int createGenDictDynamic_h(
	size_t dictionaryDynamicSize,
	std::vector<Dict::Word> &dynamicWTOI,
	std::vector<Dict::Word> &dynamicITOW)
{
	std::ofstream out("genDictDynamic.h");
	if(!out){
		std::cerr << "ERROR: cant't open genDictDynamic.h for writing";
		return -1;
	}
	
	out << "#ifndef GEN_DICT_DYNAMIC\n";
	out << "#define GEN_DICT_DYNAMIC\n";
	out << "\n";
	out << "#ifndef DICTUTILS_H\n";
	out << "#include \"../../include/dict/dictutils.h\"\n";
	out << "#endif\n";
	out << "\n";
	out << "extern const Dict::Dictionary<" << dictionaryDynamicSize << "> DictionaryDynamic;\n";
	out << "#endif\n";
	out.close();
	
	return 0;
}

int createGenDictDynamic_cpp(
	size_t dictionaryDynamicSize,
	std::vector<Dict::Word> &dynamicWTOI,
	std::vector<Dict::Word> &dynamicITOW)
{
	std::ofstream out("genDictDynamic.cpp");
	if(!out){
		std::cerr << "ERROR: cant't open genDictDynamic.cpp for writing";
		return -1;
	}
	
	out << "#include \"genDictDynamic.h\"\n";
	out << "\n";
	out << "const Dict::Dictionary<" << dictionaryDynamicSize << "> DictionaryDynamic = \n";
	out << "{\n";
	out << "\t{\n";
	size_t restword = dynamicWTOI.size();
	for(const auto &e:dynamicWTOI)
	{
		std::clog << "A "<< restword << "            \r";
		restword--;
		if(e.word)
			out << "\t\t{\"" + stringCoder(e.word) + "\", " <<  e.index << " }" << (restword ? "," : "") << "\n";
		else
			out << "\t\t{}" << (restword ? "," : "") << "\n";
	}
	
	out << "\t},\n";
	
	out << "\t{\n";
	restword = dynamicITOW.size();
	for(const auto &e:dynamicITOW)
	{
		std::clog << "B "<< restword << "            \r";
		restword--;
		if(e.word)
			out << "\t\t{\"" + stringCoder(e.word) + "\", " <<  e.index << " }" << (restword ? "," : "") << "\n";
		else
			out << "\t\t{}" << (restword ? "," : "") << "\n";
	}
	
	out << "\t}\n";
	
	out << "};\n";
	
	out.close();
	
	return 0;
}

int createGenWordList_h(
	size_t dictionaryDynamicSize,
	std::vector<Dict::Word> &dynamicWTOI,
	std::vector<Dict::Word> &dynamicITOW)
{
	std::ofstream out("genWordList.h");
	if(!out){
		std::cerr << "ERROR: cant't open genWordList.h for writing";
		return -1;
	}
	
	out << "#ifndef GEN_WORD_LIST\n";
	out << "#define GEN_WORD_LIST\n";
	out << "\n";
	out << "#ifndef DICTUTILS_H\n";
	out << "#include \"../../include/dict/dictutils.h\"\n";
	out << "#endif\n";
	out << "\n";
	out << "const Dict::Dictionary<" << dictionaryDynamicSize << "> DictionaryWordList = \n";
	out << "{\n";
	out << "\t{\n";
	size_t restword = dynamicWTOI.size();
	for(const auto &e:dynamicWTOI)
	{
		restword--;
		if(e.word)
			out << "\t\t{\"" + stringCoder(e.word) + "\", " <<  e.index << " }" << (restword ? "," : "") << "\n";
		else
			out << "\t\t{}" << (restword ? "," : "") << "\n";
	}
	
	out << "\t},\n";
	
	out << "\t{\n";
	restword = dynamicITOW.size();
	for(const auto &e:dynamicITOW)
	{
		restword--;
		if(e.word)
			out << "\t\t{\"" + stringCoder(e.word) + "\", " <<  e.index << " }" << (restword ? "," : "") << "\n";
		else
			out << "\t\t{}" << (restword ? "," : "") << "\n";
	}
	
	out << "\t}\n";
	out << "};\n";
	out << "#endif\n";
	
	out.close();
	
	return 0;
}





int bakeDictionary()
{
	std::vector<Dict::Word> constdict;
	std::vector<Dict::Word> dynamicdict;
	std::map<const char*, size_t> newwordindexmap;
	
	std::map<const char*, size_t> wordindexmap;
	std::map<size_t, const char*> indexwordmap;
	
	size_t nextindex = 0;
	
	
	
	loadOldWordIndex(wordindexmap, indexwordmap);
	
	loadWordLists(constdict, dynamicdict, newwordindexmap);
	 
	removeObsoletteWords(wordindexmap, indexwordmap, newwordindexmap);
	
	inheritOrGenerateIndexesForConstWords(
		nextindex, constdict, wordindexmap, indexwordmap, newwordindexmap);
	
	inheritOrGenerateIndexesForDynamicWords(
		nextindex, dynamicdict, wordindexmap, indexwordmap, newwordindexmap);
	
	size_t dictionaryConstSize = roundSize(constdict.size());
	size_t dictionaryDynamicSize = roundSize(newwordindexmap.size());
	std::clog << "dictionaryConstSize:\t" << dictionaryConstSize << std::endl;
	std::clog << "dictionaryDynamicSize:\t" << dictionaryDynamicSize << std::endl;
	
	std::vector<Dict::Word> constWTOI(dictionaryConstSize, {nullptr, size_t(-1)});
	std::vector<Dict::Word> constITOW(dictionaryConstSize, {nullptr, size_t(-1)});
	std::vector<Dict::Word> dynamicWTOI(dictionaryDynamicSize, {nullptr, size_t(-1)});
	std::vector<Dict::Word> dynamicITOW(dictionaryDynamicSize, {nullptr, size_t(-1)});
	
	std::clog << "build const hashmap:"<<std::endl;
	buildHashMap(constdict, constWTOI, constITOW);
	
	std::clog << "build dynamic hashmap:"<<std::endl;
	buildHashMap(dynamicdict, dynamicWTOI, dynamicITOW);
	
	std::clog << "create genDictConstexpr.h..." << std::endl; 
	if(createGenDictConstexpr_h(dictionaryConstSize, constWTOI, constITOW))		return -1;
	
	std::clog << "create genDictDynamic.h..." << std::endl;
	if(createGenDictDynamic_h(dictionaryDynamicSize, dynamicWTOI, dynamicITOW))		return -1;
	
	std::clog << "create genDictDynamic.cpp..." << std::endl; 
	if(createGenDictDynamic_cpp(dictionaryDynamicSize, dynamicWTOI, dynamicITOW))		return -1;
	
	std::clog << "create genWordList.h..." << std::endl; 
	if(createGenWordList_h(dictionaryDynamicSize, dynamicWTOI, dynamicITOW))		return -1;
	
	
	
	std::clog << "SUCCESS ALL CREATED\n";
	return 0;
}
