#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/matrix/SimdStorage.hpp"
#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"
#include "MatrixMockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::scalar;
using namespace caramel_math::matrix::literals;
using namespace caramel_math::matrix::test;

namespace /* anonymous */ {

class MatrixSimdStorageTest : public MockErrorHandlerFixtureTest {
};

TEST_F(MatrixSimdStorageTest, IsDefaultConstructible) {
	auto storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>();
}

TEST_F(MatrixSimdStorageTest, IsConstructibleWithListOfValues) {
	using Storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>;
	auto storage = Storage(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f
		);

	EXPECT_FLOAT_EQ(storage.get(0_row, 0_col), 0.0f);
	EXPECT_FLOAT_EQ(storage.get(0_row, 1_col), 1.0f);
	EXPECT_FLOAT_EQ(storage.get(0_row, 2_col), 2.0f);
	EXPECT_FLOAT_EQ(storage.get(0_row, 3_col), 3.0f);
	EXPECT_FLOAT_EQ(storage.get(1_row, 0_col), 4.0f);
	EXPECT_FLOAT_EQ(storage.get(1_row, 1_col), 5.0f);
	EXPECT_FLOAT_EQ(storage.get(1_row, 2_col), 6.0f);
	EXPECT_FLOAT_EQ(storage.get(1_row, 3_col), 7.0f);
	EXPECT_FLOAT_EQ(storage.get(2_row, 0_col), 8.0f);
	EXPECT_FLOAT_EQ(storage.get(2_row, 1_col), 9.0f);
	EXPECT_FLOAT_EQ(storage.get(2_row, 2_col), 10.0f);
	EXPECT_FLOAT_EQ(storage.get(2_row, 3_col), 11.0f);
	EXPECT_FLOAT_EQ(storage.get(3_row, 0_col), 12.0f);
	EXPECT_FLOAT_EQ(storage.get(3_row, 1_col), 13.0f);
	EXPECT_FLOAT_EQ(storage.get(3_row, 2_col), 14.0f);
	EXPECT_FLOAT_EQ(storage.get(3_row, 3_col), 15.0f);
}

TEST_F(MatrixSimdStorageTest, MatrixMultiplicationWorks) {
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

TEST_F(MatrixSimdStorageTest, ArrayStorageIsCopyable) {
	using Storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>;
	const auto storage = Storage(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f
		);
	const auto copy = storage;

	EXPECT_FLOAT_EQ(copy.get(0_row, 0_col), 0.0f);
	EXPECT_FLOAT_EQ(copy.get(0_row, 1_col), 1.0f);
	EXPECT_FLOAT_EQ(copy.get(0_row, 2_col), 2.0f);
	EXPECT_FLOAT_EQ(copy.get(0_row, 3_col), 3.0f);
	EXPECT_FLOAT_EQ(copy.get(1_row, 0_col), 4.0f);
	EXPECT_FLOAT_EQ(copy.get(1_row, 1_col), 5.0f);
	EXPECT_FLOAT_EQ(copy.get(1_row, 2_col), 6.0f);
	EXPECT_FLOAT_EQ(copy.get(1_row, 3_col), 7.0f);
	EXPECT_FLOAT_EQ(copy.get(2_row, 0_col), 8.0f);
	EXPECT_FLOAT_EQ(copy.get(2_row, 1_col), 9.0f);
	EXPECT_FLOAT_EQ(copy.get(2_row, 2_col), 10.0f);
	EXPECT_FLOAT_EQ(copy.get(2_row, 3_col), 11.0f);
	EXPECT_FLOAT_EQ(copy.get(3_row, 0_col), 12.0f);
	EXPECT_FLOAT_EQ(copy.get(3_row, 1_col), 13.0f);
	EXPECT_FLOAT_EQ(copy.get(3_row, 2_col), 14.0f);
	EXPECT_FLOAT_EQ(copy.get(3_row, 3_col), 15.0f);
}

TEST_F(MatrixSimdStorageTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>();
	storage.set(0_row, 0_col, 42.0f);
	storage.set(0_row, 1_col, 666.0f);

	EXPECT_FLOAT_EQ(storage.get(0_row, 0_col), 42.0f);
	EXPECT_FLOAT_EQ(storage.get(0_row, 1_col), 666.0f);
}

TEST_F(MatrixSimdStorageTest, GetAndSetReturnAndUpdateStoredColumn) {
	auto storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>();
	storage.set(1_col, simd::Float4({ 0.0f, 1.0f, 2.0f, 3.0f }));

	const auto column = storage.get(1_col);
	const auto columnXyzw = column.xyzw();
	EXPECT_FLOAT_EQ(columnXyzw[0], 0.0f);
	EXPECT_FLOAT_EQ(columnXyzw[1], 1.0f);
	EXPECT_FLOAT_EQ(columnXyzw[2], 2.0f);
	EXPECT_FLOAT_EQ(columnXyzw[3], 3.0f);
}

TEST_F(MatrixSimdStorageTest, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = SimdStorage<BasicScalarTraits<float>, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(4_row, 0_col)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(4_row, 0_col);
		EXPECT_EQ(errorValue, value);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0_row, 4_col)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(0_row, 4_col);
		EXPECT_EQ(errorValue, value);
	}

}

TEST_F(MatrixSimdStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = SimdStorage<BasicScalarTraits<float>, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0_row, 0_col)));
	static_assert(noexcept(storage.get(0_col)));
}

TEST_F(MatrixSimdStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = SimdStorage<BasicScalarTraits<float>, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0_row, 0_col)));
	static_assert(!noexcept(storage.get(0_col)));
}

TEST_F(MatrixSimdStorageTest, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = SimdStorage<BasicScalarTraits<float>, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(4_row, 0_col)).WillOnce(testing::Return(errorValue));
		storage.set(4_row, 0_col, 0.0f);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0_row, 4_col)).WillOnce(testing::Return(errorValue));
		storage.set(0_row, 4_col, 0.0f);
	}

}

TEST_F(MatrixSimdStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = SimdStorage<BasicScalarTraits<float>, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0_row, 0_col, 0.0f)));
	static_assert(noexcept(storage.set(0_col, simd::Float4())));
}

TEST_F(MatrixSimdStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = SimdStorage<BasicScalarTraits<float>, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0_row, 0_col, 0.0f)));
	static_assert(!noexcept(storage.set(0_col, simd::Float4())));
}

} // anonymous namespace
