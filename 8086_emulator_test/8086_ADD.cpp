#include <gtest/gtest.h>

#include "8086TestFixture.hpp"

using namespace lp::m8086;

struct Add2RegistersTestValues
{
	Processor* processor;
	Byte srcValue;
	Byte destValue;
	Registers srcReg;
	Registers destReg;
	Flags expectedFlags;
	Word expectedResult;
};

void addTwoByteRegisters(
	Add2RegistersTestValues testValues)
{
	Processor& processor = *testValues.processor;
	processor.setRegister(testValues.destReg, testValues.destValue, false);
	processor.setRegister(testValues.srcReg, testValues.srcValue, false);
	processor.Flags.C = !testValues.expectedFlags.C;
	processor.Flags.Z = !testValues.expectedFlags.Z;
	processor.Flags.S = !testValues.expectedFlags.S;
	processor.Flags.O = !testValues.expectedFlags.O;
	processor.Flags.P = !testValues.expectedFlags.P;
	processor.Flags.A = !testValues.expectedFlags.A;

	processor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	processor.mMemory[0xFFFF1] =
		ModRMByte(Mod::RegisterValue, testValues.srcReg, testValues.destReg);

	const int instructionsToExecute = 1;

	processor.executeInstruction(instructionsToExecute);

	EXPECT_EQ(processor.CS, 0xFFFF);
	EXPECT_EQ(processor.IP, 0x0002);

	if (testValues.srcReg != testValues.destReg) {
		EXPECT_EQ(processor.getRegister(testValues.srcReg, false), testValues.srcValue);
	}
	EXPECT_EQ(processor.getRegister(testValues.destReg, false), testValues.expectedResult);
	EXPECT_EQ(processor.Flags, testValues.expectedFlags);
}

TEST_F(M8086Test, ADDEbGbCanAddTwoByteRegistersALPlusALResultInAL)
{
	Flags expectedFlags;
	expectedFlags.C = false;
	expectedFlags.Z = false;
	expectedFlags.S = false;
	expectedFlags.O = false;
	expectedFlags.P = false;
	expectedFlags.A = false;

	Add2RegistersTestValues testValues;
	testValues.processor = &mProcessor;
	testValues.destReg = Registers::AL;
	testValues.srcReg = Registers::AL;
	testValues.destValue = 0x15;
	testValues.srcValue = 0x15;
	testValues.expectedResult = 0x2A;
	testValues.expectedFlags = expectedFlags;

	addTwoByteRegisters(testValues);
}

TEST_F(M8086Test, ADDEbGbCanAddTwoByteRegistersBHPlusCLResultInBH)
{
	Flags expectedFlags;
	expectedFlags.C = false;
	expectedFlags.Z = false;
	expectedFlags.S = true;
	expectedFlags.O = false;
	expectedFlags.P = true;
	expectedFlags.A = false;

	Add2RegistersTestValues testValues;
	testValues.processor = &mProcessor;
	testValues.destReg = Registers::BH;
	testValues.destValue = 0x15;
	testValues.srcReg = Registers::CL;
	testValues.srcValue = 0xEA;
	testValues.expectedResult = 0xFF;
	testValues.expectedFlags = expectedFlags;

	addTwoByteRegisters(testValues);
}

//TEST_F(M8086Test, ADDALIbCanAddAByteToALAndStoreTheValueIntoAL)
//{
//	mProcessor.AL = 0x10;
//	mProcessor.mMemory[0x8000] = Processor::OpCodes::ADD_AL_Ib;
//	mProcessor.mMemory[0x8001] = 0x74;
//
//	EXPECT_EQ(mProcessor.AL, 0x84);
//}