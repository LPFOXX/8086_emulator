#include <gtest/gtest.h>

#include "Registers.hpp"
#include "Processor.hpp"

using namespace lp::emul8086;
using ::testing::Test;

class GetRegisterTest : public Test
{
public:
	void SetUp() override
	{
		mProcessor.reset();
	}
protected:
	Processor mProcessor;
};

TEST_F(GetRegisterTest, CanGetGeneralPurposeRegistersLoByteValues)
{
	mProcessor.AL = 0x51;
	mProcessor.BL = 0x6E;
	mProcessor.CL = 0x68;
	mProcessor.DL = 0xB8;

	EXPECT_EQ(0x51, mProcessor.getRegister(Registers::AL, false));
	EXPECT_EQ(0x6E, mProcessor.getRegister(Registers::BL, false));
	EXPECT_EQ(0x68, mProcessor.getRegister(Registers::CL, false));
	EXPECT_EQ(0xB8, mProcessor.getRegister(Registers::DL, false));
}

TEST_F(GetRegisterTest, CanGetGeneralPurposeRegistersHiByteValues)
{
	mProcessor.AH = 0x51;
	mProcessor.BH = 0x6E;
	mProcessor.CH = 0x68;
	mProcessor.DH = 0xB8;

	EXPECT_EQ(0x51, mProcessor.getRegister(Registers::AH, false));
	EXPECT_EQ(0x6E, mProcessor.getRegister(Registers::BH, false));
	EXPECT_EQ(0x68, mProcessor.getRegister(Registers::CH, false));
	EXPECT_EQ(0xB8, mProcessor.getRegister(Registers::DH, false));
}

TEST_F(GetRegisterTest, CanGetGeneralPurposeRegistersWordValues)
{
	mProcessor.AX = 0x5115;
	mProcessor.BX = 0x6EE6;
	mProcessor.CX = 0x6886;
	mProcessor.DX = 0xB88B;

	EXPECT_EQ(0x5115, mProcessor.getRegister(Registers::AX, true));
	EXPECT_EQ(0x6EE6, mProcessor.getRegister(Registers::BX, true));
	EXPECT_EQ(0x6886, mProcessor.getRegister(Registers::CX, true));
	EXPECT_EQ(0xB88B, mProcessor.getRegister(Registers::DX, true));
}

TEST_F(GetRegisterTest, CanGetSegmentRegistersWordValues)
{
	mProcessor.SP = 0x5115;
	mProcessor.BP = 0x6EE6;
	mProcessor.SI = 0x6886;
	mProcessor.DI = 0xB88B;

	EXPECT_EQ(0x5115, mProcessor.getRegister(Registers::SP, true));
	EXPECT_EQ(0x6EE6, mProcessor.getRegister(Registers::BP, true));
	EXPECT_EQ(0x6886, mProcessor.getRegister(Registers::SI, true));
	EXPECT_EQ(0xB88B, mProcessor.getRegister(Registers::DI, true));
}
