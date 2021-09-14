#include "pch.h"

#include "../../StringIDSolution/SIDlib/include/siddict.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestCaseName, TestDefaultSID) {
	sid::SID defaultSID = {};
	EXPECT_EQ(defaultSID.val, 0);
	EXPECT_EQ(defaultSID, 0);	//operator uint64_tc
}

TEST(TestCaseName, TestAlnumEmpty) {
	sid::SID sidfunction = sid::packAlnum("", 0);
	sid::SID sidliteral = ""_SIDALNUM;

	EXPECT_EQ(sidfunction, sidliteral);
}
TEST(TestCaseName, TestAlnumHelloWorld) {
	sid::SID sidfunction = sid::packAlnum("HelloWorld", 10);
	sid::SID sidliteral = "helloworld"_SIDALNUM;

	EXPECT_EQ(sidfunction, sidliteral);
}

