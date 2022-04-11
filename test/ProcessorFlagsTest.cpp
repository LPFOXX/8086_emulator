#include <gtest/gtest.h>

#include "ProcessorTestFixture.hpp"

using namespace lp::emul8086;

TEST_F(ProcessorTestFixture, CanSetAFlagAndTheFlagRegisterIsModified)
{
	mProcessor.Flags.A = 1;

	EXPECT_EQ(mProcessor.Flags, FlagMasks::AuxiliaryFlagMask);
}

TEST_F(ProcessorTestFixture, CanSetMultipleFlagsAndTheFlagRegisterIsModifiedAccordingly)
{
	mProcessor.Flags.A = true;
	mProcessor.Flags.C = true;
	mProcessor.Flags.O = true;

	const Word multipleFlagsSet =
		(Word)FlagMasks::AuxiliaryFlagMask |
		(Word)FlagMasks::CarryFlagMask |
		(Word)FlagMasks::OverflowFlagMask;

	const ProcessorFlags flags = mProcessor.Flags;

	EXPECT_EQ(flags, multipleFlagsSet);
	EXPECT_TRUE(flags & FlagMasks::AuxiliaryFlagMask);
	EXPECT_TRUE(flags & FlagMasks::CarryFlagMask);
	EXPECT_TRUE(flags & FlagMasks::OverflowFlagMask);
}
