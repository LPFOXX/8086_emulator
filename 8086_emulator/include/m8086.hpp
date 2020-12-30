#pragma once

#include <array>

#define REGISTER(name)					\
union									\
{										\
	struct								\
	{									\
		Register name##L : 8;			\
		Register name##H : 8;			\
	};									\
	Register name##X;					\
}										\

#define GET_MEM_ADDRESS(CS, IP) (((CS) << 4) + (IP))

namespace lp
{
	namespace m8086
	{
		using Byte = uint8_t;
		using Word = uint16_t;
		using Register = Word;

		enum Registers : Byte
		{
			// 8bits
			// Lo
			AL = 0b000,
			BL = 0b011,
			CL = 0b001,
			DL = 0b010,
			// Hi
			AH = 0b100,
			BH = 0b111,
			CH = 0b101,
			DH = 0b110,

			// 16bits
			AX = 0b000,
			BX = 0b011,
			CX = 0b001,
			DX = 0b010,
			SP = 0b100,
			BP = 0b101,
			SI = 0b110,
			DI = 0b111
		};

		enum Mod : Byte
		{
			NoDisplacement = 0b00,
			ByteDisplacement = 0b01,
			WordDisplacement = 0b10,
			RegisterValue = 0b11,
		};

		enum RM : Byte
		{
			BXPlusSI = 0b000,
			BXPlusDI = 0b001,
			BPPlusSI = 0b010,
			BPPlusDI = 0b011,
			RM_SI = 0b100,
			RM_DI = 0b101,
			RM_BP = 0b110,
			RM_BX = 0b111,
		};

		union ModRMByte
		{
			ModRMByte(Byte b) :
				Mod(b >> 6),
				Reg((b >> 3) & 0b00000111),
				RM(b & 0b00000111)
			{
			}

			ModRMByte(Byte mod, Byte reg, Byte rm) :
				Mod(mod),
				Reg(reg),
				RM(rm)
			{
			}

			operator Byte () const
			{
				Byte b = Mod; b <<= 3;
				b |= Reg; b <<= 3;
				b |= RM;
				return b;
			}

			struct
			{
				Byte Mod : 2;	// register mode: 2bits
				Byte Reg : 3;	// register		: 3 bits
				Byte RM : 3;	// register/memory operand : 3 bits
			};
		};

		struct Memory
		{
			Byte& operator[](uint32_t index)
			{
				return mMemory[index];
			}

			// TODO: What to do when passing segment threshold
			const Byte getByte(Register& CS, Register& IP) const
			{
				Byte byte = mMemory[GET_MEM_ADDRESS(CS, IP)];
				++IP;
				return byte;
			}

			const Byte getByte(const uint32_t memAddress) const
			{
				return mMemory[memAddress];
			}

			const Word getWord(Register& CS, Register& IP) const
			{
				Byte lo = mMemory[GET_MEM_ADDRESS(CS, IP)]; ++IP;
				Byte hi = mMemory[GET_MEM_ADDRESS(CS, IP)]; ++IP;

				return (hi << 8) | lo;
			}

			void setByte(uint32_t memAddress, Byte byte)
			{
				mMemory[memAddress] = byte;
			}

			const ModRMByte getModRMByte(Register& CS, Register& IP) const
			{
				ModRMByte byte = mMemory[(CS << 4) | IP]; ++IP;
				return byte;
			}

			std::array<Byte, 1024 * 1024> mMemory = { 0 };
		};

		enum class FlagMasks : Word
		{
			CarryFlagMask = 1 << 0,
			ParityFlagMask = 1 << 2,
			AuxiliarFlagMask = 1 << 4,
			ZeroFlagMask = 1 << 6,
			SignFlagMask = 1 << 7,
			TrapFlagMask = 1 << 8,
			InteruptFlagMask = 1 << 9,
			DirectionFlagMask = 1 << 10,
			OverflowFlagMask = 1 << 11,
		};

		struct Flags
		{
			Flags()
			{
				memset(this, Word(0), sizeof(Register));
			}

			auto operator=(const Register& rhs)
			{
				memcpy(this, &rhs, sizeof(Register));
				return *this;
			}

			bool operator==(const Register& rhs) const
			{
				Register flags = *this;
				return flags == rhs;
			}

			bool operator==(const FlagMasks& rhs) const
			{
				return operator==((Register)rhs);
			}

