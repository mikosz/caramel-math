#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/Matrix.hpp"

using namespace caramel_math::matrix;

namespace /* anonymous */ {

struct TestStorage {
	using Scalar = float;
	static constexpr auto ROWS = 12;
	static constexpr auto COLUMNS = 34;
};

struct NoexceptStorage : TestStorage {

	float& get(size_t, size_t) noexcept {
		static auto f = 0.0f;
		return f;
	}

};

struct PotentiallyThrowingStorage : TestStorage {

	float& get(size_t, size_t) {
		static auto f = 0.0f;
		return f;
	}

};

struct MockStorage {

	static MockStorage* instance;

	MOCK_METHOD2(get, float& (size_t, size_t));

};

MockStorage* MockStorage::instance = nullptr;

struct MockStorageProxy {

	using Scalar = float;
	static constexpr auto ROWS = 1;
	static constexpr auto COLUMNS = 2;

	float& get(size_t row, size_t column) {
		return MockStorage::instance->get(row, column);
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
		auto f = float();
		EXPECT_CALL(*MockStorage::instance, get(0, 1)).WillOnce(testing::ReturnRef(f));
		auto& result = matrix.get(0, 1);
		EXPECT_EQ(&f, &result);
	}

	{
		auto f = float();
		EXPECT_CALL(*MockStorage::instance, get(0, 1)).WillOnce(testing::ReturnRef(f));

		const auto& constMatrix = matrix;
		const auto& result = constMatrix.get(0, 1);
		EXPECT_EQ(&f, &result);
	}
}

TEST(MatrixTestCase, MatrixConstantsDeriveFromStorage) {
	using MatrixType = Matrix<NoexceptStorage>;
	static_assert(std::is_same_v<MatrixType::Scalar, float>);
	static_assert(MatrixType::ROWS == 12);
	static_assert(MatrixType::COLUMNS == 34);
}

TEST(MatrixGetTest, GetIsNoexceptIfStorageGetIsNoexcept) {
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

} // anonymous namespace
