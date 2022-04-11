#include "primitives.hpp"
#include "Processor.hpp"
#include "Registers.hpp"

namespace lp::emul8086 {
	bool Processor::CheckParity(DoubleWord number)
	{
		int parityCount = 0;
		for (int i = 0; i < 16; ++i) {
			parityCount += (number & (1 << i)) == 0 ? 0 : 1;
		}

		return (parityCount & 0x0001) == 0;
	}

	/*void Processor::EnforceByte(Byte reg) {
		switch (reg) {
		case Registers::AL:
		case Registers::BL:
		case Registers::CL:
		case Registers::DL:
		case Registers::AH:
		case Registers::BH:
		case Registers::CH:
		case Registers::DH:
			return;
		default:
			throw - 1;
		}
	}*/

	/*void Processor::EnforceWord(Byte reg) {
		switch (reg) {
		case Registers::AX:
		case Registers::BX:
		case Registers::CX:
		case Registers::DX:
		case Registers::SP:
		case Registers::BP:
		case Registers::SI:
		case Registers::DI:
			return;
		default:
			throw - 1;
		}
	}*/

	void Processor::reset() {
		CS = 0xFFFF;
		IP = 0x0000;

		Flags = 0x0000;

		AX = 0;
		BX = 0;
		CX = 0;
		DX = 0;
		SP = 0;
		BP = 0;
		SI = 0;
		DI = 0;
		DS = 0;
		ES = 0;
		SS = 0;
	}

	Register Processor::getRegister(Byte reg, bool word) const
	{
		return (Register)0;
	}

	Register Processor::getRegister(Registers reg, bool word) const
	{
		if (!word) {
			switch (reg) {
			case Registers::AL:
				return AL;
			case Registers::BL:
				return BL;
			case Registers::CL:
				return CL;
			case Registers::DL:
				return DL;
			case Registers::AH:
				return AH;
			case Registers::BH:
				return BH;
			case Registers::CH:
				return CH;
			case Registers::DH:
				return DH;
			default:
				throw - 1;
			}
		}
		else {
			switch (reg) {
			case Registers::AX:
				return AX;
			case Registers::BX:
				return BX;
			case Registers::CX:
				return CX;
			case Registers::DX:
				return DX;
			case Registers::SP:
				return SP;
			case Registers::BP:
				return BP;
			case Registers::SI:
				return SI;
			case Registers::DI:
				return DI;
			default:
				throw - 1;
			}
		}
	}

	void Processor::setRegister(Registers reg, Word value, bool word) {
		if (!word) {
			switch (reg) {
			case Registers::AL:
				AL = (Byte)value;
				break;
			case Registers::BL:
				BL = (Byte)value;
				break;
			case Registers::CL:
				CL = (Byte)value;
				break;
			case Registers::DL:
				DL = (Byte)value;
				break;
			case Registers::AH:
				AH = (Byte)value;
				break;
			case Registers::BH:
				BH = (Byte)value;
				break;
			case Registers::CH:
				CH = (Byte)value;
				break;
			case Registers::DH:
				DH = (Byte)value;
				break;
			default:
				throw - 1;
			}
		}
		else {
			switch (reg) {
			case Registers::AX:
				AX = value;
				break;
			case Registers::BX:
				BX = value;
				break;
			case Registers::CX:
				CX = value;
				break;
			case Registers::DX:
				DX = value;
				break;
			case Registers::SP:
				SP = value;
				break;
			case Registers::BP:
				BP = value;
				break;
			case Registers::SI:
				SI = value;
				break;
			case Registers::DI:
				DI = value;
				break;
			default:
				throw - 1;
			}
		}
	}

	void Processor::setRegister(Rm reg, Word value, bool word)
	{
		setRegister(static_cast<Byte>(reg), value, word);
	}

	void Processor::setRegister(Byte reg, Word value, bool word)
	{

	}

