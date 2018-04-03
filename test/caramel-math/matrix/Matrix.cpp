#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AffineTransformStorage.hpp"
#include "caramel-math/matrix/SimdStorage.hpp"
#include "caramel-math/matrix/AssertErrorHandler.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::scalar;
using namespace caramel_math::matrix::literals;

namespace /* anonymous */ {

struct MockStorage {

	static MockStorage* instance;

	MOCK_CONST_METHOD2(get, float (Row, Column));

	MOCK_METHOD3(set, void (Row, Column, float));

};

MockStorage* MockStorage::instance = nullptr;

template <size_t ROWS_PARAM, size_t COLUMNS_PARAM>
struct MockStorageProxy {

	using Scalar = float;
	using GetReturnType = Scalar;
	static constexpr auto ROWS = ROWS_PARAM;
	static constexpr auto COLUMNS = COLUMNS_PARAM;

	float get(Row row, Column column) const {
		return MockStorage::instance->get(row, column);
	}

	void set(Row row, Column column, float value) {
		return MockStorage::instance->set(row, column, value);
	}

};

class MockStorageFixture {
public:

	MockStorageFixture()
	{
		MockStorage::instance = &storage_;
	}

	~MockStorageFixture() {
		MockStorage::instance = nullptr;
	}

private:

