#include <gtest/gtest.h>

#include "ProcessorTestFixture.hpp"

using namespace lp::emul8086;

TEST_F(ProcessorTestFixture, CanSetAXRegisterAndGetMostSignificantByteInAH)
{
	mProcessor.AX = 0x0504;

	EXPECT_EQ(mProcessor.AH, 0x05);
}

TEST_F(ProcessorTestFixture, CanSetAXRegisterAndGetLeastSignificantByteInAL)
{
	mProcessor.AX = 0x0504;

	EXPECT_EQ(mProcessor.AL, 0x04);
}

TEST_F(ProcessorTestFixture, CanSetAHAndALRegistersAndGetTheValueInAX)
{
	mProcessor.AL = 0x15;
	mProcessor.AH = 0x30;

	EXPECT_EQ(mProcessor.AX, 0x3015);
}

TEST_F(ProcessorTestFixture, CanSetBXRegisterAndGetMostSignificantByteInBH)
{
	mProcessor.BX = 0x0504;

	EXPECT_EQ(mProcessor.BH, 0x05);
}

TEST_F(ProcessorTestFixture, CanSetBXRegisterAndGetLeastSignificantByteInBL)
{
	mProcessor.BX = 0x0504;

	EXPECT_EQ(mProcessor.BL, 0x04);
}

TEST_F(ProcessorTestFixture, CanSetBHAndALRegistersAndGetTheValueInBX)
{
	mProcessor.BL = 0x15;
	mProcessor.BH = 0x30;

	EXPECT_EQ(mProcessor.BX, 0x3015);
}

TEST_F(ProcessorTestFixture, CanSetCXRegisterAndGetMostSignificantByteInCH)
{
	mProcessor.CX = 0x0504;

	EXPECT_EQ(mProcessor.CH, 0x05);
}

TEST_F(ProcessorTestFixture, CanSetCXRegisterAndGetLeastSignificantByteInCL)
{
	mProcessor.CX = 0x0504;

	EXPECT_EQ(mProcessor.CL, 0x04);
}

TEST_F(ProcessorTestFixture, CanSetCHAndALRegistersAndGetTheValueInCX)
{
	mProcessor.CL = 0x15;
	mProcessor.CH = 0x30;

	EXPECT_EQ(mProcessor.CX, 0x3015);
}

TEST_F(ProcessorTestFixture, CanSetDXRegisterAndGetMostSignificantByteInDH)
{
	mProcessor.DX = 0x0504;

	EXPECT_EQ(mProcessor.DH, 0x05);
}

TEST_F(ProcessorTestFixture, CanSetDXRegisterAndGetLeastSignificantByteInDL)
{
	mProcessor.DX = 0x0504;

	EXPECT_EQ(mProcessor.DL, 0x04);
}

TEST_F(ProcessorTestFixture, CanSetDHAndALRegistersAndGetTheValueInDX)
{
	mProcessor.DL = 0x15;
	mProcessor.DH = 0x30;

	EXPECT_EQ(mProcessor.DX, 0x3015);
}
