#pragma once

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

namespace lp
{
	namespace m8086
	{
		using Byte = unsigned char;
		using Word = unsigned short;
		using Register = Word;

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
			auto operator=(const Register& rhs)
			{
				memcpy(this, &rhs, sizeof(Register));
				return *this;
			}

			bool operator==(const Register& rhs) const
			{
				return memcmp(this, &rhs, sizeof(Register)) == 0;
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
		};
	}
}