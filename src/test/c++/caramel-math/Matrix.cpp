#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/Matrix.hpp"

using namespace caramel_math;

namespace /* anonymous */ {

struct MockStorage {

	static MockStorage* instance;

	MOCK_METHOD2(getNonConst, float& (size_t, size_t));

	MOCK_CONST_METHOD2(getConst, float& (size_t, size_t));

};

MockStorage* MockStorage::instance = nullptr;

struct MockStorageProxy {

	float& get(size_t row, size_t column) {
		return MockStorage::instance->getNonConst(row, column);
	}

	const float& get(size_t row, size_t column) const {
		return const_cast<const MockStorage*>(MockStorage::instance)->getConst(row, column);
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
		EXPECT_CALL(*MockStorage::instance, getNonConst(0, 1));
		matrix.get(0, 1);
	}

	{
		EXPECT_CALL(*MockStorage::instance, getConst(0, 1));

		const auto& constMatrix = matrix;
		constMatrix.get(0, 1);
	}
}

//TEST(GetIsNoexceptIfStorageGetIsNoexcept) {
//
//}

} // anonymous namespace
