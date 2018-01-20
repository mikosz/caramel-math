#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/matrix-ops.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
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

	const auto t = transposed<Matrix>(matrix);

	EXPECT_EQ(t.get(0, 0), 0);
	EXPECT_EQ(t.get(0, 1), 2);
	EXPECT_EQ(t.get(1, 0), 1);
	EXPECT_EQ(t.get(1, 1), 3);
}

TEST_F(MatrixOpsTest, TransposedTransposedYieldsOriginalMatrix) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>;
	const auto matrix = Matrix(
		0, 1,
		2, 3
		);

	transposed<Matrix>(matrix);
	EXPECT_EQ(t.get(1, 1), 3);
}

} // anonymous namespace
