#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/matrix/SimdStorage.hpp"
#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

TEST(SimdStorageTest, IsDefaultConstructible) {
	auto storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>();
}

TEST(SimdStorageTest, IsConstructibleWithListOfValues) {
	using Storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>;
	auto storage = Storage(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f
		);

	EXPECT_FLOAT_EQ(storage.get(0, 0), 0.0f);
	EXPECT_FLOAT_EQ(storage.get(0, 1), 1.0f);
	EXPECT_FLOAT_EQ(storage.get(0, 2), 2.0f);
	EXPECT_FLOAT_EQ(storage.get(0, 3), 3.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 0), 4.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 1), 5.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 2), 6.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 3), 7.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 0), 8.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 1), 9.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 2), 10.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 3), 11.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 0), 12.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 1), 13.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 2), 14.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 3), 15.0f);
}

TEST(SimdStorageTest, MatrixMultiplicationWorks) {
	using Matrix = Matrix<SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>>;
	const auto lhs = Matrix(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f
		);
	const auto rhs = Matrix(
		0.0f, 2.0f, 4.0f, 6.0f,
		8.0f, 10.0f, 12.0f, 14.0f,
		16.0f, 18.0f, 20.0f, 22.0f,
		24.0f, 26.0f, 28.0f, 30.0f
		);

	const auto product = lhs * rhs;

	const auto expected = Matrix(
		112.0f, 124.0f, 136.0f, 148.0f,
		304.0f, 348.0f, 392.0f, 436.0f,
		496.0f, 572.0f, 648.0f, 724.0f,
		688.0f, 796.0f, 904.0f, 1012.0f
		);

	EXPECT_EQ(product, expected);
}

} // anonymous namespace