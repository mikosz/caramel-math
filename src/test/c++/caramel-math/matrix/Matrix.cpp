#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

struct TestStorage {
	using Scalar = float;
	static constexpr auto ROWS = 12;
	static constexpr auto COLUMNS = 34;
};

struct NoexceptStorage : TestStorage {

	float get(size_t, size_t) const noexcept {
		static auto f = 0.0f;
		return f;
	}

	void set(size_t, size_t, float) noexcept {
	}

};

struct PotentiallyThrowingStorage : TestStorage {

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

TEST_F(MatrixStorageFixture, GetCallsStorageGet) {
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

TEST_F(MatrixStorageFixture, SetCallsStorageSet) {
	auto matrix = Matrix<MockStorageProxy>();

	const auto f = 42.0f;
	EXPECT_CALL(*MockStorage::instance, set(0, 1, f)).Times(1);
	matrix.set(0, 1, f);
}

TEST(MatrixTest, MatrixConstantsDeriveFromStorage) {
	using MatrixType = Matrix<NoexceptStorage>;
	static_assert(std::is_same_v<MatrixType::Scalar, float>);
	static_assert(MatrixType::ROWS == 12);
	static_assert(MatrixType::COLUMNS == 34);
}

TEST(MatrixTest, GetIsNoexceptIfStorageGetIsNoexcept) {
	auto matrix = Matrix<NoexceptStorage>();
	static_assert(noexcept(matrix.get(0, 0)));

	const auto& constMatrix = matrix;
	static_assert(noexcept(constMatrix.get(0, 0)));
}

TEST(MatrixGetTest, GetIsPotentiallyThrowingIfStorageGetIsPotentiallyThrowing) {
	auto matrix = Matrix<PotentiallyThrowingStorage>();
	static_assert(!noexcept(matrix.get(0, 0)));

	const auto& constMatrix = matrix;
	static_assert(!noexcept(constMatrix.get(0, 0)));
}

TEST(MatrixTest, SetIsNoexceptIfStorageSetIsNoexcept) {
	auto matrix = Matrix<NoexceptStorage>();
	static_assert(noexcept(matrix.set(0, 0, 0.0f)));
}

TEST(MatrixTest, SetIsPotentiallyThrowingIfStorageSetIsPotentiallyThrowing) {
	auto matrix = Matrix<PotentiallyThrowingStorage>();
	static_assert(!noexcept(matrix.set(0, 0, 0.0f)));
}

TEST(MatrixTest, MatrixMultiplicationWorks) {
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
	EXPECT_EQ(result.get(0, 0), 10);
	EXPECT_EQ(result.get(0, 1), 11);
	EXPECT_EQ(result.get(0, 2), 12);
	EXPECT_EQ(result.get(1, 0), 110);
	EXPECT_EQ(result.get(1, 1), 131);
	EXPECT_EQ(result.get(1, 2), 152);
}

} // anonymous namespace
