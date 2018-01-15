#include <gtest/gtest.h>

#include "caramel-math/matrix/ViewStorage.hpp"

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AssertErrorHandler.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

TEST(ViewMatrixTest, TransposedViewHasRowsSwappedWithColumns) {
	using ViewedMatrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>>;
	using TransposedView = decltype(viewTransposed(std::declval<ViewedMatrix&>()));
	static_assert(TransposedView::COLUMNS == 1);
	static_assert(TransposedView::ROWS == 2);
}

TEST(ViewMatrixText, TransposedViewStorageGetReturnsTransposedData) {
	auto viewedMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, AssertErrorHandler>>();
	viewedMatrix.set(0, 0, 0);
	viewedMatrix.set(0, 1, 1);
	viewedMatrix.set(1, 0, 2);
	viewedMatrix.set(1, 1, 3);

	auto transposedView = viewTransposed(viewedMatrix);

	EXPECT_EQ(transposedView.get(0, 0), 0);
	EXPECT_EQ(transposedView.get(1, 0), 1);
	EXPECT_EQ(transposedView.get(0, 1), 2);
	EXPECT_EQ(transposedView.get(1, 1), 3);
}

TEST(ViewMatrixTest, ViewStorageGetIsNoexceptIfMatrixGetIsNoexcept) {
	auto viewedMatrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 2, 2, AssertErrorHandler>>();
	static_assert(noexcept(viewedMatrix.get(0, 0)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(noexcept(transposedView.get(0, 0)));
}

TEST(ViewMatrixTest, ViewStorageGetIsPotentiallyThrowingIfMatrixGetIsPotentiallyThrowing) {
	auto viewedMatrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 2, 2, ThrowingErrorHandler>>();
	static_assert(!noexcept(viewedMatrix.get(0, 0)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(!noexcept(transposedView.get(0, 0)));
}

TEST(ViewMatrixText, TransposedViewStorageSetUpdatesTransposedData) {
	auto viewedMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, AssertErrorHandler>>();
	viewedMatrix.set(0, 0, 0);
	viewedMatrix.set(0, 1, 1);
	viewedMatrix.set(1, 0, 2);
	viewedMatrix.set(1, 1, 3);

	auto transposedView = viewTransposed(viewedMatrix);

	transposedView.set(1, 0, 5);

	EXPECT_EQ(transposedView.get(1, 0), 5);
	EXPECT_EQ(viewedMatrix.get(0, 1), 5);
}

TEST(ViewMatrixTest, ViewStorageSetIsNoexceptIfMatrixSetIsNoexcept) {
	auto viewedMatrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 2, 2, AssertErrorHandler>>();
	static_assert(noexcept(viewedMatrix.set(0, 0, 0.0f)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(noexcept(transposedView.set(0, 0, 0.0f)));
}

TEST(ViewMatrixTest, ViewStorageSetIsPotentiallyThrowingIfMatrixSetIsPotentiallyThrowing) {
	auto viewedMatrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 2, 2, ThrowingErrorHandler>>();
	static_assert(!noexcept(viewedMatrix.set(0, 0, 0.0f)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(!noexcept(transposedView.set(0, 0, 0.0f)));
}
