#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/matrix-ops.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AffineTransformStorage.hpp"
#include "caramel-math/matrix/AssertErrorHandler.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

template <size_t ROWS_VALUE, size_t COLUMNS_VALUE>
struct TestStorage {
	using ScalarTraits = BasicScalarTraits<float>;
	using Scalar = typename ScalarTraits::Scalar;
	static constexpr auto ROWS = ROWS_VALUE;
	static constexpr auto COLUMNS = COLUMNS_VALUE;
};

template <size_t ROWS_VALUE, size_t COLUMNS_VALUE>
struct NoexceptStorage : TestStorage<ROWS_VALUE, COLUMNS_VALUE> {

	float get(size_t, size_t) const noexcept {
		static auto f = 0.0f;
		return f;
	}

	void set(size_t, size_t, float) noexcept {
	}

};

template <size_t ROWS_VALUE, size_t COLUMNS_VALUE>
struct PotentiallyThrowingStorage : TestStorage<ROWS_VALUE, COLUMNS_VALUE> {

	float get(size_t, size_t) const {
		static auto f = 0.0f;
		return f;
	}

	void set(size_t, size_t, float) {
	}

};

struct MockStorage {

	static MockStorage* instance;

	MOCK_CONST_METHOD2(get, float (size_t, size_t));

	MOCK_METHOD3(set, void (size_t, size_t, float));

};

MockStorage* MockStorage::instance = nullptr;

struct MockStorageProxy {

	using Scalar = float;
	static constexpr auto ROWS = 1;
	static constexpr auto COLUMNS = 2;

	float get(size_t row, size_t column) const {
		return MockStorage::instance->get(row, column);
	}

	void set(size_t row, size_t column, float value) {
		return MockStorage::instance->set(row, column, value);
	}

};

class MatrixStorageFixture : public testing::Test {
public:

	MatrixStorageFixture()
	{
		MockStorage::instance = &storage_;
	}

	~MatrixStorageFixture() {
		MockStorage::instance = nullptr;
	}

private:

	MockStorage storage_;

};

class MatrixTest : public MatrixStorageFixture {
};

TEST_F(MatrixTest, MatricesAreCopyableAndCopyAssignable) {
	auto source = Matrix<ArrayStorage<BasicScalarTraits<int>, 3, 2, AssertErrorHandler>>(
		0, 1,
		2, 3,
		4, 5
		);
	auto target = source;

	EXPECT_EQ(source, target);

	source.set(0, 1, 42);

	EXPECT_NE(source, target);

	target = source;

	EXPECT_EQ(source, target);
}

TEST_F(MatrixTest, MatricesAreConvertibleToCompatibleMatrices) {
	auto source = Matrix<AffineTransformStorage<BasicScalarTraits<int>, AssertErrorHandler>>(
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 10
		);
	auto target = Matrix<ArrayStorage<BasicScalarTraits<int>, 4, 4, AssertErrorHandler>>(source);

	EXPECT_EQ(source, target);

	source.set(0, 1, 42);

	EXPECT_NE(source, target);

	target = source;

	EXPECT_EQ(source, target);
}

TEST_F(MatrixTest, ZeroMatrixHasZeroesEverywhere) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 3, 2, AssertErrorHandler>>;

	const auto zero = Matrix::ZERO;

	EXPECT_EQ(zero.get(0, 0), 0);
	EXPECT_EQ(zero.get(0, 1), 0);
	EXPECT_EQ(zero.get(1, 0), 0);
	EXPECT_EQ(zero.get(1, 1), 0);
	EXPECT_EQ(zero.get(2, 0), 0);
	EXPECT_EQ(zero.get(2, 1), 0);
}

TEST_F(MatrixTest, IdentityMatrixHasOnesOnDiagonal) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 3, 2, AssertErrorHandler>>;

	const auto zero = Matrix::IDENTITY;

	EXPECT_EQ(zero.get(0, 0), 1);
	EXPECT_EQ(zero.get(0, 1), 0);
	EXPECT_EQ(zero.get(1, 0), 0);
	EXPECT_EQ(zero.get(1, 1), 1);
	EXPECT_EQ(zero.get(2, 0), 0);
	EXPECT_EQ(zero.get(2, 1), 0);
}

