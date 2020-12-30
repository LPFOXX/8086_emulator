#include <gtest/gtest.h>

#include <m8086.hpp>

using namespace lp::m8086;

class M8086ADDEbGb8BitDisplacementTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		mProcessor.reset();
		mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	}

protected:
	int mInstructionsToExecute = 1;
	Processor mProcessor;
};

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeSI)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV SI, 01A2h
	// MOV [SI + 012h], 015h
	// MOV CL, 042h
	// ADD [SI + 012h], CL	-> indexed relative addressing mode: ADD (DS * 16) + SI + 0x12, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.DS = 0x0700;
	mProcessor.SI = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x12] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::RM_SI);
	mProcessor.mMemory[0xFFFF2] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.SI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x12], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeDI)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV DI,
	// MOV [DI + 012h], 015h
	// MOV CL, 042h
	// ADD [DI + 012h], CL	-> indexed relative addressing mode: ADD (DS * 16) + DI + 0x12, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.DS = 0x0700;
	mProcessor.DI = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x12] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::RM_DI);
	mProcessor.mMemory[0xFFFF2] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.DI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x12], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBX)
{
	// MOV AX, 00780h
	// MOV CS, AX
	// MOV BX, 01A2h
	// MOV[BX + 025h], 015h	; 0x1A2 + 0x50 + 0x25 = 0x217
	// MOV CL, 042h
	// ADD[BX + 025h], CL	; based relative (with 8bit displacement) addressing mode: ADD (SS * 16) + BX + 0x25, CL
	// ADD 042h, 015h		-> 057h

	mProcessor.DS = 0x0780;
	mProcessor.BX = 0x1A2;
	mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x25] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::RM_BX);
	mProcessor.mMemory[0xFFFF2] = 0x25;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x25], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBP)
{
	// MOV AX, 00700h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV [BP + 012h], 015h
	// MOV CL, 042h
	// ADD [BP + 012h], CL	-> based relative addressing mode: ADD (SS * 16) + BP + 0x12, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.SS = 0x0700;
	mProcessor.BP = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x12] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::RM_BP);
	mProcessor.mMemory[0xFFFF2] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x12], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXSI)
{
	// MOV AX, 00780h
	// MOV DS, AX
	// MOV BX, 01A2h
	// MOV SI, 050h
	// MOV[BX + SI + 012h], 015h	; 0x1A2 + 0x50 + 012h = 0x1f2
	// MOV CL, 042h
	// ADD[BX + SI + 012h], CL		; base indexed addressing mode: ADD (DS * 16) + BX + SI + 0x12, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x12;

	mProcessor.DS = 0x0780;
	mProcessor.BX = 0x1A2;
	mProcessor.SI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::BXPlusSI);
	mProcessor.mMemory[0xFFFF2] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.SI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXDI)
{
	// MOV AX, 00780h
	// MOV DS, AX
	// MOV BX, 01A2h
	// MOV DI, 050h
	// MOV[BX + DI + 12h], 015h	; 0x1A2 + 0x50 + 0x12 = 0x204
	// MOV CL, 042h
	// ADD[BX + DI + 12h], CL		; base indexed relative addressing mode: ADD (DS * 16) + BX + DI + 0x12, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x12;
	mProcessor.DS = 0x0780;
	mProcessor.BX = 0x1A2;
	mProcessor.DI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::BXPlusDI);
	mProcessor.mMemory[0xFFFF2] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.DI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBPSI)
{
	// MOV AX, 00780h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV SI, 050h
	// MOV[BP + SI + 12h], 015h	; 0x1A2 + 0x50 + 12h = 0x1f2
	// MOV CL, 042h
	// ADD[BP + SI + 12h], CL	; base indexed addressing mode: ADD (SS * 16) + BP + SI + 0x12, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x12;

	mProcessor.SS = 0x0780;
	mProcessor.BP = 0x1A2;
	mProcessor.SI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::BPPlusSI);
	mProcessor.mMemory[0xFFFF2] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.SI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGb8BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelatieveAddressingModeBPDI)
{
	// MOV AX, 00780h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV DI, 050h
	// MOV[BP + DI + 12h], 015h	; 0x1A2 + 0x50 + 12h = 0x1f2
	// MOV CL, 042h
	// ADD[BP + DI + 12h], CL	; base indexed addressing mode: ADD (SS * 16) + BP + DI + 0x12, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x12;

	mProcessor.SS = 0x0780;
	mProcessor.BP = 0x1A2;
	mProcessor.DI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::BPPlusDI);
	mProcessor.mMemory[0xFFFF2] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.DI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}