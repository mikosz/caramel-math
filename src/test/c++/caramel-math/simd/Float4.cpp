#include <gtest/gtest.h>

#include "caramel-math/simd/Float4.hpp"

using namespace caramel_math::simd;

namespace /* anonymous */ {

TEST(Float4Test, InitialisationAndReadAccessWorks) {
	auto f4 = Float4({ 0.0f, 1.0f, 2.0f, 3.0f });
	const auto xyzw = f4.xyzw();

	EXPECT_EQ(xyzw[0], 0.0f);
	EXPECT_EQ(xyzw[1], 1.0f);
	EXPECT_EQ(xyzw[2], 2.0f);
	EXPECT_EQ(xyzw[3], 3.0f);
}

} // anonymous namespace
