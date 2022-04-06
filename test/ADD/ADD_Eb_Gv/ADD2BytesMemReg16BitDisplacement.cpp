//#include <gtest/gtest.h>
//#include "8086.hpp"
//
//#include "Processor.hpp"
//
//using namespace lp::emul8086;
//
//class M8086ADDEbGb16BitDisplacementTest : public ::testing::Test
//{
//protected:
//	void SetUp() override
//	{
//		mProcessor.reset();
//		mProcessor.mMemory[0xFFFF0] = Processor::OpCodes::ADD_Eb_Gb;
//	}
//
//protected:
//	int mInstructionsToExecute = 1;
//	Processor mProcessor;
//};
//
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeSI16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV DS, AX
////	// MOV CL, 42h
////	// MOV SI, 01A2h
////	// MOV [SI + 1224h], 15h
////	// ADD [SI + 1224h], CL		; indexed relative addressing mode: ADD (DS * 16) + SI + 0x1224, CL
////	// ;ADD 15h, 42h			; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x1224;
////
////	mProcessor.DS = 0x0700;
////	mProcessor.SI = 0x01A2;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_SI);
////	mProcessor.mMemory[0xFFFF2] = 0x24;
////	mProcessor.mMemory[0xFFFF3] = 0x12;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.DS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.SI, 0x01A2);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
////
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithIndexedRelativeAddressingModeDI16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV DS, AX
////	// MOV CL, 42h
////	// MOV DI, 01A2h
////	// MOV [DI + 1224h], 15h
////	// ADD [DI + 1224h], CL		; indexed relative addressing mode: ADD (DS * 16) + DI + 0x1224, CL
////	// ;ADD 15h, 42h			; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x1224;
////
////	mProcessor.DS = 0x0700;
////	mProcessor.DI = 0x01A2;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_DI);
////	mProcessor.mMemory[0xFFFF2] = 0x24;
////	mProcessor.mMemory[0xFFFF3] = 0x12;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.DS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.DI, 0x01A2);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
////
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBX16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV DS, AX
////	// MOV CL, 42h
////	// MOV BX, 01A2h
////	// MOV[BX + 1525h], 15h	; 0x01A2 + 0x50 + 0x1525 = 0x1717
////	// ADD[BX + 1525h], CL	; based relative addressing mode: ADD (DS * 16) + BX + 0x1525, CL
////	// ;ADD 15h, 42h		; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x1525;
////
////	mProcessor.DS = 0x0700;
////	mProcessor.BX = 0x01A2;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_BX);
////	mProcessor.mMemory[0xFFFF2] = 0x25;
////	mProcessor.mMemory[0xFFFF3] = 0x15;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.DS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.BX, 0x01A2);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
////
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBasedRelativeAddressingModeBP16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV SS, AX
////	// MOV CL, 42h
////	// MOV BP, 01A2h
////	// MOV [BP + 1224h], 15h
////	// ADD [BP + 1224h], CL		; based relative addressing mode: ADD (SS * 16) + BP + 0x1224, CL
////	// ;ADD 15h, 42h			; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x1224;
////
////	mProcessor.SS = 0x0700;
////	mProcessor.BP = 0x01A2;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::RM_BP);
////	mProcessor.mMemory[0xFFFF2] = 0x24;
////	mProcessor.mMemory[0xFFFF3] = 0x12;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.SS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.BP, 0x01A2);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
////
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXSI16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV DS, AX
////	// MOV CL, 42h
////	// MOV BX, 01A2h
////	// MOV SI, 50h
////	// MOV[BX + SI + 1224h], 15h	; 0x01A2 + 0x50 + 0x1224 = 0x1416
////	// ADD[BX + SI + 1224h], CL		; base indexed relative addressing mode: ADD (DS * 16) + BX + SI + 0x1224, CL
////	// ;ADD 15h, 42h				; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x50 + 0x1224;
////
////	mProcessor.DS = 0x0700;
////	mProcessor.BX = 0x01A2;
////	mProcessor.SI = 0x50;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BXPlusSI);
////	mProcessor.mMemory[0xFFFF2] = 0x24;
////	mProcessor.mMemory[0xFFFF3] = 0x12;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.DS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.BX, 0x01A2);
////	EXPECT_EQ(mProcessor.SI, 0x50);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
////
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBXDI16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV DS, AX
////	// MOV CL, 42h
////	// MOV BX, 01A2h
////	// MOV DI, 50h
////	// MOV[BX + DI + 1224h], 15h	; 0x01A2 + 0x50 + 0x1224 = 0x1416
////	// ADD[BX + DI + 1224h], CL		; base indexed addressing mode: ADD (DS * 16) + BX + DI + 0x1224, CL
////	// ;ADD 15h, 42h				; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x50 + 0x1224;
////
////	mProcessor.DS = 0x0700;
////	mProcessor.BX = 0x01A2;
////	mProcessor.DI = 0x50;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BXPlusDI);
////	mProcessor.mMemory[0xFFFF2] = 0x24;
////	mProcessor.mMemory[0xFFFF3] = 0x12;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.DS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.BX, 0x01A2);
////	EXPECT_EQ(mProcessor.DI, 0x50);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
////
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelativeAddressingModeBPSI16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV SS, AX
////	// MOV CL, 42h
////	// MOV BP, 01A2h
////	// MOV SI, 50h
////	// MOV[BP + SI + 1224h], 015h	; 0x01A2 + 0x50 + 0x1224 = 0x1416
////	// ADD[BP + SI + 1224h], CL		; base indexed relative addressing mode: ADD (SS * 16) + BP + SI + 0x1224, CL
////	// ;ADD 15h, 42h				; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x1A2 + 0x50 + 0x1224;
////
////	mProcessor.SS = 0x0700;
////	mProcessor.BP = 0x01A2;
////	mProcessor.SI = 0x50;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BPPlusSI);
////	mProcessor.mMemory[0xFFFF2] = 0x24;
////	mProcessor.mMemory[0xFFFF3] = 0x12;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.SS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.BP, 0x01A2);
////	EXPECT_EQ(mProcessor.SI, 0x50);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
////
////TEST_F(M8086ADDEbGb16BitDisplacementTest, CanAdd8BitGPRToValuesFromMemoryAddressWithBaseIndexedRelaticeAddressingModeBPDI16BitDisplacement)
////{
////	// MOV AX, 0700h
////	// MOV SS, AX
////	// MOV CL, 42h
////	// MOV BP, 01A2h
////	// MOV DI, 50h
////	// MOV[BP + DI + 1224h], 15h	; 0x01A2 + 0x50 + 0x1224 = 0x1416
////	// ADD[BP + DI + 1224h], CL		; base indexed relative addressing mode: ADD (SS * 16) + BP + DI + 0x1224, CL
////	// ;ADD 15h, 42h				; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2 + 0x50 + 0x1224;
////
////	mProcessor.SS = 0x0700;
////	mProcessor.BP = 0x01A2;
////	mProcessor.DI = 0x50;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.CL = 0x42;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::WordDisplacement, Registers::CL, RM::BPPlusDI);
////	mProcessor.mMemory[0xFFFF2] = 0x24;
////	mProcessor.mMemory[0xFFFF3] = 0x12;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.SS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
////	EXPECT_EQ(mProcessor.BP, 0x01A2);
////	EXPECT_EQ(mProcessor.DI, 0x50);
////	EXPECT_EQ(mProcessor.mMemory[memoryAddress], 0x57);
////
////	const Flags& flags = mProcessor.Flags;
////	EXPECT_FALSE(flags.C);
////	EXPECT_FALSE(flags.Z);
////	EXPECT_FALSE(flags.S);
////	EXPECT_FALSE(flags.O);
////	EXPECT_FALSE(flags.P);
////	EXPECT_FALSE(flags.A);
////}
