/**
 * This file tests adding two bytes together.
 *	source is a GPR.
 *	destination is a memory address.
 *	result (destination + source) is stored in the memory address.
 */

 // ADD CX, [481]		-> direct addressing mode: ADD CX, (DS * 16) + 481
 // ADD [481], CX		-> direct addressing mode: ADD (DS * 16) + 481, CX
 // ADD AL, [SI]		-> register indirect addressing mode: ADD AL, (DS * 16) + SI
 // ADD [SI], AL		-> register indirect addressing mode: ADD (DS * 16) + SI, AL
 // ADD DX, [BX + 5]	-> based relative addressing mode: ADD DX, (DS * 16) + BX + 5
 // ADD [BX + 5], DX	-> based relative addressing mode: ADD (DS * 16) + BX + 5, DX
 // ADD DX, [BP + 5]	-> based relative addressing mode: ADD DX, (SS * 16) + BP + 5
 // ADD [BP + 5], DX	-> based relative addressing mode: ADD (DS * 16) + BP + 5, DX
 // ADD AL, [DI] + 12	-> indexed relative addressing mode: ADD AL, (DS * 16) + DI + 12
 // ADD [DI] + 12, AL	-> indexed relative addressing mode: ADD (DS * 16) + DI + 12, AL
 // ADD AX, [BX+SI+20]	-> based indexed addressing mode: ADD AX, (DS * 16) + SI + BX + 20
 // ADD [BX+SI+20],AX	-> based indexed addressing mode: ADD (DS * 16) + SI + BX + 20, AX
 // ADD AX, [BP+SI+20]	-> based indexed addressing mode: ADD AX, (SS * 16) + SI + BP + 20
 // ADD [BP+SI+20], AX	-> based indexed addressing mode: ADD (SS * 16) + SI + BP + 20, AX

#include <8086TestFixture.hpp>

using namespace lp::m8086;

