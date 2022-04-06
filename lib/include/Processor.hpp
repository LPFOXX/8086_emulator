#pragma once

#include "primitives.hpp"
#include "ProcessorFlags.hpp"
#include "Memory.hpp"

namespace lp::emul8086 {
	class Processor {
	public:
		Processor() = default;
		virtual ~Processor() = default;

		enum OpCodes : Byte {
			ADD_Eb_Gb = 0x00,
			ADD_AL_Ib = 0x04,
			MOV_AL_Ob = 0xA0,
		};

		static bool CheckParity(DoubleWord number);
		static void EnforceByte(Byte reg);
		static void EnforceWord(Byte reg);

		void reset();
		
		Register getRegister(Byte reg, bool word = false) const;
		Register getRegister(Rm reg, bool word = false) const;
		
		void setRegister(Byte reg, Word value, bool word = false);
		void setRegister(Rm reg, Word value, bool word = false);

		Address getMemoryAddress(const Rm addressingMode, const Word displacement = 0x0000) const;

		void executeInstructions(int instructionCount = -1);

		// Execution Unit (EU) registers
		// data registers or general purpose registers
		REGISTER(A);
		REGISTER(B);
		REGISTER(C);
		REGISTER(D);
		// pointer and stack registers:
		Register SP{};    // Stack Pointer
		Register BP{};    // Base Pointer
		Register SI{};    // Segment Index
		Register DI{};    // Data Index

		// Bus Interface Unit (BIU) registers
		// Segment registers:
		Register CS{};    // Code Segment
		Register DS{};    // Data Segment
		Register SS{};    // Stack Segment
		Register ES{};    // Extra Segment
		Register IP{};    // Instruction Pointer

	private:
		// ProcessorFlags
		ProcessorFlags Flags;

		// Memory:
		Memory mMemory;

		void ADDEbGb(bool isWordInstruction);
		void ADDChangeFlags(uint32_t result, Word destination, Word source, bool isWordInstruction);
	};
}
