#include <gtest/gtest.h>

#include <m8086.hpp>

using namespace lp::m8086;

class M8086ADDEbGbNoDisplacementTest : public ::testing::Test
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

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeSI)
{
	// MOV AX, 0700h
	// MOV DS, AX
	// MOV CL, 42h
	// MOV SI, 01A2h
	// MOV [SI], 15h
	// ADD [SI], CL		;	register indirect addressing mode: ADD (DS * 16) + SI, CL
	// ;ADD 15h, 42h	;	0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2;

	mProcessor.DS = 0x0700;
	mProcessor.SI = 0x01A2;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_SI);

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.SI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeDI)
{
	// MOV AX, 0700h
	// MOV DS, AX
	// MOV CL, 42h
	// MOV DI, 0142h
	// MOV [DI], 15h
	// ADD [DI], CL		;	register indirect addressing mode: ADD (DS * 16) + DI, CL
	// ;ADD 15h, 42h	;	0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2;

	mProcessor.DS = 0x0700;
	mProcessor.DI = 0x01A2;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_DI);

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.DI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeBX)
{
	// MOV AX, 0700h
	// MOV DS, AX
	// MOV CL, 42h
	// MOV BX, 01A2h
	// MOV [BX], 15h
	// ADD [BX], CL		;	register indirect addressing mode: ADD (DS * 16) + BX, CL
	// ;ADD 15h, 42h	;	0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2;

	mProcessor.DS = 0x0700;
	mProcessor.BX = 0x01A2;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_BX);

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeBP)
{
	// MOV AX, 0700h
	// MOV SS, AX
	// MOV CL, 42h
	// MOV BP, 01A2h
	// MOV [BP], 15h
	// ADD [BP], CL		;	register indirect addressing mode: ADD (SS * 16) + BP, CL
	// ;ADD 15h, 42h	;	0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2;

	mProcessor.SS = 0x0700;
	mProcessor.BP = 0x01A2;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::RM_BP);
	mProcessor.mMemory[0xFFFF2] = 0x00;

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBXSI)
{
	// MOV AX, 0700h
	// MOV DS, AX
	// MOV CL, 42h
	// MOV BX, 01A2h
	// MOV SI, 50h
	// MOV[BX + SI], 15h	; 0x01A2 + 0x50 = 0x1F2
	// ADD[BX + SI], CL		; base indexed addressing mode: ADD (DS * 16) + BX + SI, CL
	// ;ADD 15h, 42h		; 0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x50;

	mProcessor.DS = 0x0700;
	mProcessor.BX = 0x01A2;
	mProcessor.SI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BXPlusSI);

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x01A2);
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

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBXDI)
{
	// MOV AX, 0700h
	// MOV DS, AX
	// MOV CL, 42h
	// MOV BX, 01A2h
	// MOV DI, 50h
	// MOV[BX + DI], 15h	; 0x01A2 + 0x50 = 0x1F2
	// ADD[BX + DI], CL		; base indexed addressing mode: ADD (DS * 16) + BX + DI, CL
	// ;ADD 15h, 42h		; 0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x50;

	mProcessor.DS = 0x0700;
	mProcessor.BX = 0x01A2;
	mProcessor.DI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BXPlusDI);

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x01A2);
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

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBPSI)
{
	// MOV AX, 0700h
	// MOV SS, AX
	// MOV CL, 42h
	// MOV BP, 01A2h
	// MOV SI, 50h
	// MOV[BP + SI], 15h	; 0x01A2 + 0x50 = 0x1F2
	// ADD[BP + SI], CL		; base indexed addressing mode: ADD (SS * 16) + BP + SI, CL
	// ;ADD 15h, 42h		; 0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x50;

	mProcessor.SS = 0x0700;
	mProcessor.BP = 0x01A2;
	mProcessor.SI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BPPlusSI);

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x01A2);
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

TEST_F(M8086ADDEbGbNoDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBPDI)
{
	// MOV AX, 0700h
	// MOV SS, AX
	// MOV CL, 42h
	// MOV BP, 01A2h
	// MOV DI, 50h
	// MOV[BP + DI], 15h	; 0x01A2 + 0x50 = 0x1F2
	// ADD[BP + DI], CL		; base indexed addressing mode: ADD (SS * 16) + BP + DI, CL
	// ;ADD 15h, 42h		; 0x57

	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x50;

	mProcessor.SS = 0x0700;
	mProcessor.BP = 0x01A2;
	mProcessor.DI = 0x50;
	mProcessor.mMemory[memoryAddress] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BPPlusDI);

	mProcessor.executeInstructions(mInstructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x01A2);
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