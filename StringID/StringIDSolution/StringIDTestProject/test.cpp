#include "pch.h"

#include "../../StringIDSolution/SIDlib/SID.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestCaseName, TestFun) {
	EXPECT_EQ(fun(15, 12), 15+12*5);
}
