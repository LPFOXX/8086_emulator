#pragma once

#include <gtest/gtest.h>

#include <m8086.hpp>

class M8086Test : public ::testing::Test
{
protected:
	lp::m8086::Processor mProcessor;
};