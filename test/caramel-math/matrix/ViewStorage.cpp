#include <gtest/gtest.h>

#include "caramel-math/matrix/ViewStorage.hpp"

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AssertErrorHandler.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::matrix::literals;

TEST(ViewMatrixTest, TransposedViewHasRowsSwappedWithColumns) {
	using ViewedMatrix = Matrix<ArrayStorage<scalar::BasicScalarTraits<float>, 1, 2, AssertErrorHandler>>;
	using TransposedView = decltype(viewTransposed(std::declval<ViewedMatrix&>()));
	static_assert(TransposedView::COLUMNS == 1);
	static_assert(TransposedView::ROWS == 2);
}

TEST(ViewMatrixText, TransposedViewStorageGetReturnsTransposedData) {
	auto viewedMatrix = Matrix<ArrayStorage<scalar::BasicScalarTraits<int>, 2, 2, AssertErrorHandler>>();
	viewedMatrix.set(0_row, 0_col, 0);
	viewedMatrix.set(0_row, 1_col, 1);
	viewedMatrix.set(1_row, 0_col, 2);
	viewedMatrix.set(1_row, 1_col, 3);

	auto transposedView = viewTransposed(viewedMatrix);

	EXPECT_EQ(transposedView.get(0_row, 0_col), 0);
	EXPECT_EQ(transposedView.get(1_row, 0_col), 1);
	EXPECT_EQ(transposedView.get(0_row, 1_col), 2);
	EXPECT_EQ(transposedView.get(1_row, 1_col), 3);
}

TEST(ViewMatrixTest, ViewStorageGetIsNoexceptIfMatrixGetIsNoexcept) {
	auto viewedMatrix = Matrix<ArrayStorage<scalar::BasicScalarTraits<float>, 2, 2, AssertErrorHandler>>();
	static_assert(noexcept(viewedMatrix.get(0_row, 0_col)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(noexcept(transposedView.get(0_row, 0_col)));
}

TEST(ViewMatrixTest, ViewStorageGetIsPotentiallyThrowingIfMatrixGetIsPotentiallyThrowing) {
	auto viewedMatrix = Matrix<ArrayStorage<scalar::BasicScalarTraits<float>, 2, 2, ThrowingErrorHandler>>();
	static_assert(!noexcept(viewedMatrix.get(0_row, 0_col)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(!noexcept(transposedView.get(0_row, 0_col)));
}

TEST(ViewMatrixText, TransposedViewStorageSetUpdatesTransposedData) {
	auto viewedMatrix = Matrix<ArrayStorage<scalar::BasicScalarTraits<int>, 2, 2, AssertErrorHandler>>();
	viewedMatrix.set(0_row, 0_col, 0);
	viewedMatrix.set(0_row, 1_col, 1);
	viewedMatrix.set(1_row, 0_col, 2);
	viewedMatrix.set(1_row, 1_col, 3);

	auto transposedView = viewTransposed(viewedMatrix);

	transposedView.set(1_row, 0_col, 5);

	EXPECT_EQ(transposedView.get(1_row, 0_col), 5);
	EXPECT_EQ(viewedMatrix.get(0_row, 1_col), 5);
}

TEST(ViewMatrixTest, ViewStorageSetIsNoexceptIfMatrixSetIsNoexcept) {
	auto viewedMatrix = Matrix<ArrayStorage<scalar::BasicScalarTraits<float>, 2, 2, AssertErrorHandler>>();
	static_assert(noexcept(viewedMatrix.set(0_row, 0_col, 0.0f)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(noexcept(transposedView.set(0_row, 0_col, 0.0f)));
}

TEST(ViewMatrixTest, ViewStorageSetIsPotentiallyThrowingIfMatrixSetIsPotentiallyThrowing) {
	auto viewedMatrix = Matrix<ArrayStorage<scalar::BasicScalarTraits<float>, 2, 2, ThrowingErrorHandler>>();
	static_assert(!noexcept(viewedMatrix.set(0_row, 0_col, 0.0f)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(!noexcept(transposedView.set(0_row, 0_col, 0.0f)));
}
