#pragma once

#include <string>
#include "primitives.hpp"
#include "FlagMasks.hpp"

namespace lp::emul8086 {
	struct ProcessorFlags {
		ProcessorFlags() {
			memset(this, Word(0), sizeof(Register));
		}

		ProcessorFlags& operator=(const Register& rhs) {
			memcpy(this, &rhs, sizeof(Register));
			return *this;
		}

		bool operator==(const Register& rhs) const {
			Register flags = static_cast<Register>(*this);
			return flags == rhs;
		}

		bool operator==(const FlagMasks& rhs) const {
			return operator==((Register)rhs);
		}

		explicit operator Word() const {
			Word word = 0;
			memcpy(&word, this, 2);
			return word;
		}

		bool operator&(const FlagMasks mask) const {
			return (static_cast<Word>(*this) & static_cast<Word>(mask)) > 0;
		}

		struct {
			Register C : 1;  // bit 0: Carry flag
			Register : 1;   // bit 1: Unused flag
			Register P : 1;  // bit 2: Parity flag
			Register : 1;   // bit 3: Unused flag
			Register A : 1;  // bit 4: Auxiliary Carry flag
			Register : 1;   // bit 5: Unused flag
			Register Z : 1;  // bit 6: Zero flag
			Register S : 1;  // bit 7: Sign flag
			Register T : 1;  // bit 8: Trap flag
			Register I : 1;  // bit 9: Interrupt flag
			Register D : 1;  // bit 10: Direction flag
			Register O : 1;  // bit 11: Overflow flag
			Register : 1;   // bit 12: Unused flag
			Register : 1;   // bit 13: Unused flag
			Register : 1;   // bit 14: Unused flag
			Register : 1;   // bit 15: Unused flag
		};
	};
}
