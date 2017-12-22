#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/Matrix.hpp"

using namespace caramel_math::matrix;

namespace /* anonymous */ {

struct NoexceptStorage {

	float& get(size_t, size_t) noexcept {
		static auto f = 0.0f;
		return f;
	}

};

struct PotentiallyThrowingStorage {

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
	auto matrix = Matrix<float, 1, 2, MockStorageProxy>();

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

TEST(MatrixGetTest, GetIsNoexceptIfStorageGetIsNoexcept) {
	auto matrix = Matrix<float, 12, 34, NoexceptStorage>();
	static_assert(noexcept(matrix.get(0, 0)));

	const auto& constMatrix = matrix;
	static_assert(noexcept(constMatrix.get(0, 0)));
}

TEST(MatrixGetTest, GetIsPotentiallyThrowingIfStorageGetIsPotentiallyThrowing) {
	auto matrix = Matrix<float, 12, 34, PotentiallyThrowingStorage>();
	static_assert(!noexcept(matrix.get(0, 0)));

	const auto& constMatrix = matrix;
	static_assert(!noexcept(constMatrix.get(0, 0)));
}

} // anonymous namespace
