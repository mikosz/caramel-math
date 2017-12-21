#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>

#include "caramel-math/matrix/Matrix.hpp"

using namespace caramel_math;

namespace /* anonymous */ {

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

class StorageProxyFixture : public testing::Test {
public:

	StorageProxyFixture()
	{
		MockStorage::instance = &storage_;
	}

	~StorageProxyFixture() {
		MockStorage::instance = nullptr;
	}

private:

	MockStorage storage_;

};

TEST_F(StorageProxyFixture, GetCallsStorageGet) {
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

//TEST(GetIsNoexceptIfStorageGetIsNoexcept) {
//
//}

} // anonymous namespace
