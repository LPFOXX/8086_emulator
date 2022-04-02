#pragma once

#include <gtest/gtest.h>
#include "8086.hpp"

class M8086Test : public ::testing::Test
{
	void SetUp() override
	{
		mProcessor.reset();
	}
protected:
	lp::m8086::Processor mProcessor;
};