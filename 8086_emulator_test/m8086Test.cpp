#include <gtest/gtest.h>

#include "8086TestFixture.hpp"

TEST_F(M8086Test, CanSetAXRegisterAndGetMostSignificantByteInAH)
{
	mProcessor.AX = 0x0504;

	EXPECT_EQ(mProcessor.AH, 0x05);
}

TEST_F(M8086Test, CanSetAXRegisterAndGetLeastSignificantByteInAL)
{
	mProcessor.AX = 0x0504;

	EXPECT_EQ(mProcessor.AL, 0x04);
}

TEST_F(M8086Test, CanSetAHAndALRegistersAndGetTheValueInAX)
{
	mProcessor.AL = 0x15;
	mProcessor.AH = 0x30;

	EXPECT_EQ(mProcessor.AX, 0x3015);
}

TEST_F(M8086Test, CanSetBXRegisterAndGetMostSignificantByteInBH)
{
	mProcessor.BX = 0x0504;

	EXPECT_EQ(mProcessor.BH, 0x05);
}

TEST_F(M8086Test, CanSetBXRegisterAndGetLeastSignificantByteInBL)
{
	mProcessor.BX = 0x0504;

	EXPECT_EQ(mProcessor.BL, 0x04);
}

TEST_F(M8086Test, CanSetBHAndALRegistersAndGetTheValueInBX)
{
	mProcessor.BL = 0x15;
	mProcessor.BH = 0x30;

	EXPECT_EQ(mProcessor.BX, 0x3015);
}

TEST_F(M8086Test, CanSetCXRegisterAndGetMostSignificantByteInCH)
{
	mProcessor.CX = 0x0504;

	EXPECT_EQ(mProcessor.CH, 0x05);
}

TEST_F(M8086Test, CanSetCXRegisterAndGetLeastSignificantByteInCL)
{
	mProcessor.CX = 0x0504;

	EXPECT_EQ(mProcessor.CL, 0x04);
}

TEST_F(M8086Test, CanSetCHAndALRegistersAndGetTheValueInCX)
{
	mProcessor.CL = 0x15;
	mProcessor.CH = 0x30;

	EXPECT_EQ(mProcessor.CX, 0x3015);
}

TEST_F(M8086Test, CanSetDXRegisterAndGetMostSignificantByteInDH)
{
	mProcessor.DX = 0x0504;

	EXPECT_EQ(mProcessor.DH, 0x05);
}

TEST_F(M8086Test, CanSetDXRegisterAndGetLeastSignificantByteInDL)
{
	mProcessor.DX = 0x0504;

	EXPECT_EQ(mProcessor.DL, 0x04);
}

TEST_F(M8086Test, CanSetDHAndALRegistersAndGetTheValueInDX)
{
	mProcessor.DL = 0x15;
	mProcessor.DH = 0x30;

	EXPECT_EQ(mProcessor.DX, 0x3015);
}

TEST_F(M8086Test, CanSetAFlagAndTheFlagRegisterIsModified)
{
	using namespace lp;
	mProcessor.Flags = 0x0000;
	mProcessor.Flags.A = 1;

	EXPECT_EQ(mProcessor.Flags, m8086::FlagMasks::AuxiliarFlagMask);
}

TEST_F(M8086Test, CanSetMultipleFlagsAndTheFlagRegisterIsModifiedAccordingly)
{
	using namespace lp::m8086;

	mProcessor.Flags = 0x0000;
	mProcessor.Flags.A = true;
	mProcessor.Flags.C = true;
	mProcessor.Flags.O = true;

	const Word multipleFlagsSet =
		(Word)FlagMasks::AuxiliarFlagMask |
		(Word)FlagMasks::CarryFlagMask |
		(Word)FlagMasks::OverflowFlagMask;

	const Word flags = mProcessor.Flags;

	EXPECT_EQ(flags, multipleFlagsSet);
	EXPECT_TRUE((flags & (Word)FlagMasks::AuxiliarFlagMask) > 0);
	EXPECT_TRUE((flags & (Word)FlagMasks::CarryFlagMask) > 0);
	EXPECT_TRUE((flags & (Word)FlagMasks::OverflowFlagMask) > 0);
}