class M8086ADDEbGbTest : public ::testing::Test
{
public:
	void SetUp() override
	{
		mProcessor.reset();
	}

protected:
	Processor mProcessor;
};

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithDirectAddressingMode)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV [01a2h], 015h
	// MOV CL, 042h
	// ADD [01A2h], CL		-> direct addressing mode: ADD (DS * 16) + 0x1A2, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.CL = 0x42;
	mProcessor.DS = 0x0700;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2] = 0x15;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_BP);
	mProcessor.mMemory[0xFFFF2] = 0xA2;
	mProcessor.mMemory[0xFFFF3] = 0x01;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeSI)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV [SI], 015h
	// MOV CL, 042h
	// ADD [SI], CL		-> register indirect addressing mode: ADD (DS * 16) + SI, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.DS = 0x0700;
	mProcessor.SI = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_SI);

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.SI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeSI8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeSI16BitDisplacement)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV SI, 01A2h
	// MOV [SI + 01224h], 015h
	// MOV CL, 042h
	// ADD [SI + 01224h], CL	-> indexed relative addressing mode: ADD (DS * 16) + SI + 0x1224, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.DS = 0x0700;
	mProcessor.SI = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x1224] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_SI);
	mProcessor.mMemory[0xFFFF2] = 0x24;
	mProcessor.mMemory[0xFFFF3] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.SI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x1224], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeDI)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV DI,
	// MOV [DI], 015h
	// MOV CL, 042h
	// ADD [DI], CL		-> register indirect addressing mode: ADD (DS * 16) + DI, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.DS = 0x0700;
	mProcessor.DI = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_DI);

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.DI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeDI8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeDI16BitDisplacement)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV DI,
	// MOV [DI + 01224h], 015h
	// MOV CL, 042h
	// ADD [DI + 01224h], CL -> indexed relative addressing mode: ADD (DS * 16) + DI + 0x1224, CL
	// ADD 042h, 015h		 -> 057h
	mProcessor.DS = 0x0700;
	mProcessor.DI = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x1224] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_DI);
	mProcessor.mMemory[0xFFFF2] = 0x24;
	mProcessor.mMemory[0xFFFF3] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.DI, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x1224], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeBX)
{
	// MOV AX, 00700h
	// MOV DS, AX
	// MOV BX, 01A2h
	// MOV [BX], 015h
	// MOV CL, 042h
	// ADD [BX], CL		-> register indirect addressing mode: ADD (DS * 16) + BX, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.DS = 0x0700;
	mProcessor.BX = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_BX);

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBX8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBX16BitDisplacement)
{
	// MOV AX, 00780h
	// MOV CS, AX
	// MOV BX, 01A2h
	// MOV[BX + 01525h], 015h	; 0x1A2 + 0x50 + 0x1525 = 0x217
	// MOV CL, 042h
	// ADD[BX + 01525h], CL	; based relative (with bit displacement) addressing mode: ADD (SS * 16) + BX + 0x25, CL
	// ADD 042h, 015h		-> 057h

	mProcessor.DS = 0x0780;
	mProcessor.BX = 0x1A2;
	mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x1525] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_BX);
	mProcessor.mMemory[0xFFFF2] = 0x25;
	mProcessor.mMemory[0xFFFF3] = 0x15;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x1525], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithRegisterIndirectAddressingModeBP)
{
	// MOV AX, 00700h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV [BP], 015h
	// MOV CL, 042h
	// ADD [BP], CL		-> register indirect addressing mode: ADD (DS * 16) + BP, CL
	// ADD 042h, 015h		-> 057h
	mProcessor.SS = 0x0700;
	mProcessor.BP = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::ByteDisplacement, Registers::CL, RM::RM_BP);
	mProcessor.mMemory[0xFFFF2] = 0x00;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0003);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBP8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBP16BitDisplacement)
{
	// MOV AX, 00700h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV [BP + 01224h], 015h
	// MOV CL, 042h
	// ADD [BP + 01224h], CL -> based relative addressing mode: ADD (SS * 16) + BP + 0x1224, CL
	// ADD 042h, 015h		 -> 057h
	mProcessor.SS = 0x0700;
	mProcessor.BP = 0x1A2;
	mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x1224] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_BP);
	mProcessor.mMemory[0xFFFF2] = 0x24;
	mProcessor.mMemory[0xFFFF3] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0700);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.mMemory[(0x0700 << 4) + 0x1A2 + 0x1224], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBXSI)
{
	// MOV AX, 00780h
	// MOV DS, AX
	// MOV BX, 01A2h
	// MOV SI, 050h
	// MOV[BX + SI], 015h	; 0x1A2 + 0x50 = 0x1f2
	// MOV CL, 042h
	// ADD[BX + SI], CL		; base indexed addressing mode: ADD (DS * 16) + BX + SI, CL
	// ADD 042h, 015h		-> 057h

	mProcessor.DS = 0x0780;
	mProcessor.BX = 0x1A2;
	mProcessor.SI = 0x50;
	mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BXPlusSI);

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.SI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXSI8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXSI16BitDisplacement)
{
	// MOV AX, 00780h
	// MOV DS, AX
	// MOV BX, 01A2h
	// MOV SI, 050h
	// MOV[BX + SI + 012h], 015h	; 0x1A2 + 0x50 + 0x12 = 0x1f2
	// MOV CL, 042h
	// ADD[BX + SI + 012], CL		; base indexed addressing mode: ADD (DS * 16) + BX + SI + 0x12, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x1224;

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
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BXPlusSI);
	mProcessor.mMemory[0xFFFF2] = 0x24;
	mProcessor.mMemory[0xFFFF3] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBXDI)
{
	// MOV AX, 00780h
	// MOV DS, AX
	// MOV BX, 01A2h
	// MOV DI, 050h
	// MOV[BX + DI], 015h	; 0x1A2 + 0x50 = 0x1f2
	// MOV CL, 042h
	// ADD[BX + DI], CL		; base indexed addressing mode: ADD (DS * 16) + BX + DI, CL
	// ADD 042h, 015h		-> 057h

	mProcessor.DS = 0x0780;
	mProcessor.BX = 0x1A2;
	mProcessor.DI = 0x50;
	mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BXPlusDI);

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BX, 0x1A2);
	EXPECT_EQ(mProcessor.DI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXDI8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXDI16BitDisplacement)
{
	// MOV AX, 00780h
	// MOV DS, AX
	// MOV BX, 01A2h
	// MOV DI, 050h
	// MOV[BX + DI + 1224h], 015h	; 0x1A2 + 0x50 + 0x1224 = 0x1416
	// MOV CL, 042h
	// ADD[BX + DI + 1224h], CL		; base indexed addressing mode: ADD (DS * 16) + BX + DI + 0x1224, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x1224;

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
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BXPlusDI);
	mProcessor.mMemory[0xFFFF2] = 0x24;
	mProcessor.mMemory[0xFFFF3] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.DS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBPSI)
{
	// MOV AX, 00780h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV SI, 050h
	// MOV[BP + SI], 015h	; 0x1A2 + 0x50 = 0x1f2
	// MOV CL, 042h
	// ADD[BP + SI], CL		; base indexed addressing mode: ADD (SS * 16) + BP + SI, CL
	// ADD 042h, 015h		-> 057h

	mProcessor.SS = 0x0780;
	mProcessor.BP = 0x1A2;
	mProcessor.SI = 0x50;
	mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BPPlusSI);

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.SI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBPSI8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBPSI16BitDisplacement)
{
	// MOV AX, 00780h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV SI, 050h
	// MOV[BP + SI + 1224h], 015h	; 0x1A2 + 0x50 + 0x1224 = 0x1f2
	// MOV CL, 042h
	// ADD[BP + SI + 1224h], CL		; base indexed addressing mode: ADD (SS * 16) + BP + SI + 0x1224, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x1224;

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
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BPPlusSI);
	mProcessor.mMemory[0xFFFF2] = 0x24;
	mProcessor.mMemory[0xFFFF3] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedAddressingModeBPDI)
{
	// MOV AX, 00780h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV DI, 050h
	// MOV[BP + DI], 015h	; 0x1A2 + 0x50 = 0x1f2
	// MOV CL, 042h
	// ADD[BP + DI], CL		; base indexed addressing mode: ADD (SS * 16) + BP + DI, CL
	// ADD 042h, 015h		-> 057h

	mProcessor.SS = 0x0780;
	mProcessor.BP = 0x1A2;
	mProcessor.DI = 0x50;
	mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50] = 0x15;
	mProcessor.CL = 0x42;
	mProcessor.Flags.C = true;
	mProcessor.Flags.Z = true;
	mProcessor.Flags.S = true;
	mProcessor.Flags.O = true;
	mProcessor.Flags.P = true;
	mProcessor.Flags.A = true;

	mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::BPPlusDI);

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0002);

	EXPECT_EQ(mProcessor.CL, 0x42);
	EXPECT_EQ(mProcessor.BP, 0x1A2);
	EXPECT_EQ(mProcessor.DI, 0x50);
	EXPECT_EQ(mProcessor.mMemory[(0x0780 << 4) + 0x1A2 + 0x50], 0x57);

	const Flags& flags = mProcessor.Flags;
	EXPECT_FALSE(flags.C);
	EXPECT_FALSE(flags.Z);
	EXPECT_FALSE(flags.S);
	EXPECT_FALSE(flags.O);
	EXPECT_FALSE(flags.P);
	EXPECT_FALSE(flags.A);
}

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelatieveAddressingModeBPDI8BitDisplacement)
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

TEST_F(M8086ADDEbGbTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelaticeAddressingModeBPDI16BitDisplacement)
{
	// MOV AX, 00780h
	// MOV SS, AX
	// MOV BP, 01A2h
	// MOV DI, 050h
	// MOV[BP + DI + 1224h], 015h	; 0x1A2 + 0x50 + 0x1224 = 0x1f2
	// MOV CL, 042h
	// ADD[BP + DI + 1224h], CL		; base indexed addressing mode: ADD (SS * 16) + BP + DI + 0x1224, CL
	// ADD 042h, 015h		-> 057h

	const uint32_t memoryAddress = (0x0780 << 4) + 0x1A2 + 0x50 + 0x1224;

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
	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BPPlusDI);
	mProcessor.mMemory[0xFFFF2] = 0x24;
	mProcessor.mMemory[0xFFFF3] = 0x12;

	const int instructionsToExecute = 1;
	mProcessor.executeInstructions(instructionsToExecute);

	EXPECT_EQ(mProcessor.SS, 0x0780);
	EXPECT_EQ(mProcessor.CS, 0xFFFF);
	EXPECT_EQ(mProcessor.IP, 0x0004);

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