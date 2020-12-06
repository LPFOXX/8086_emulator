#include <iostream>
#include <iomanip>

#include <m8086.hpp>

int main()
{
	lp::m8086::Processor mProcessor = { };
	mProcessor.AX = 0x0102;

	std::cout << "AX: " << std::hex << std::uppercase << std::showbase << mProcessor.AX << "\n";
	std::cout << "AH: " << mProcessor.AH << "\n";
	std::cout << "AL: " << mProcessor.AL << "\n";
	/*std::cout <<  <<
		"\nAH: " << mProcessor.AH <<
		"\nAL: " << mProcessor.AL;*/
	return 0;
}