TEST_F(MatrixTest, GetCallsStorageGet) {
	auto matrix = Matrix<MockStorageProxy>();

	{
		const auto f = 42.0f;
		EXPECT_CALL(*MockStorage::instance, get(0, 1)).WillOnce(testing::Return(f));
		const auto result = matrix.get(0, 1);
		EXPECT_EQ(f, result);
	}

	{
		const auto f = 42.0f;
		EXPECT_CALL(*MockStorage::instance, get(0, 1)).WillOnce(testing::Return(f));

		const auto& constMatrix = matrix;
		const auto result = constMatrix.get(0, 1);
		EXPECT_EQ(f, result);
	}
}

TEST_F(MatrixTest, SetCallsStorageSet) {
	auto matrix = Matrix<MockStorageProxy>();

	const auto f = 42.0f;
	EXPECT_CALL(*MockStorage::instance, set(0, 1, f)).Times(1);
	matrix.set(0, 1, f);
}

TEST_F(MatrixTest, MatrixConstantsDeriveFromStorage) {
	using MatrixType = Matrix<NoexceptStorage<12, 34>>;
	static_assert(std::is_same_v<MatrixType::Scalar, float>);
	static_assert(MatrixType::ROWS == 12);
	static_assert(MatrixType::COLUMNS == 34);
}

TEST_F(MatrixTest, GetIsNoexceptIfStorageGetIsNoexcept) {
	auto matrix = Matrix<NoexceptStorage<12, 34>>();
	static_assert(noexcept(matrix.get(0, 0)));

	const auto& constMatrix = matrix;
	static_assert(noexcept(constMatrix.get(0, 0)));
}

TEST_F(MatrixTest, GetIsPotentiallyThrowingIfStorageGetIsPotentiallyThrowing) {
	auto matrix = Matrix<PotentiallyThrowingStorage<12, 34>>();
	static_assert(!noexcept(matrix.get(0, 0)));

	const auto& constMatrix = matrix;
	static_assert(!noexcept(constMatrix.get(0, 0)));
}

TEST_F(MatrixTest, SetIsNoexceptIfStorageSetIsNoexcept) {
	auto matrix = Matrix<NoexceptStorage<12, 34>>();
	static_assert(noexcept(matrix.set(0, 0, 0.0f)));
}

TEST_F(MatrixTest, SetIsPotentiallyThrowingIfStorageSetIsPotentiallyThrowing) {
	auto matrix = Matrix<PotentiallyThrowingStorage<12, 34>>();
	static_assert(!noexcept(matrix.set(0, 0, 0.0f)));
}

TEST_F(MatrixTest, MatrixMultiplicationWorks) {
	auto matrix2x2 = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>();
	matrix2x2.set(0, 0, 0);
	matrix2x2.set(0, 1, 1);
	matrix2x2.set(1, 0, 10);
	matrix2x2.set(1, 1, 11);

	auto matrix2x3 = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 3, ThrowingErrorHandler>>();
	matrix2x3.set(0, 0, 0);
	matrix2x3.set(0, 1, 1);
	matrix2x3.set(0, 2, 2);
	matrix2x3.set(1, 0, 10);
	matrix2x3.set(1, 1, 11);
	matrix2x3.set(1, 2, 12);

	const auto result = matrix2x2 * matrix2x3;

	static_assert(std::is_same_v<
		decltype(result),
		const Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 3, ThrowingErrorHandler>>
		>);

	EXPECT_EQ(result.get(0, 0), 10);
	EXPECT_EQ(result.get(0, 1), 11);
	EXPECT_EQ(result.get(0, 2), 12);
	EXPECT_EQ(result.get(1, 0), 110);
	EXPECT_EQ(result.get(1, 1), 131);
	EXPECT_EQ(result.get(1, 2), 152);
}

