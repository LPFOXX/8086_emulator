///**
// * This file tests adding two bytes together.
// *	source is a GPR.
// *	destination is a memory address.
// *	result (destination + source) is stored in the memory address.
// */
//
// // ADD CX, [481]		-> direct addressing mode: ADD CX, (DS * 16) + 481
// // ADD [481], CX		-> direct addressing mode: ADD (DS * 16) + 481, CX
// // ADD AL, [SI]		-> register indirect addressing mode: ADD AL, (DS * 16) + SI
// // ADD [SI], AL		-> register indirect addressing mode: ADD (DS * 16) + SI, AL
// // ADD DX, [BX + 5]	-> based relative addressing mode: ADD DX, (DS * 16) + BX + 5
// // ADD [BX + 5], DX	-> based relative addressing mode: ADD (DS * 16) + BX + 5, DX
// // ADD DX, [BP + 5]	-> based relative addressing mode: ADD DX, (SS * 16) + BP + 5
// // ADD [BP + 5], DX	-> based relative addressing mode: ADD (DS * 16) + BP + 5, DX
// // ADD AL, [DI] + 12	-> indexed relative addressing mode: ADD AL, (DS * 16) + DI + 12
// // ADD [DI] + 12, AL	-> indexed relative addressing mode: ADD (DS * 16) + DI + 12, AL
// // ADD AX, [BX+SI+20]	-> based indexed addressing mode: ADD AX, (DS * 16) + SI + BX + 20
// // ADD [BX+SI+20],AX	-> based indexed addressing mode: ADD (DS * 16) + SI + BX + 20, AX
// // ADD AX, [BP+SI+20]	-> based indexed addressing mode: ADD AX, (SS * 16) + SI + BP + 20
// // ADD [BP+SI+20], AX	-> based indexed addressing mode: ADD (SS * 16) + SI + BP + 20, AX
//
//#include "8086TestFixture.hpp"
//
//#include "Processor.hpp"
//
//using namespace lp::emul8086;
//
//class M8086ADDEbGbDirectAddressingModeTest : public ::testing::Test
//{
//public:
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
////TEST_F(M8086ADDEbGbDirectAddressingModeTest, CanAdd8BitGPRToValuesFromMemoryAddressWithDirectAddressingMode)
////{
////	// MOV AX, 0700h
////	// MOV DS, AX
////	// MOV CL, 42h
////	// MOV [01A2h], 15h
////	// ADD [01A2h], CL		; direct addressing mode: ADD (DS * 16) + 0x01A2, CL
////	// ;ADD 15h, 42h		; 0x57
////
////	const uint32_t memoryAddress = (0x0700 << 4) + 0x01A2;
////
////	mProcessor.CL = 0x42;
////	mProcessor.DS = 0x0700;
////	mProcessor.mMemory[memoryAddress] = 0x15;
////	mProcessor.Flags.C = true;
////	mProcessor.Flags.Z = true;
////	mProcessor.Flags.S = true;
////	mProcessor.Flags.O = true;
////	mProcessor.Flags.P = true;
////	mProcessor.Flags.A = true;
////
////	mProcessor.mMemory[0xFFFF1] = ModRMByte(Mod::NoDisplacement, Registers::CL, RM::RM_BP);
////	mProcessor.mMemory[0xFFFF2] = 0xA2;
////	mProcessor.mMemory[0xFFFF3] = 0x01;
////
////	mProcessor.executeInstructions(mInstructionsToExecute);
////
////	EXPECT_EQ(mProcessor.DS, 0x0700);
////	EXPECT_EQ(mProcessor.CS, 0xFFFF);
////	EXPECT_EQ(mProcessor.IP, 0x0004);
////
////	EXPECT_EQ(mProcessor.CL, 0x42);
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