	MockStorage storage_;

};

template <class StorageType>
struct IsAffineStorage {
	enum { VALUE = false };
};

template <class ScalarTraits, class ErrorHandler>
struct IsAffineStorage<AffineTransformStorage<ScalarTraits, ErrorHandler>> {
	enum { VALUE = true };
};

template <class StorageType>
constexpr auto IsAffineStorageV = IsAffineStorage<StorageType>::VALUE;

// Family of tests for concrete storages (all storages except for views)
template <class StorageType>
class ConcreteStorageTest : public testing::Test {
public:
	static constexpr auto MUTABLE_ROWS = IsAffineStorageV<StorageType> ? 3 : StorageType::ROWS;
};

using ConcreteStorageTypes = testing::Types<
	ArrayStorage<BasicScalarTraits<float>, 2, 3, ThrowingErrorHandler>,
	ArrayStorage<BasicScalarTraits<float>, 2, 3, AssertErrorHandler>,
	ArrayStorage<BasicScalarTraits<float>, 3, 3, ThrowingErrorHandler>,
	ArrayStorage<BasicScalarTraits<float>, 3, 3, AssertErrorHandler>,
	AffineTransformStorage<BasicScalarTraits<float>, ThrowingErrorHandler>,
	AffineTransformStorage<BasicScalarTraits<float>, AssertErrorHandler>,
	SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>,
	SimdStorage<BasicScalarTraits<float>, AssertErrorHandler>
	>;

TYPED_TEST_CASE(ConcreteStorageTest, ConcreteStorageTypes);

// Family of tests for storages storing any value (all concrete storages except for affine transforms)
template <class StorageType>
class AnyValueStorageTest : public testing::Test {
};

using AnyValueStorageTypes = testing::Types<
	ArrayStorage<BasicScalarTraits<float>, 2, 3, ThrowingErrorHandler>,
	ArrayStorage<BasicScalarTraits<float>, 2, 3, AssertErrorHandler>,
	ArrayStorage<BasicScalarTraits<float>, 3, 3, ThrowingErrorHandler>,
	ArrayStorage<BasicScalarTraits<float>, 3, 3, AssertErrorHandler>,
	SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>,
	SimdStorage<BasicScalarTraits<float>, AssertErrorHandler>
	>;

TYPED_TEST_CASE(AnyValueStorageTest, AnyValueStorageTypes);

TYPED_TEST(ConcreteStorageTest, MatricesHaveCopySemantics) {
	using MatrixType = Matrix<TypeParam>;

	auto source = MatrixType();

	for (auto row = 0_row; row.value() < TestFixture::MUTABLE_ROWS; ++row) {
		for (auto column = 0_col; column.value() < MatrixType::COLUMNS; ++column) {
			source.set(row, column, static_cast<float>(row.value() * 10 + column.value()));
		}
	}

	auto copyConstructorTarget = source;

	EXPECT_EQ(source, copyConstructorTarget);

	auto copyAssignmentTarget = MatrixType();
	copyAssignmentTarget = source;

	EXPECT_EQ(source, copyAssignmentTarget);
}

TYPED_TEST(ConcreteStorageTest, MatricesAreEqualityComparable) {
	using MatrixType = Matrix<TypeParam>;

	auto lhs = MatrixType();
	auto rhs = MatrixType();

	for (auto row = 0_row; row.value() < TestFixture::MUTABLE_ROWS; ++row) {
		for (auto column = 0_col; column.value() < MatrixType::COLUMNS; ++column) {
			lhs.set(row, column, static_cast<float>(row.value() * 10 + column.value()));
			rhs.set(row, column, static_cast<float>(row.value() * 10 + column.value()));
		}
	}

	EXPECT_EQ(lhs, rhs);

	rhs.set(1_row, 2_col, 0.0f);

	EXPECT_NE(lhs, rhs);
}

TYPED_TEST(ConcreteStorageTest, EqualityComparisonIsNoexceptIfStorageIsNoexcept) {
	using MatrixType = Matrix<TypeParam>;

	if constexpr (std::is_same_v<typename TypeParam::ErrorHandler, ThrowingErrorHandler>) {
		static_assert(!noexcept(std::declval<MatrixType>() == std::declval<MatrixType>()));
		static_assert(!noexcept(std::declval<MatrixType&>() != std::declval<MatrixType>()));
	} else {
		static_assert(noexcept(std::declval<MatrixType>() == std::declval<MatrixType>()));
		static_assert(noexcept(std::declval<MatrixType&>() != std::declval<MatrixType>()));
	}
}

TYPED_TEST(ConcreteStorageTest, IdentityMatrixHasOnesOnDiagonal) {
	using MatrixType = Matrix<TypeParam>;

	const auto identity = MatrixType::IDENTITY;

	for (auto row = 0_row; row.value() < TypeParam::ROWS; ++row) {
		for (auto column = 0_col; column.value() < TypeParam::COLUMNS; ++column) {
			const auto value = identity.get(row, column);
			if (row.value() == column.value()) {
				EXPECT_EQ(value, 1.0f);
			} else {
				EXPECT_EQ(value, 0.0f);
			}
		}
	}
}

TYPED_TEST(AnyValueStorageTest, ZeroMatrixHasZeroesEverywhere) {
	using MatrixType = Matrix<TypeParam>;

	const auto zero = MatrixType::ZERO;

	for (auto row = 0_row; row.value() < TypeParam::ROWS; ++row) {
		for (auto column = 0_col; column.value() < TypeParam::COLUMNS; ++column) {
			EXPECT_EQ(zero.get(row, column), 0);
		}
	}
}

TYPED_TEST(ConcreteStorageTest, GetAndSetHandleErrorsAppropriately) {
	using MatrixType = Matrix<TypeParam>;

	auto matrix = MatrixType();

	if constexpr (std::is_same_v<typename TypeParam::ErrorHandler, ThrowingErrorHandler>) {
		static_assert(!noexcept(matrix.get(0_row, 0_col)));
		static_assert(!noexcept(matrix.set(0_row, 0_col, 0.0f)));
		EXPECT_THROW(matrix.get(Row(MatrixType::ROWS), 0_col), InvalidMatrixDataAccess);
		EXPECT_THROW(matrix.get(0_row, Column(MatrixType::COLUMNS)), InvalidMatrixDataAccess);
		EXPECT_THROW(matrix.set(Row(MatrixType::ROWS), 0_col, 0.0f), InvalidMatrixDataAccess);
		EXPECT_THROW(matrix.set(0_row, Column(MatrixType::COLUMNS), 0.0f), InvalidMatrixDataAccess);
	} else {
		static_assert(noexcept(matrix.get(0_row, 0_col)));
		static_assert(noexcept(matrix.set(0_row, 0_col, 0.0f)));
		EXPECT_DEATH(matrix.get(Row(MatrixType::ROWS), 0_col), "Invalid matrix data access");
		EXPECT_DEATH(matrix.get(0_row, Column(MatrixType::COLUMNS)), "Invalid matrix data access");
		EXPECT_DEATH(matrix.set(Row(MatrixType::ROWS), 0_col, 0.0f), "Invalid matrix data access");
		EXPECT_DEATH(matrix.set(0_row, Column(MatrixType::COLUMNS), 0.0f), "Invalid matrix data access");
	}
}

TYPED_TEST(ConcreteStorageTest, MatricesAreConvertibleToCompatibleMatrices) {
	using MatrixType = Matrix<TypeParam>;

	MockStorageFixture scopedFixture{};

	auto valueMaker = [](const auto row, const auto column) {
			if (row.value() < TestFixture::MUTABLE_ROWS) {
				return row.value() * 10.0f + column.value();
			} else if (column.value() < MatrixType::COLUMNS - 1) {
				return 0.0f;
			} else {
				return 1.0f;
			}
		};

	for (auto row = 0_row; row.value() < MatrixType::ROWS; ++row) {
		for (auto column = 0_col; column.value() < MatrixType::COLUMNS; ++column) {
			EXPECT_CALL(*MockStorage::instance, get(row, column))
				.WillRepeatedly(testing::Return(valueMaker(row, column)))
				;
		}
	}

	auto source = Matrix<MockStorageProxy<MatrixType::ROWS, MatrixType::COLUMNS>>();

	const auto constructedTarget = MatrixType(source);

	auto assignedTarget = MatrixType();
	assignedTarget = source;

	for (auto row = 0_row; row.value() < MatrixType::ROWS; ++row) {
		for (auto column = 0_col; column.value() < MatrixType::COLUMNS; ++column) {
			EXPECT_FLOAT_EQ(constructedTarget.get(row, column), valueMaker(row, column));
			EXPECT_FLOAT_EQ(assignedTarget.get(row, column), valueMaker(row, column));
		}
	}
}

TYPED_TEST(ConcreteStorageTest, MultiplicationIsNoexceptIfStorageIsNoexcept) {
	using MatrixType = Matrix<TypeParam>;

	if constexpr (std::is_same_v<typename TypeParam::ErrorHandler, ThrowingErrorHandler>) {
		static_assert(!noexcept(std::declval<MatrixType>() * transposed(std::declval<MatrixType>())));
		if constexpr (MatrixType::ROWS == MatrixType::COLUMNS) {
			static_assert(!noexcept(std::declval<MatrixType&>() *= transposed(std::declval<MatrixType>())));
		}
	} else {
		static_assert(noexcept(std::declval<MatrixType>() * transposed(std::declval<MatrixType>())));
		if constexpr (MatrixType::ROWS == MatrixType::COLUMNS) {
			static_assert(noexcept(std::declval<MatrixType&>() *= transposed(std::declval<MatrixType>())));
		}
	}
}

TYPED_TEST(AnyValueStorageTest, MatricesAreScalarMultiplicative) {
	using MatrixType = Matrix<TypeParam>;

	auto matrix = MatrixType();
	auto twoTimesMatrix = MatrixType();

	auto index = 0;
	for (auto row = 0_row; row.value() < MatrixType::ROWS; ++row) {
		for (auto column = 0_col; column.value() < MatrixType::COLUMNS; ++column) {
			matrix.set(row, column, MatrixType::Scalar(index));
			twoTimesMatrix.set(row, column, MatrixType::Scalar(index * 2));
			++index;
		}
	}

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

TYPED_TEST(AnyValueStorageTest, MatrixMultiplicationByScalarIsNoexceptIfStorageIsNoexcept) {
	using MatrixType = Matrix<TypeParam>;

	if constexpr (std::is_same_v<typename TypeParam::ErrorHandler, ThrowingErrorHandler>) {
		static_assert(!noexcept(std::declval<MatrixType>() * 0.0f));
		static_assert(!noexcept(0.0f * std::declval<MatrixType>()));
		static_assert(!noexcept(std::declval<MatrixType&>() *= 0.0f));
		static_assert(!noexcept(std::declval<MatrixType>() / 0.0f));
		static_assert(!noexcept(0.0f / std::declval<MatrixType>()));
		static_assert(!noexcept(std::declval<MatrixType&>() /= 0.0f));
	} else {
		static_assert(noexcept(std::declval<MatrixType>() * 0.0f));
		static_assert(noexcept(0.0f * std::declval<MatrixType>()));
		static_assert(noexcept(std::declval<MatrixType&>() *= 0.0f));
		static_assert(noexcept(std::declval<MatrixType>() / 0.0f));
		static_assert(noexcept(0.0f / std::declval<MatrixType>()));
		static_assert(noexcept(std::declval<MatrixType&>() /= 0.0f));
	}
}

TYPED_TEST(ConcreteStorageTest, TransposeTransposesMatrix) {
	using MatrixType = Matrix<TypeParam>;

	auto matrix = MatrixType();

	const auto MUTABLE_COLUMNS =
		TestFixture::MUTABLE_ROWS == MatrixType::ROWS ?
		MatrixType::COLUMNS :
		TestFixture::MUTABLE_ROWS
		;

	auto index = 0;
	for (auto row = 0_row; row.value() < TestFixture::MUTABLE_ROWS; ++row) {
		for (auto column = 0_col; column.value() < MUTABLE_COLUMNS; ++column) {
			matrix.set(row, column, MatrixType::Scalar(index));
			++index;
		}
	}

	const auto t = transposed(matrix);

	for (auto row = 0_row; row.value() < MatrixType::ROWS; ++row) {
		for (auto column = 0_col; column.value() < MatrixType::COLUMNS; ++column) {
			EXPECT_EQ(t.get(Row(column.value()), Column(row.value())), matrix.get(row, column));
		}
	}

	if constexpr (MatrixType::ROWS == MatrixType::COLUMNS) {
		matrix.transpose();
		EXPECT_EQ(t, matrix);
	}
}

// --- non-generic tests

TEST(MatrixTest, MatrixMultiplicationWorks) {
	auto matrix2x2 = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>();
	matrix2x2.set(0_row, 0_col, 0);
	matrix2x2.set(0_row, 1_col, 1);
	matrix2x2.set(1_row, 0_col, 10);
	matrix2x2.set(1_row, 1_col, 11);

	auto matrix2x3 = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 3, ThrowingErrorHandler>>();
	matrix2x3.set(0_row, 0_col, 0);
	matrix2x3.set(0_row, 1_col, 1);
	matrix2x3.set(0_row, 2_col, 2);
	matrix2x3.set(1_row, 0_col, 10);
	matrix2x3.set(1_row, 1_col, 11);
	matrix2x3.set(1_row, 2_col, 12);

	const auto result = matrix2x2 * matrix2x3;

	static_assert(std::is_same_v<
		decltype(result),
		const Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 3, ThrowingErrorHandler>>
		>);

	EXPECT_EQ(result.get(0_row, 0_col), 10);
	EXPECT_EQ(result.get(0_row, 1_col), 11);
	EXPECT_EQ(result.get(0_row, 2_col), 12);
	EXPECT_EQ(result.get(1_row, 0_col), 110);
	EXPECT_EQ(result.get(1_row, 1_col), 131);
	EXPECT_EQ(result.get(1_row, 2_col), 152);
}

TEST(MatrixTest, MatrixMultiplicationWithAssignmentWorks) {
	auto lhsMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>();
	lhsMatrix.set(0_row, 0_col, 0);
	lhsMatrix.set(0_row, 1_col, 1);
	lhsMatrix.set(1_row, 0_col, 10);
	lhsMatrix.set(1_row, 1_col, 11);

	auto rhsMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>();
	rhsMatrix.set(0_row, 0_col, 0);
	rhsMatrix.set(0_row, 1_col, 2);
	rhsMatrix.set(1_row, 0_col, 20);
	rhsMatrix.set(1_row, 1_col, 22);

	const auto& result = (lhsMatrix *= rhsMatrix);

	static_assert(std::is_same_v<
		decltype(result),
		const Matrix<ArrayStorage<BasicScalarTraits<int>, 2, 2, ThrowingErrorHandler>>&
		>);

	EXPECT_EQ(&result, &lhsMatrix);
	EXPECT_EQ(result.get(0_row, 0_col), 20);
	EXPECT_EQ(result.get(0_row, 1_col), 22);
	EXPECT_EQ(result.get(1_row, 0_col), 220);
	EXPECT_EQ(result.get(1_row, 1_col), 262);
}

TEST(MatrixTest, AffineTransformMatrixMultiplicationWorks) {
	auto lhsMatrix = Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>();
	lhsMatrix.set(0_row, 0_col, 0);
	lhsMatrix.set(0_row, 1_col, 1);
	lhsMatrix.set(0_row, 2_col, 2);
	lhsMatrix.set(0_row, 3_col, 3);
	lhsMatrix.set(1_row, 0_col, 10);
	lhsMatrix.set(1_row, 1_col, 11);
	lhsMatrix.set(1_row, 2_col, 12);
	lhsMatrix.set(1_row, 3_col, 13);
	lhsMatrix.set(2_row, 0_col, 20);
	lhsMatrix.set(2_row, 1_col, 21);
	lhsMatrix.set(2_row, 2_col, 22);
	lhsMatrix.set(2_row, 3_col, 23);

	auto rhsMatrix = Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>();
	rhsMatrix.set(0_row, 0_col, 0);
	rhsMatrix.set(0_row, 1_col, 1);
	rhsMatrix.set(0_row, 2_col, 2);
	rhsMatrix.set(0_row, 3_col, 3);
	rhsMatrix.set(1_row, 0_col, 20);
	rhsMatrix.set(1_row, 1_col, 21);
	rhsMatrix.set(1_row, 2_col, 22);
	rhsMatrix.set(1_row, 3_col, 23);
	rhsMatrix.set(2_row, 0_col, 40);
	rhsMatrix.set(2_row, 1_col, 41);
	rhsMatrix.set(2_row, 2_col, 42);
	rhsMatrix.set(2_row, 3_col, 43);

	const auto result = lhsMatrix * rhsMatrix;

	static_assert(std::is_same_v<
		decltype(result),
		const Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>
		>);

	EXPECT_EQ(result.get(0_row, 0_col), 100);
	EXPECT_EQ(result.get(0_row, 1_col), 103);
	EXPECT_EQ(result.get(0_row, 2_col), 106);
	EXPECT_EQ(result.get(0_row, 3_col), 112);
	EXPECT_EQ(result.get(1_row, 0_col), 700);
	EXPECT_EQ(result.get(1_row, 1_col), 733);
	EXPECT_EQ(result.get(1_row, 2_col), 766);
	EXPECT_EQ(result.get(1_row, 3_col), 812);
	EXPECT_EQ(result.get(2_row, 0_col), 1300);
	EXPECT_EQ(result.get(2_row, 1_col), 1363);
	EXPECT_EQ(result.get(2_row, 2_col), 1426);
	EXPECT_EQ(result.get(2_row, 3_col), 1512);
	EXPECT_EQ(result.get(3_row, 0_col), 0);
	EXPECT_EQ(result.get(3_row, 1_col), 0);
	EXPECT_EQ(result.get(3_row, 2_col), 0);
	EXPECT_EQ(result.get(3_row, 3_col), 1);
}

TEST(MatrixTest, TransposedYieldsEffectiveTypeForSquareMatrices) {
	using ArrayStorageMatrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 4, 4, ThrowingErrorHandler>>;
	static_assert(std::is_same_v<decltype(transposed(std::declval<ArrayStorageMatrix>())), ArrayStorageMatrix>);

	using AffineTransformMatrix = Matrix<AffineTransformStorage<BasicScalarTraits<int>, ThrowingErrorHandler>>;
	static_assert(std::is_same_v<decltype(transposed(std::declval<AffineTransformMatrix>())), ArrayStorageMatrix>);

	using SimdMatrix = Matrix<SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>>;
	static_assert(std::is_same_v<decltype(transposed(std::declval<SimdMatrix>())), SimdMatrix>);
}

TEST(MatrixTest, TransposedYieldsOriginalMatrixForTransposedViewMatrix) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 4, 4, ThrowingErrorHandler>>;
	const auto m = Matrix();
	const auto tv = viewTransposed(m);
	const auto& ttv = transposed(tv);

	EXPECT_EQ(&ttv, &m);
}

TEST(MatrixTest, DeterminantYieldsMatrixDeterminant) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<int>, 3, 3, ThrowingErrorHandler>>;
	const auto m = Matrix(
		-1, 1, 2,
		-2, 3, -3,
		4, -4, 5
		);