	Address Processor::getMemoryAddress(const Rm addressingMode, const Word displacement) const {
		switch (addressingMode) {
		case Rm::BXPlusSI:
			return (DS << 4) + BX + SI + displacement;
		case Rm::BXPlusDI:
			return (DS << 4) + BX + DI + displacement;
		case Rm::BPPlusSI:
			return (SS << 4) + BP + SI + displacement;
		case Rm::BPPlusDI:
			return (SS << 4) + BP + DI + displacement;
		case Rm::RM_SI:
			return (DS << 4) + SI + displacement;
		case Rm::RM_DI:
			return (DS << 4) + DI + displacement;
		case Rm::RM_BP:
			return (SS << 4) + BP + displacement;
		case Rm::RM_BX:
			return (DS << 4) + BX + displacement;
		default:
			throw - 1;
		}
	}

	void Processor::executeInstructions(int instructionCount) {
		while (instructionCount > 0 || instructionCount == -1) {
			const Byte instruction = mMemory.getByte(CS, IP);

			const bool isWordInstruction = (instruction & 0b00000001) > 0;

			switch (instruction) {
			case OpCodes::ADD_Eb_Gb:
				ADDEbGb(isWordInstruction);
				break;
			default:
				throw - 1;
			}
			--instructionCount;
		}
	}

	void Processor::ADDEbGb(bool isWordInstruction) {
		ModRmByte modRmByte = mMemory.getModRmByte(CS, IP);

		Mod mod = static_cast<Mod>(modRmByte.Mod);
		Rm rm = static_cast<Rm>(modRmByte.Rm);
		Byte reg = modRmByte.Reg;
		
		Register source = getRegister(reg);

		switch (static_cast<Mod>(modRmByte.Mod)) {
		case Mod::RegisterValue: {
			Register destination = getRegister(rm);
			uint32_t result = destination + source;
			setRegister(rm, result, isWordInstruction);

			ADDChangeFlags(result, destination, source, isWordInstruction);
			break;
		}
		case Mod::NoDisplacement: {
			if (rm == Rm::RM_BP) {
				// 16bits displacement
				Word displacement = mMemory.getWord(CS, IP);
				Address memoryAddress = (DS << 4) + displacement;
				const Byte memoryContent = mMemory[memoryAddress];
				uint32_t result = memoryContent + source;

				mMemory[memoryAddress] = result;
				ADDChangeFlags(result, memoryContent, source, isWordInstruction);
			}
			else {
				// no displacement
				Address memoryAddress = getMemoryAddress(rm);
				const Byte memoryContent = mMemory[memoryAddress];
				uint32_t result = memoryContent + source;

				mMemory[memoryAddress] = result;
				ADDChangeFlags(result, memoryContent, source, isWordInstruction);
			}
			break;
		}
		case Mod::ByteDisplacement: {
			Byte displacement = mMemory.getByte(CS, IP);
			Address memoryAddress = getMemoryAddress(rm, displacement);
			const Byte memoryContent = mMemory[memoryAddress];
			uint32_t result = memoryContent + source;

			mMemory[memoryAddress] = result;
			ADDChangeFlags(result, memoryContent, source, isWordInstruction);
			break;
		}
		case Mod::WordDisplacement: {
			Word displacement = mMemory.getWord(CS, IP);
			Address memoryAddress = getMemoryAddress(rm, displacement);
			const Byte memoryContent = mMemory[memoryAddress];
			uint32_t result = memoryContent + source;

			mMemory[memoryAddress] = result;
			ADDChangeFlags(result, memoryContent, source, isWordInstruction);
			break;
		}
		default:
			throw - 1;
		}
	}

	void Processor::ADDChangeFlags(uint32_t result, Word destination, Word source, bool isWordInstruction) {
		Word negativeBit = isWordInstruction ? 0x8000 : 0x80;
		Word operandSizeMask = isWordInstruction ? 0xFFFF : 0xFF;
		Word actualResult = result & operandSizeMask;
		bool sameSign = !((destination ^ source) & negativeBit);

		Flags.C = result > (uint32_t)(operandSizeMask);
		Flags.Z = result == 0;
		Flags.S = (result & negativeBit) > 0;
		Flags.P = CheckParity(actualResult);
		Flags.A = (destination & 0x0F) + (source & 0x0F) > 0x0F;
		Flags.O = sameSign && ((actualResult ^ source) & negativeBit);
	}
}