TEST_F(MatrixTest, MatrixMultiplicationWithAssignmentWorks) {
	auto lhsMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>();
	lhsMatrix.set(0, 0, 0);
	lhsMatrix.set(0, 1, 1);
	lhsMatrix.set(1, 0, 10);
	lhsMatrix.set(1, 1, 11);

	auto rhsMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>();
	rhsMatrix.set(0, 0, 0);
	rhsMatrix.set(0, 1, 2);
	rhsMatrix.set(1, 0, 20);
	rhsMatrix.set(1, 1, 22);

	const auto& result = (lhsMatrix *= rhsMatrix);

	static_assert(std::is_same_v<
		decltype(result),
		const Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>&
		>);

	EXPECT_EQ(&result, &lhsMatrix);
	EXPECT_EQ(result.get(0, 0), 20);
	EXPECT_EQ(result.get(0, 1), 22);
	EXPECT_EQ(result.get(1, 0), 220);
	EXPECT_EQ(result.get(1, 1), 262);
}

TEST_F(MatrixTest, AffineTransformMatrixMultiplicationWorks) {
	auto lhsMatrix = Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>();
	lhsMatrix.set(0, 0, 0);
	lhsMatrix.set(0, 1, 1);
	lhsMatrix.set(0, 2, 2);
	lhsMatrix.set(0, 3, 3);
	lhsMatrix.set(1, 0, 10);
	lhsMatrix.set(1, 1, 11);
	lhsMatrix.set(1, 2, 12);
	lhsMatrix.set(1, 3, 13);
	lhsMatrix.set(2, 0, 20);
	lhsMatrix.set(2, 1, 21);
	lhsMatrix.set(2, 2, 22);
	lhsMatrix.set(2, 3, 23);

	auto rhsMatrix = Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>();
	rhsMatrix.set(0, 0, 0);
	rhsMatrix.set(0, 1, 1);
	rhsMatrix.set(0, 2, 2);
	rhsMatrix.set(0, 3, 3);
	rhsMatrix.set(1, 0, 20);
	rhsMatrix.set(1, 1, 21);
	rhsMatrix.set(1, 2, 22);
	rhsMatrix.set(1, 3, 23);
	rhsMatrix.set(2, 0, 40);
	rhsMatrix.set(2, 1, 41);
	rhsMatrix.set(2, 2, 42);
	rhsMatrix.set(2, 3, 43);

	const auto result = lhsMatrix * rhsMatrix;

	static_assert(std::is_same_v<
		decltype(result),
		const Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>
		>);

	EXPECT_EQ(result.get(0, 0), 100);
	EXPECT_EQ(result.get(0, 1), 103);
	EXPECT_EQ(result.get(0, 2), 106);
	EXPECT_EQ(result.get(0, 3), 112);
	EXPECT_EQ(result.get(1, 0), 700);
	EXPECT_EQ(result.get(1, 1), 733);
	EXPECT_EQ(result.get(1, 2), 766);
	EXPECT_EQ(result.get(1, 3), 812);
	EXPECT_EQ(result.get(2, 0), 1300);
	EXPECT_EQ(result.get(2, 1), 1363);
	EXPECT_EQ(result.get(2, 2), 1426);
	EXPECT_EQ(result.get(2, 3), 1512);
	EXPECT_EQ(result.get(3, 0), 0);
	EXPECT_EQ(result.get(3, 1), 0);
	EXPECT_EQ(result.get(3, 2), 0);
	EXPECT_EQ(result.get(3, 3), 1);
}

TEST_F(MatrixTest, MultiplicationIsNoexceptIfStorageIsNoexcept) {
	using Matrix = Matrix<NoexceptStorage<12, 12>>;
	static_assert(noexcept(std::declval<Matrix>() * std::declval<Matrix>()));
	static_assert(noexcept(std::declval<Matrix&>() *= std::declval<Matrix>()));
}

