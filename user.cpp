#include <iostream>


#include "./tools/baker/genDictConstexpr.h"
#include "./tools/baker/genDictDynamic.h"

#include "./include/siddict.h"

#define PRINT(WORD) std::cout << #WORD << ":\t" << size_t(DictFind(dictionary, #WORD )) << "\n";

int main()
{
	//std::cout  << "exampleConstexpr:\t" << unshuffle(exampleConstexpr) << std::endl;
	//std::cout  << "exampleDynamic:\t" << unshuffle(exampleDynamic) << std::endl;
	
	
	//compile time:
constexpr sid::SID exampleConstexpr = SIDDICT4C("enemy", "tank", "01", "");
constexpr std::array<const char*, 4> exampleConstexprUnpacked=  unpackWords4C(exampleConstexpr);

//runtime:
const sid::SID exampleDynamic = SIDDICT4D("enemy", "tank", "01", "");
const std::array<const char*, 4> exampleDynamicUnpacked=  unpackWords4D(exampleDynamic);
	
	
	
	for(auto e: exampleConstexprUnpacked)
		if(e)
			std::cout << "\"" << e << "\" ";
		else
			std::cout << nullptr  << std::endl;
	
	std::cout  << std::endl;
	for(auto e: exampleDynamicUnpacked)
		if(e)
			std::cout << "\"" << e << "\" ";
		else
			std::cout << nullptr  << std::endl;
	std::cout  << std::endl;
	
	//std::cout << "Misses: " << dictionary.misses << "\n";
	//std::cout << "size: " << dictionary.size << "\n";
	while(1)
	{
		std::string text;
		//std::cin >> text;
		 std::getline (std::cin,text);
		switch(DDict(text.c_str()))
		{
			case "example constexpr word 1"_CDict:		std::cout << "you write mouse?\n"; break;
			case "example_constexpr_word_2"_CDict:		std::cout << "you perform benchmark?\n"; break;
			case "EXAMPLE_CONSTEXPR_WORD_3"_CDict:		std::cout << "you write mechanical keyboard?\n"; break;
			case "bad word"_CDict:						std::cout << "I don't understand, even don't have word in my database!\n"; break;
			default:									std::cout << "I don't understand!\n"; break;
		}
		std::cout << "Index in DB:\t" << DDict(text.c_str()) << "\n";
	}
	//PRINT(hello);
	//PRINT(world);
	//PRINT(foo);
	//PRINT(bar);
	//PRINT(zombie);
	//PRINT(war);
	//PRINT(player);
	//PRINT(bard);
	//PRINT(keyboard);
	//PRINT(mouse);
	//PRINT(speaker);
	
	//PRINT(Dict::_DEPOSES);
}




