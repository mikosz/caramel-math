#include <gtest/gtest.h>

#include "caramel-math/matrix/ViewStorage.hpp"

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AssertErrorHandler.hpp"

using namespace caramel_math::matrix;

TEST(ViewMatrixText, TransposedViewStorageReturnsTransposedData) {
	using ViewedMatrix = Matrix<float, 2, 2, ArrayStorage<float, 2, 2, AssertErrorHandler>>;
	auto viewedMatrix = ViewedMatrix();
	viewedMatrix.get(0, 0) = 0;
	viewedMatrix.get(0, 1) = 1;
	viewedMatrix.get(1, 0) = 2;
	viewedMatrix.get(1, 1) = 3;

	// TODO: first three Matrix parameters could be deduced from storage
	auto transposedView = Matrix<float, 2, 2, TransposedViewStorage<ViewedMatrix>>(viewedMatrix);

	EXPECT_EQ(transposedView.get(0, 0), 0);
	EXPECT_EQ(transposedView.get(1, 0), 1);
	EXPECT_EQ(transposedView.get(0, 1), 2);
	EXPECT_EQ(transposedView.get(1, 1), 3);
}
