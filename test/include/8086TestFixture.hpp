#pragma once

#include <gtest/gtest.h>
#include "8086.hpp"

#include "Processor.hpp"

class M8086Test : public ::testing::Test
{
	void SetUp() override
	{
		mProcessor.reset();
	}
protected:
	lp::emul8086::Processor mProcessor;
};