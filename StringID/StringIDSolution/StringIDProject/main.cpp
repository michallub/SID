#include <iostream>
#include "../SIDlib/include/sid/SID.h"
#include <string>


template<uint64_t NAME>
void testfun()
{
	for (auto e : sid::unpack(sid::SID{ NAME }))
		std::cout << e;
	std::cout << std::endl;
}

int main()
{
	//SID can be use in switch cases
	sid::SID stringid;
	do
	{
		std::string text;
		std::getline(std::cin, text);
		stringid = sid::packAlnum(text.data(), text.size());
		switch (stringid)
		{
		case ""_SIDALNUM: std::cout << "Nothing to say?\n"; break;
		case "hello"_SIDALNUM: std::cout << "world!\n"; break;
		case "1 2 3"_SIDALNUM: std::cout << "4 5 6!\n"; break;
		default:std::cout << "I don't understand!\n";
		}


	} while (stringid != "quit"_SIDALNUM);
	std::cout << "bye!\n";

	//SID can be use as template params
	testfun<"Foo"_SIDASCII>();
	testfun<"Bar"_SIDBASE64>();




}



