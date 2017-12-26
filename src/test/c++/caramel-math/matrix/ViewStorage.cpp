#include <gtest/gtest.h>

#include "caramel-math/matrix/ViewStorage.hpp"

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AssertErrorHandler.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"

using namespace caramel_math::matrix;

TEST(ViewMatrixTest, TransposedViewHasRowsSwappedWithColumns) {
	using ViewedMatrix = Matrix<ArrayStorage<float, 1, 2, AssertErrorHandler>>;
	using TransposedView = decltype(viewTransposed(std::declval<ViewedMatrix&>()));
	static_assert(TransposedView::COLUMNS == 1);
	static_assert(TransposedView::ROWS == 2);
}

TEST(ViewMatrixText, TransposedViewStorageReturnsTransposedData) {
	auto viewedMatrix = Matrix<ArrayStorage<float, 2, 2, AssertErrorHandler>>();
	viewedMatrix.get(0, 0) = 0;
	viewedMatrix.get(0, 1) = 1;
	viewedMatrix.get(1, 0) = 2;
	viewedMatrix.get(1, 1) = 3;

	auto transposedView = viewTransposed(viewedMatrix);

	EXPECT_EQ(transposedView.get(0, 0), 0);
	EXPECT_EQ(transposedView.get(1, 0), 1);
	EXPECT_EQ(transposedView.get(0, 1), 2);
	EXPECT_EQ(transposedView.get(1, 1), 3);
}

TEST(ViewMatrixTest, ViewStorageGetIsNoexceptIfMatrixGetIsNoexcept) {
	auto viewedMatrix = Matrix<ArrayStorage<float, 2, 2, AssertErrorHandler>>();
	static_assert(noexcept(viewedMatrix.get(0, 0)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(noexcept(transposedView.get(0, 0)));
}

TEST(ViewMatrixTest, ViewStorageGetIsPotentiallyThrowingIfMatrixGetIsPotentiallyThrowing) {
	auto viewedMatrix = Matrix<ArrayStorage<float, 2, 2, ThrowingErrorHandler>>();
	static_assert(!noexcept(viewedMatrix.get(0, 0)));

	auto transposedView = viewTransposed(viewedMatrix);
	static_assert(!noexcept(transposedView.get(0, 0)));
}

TEST(ViewMatrixTest, ViewTransposedReturnsConstMatrixWhenViewingConst) {
	using ViewedMatrix = Matrix<ArrayStorage<float, 2, 2, ThrowingErrorHandler>>();
	static_assert(std::is_const_v<decltype(viewTransposed(std::declval<ViewedMatrix>())));
}