			operator Word() const
			{
				Word word = 0;
				memcpy(&word, this, 2);
				return word;
			}

			struct
			{
				Register C : 1;	// bit 0: Carry flag
				Register : 1;	// bit 1: Unused flag
				Register P : 1;	// bit 2: Parity flag
				Register : 1;	// bit 3: Unused flag
				Register A : 1; // bit 4: Auxiliar Carry flag
				Register : 1;	// bit 5: Unused flag
				Register Z : 1;	// bit 6: Zero flag
				Register S : 1;	// bit 7: Sign flag
				Register T : 1;	// bit 8: Trap flag
				Register I : 1;	// bit 9: Interup flag
				Register D : 1;	// bit 10: Direction flag
				Register O : 1;	// bit 11: Overflow flag
				Register : 1;	// bit 12: Unused flag
				Register : 1;	// bit 13: Unused flag
				Register : 1;	// bit 14: Unused flag
				Register : 1;	// bit 15: Unused flag
			};
		};

		struct Processor
		{
			void reset()
			{
				CS = 0xFFFF;
				IP = 0x0000;

				Flags = 0;

				AX = 0; BX = 0; CX = 0; DX = 0;
				SP = 0; BP = 0; SI = 0; DI = 0;
				DS = 0; ES = 0; SS = 0;
			}

			static bool CheckParity(uint32_t number)
			{
				uint32_t shiftAmount = 1;
				uint32_t value = number;

				while (number > (uint32_t)(1 << shiftAmount)) {
					value ^= value >> shiftAmount;
					shiftAmount <<= 1;
				}
				return ((value) & 1) == 0;
			}

			Register getRegister(Byte reg, bool word = false) const
			{
				if (!word) {
					switch (reg) {
						case Registers::AL: return AL;
						case Registers::BL: return BL;
						case Registers::CL: return CL;
						case Registers::DL: return DL;
						case Registers::AH: return AH;
						case Registers::BH: return BH;
						case Registers::CH: return CH;
						case Registers::DH: return DH;
						default: throw - 1;
					}
				}
				else {
					switch (reg) {
						case Registers::AX: return AX;
						case Registers::BX: return BX;
						case Registers::CX: return CX;
						case Registers::DX: return DX;
						case Registers::SP: return SP;
						case Registers::BP: return BP;
						case Registers::SI: return SI;
						case Registers::DI: return DI;
						default: throw - 1;
					}
				}
			}

			void enforceByte(Byte reg)
			{
				switch (reg) {
					case Registers::AL:
					case Registers::BL:
					case Registers::CL:
					case Registers::DL:
					case Registers::AH:
					case Registers::BH:
					case Registers::CH:
					case Registers::DH: return;
					default: throw - 1;
				}
			}

			void enforceWord(Byte reg)
			{
				switch (reg) {
					case Registers::AX:
					case Registers::BX:
					case Registers::CX:
					case Registers::DX:
					case Registers::SP:
					case Registers::BP:
					case Registers::SI:
					case Registers::DI: return;
					default: throw - 1;
				}
			}

			void setRegister(Byte reg, Word value, bool word = false)
			{
				if (!word) {
					switch (reg) {
						case Registers::AL: AL = (Byte)value; break;
						case Registers::BL: BL = (Byte)value; break;
						case Registers::CL: CL = (Byte)value; break;
						case Registers::DL: DL = (Byte)value; break;
						case Registers::AH: AH = (Byte)value; break;
						case Registers::BH: BH = (Byte)value; break;
						case Registers::CH: CH = (Byte)value; break;
						case Registers::DH: DH = (Byte)value; break;
						default: throw - 1;
					}
				}
				else {
					switch (reg) {
						case Registers::AX: AX = value; break;
						case Registers::BX: BX = value; break;
						case Registers::CX: CX = value; break;
						case Registers::DX: DX = value; break;
						case Registers::SP: SP = value; break;
						case Registers::BP: BP = value; break;
						case Registers::SI: SI = value; break;
						case Registers::DI: DI = value; break;
						default: throw - 1;
					}
				}
			}

