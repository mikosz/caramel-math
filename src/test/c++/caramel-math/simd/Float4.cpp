#include <gtest/gtest.h>

#include "caramel-math/simd/Float4.hpp"

using namespace caramel_math::simd;

namespace /* anonymous */ {

TEST(Float4Test, InitialisationAndReadAccessWorks) {
	auto f4 = Float4({ 0.0f, 1.0f, 2.0f, 3.0f });
	const auto xyzw = f4.xyzw();

	EXPECT_FLOAT_EQ(xyzw[0], 0.0f);
	EXPECT_FLOAT_EQ(xyzw[1], 1.0f);
	EXPECT_FLOAT_EQ(xyzw[2], 2.0f);
	EXPECT_FLOAT_EQ(xyzw[3], 3.0f);
}

TEST(Float4Test, Assignment) {
	auto f4 = Float4();
	auto zeroOneTwoThree = Float4({ 0.0f, 1.0f, 2.0f, 3.0f });
	f4 = zeroOneTwoThree;

	const auto xyzw = f4.xyzw();

	EXPECT_FLOAT_EQ(xyzw[0], 0.0f);
	EXPECT_FLOAT_EQ(xyzw[1], 1.0f);
	EXPECT_FLOAT_EQ(xyzw[2], 2.0f);
	EXPECT_FLOAT_EQ(xyzw[3], 3.0f);
}

TEST(Float4Test, Float4IsAdditive) {
	const auto lhs = Float4({ 0.0f, 1.0f, 2.0f, 3.0f });
	const auto rhs = Float4({ 1.0f, 2.0f, 3.0f, 4.0f });

	const auto sum = lhs + rhs;
	const auto sumXyzw = sum.xyzw();
	EXPECT_FLOAT_EQ(sumXyzw[0], 1.0f);
	EXPECT_FLOAT_EQ(sumXyzw[1], 3.0f);
	EXPECT_FLOAT_EQ(sumXyzw[2], 5.0f);
	EXPECT_FLOAT_EQ(sumXyzw[3], 7.0f);

	auto sumEq = lhs;
	sumEq += rhs;
	const auto sumEqXyzw = sumEq.xyzw();
	EXPECT_FLOAT_EQ(sumEqXyzw[0], 1.0f);
	EXPECT_FLOAT_EQ(sumEqXyzw[1], 3.0f);
	EXPECT_FLOAT_EQ(sumEqXyzw[2], 5.0f);
	EXPECT_FLOAT_EQ(sumEqXyzw[3], 7.0f);

	const auto diff = lhs - rhs;
	const auto diffXyzw = diff.xyzw();
	EXPECT_FLOAT_EQ(diffXyzw[0], -1.0f);
	EXPECT_FLOAT_EQ(diffXyzw[1], -1.0f);
	EXPECT_FLOAT_EQ(diffXyzw[2], -1.0f);
	EXPECT_FLOAT_EQ(diffXyzw[3], -1.0f);

	auto diffEq = lhs;
	diffEq -= rhs;
	const auto diffEqXyzw = diffEq.xyzw();
	EXPECT_FLOAT_EQ(diffEqXyzw[0], -1.0f);
	EXPECT_FLOAT_EQ(diffEqXyzw[1], -1.0f);
	EXPECT_FLOAT_EQ(diffEqXyzw[2], -1.0f);
	EXPECT_FLOAT_EQ(diffEqXyzw[3], -1.0f);
}

TEST(Float4Test, Float4IsMultiplicative) {
	const auto lhs = Float4({ 0.0f, 1.0f, 2.0f, 3.0f });
	const auto rhs = Float4({ 1.0f, 2.0f, 3.0f, 4.0f });

	const auto product = lhs * rhs;
	const auto productXyzw = product.xyzw();
	EXPECT_FLOAT_EQ(productXyzw[0], 0.0f);
	EXPECT_FLOAT_EQ(productXyzw[1], 2.0f);
	EXPECT_FLOAT_EQ(productXyzw[2], 6.0f);
	EXPECT_FLOAT_EQ(productXyzw[3], 12.0f);

	auto productEq = lhs;
	productEq *= rhs;
	const auto productEqXyzw = productEq.xyzw();
	EXPECT_FLOAT_EQ(productEqXyzw[0], 0.0f);
	EXPECT_FLOAT_EQ(productEqXyzw[1], 2.0f);
	EXPECT_FLOAT_EQ(productEqXyzw[2], 6.0f);
	EXPECT_FLOAT_EQ(productEqXyzw[3], 12.0f);

	const auto quotient = lhs / rhs;
	const auto quotientXyzw = quotient.xyzw();
	EXPECT_FLOAT_EQ(quotientXyzw[0], 0.0f);
	EXPECT_FLOAT_EQ(quotientXyzw[1], 0.5f);
	EXPECT_FLOAT_EQ(quotientXyzw[2], 2.0f / 3.0f);
	EXPECT_FLOAT_EQ(quotientXyzw[3], 3.0f / 4.0f);

	auto quotientEq = lhs;
	quotientEq /= rhs;
	const auto quotientEqXyzw = quotientEq.xyzw();
	EXPECT_FLOAT_EQ(quotientEqXyzw[0], 0.0f);
	EXPECT_FLOAT_EQ(quotientEqXyzw[1], 0.5f);
	EXPECT_FLOAT_EQ(quotientEqXyzw[2], 2.0f / 3.0f);
	EXPECT_FLOAT_EQ(quotientEqXyzw[3], 3.0f / 4.0f);
}

} // anonymous namespace
