#include <utility>
#include <gtest/gtest.h>

#include "8086TestFixture.hpp"

using namespace lp::m8086;
using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

struct Add2RegistersTestValues
{
	struct SameRegs
	{
		Word value;
		Flags expectedFlags;
		Word expectedResult;
	};

	Processor* processor;
	Byte srcValue;
	Byte destValue;
	Registers srcReg;
	Registers destReg;
	Flags expectedFlags;
	Word expectedResult;
	SameRegs sameRegs;
};

class ADDEbGbWithEvery8BitGeneralPurposeRegisters : public TestWithParam<std::tuple<Registers, Registers>>
{
public:
	void SetUp() override
	{
		mProcessor.reset();
	}

protected:
	Processor mProcessor;
};

void addSameByteRegister(
	const int instructionsToExecute,
	const Add2RegistersTestValues& testValues)
{
	Processor& processor = *testValues.processor;
	Add2RegistersTestValues::SameRegs sameRegs = testValues.sameRegs;

	processor.setRegister(testValues.srcReg, sameRegs.value, false);

	processor.Flags.C = !sameRegs.expectedFlags.C;
	processor.Flags.Z = !sameRegs.expectedFlags.Z;
	processor.Flags.S = !sameRegs.expectedFlags.S;
	processor.Flags.O = !sameRegs.expectedFlags.O;
	processor.Flags.P = !sameRegs.expectedFlags.P;
	processor.Flags.A = !sameRegs.expectedFlags.A;

	processor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	processor.mMemory[0xFFFF1] =
		ModRMByte(Mod::RegisterValue, testValues.srcReg, testValues.srcReg);

	processor.executeInstruction(instructionsToExecute);

	EXPECT_EQ(processor.CS, 0xFFFF);
	EXPECT_EQ(processor.IP, 0x0002);

	EXPECT_EQ(processor.getRegister(testValues.srcReg, false), sameRegs.expectedResult);
	EXPECT_EQ(processor.Flags, sameRegs.expectedFlags);
}

void addDifferentByteRegisters(
	const int instructionsToExecute,
	const Add2RegistersTestValues& testValues)
{
	Processor& processor = *testValues.processor;

	processor.setRegister(testValues.srcReg, testValues.srcValue, false);
	processor.setRegister(testValues.destReg, testValues.destValue, false);

	processor.Flags.C = !testValues.expectedFlags.C;
	processor.Flags.Z = !testValues.expectedFlags.Z;
	processor.Flags.S = !testValues.expectedFlags.S;
	processor.Flags.O = !testValues.expectedFlags.O;
	processor.Flags.P = !testValues.expectedFlags.P;
	processor.Flags.A = !testValues.expectedFlags.A;

	processor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	processor.mMemory[0xFFFF1] =
		ModRMByte(Mod::RegisterValue, testValues.srcReg, testValues.destReg);

	processor.executeInstruction(instructionsToExecute);

	EXPECT_EQ(processor.CS, 0xFFFF);
	EXPECT_EQ(processor.IP, 0x0002);

	EXPECT_EQ(processor.getRegister(testValues.srcReg, false), testValues.srcValue);
	EXPECT_EQ(processor.getRegister(testValues.destReg, false), testValues.expectedResult);
	EXPECT_EQ(processor.Flags, testValues.expectedFlags);
}

void addTwoByteRegisters(const Add2RegistersTestValues& testValues)
{
	const int instructionsToExecute = 1;

	if (testValues.srcReg == testValues.destReg) {
		addSameByteRegister(instructionsToExecute, testValues);
	}
	else {
		addDifferentByteRegisters(instructionsToExecute, testValues);
	}
}

TEST_P(ADDEbGbWithEvery8BitGeneralPurposeRegisters, CanAddTwo8BitRegistersAndStoreTheResultOnTheFirstOne)
{
	const auto& [dest, src] = GetParam();

	Add2RegistersTestValues testValues;
	testValues.processor = &mProcessor;
	testValues.destReg = dest;
	testValues.srcReg = src;

	if (dest == src) {
		Flags sameRegsExpectedFlags;
		sameRegsExpectedFlags.C = false;
		sameRegsExpectedFlags.Z = false;
		sameRegsExpectedFlags.S = false;
		sameRegsExpectedFlags.O = false;
		sameRegsExpectedFlags.P = false;
		sameRegsExpectedFlags.A = false;

		testValues.sameRegs.value = 0x15;
		testValues.sameRegs.expectedResult = 0x2A;
		testValues.sameRegs.expectedFlags = sameRegsExpectedFlags;
	}
	else {
		Flags expectedFlags;
		expectedFlags.C = false;
		expectedFlags.Z = false;
		expectedFlags.S = false;
		expectedFlags.O = false;
		expectedFlags.P = true;
		expectedFlags.A = false;

		testValues.srcValue = 0x2A;
		testValues.destValue = 0x15;
		testValues.expectedResult = dest == src ? 0x2A : 0x3F;
		testValues.expectedFlags = expectedFlags;
	}

	addTwoByteRegisters(testValues);
}

const std::vector<Registers> registers = { {
	Registers::AL,
	Registers::BL,
	Registers::CL,
	Registers::DL,
	Registers::AH,
	Registers::BH,
	Registers::CH,
	Registers::DH,
} };

INSTANTIATE_TEST_SUITE_P(
	AddAll8BitGPR2By2,
	ADDEbGbWithEvery8BitGeneralPurposeRegisters,
	Combine(
	ValuesIn(registers),
	ValuesIn(registers)));

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
	testValues.sameRegs.value = 0x15;
	testValues.sameRegs.expectedResult = 0x2A;
	testValues.sameRegs.expectedFlags = expectedFlags;

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