TEST_F(MatrixTest, MultiplicationIsPotentiallyThrowingIfStorageIsPotentiallyThrowing) {
	using Matrix = Matrix<PotentiallyThrowingStorage<12, 12>>;
	static_assert(!noexcept(std::declval<Matrix>() * std::declval<Matrix>()));
	static_assert(!noexcept(std::declval<Matrix&>() *= std::declval<Matrix>()));
}

TEST_F(MatrixTest, MatricesAreEqualityComparable) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 3, ThrowingErrorHandler>>;
	auto matrix = Matrix(0, 1, 2, 3, 4, 5);
	auto equal = matrix;
	auto unequal = matrix;
	unequal.set(1, 2, 42);

	EXPECT_TRUE(matrix == equal);
	EXPECT_FALSE(matrix == unequal);

	EXPECT_TRUE(matrix != unequal);
	EXPECT_FALSE(matrix != equal);
}

TEST_F(MatrixTest, EqualityComparisonIsNoexceptIfStorageIsNoexcept) {
	using Matrix = Matrix<NoexceptStorage<12, 12>>;
	static_assert(noexcept(std::declval<Matrix>() == std::declval<Matrix>()));
	static_assert(noexcept(std::declval<Matrix&>() != std::declval<Matrix>()));
}

TEST_F(MatrixTest, EqualityComparisonIsPotentiallyThrowingIfStorageIsPotentiallyThrowing) {
	using Matrix = Matrix<PotentiallyThrowingStorage<12, 12>>;
	static_assert(!noexcept(std::declval<Matrix>() == std::declval<Matrix>()));
	static_assert(!noexcept(std::declval<Matrix&>() != std::declval<Matrix>()));
}

TEST_F(MatrixTest, MatricesAreScalarMultiplicative) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 3, ThrowingErrorHandler>>;
	const auto matrix = Matrix(0, 1, 2, 3, 4, 5);
	const auto twoTimesMatrix = Matrix(0, 2, 4, 6, 8, 10);

	EXPECT_EQ(matrix * 2, twoTimesMatrix);
	EXPECT_EQ(2 * matrix, twoTimesMatrix);
	
	auto multiplicationCopy = matrix;
	multiplicationCopy *= 2;
	EXPECT_EQ(multiplicationCopy, twoTimesMatrix);

	EXPECT_EQ(twoTimesMatrix / 2, matrix);
	EXPECT_EQ(2 / twoTimesMatrix, matrix);

	auto divisionCopy = twoTimesMatrix;
	divisionCopy /= 2;
	EXPECT_EQ(divisionCopy, matrix);
}

TEST_F(MatrixTest, MatrixMultiplicationByScalarIsNoexceptIfStorageIsNoexcept) {
	using Matrix = Matrix<NoexceptStorage<12, 12>>;
	static_assert(noexcept(std::declval<Matrix>() * 0.0f));
	static_assert(noexcept(0.0f * std::declval<Matrix>()));
	static_assert(noexcept(std::declval<Matrix&>() *= 0.0f));
	static_assert(noexcept(std::declval<Matrix>() / 0.0f));
	static_assert(noexcept(0.0f / std::declval<Matrix>()));
	static_assert(noexcept(std::declval<Matrix&>() /= 0.0f));
}

TEST_F(MatrixTest, MatrixMultiplicationByScalarIsPotentiallyThrowingIfStorageIsPotentiallyThrowing) {
	using Matrix = Matrix<PotentiallyThrowingStorage<12, 12>>;
	static_assert(!noexcept(std::declval<Matrix>() * 0.0f));
	static_assert(!noexcept(0.0f * std::declval<Matrix>()));
	static_assert(!noexcept(std::declval<Matrix&>() *= 0.0f));
	static_assert(!noexcept(std::declval<Matrix>() / 0.0f));
	static_assert(!noexcept(0.0f / std::declval<Matrix>()));
	static_assert(!noexcept(std::declval<Matrix&>() /= 0.0f));
}

} // anonymous namespace
