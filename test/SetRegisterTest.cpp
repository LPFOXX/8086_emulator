#include <gtest/gtest.h>

#include "8086.hpp"
#include "Processor.hpp"

using ::testing::Test;
using namespace lp::emul8086;

class SetRegisterTest : public Test
{
public:
	void SetUp() override
	{
		mProcessor.reset();
	}

protected:
	Processor mProcessor;
};

TEST_F(SetRegisterTest, CanSetGeneralPurposeRegistersLowByte)
{
	mProcessor.setRegister(Registers::AL, 0x51, false);
	mProcessor.setRegister(Registers::BL, 0x6E, false);
	mProcessor.setRegister(Registers::CL, 0x68, false);
	mProcessor.setRegister(Registers::DL, 0xB8, false);

	EXPECT_EQ(0x51, mProcessor.AL);
	EXPECT_EQ(0x6E, mProcessor.BL);
	EXPECT_EQ(0x68, mProcessor.CL);
	EXPECT_EQ(0xB8, mProcessor.DL);
}
