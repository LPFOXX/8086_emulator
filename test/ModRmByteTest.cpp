#include <gtest/gtest.h>

#include "ModRmByte.hpp"

using namespace lp::emul8086;

class ModRmByteTest : public ::testing::Test
{
public:
	
};

Byte modRmByte2Byte(Byte mod, Byte reg, Byte rm)
{
	Byte byte = 0;
	byte |= mod; byte <<= 3;
	byte |= reg; byte <<= 3;
	byte |= rm;

	return byte;
}


TEST(ModRmByteTest, CanSetModRMByteAndGetAllValuesIndivially)
{
	ModRmByte byte = 0x89;	// 0b10 001 001

	EXPECT_EQ(byte.Mod, 0b10);	// which is 2
	EXPECT_EQ(byte.Reg, 0b001);	// which is 1
	EXPECT_EQ(byte.Rm, 0b001);	// which is 1
}

TEST(ModRmByteTest, CanSetModValueOfRmByteAndTheByteValue)
{
	ModRmByte byte = 0;
	byte.Mod = 0b11;	// which is 3

	// 1100 0000 -> C0
	Byte result = modRmByte2Byte(byte.Mod, byte.Reg, byte.Rm);

	EXPECT_EQ(0xC0, result);	// which is 192 = 0xC0
}

TEST(ModRmByteTest, CanSetRegValueOfRmByteAndTheByteValue)
{
	ModRmByte byte = 0;
	byte.Reg = 0b010;	// which is 2

	// 0001 0000 -> 10
	Byte result = modRmByte2Byte(byte.Mod, byte.Reg, byte.Rm);

	EXPECT_EQ(0x10, result);	// which is 16 = 0x10
}

TEST(ModRmByteTest, CanSetRmValueOfRmByteAndTheByteValue)
{
	ModRmByte byte = 0;
	byte.Rm = 0b101;	// which is 5

	// 0000 0101 -> 05
	Byte result = modRmByte2Byte(byte.Mod, byte.Reg, byte.Rm);

	EXPECT_EQ(0x05, result);	// which is 5 = 0x05
}

TEST(ModRmByteTest, CanSetModRegAndRmValueOfRmByteAndTheByteValue)
{
	ModRmByte byte = 0;
	byte.Mod = 0b11;	// which is 3
	byte.Reg = 0b010;	// which is 2
	byte.Rm = 0b101;	// which is 5

	// 1101 0101 -> D5
	Byte result = modRmByte2Byte(byte.Mod, byte.Reg, byte.Rm);

	EXPECT_EQ(0xD5, result);	// which is 213 = 0xD5
}