	EXPECT_EQ(determinant(m), -13);
}

TEST(MatrixTest, InverseYieldsMatrixInverse) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 3, 3, ThrowingErrorHandler>>;
	const auto m = Matrix(
		-1.0f, 1.0f, 2.0f,
		-2.0f, 3.0f, -3.0f,
		4.0f, -4.0f, 5.0f
		);

	const auto i = inverse(m);

	ASSERT_TRUE(i);
	EXPECT_FLOAT_EQ(i->get(0_row, 0_col), -3.0f / 13.0f);
	EXPECT_FLOAT_EQ(i->get(0_row, 1_col), 1.0f);
	EXPECT_FLOAT_EQ(i->get(0_row, 2_col), 9.0f / 13.0f);
	EXPECT_FLOAT_EQ(i->get(1_row, 0_col), 2.0f / 13.0f);
	EXPECT_FLOAT_EQ(i->get(1_row, 1_col), 1.0f);
	EXPECT_FLOAT_EQ(i->get(1_row, 2_col), 7.0f / 13.0f);
	EXPECT_FLOAT_EQ(i->get(2_row, 0_col), 4.0f / 13.0f);
	EXPECT_FLOAT_EQ(i->get(2_row, 1_col), 0.0f);
	EXPECT_FLOAT_EQ(i->get(2_row, 2_col), 1.0f / 13.0f);
}

