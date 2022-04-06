#include <tuple>

#include <gtest/gtest.h>
#include "Processor.hpp"

using namespace lp::emul8086;
using ::testing::TestWithParam;
using ::testing::ValuesIn;

struct ParityCheck
{
	ParityCheck(const DoubleWord v, bool p) : value(v), isPar(p) {}

	DoubleWord value;
	bool isPar;		// when true, parity is even
};

class CheckParityTestWithParam : public TestWithParam<std::tuple<Word, bool>>
{
};

TEST(TestSometing, TestName)
{
	EXPECT_EQ(1, 2 - 1);
}

TEST_P(CheckParityTestWithParam, CanTest8BitValues)
{
	const auto& [value, isPar] = GetParam();
	EXPECT_EQ(Processor::CheckParity(value), isPar);
}

const std::vector<std::tuple<Word, bool>> values8bits = {
	{0b10001110, true},
	{0b11000001, false},
	{0b11100000, false},
	{0b01110100, true},
	{0b01000000, false},
	{0b00011110, true},
	{0b01110001, true},
	{0b11001011, false},
	{0b11011101, true},
	{0b00010101, false},
	{0b01110001, true},
	{0b01000010, true},
	{0b00111111, true},
	{0b01001111, false},
	{0b11010001, true},
	{0b10000000, false},
	{0b11111010, true},
	{0b11011100, false},
	{0b11011100, false},
	{0b00001001, true},
	{0b10101010, true},
	{0b11100001, true},
	{0b00110111, false},
	{0b10010011, true},
	{0b10110100, true},
	{0b11101000, true},
	{0b01100110, true},
	{0b00111101, false},
	{0b00101001, false},
	{0b00111000, false},
	{0b11000100, false},
	{0b11101111, false},
	{0b11111000, false},
	{0b10101100, true},
	{0b10101011, false},
	{0b00010011, false},
	{0b11001110, false},
	{0b00100100, true},
	{0b01110010, true},
	{0b11110011, true},
	{0b00011101, true},
	{0b01001111, false},
	{0b00000110, true},
	{0b11110011, true},
	{0b01111110, true},
	{0b01100111, false},
	{0b11101110, true},
	{0b01111101, true},
	{0b10000010, true},
	{0b10101011, false},
	{0b11101001, false},
	{0b01000110, false}
};

const std::vector<std::tuple<Word, bool>> values16bits = {
			{ 0b1000010100111011, true },
			{ 0b1000111001001101, true },
			{ 0b1010000110001011, false },
			{ 0b1011000000001001, false },
			{ 0b1011100000100010, true },
			{ 0b0010100110110010, false },
			{ 0b0010100100001101, true },
			{ 0b1111110010110000, false },
			{ 0b1101000000011111, true },
			{ 0b0100000111011100, false },
			{ 0b0100010010101010, true },
			{ 0b1001001010010110, false },
			{ 0b1111000101100010, true },
			{ 0b1110001111010101, true },
			{ 0b1001101010011111, true },
			{ 0b1101001110101110, true },
			{ 0b0111110101111101, true },
			{ 0b0110000010001000, true },
			{ 0b1110010001010001, false },
			{ 0b0000010100001010, true },
			{ 0b0101111110110111, true },
			{ 0b0111110000010001, false },
			{ 0b1111001000110001, true },
			{ 0b1001011000010100, true },
			{ 0b1001100101011110, false },
			{ 0b0101101010000011, false },
			{ 0b1000110010010101, false },
			{ 0b1110011110100010, false },
			{ 0b0101001100100000, false },
			{ 0b1101000101100011, true },
			{ 0b0111100010100100, false },
			{ 0b0100001110100111, true },
			{ 0b1010011010100000, true },
			{ 0b1101000000100100, false },
			{ 0b1100100000110110, false },
			{ 0b0110100011101100, true },
			{ 0b0111110111001110, false },
			{ 0b0011100000010100, false },
			{ 0b0111010111100001, false },
			{ 0b1000100110100110, false },
			{ 0b0011001100000100, false },
			{ 0b1001101000100100, true },
			{ 0b0111001110010000, false },
			{ 0b0011101100010010, false },
			{ 0b0011011000100111, true },
			{ 0b1011000110001010, false },
			{ 0b1111100111110101, true },
			{ 0b1101010100100001, false },
			{ 0b1001011010011001, true },
			{ 0b1000010011010001, true },
};

INSTANTIATE_TEST_SUITE_P(BulkTest8Bits, CheckParityTestWithParam, ValuesIn(values8bits));
INSTANTIATE_TEST_SUITE_P(BulkTest16Bits, CheckParityTestWithParam, ValuesIn(values16bits));
