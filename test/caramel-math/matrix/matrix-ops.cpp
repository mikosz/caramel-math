#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/matrix-ops.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AffineTransformStorage.hpp"
#include "caramel-math/matrix/SimdStorage.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

class MatrixOpsTest : public testing::Test {
};

TEST_F(MatrixOpsTest, TransposedYieldsTransposedMatrix) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>;
	const auto matrix = Matrix(
		0, 1,
		2, 3
		);

	const auto t = transposed(matrix);

	EXPECT_EQ(t.get(0, 0), 0);
	EXPECT_EQ(t.get(0, 1), 2);
	EXPECT_EQ(t.get(1, 0), 1);
	EXPECT_EQ(t.get(1, 1), 3);
}

TEST_F(MatrixOpsTest, TransposedYieldsEffectiveTypeForSquareMatrices) {
	using ArrayStorageMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 4, 4, ThrowingErrorHandler>>;
	static_assert(std::is_same_v<decltype(transposed(std::declval<ArrayStorageMatrix>())), ArrayStorageMatrix>);

	using AffineTransformMatrix = Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>;
	static_assert(std::is_same_v<decltype(transposed(std::declval<AffineTransformMatrix>())), ArrayStorageMatrix>);

	using SimdMatrix = Matrix<SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>>;
	static_assert(std::is_same_v<decltype(transposed(std::declval<SimdMatrix>())), SimdMatrix>);
}

TEST_F(MatrixOpsTest, TransposedYieldsOriginalMatrixForTransposedViewMatrix) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 4, 4, ThrowingErrorHandler>>;
	const auto m = Matrix();
	const auto tv = viewTransposed(m);
	const auto& ttv = transposed(tv);

	EXPECT_EQ(&ttv, &m);
}

TEST_F(MatrixOpsTest, DeterminantYieldsMatrixDeterminant) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 3, 3, ThrowingErrorHandler>>;
	const auto m = Matrix(
		-1, 1, 2,
		-2, 3, -3,
		4, -4, 5
		);

	EXPECT_EQ(determinant(m), -13);
}

TEST_F(MatrixOpsTest, InverseYieldsMatrixInverse) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 3, 3, ThrowingErrorHandler>>;
	const auto m = Matrix(
		-1.0f, 1.0f, 2.0f,
		-2.0f, 3.0f, -3.0f,
		4.0f, -4.0f, 5.0f
		);

	const auto i = inverse(m);

	ASSERT_TRUE(i);
	EXPECT_FLOAT_EQ(i->get(0, 0), -3.0f / 13.0f);
	// TODO: ...
}

TEST_F(MatrixOpsTest, InverseAffineTransformYieldsMatrixInverse) {
	using Matrix = Matrix<AffineTransformStorage<BasicScalarTraits<float>, ThrowingErrorHandler>>;
	auto m = Matrix::IDENTITY;

	m.set(0, 3, 1.0f);
	m.set(1, 3, 2.0f);
	m.set(2, 3, 3.0f);

	const auto i = inverse(m);

	ASSERT_TRUE(i);

	EXPECT_FLOAT_EQ(i->get(0, 0), 1.0f);
	EXPECT_FLOAT_EQ(i->get(0, 1), 0.0f);
	EXPECT_FLOAT_EQ(i->get(0, 2), 0.0f);
	EXPECT_FLOAT_EQ(i->get(0, 3), -1.0f);

	EXPECT_FLOAT_EQ(i->get(1, 0), 0.0f);
	EXPECT_FLOAT_EQ(i->get(1, 1), 1.0f);
	EXPECT_FLOAT_EQ(i->get(1, 2), 0.0f);
	EXPECT_FLOAT_EQ(i->get(1, 3), -2.0f);

	EXPECT_FLOAT_EQ(i->get(2, 0), 0.0f);
	EXPECT_FLOAT_EQ(i->get(2, 1), 0.0f);
	EXPECT_FLOAT_EQ(i->get(2, 2), 1.0f);
	EXPECT_FLOAT_EQ(i->get(2, 3), -3.0f);
}

} // anonymous namespace
