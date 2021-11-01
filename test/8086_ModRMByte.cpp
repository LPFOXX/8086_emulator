#include <gtest/gtest.h>
#include <m8086.hpp>

using namespace lp::m8086;

TEST(M8086ModRM, CanSetModRMByteAndGetAllValuesIndivially)
{
	ModRMByte byte = 0x89;	// 0b10 001 001

	EXPECT_EQ(byte.Mod, 0b10);	// which is 2
	EXPECT_EQ(byte.Reg, 0b001);	// which is 1
	EXPECT_EQ(byte.RM, 0b001);	// which is 1
}
//
//TEST(M8086ModRM, CanSetModValueOfRMByteAndTheByteValue)
//{
//	ModRMByte byte = 0;
//	byte.Mod = 0b11;	// which is 3
//	// 1100 0000 -> C0
//
//	EXPECT_EQ(byte, 0xC0);	// which is 192 = 0xC0
//}
//
//TEST(M8086ModRM, CanSetRegValueOfRMByteAndTheByteValue)
//{
//	ModRMByte byte = 0;
//	byte.Reg = 0b010;	// which is 2
//	// 0001 0000 -> 10
//
//	EXPECT_EQ(byte, 0x10);	// which is 16 = 0xC0
//}
//
//TEST(M8086ModRM, CanSetRMValueOfRMByteAndTheByteValue)
//{
//	ModRMByte byte = 0;
//	byte.RM = 0b101;	// which is 5
//	// 0000 0101 -> 05
//
//	EXPECT_EQ(byte, 0x05);	// which is 5 = 0x05
//}
//
//TEST(M8086ModRM, CanSetModRegAndRMValueOfRMByteAndTheByteValue)
//{
//	ModRMByte byte = 0;
//	byte.Mod = 0b11;	// which is 3
//	byte.Reg = 0b010;	// which is 2
//	byte.RM = 0b101;	// which is 5
//	// 1101 0101 -> D5
//
//	EXPECT_EQ(byte, 0xD5);	// which is 213 = 0xD5
//}