#include <gtest/gtest.h>

#include "8086TestFixture.hpp"

using namespace lp;

TEST_F(M8086Test, ADDEbGbCanAddTwoBytesAndStoreTheResultInTheFirstOperand)
{
	mProcessor.mMemory[0x8000] = m8086::Processor::OpCodes::ADD_Eb_Gb;
	// The ModR/M byte:
	// Mod: 11 -> Register addressing mode
	// Reg: 000 -> AL register
	mProcessor.mMemory[0x8001] = 0b11000101;
}

TEST_F(M8086Test, ADDALIbCanAddAByteToALAndStoreTheValueIntoAL)
{
	mProcessor.AL = 0x10;
	mProcessor.mMemory[0x8000] = m8086::Processor::OpCodes::ADD_AL_Ib;
	mProcessor.mMemory[0x8001] = 0x74;

	EXPECT_EQ(mProcessor.AL, 0x84);
}