TEST(MatrixTest, InverseAffineTransformYieldsMatrixInverse) {
	using Matrix = Matrix<AffineTransformStorage<BasicScalarTraits<float>, ThrowingErrorHandler>>;
	auto m = Matrix::IDENTITY;

	m.set(0_row, 3_col, 1.0f);
	m.set(1_row, 3_col, 2.0f);
	m.set(2_row, 3_col, 3.0f);

	const auto i = inverse(m);

	ASSERT_TRUE(i);

	EXPECT_FLOAT_EQ(i->get(0_row, 0_col), 1.0f);
	EXPECT_FLOAT_EQ(i->get(0_row, 1_col), 0.0f);
	EXPECT_FLOAT_EQ(i->get(0_row, 2_col), 0.0f);
	EXPECT_FLOAT_EQ(i->get(0_row, 3_col), -1.0f);

	EXPECT_FLOAT_EQ(i->get(1_row, 0_col), 0.0f);
	EXPECT_FLOAT_EQ(i->get(1_row, 1_col), 1.0f);
	EXPECT_FLOAT_EQ(i->get(1_row, 2_col), 0.0f);
	EXPECT_FLOAT_EQ(i->get(1_row, 3_col), -2.0f);

	EXPECT_FLOAT_EQ(i->get(2_row, 0_col), 0.0f);
	EXPECT_FLOAT_EQ(i->get(2_row, 1_col), 0.0f);
	EXPECT_FLOAT_EQ(i->get(2_row, 2_col), 1.0f);
	EXPECT_FLOAT_EQ(i->get(2_row, 3_col), -3.0f);
}

TEST(MatrixTest, InverseYieldsEmptyOptionalForNonInvertibleMatrices) {
	using Matrix = Matrix<ArrayStorage<BasicScalarTraits<float>, 2, 2, ThrowingErrorHandler>>;
	const auto m = Matrix(
		-1.0f, 1.0f,
		2.0f, -2.0f
		);

	const auto i = inverse(m);

	ASSERT_FALSE(i);
}

} // anonymous namespace