			uint32_t getMemoryAddress(const RM addressingMode, const Word displacement = 0x0000)
			{
				switch (addressingMode) {
					case BXPlusSI:	return (DS << 4) + BX + SI + displacement;
					case BXPlusDI:	return (DS << 4) + BX + DI + displacement;
					case BPPlusSI:	return (SS << 4) + BP + SI + displacement;
					case BPPlusDI:	return (SS << 4) + BP + DI + displacement;
					case RM_SI:		return (DS << 4) + SI + displacement;
					case RM_DI:		return (DS << 4) + DI + displacement;
					case RM_BP:		return (SS << 4) + BP + displacement;
					case RM_BX:		return (DS << 4) + BX + displacement;
					default: throw - 1;
				}
			}

			void executeInstructions(int instructionCount = -1)
			{
				while (instructionCount > 0 || instructionCount == -1) {
					const Byte instruction = mMemory.getByte(CS, IP);

					const bool isWordInstruction = (instruction & 0b00000001) > 0;

					switch (instruction) {
						case OpCodes::ADD_Eb_Gb: ADDEbGb(isWordInstruction); break;
						default: throw - 1;
					}
					--instructionCount;
				}
			}

			enum OpCodes : Byte
			{
				ADD_Eb_Gb = 0x00,
				ADD_AL_Ib = 0x04,
				MOV_AL_Ob = 0xA0,
			};

			// Execution Unit (EU) registers
			// data registers or general purpose registers
			REGISTER(A);
			REGISTER(B);
			REGISTER(C);
			REGISTER(D);
			// pointer and stack registers:
			Register SP;	// Stack Pointer
			Register BP;	// Base Pointer
			Register SI;	// Segment Index
			Register DI;	// Data Index

			// Bus Interface Unit (BIU) registers
			// segmente registers:
			Register CS;	// Code Segment
			Register DS;	// Data Segment
			Register SS;	// Stack Segment
			Register ES;	// Extra Segment
			Register IP;	// Instruction Pointer

			// Flags
			Flags Flags;

			// Memory:
			Memory mMemory;

		private:
			void ADDEbGb(bool isWordInstruction)
			{
				const ModRMByte mrmb = mMemory.getModRMByte(CS, IP);
				Register source = getRegister((Registers)mrmb.Reg);

				switch (mrmb.Mod) {
					case Mod::RegisterValue:
					{
						Register destination = getRegister((Registers)mrmb.RM);
						uint32_t result = destination + source;
						setRegister(mrmb.RM, (Word)result, isWordInstruction);

						ADDChangeFlags(result, destination, source, isWordInstruction);
						break;
					}
					case Mod::NoDisplacement:
					{
						if (mrmb.RM == RM::RM_BP) {
							// 16bits displacement
							Word displacement = mMemory.getWord(CS, IP);
							uint32_t memoryAddress = (DS << 4) + displacement;
							Byte memoryContent = mMemory.getByte(memoryAddress);
							uint32_t result = memoryContent + source;

							mMemory.setByte(memoryAddress, result);
							ADDChangeFlags(result, memoryContent, source, isWordInstruction);
						}
						else {
							// no displacement
							uint32_t memoryAddress = getMemoryAddress((RM)mrmb.RM, 0);
							Byte memoryContent = mMemory.getByte(memoryAddress);
							uint32_t result = memoryContent + source;

							mMemory.setByte(memoryAddress, result);
							ADDChangeFlags(result, memoryContent, source, isWordInstruction);
						}
						break;
					}
					case Mod::ByteDisplacement:
					{
						Byte displacement = mMemory.getByte(CS, IP);
						uint32_t memoryAddress = getMemoryAddress((RM)mrmb.RM, displacement);
						Byte memoryContent = mMemory.getByte(memoryAddress);
						uint32_t result = memoryContent + source;

						mMemory.setByte(memoryAddress, result);
						ADDChangeFlags(result, memoryContent, source, isWordInstruction);
						break;
					}
					case Mod::WordDisplacement:
					{
						Word displacement = mMemory.getWord(CS, IP);
						uint32_t memoryAddress = getMemoryAddress((RM)mrmb.RM, displacement);
						Byte memoryContent = mMemory.getByte(memoryAddress);
						uint32_t result = memoryContent + source;

						mMemory.setByte(memoryAddress, result);
						ADDChangeFlags(result, memoryContent, source, isWordInstruction);
						break;
					}
					default: throw - 1;
				}
			}

			void ADDChangeFlags(uint32_t result, Word destination, Word source, bool isWordInstruction)
			{
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
		};
	}
}
