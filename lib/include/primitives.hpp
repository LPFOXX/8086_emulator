#pragma once

#include <cstdint>

namespace lp::emul8086 {
	using Byte = uint8_t;
	using Word = uint16_t;
    using DoubleWord = uint32_t;
	using Register = uint16_t;
	using Address = uint32_t;

#define REGISTER(name)                      \
    union                                   \
    {                                       \
        struct                              \
        {                                   \
            Register name##L : 8;           \
            Register name##H : 8;           \
        };                                  \
        Register name##X;                   \
    }
}

#define GET_MEM_ADDRESS(CS, IP) (((CS) << 4) + (IP))
