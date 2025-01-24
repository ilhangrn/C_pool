// Copyright 2021 Embedded Artistry LLC

#include <cstdio>
#include <catch2/catch_test_macros.hpp>
#include "circular_buffer/circular_buffer.hpp"

TEST_CASE("Create circular buffer")
{
	circular_buffer<uint32_t, 10> cbuf;
	CHECK(cbuf.size() == 0);
	CHECK(cbuf.capacity() == 10);
	CHECK(cbuf.full() == false);
	CHECK(cbuf.empty() == true);
}

TEST_CASE("Circular buffer operations")
{
	circular_buffer<uint32_t, 10> cbuf;

	SECTION("Check empty")
	{
		CHECK(cbuf.empty() == true);

		for(uint32_t i = 0; i < cbuf.capacity(); i++)
		{
			cbuf.put(i);
			CHECK(cbuf.empty() == false);
		}
	}

	SECTION("Check Full")
	{
		for(uint32_t i = 0; i < cbuf.capacity(); i++)
		{
			CHECK(cbuf.full() == false);
			cbuf.put(i);
		}

		CHECK(cbuf.full() == true);
		CHECK(cbuf.size() == cbuf.capacity());
	}

	SECTION("Fill and empty buffer")
	{
		for(uint32_t i = 0; i < cbuf.capacity(); i++)
		{
			cbuf.put(i);
		}

		for(uint32_t i = 0; i < cbuf.capacity(); i++)
		{
			auto value = cbuf.get();
			CHECK(value.has_value() == true);
			CHECK(i == value.value());
		}
	}

	SECTION("Get empty value")
	{
		auto value = cbuf.get();
		CHECK(value.has_value() == false);
	}

	SECTION("Check overflow behavior")
	{
		for(uint32_t i = 0; i < cbuf.capacity(); i++)
		{
			cbuf.put(i);
		}

		// Force an overflow
		cbuf.put(cbuf.capacity());

		for(uint32_t i = 0; i < cbuf.capacity(); i++)
		{
			auto value = cbuf.get();
			CHECK(value.has_value() == true);
			CHECK(i + 1 == value.value());
		}
	}
}
