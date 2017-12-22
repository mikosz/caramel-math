#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ArrayStorage.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

template <class ScalarType>
struct NoexceptErrorHandler {

	static ScalarType& invalidAccess(size_t row, size_t column) noexcept;

};

template <class ScalarType>
struct PotentiallyThrowingErrorHandler {

	static ScalarType& invalidAccess(size_t row, size_t column);

};

struct MockErrorHandler {

	static MockErrorHandler* instance;

	MOCK_METHOD2(invalidAccess, float& (size_t, size_t));

};

MockErrorHandler* MockErrorHandler::instance = nullptr;

template <class ScalarType>
struct MockErrorHandlerProxy {

	static ScalarType& invalidAccess(size_t row, size_t column) noexcept {
		return MockErrorHandler::instance->invalidAccess(row, column);
	}

};

class MockErrorHandlerFixture : public testing::Test {
public:

	MockErrorHandlerFixture() {
		MockErrorHandler::instance = &mockErrorHandler_;
	}

	~MockErrorHandlerFixture() {
		MockErrorHandler::instance = nullptr;
	}

private:

	MockErrorHandler mockErrorHandler_;

};

} // anonymous namespace

TEST_F(MockErrorHandlerFixture, GetReturnsReferenceToStoredValue) {
	auto storage = ArrayStorage<float, 1, 2, MockErrorHandlerProxy>();
	storage.get(0, 0) = 42.0f;
	storage.get(0, 1) = 666.0f;

	EXPECT_EQ(storage.get(0, 0), 42.0f);
	EXPECT_EQ(storage.get(0, 1), 666.0f);
}

TEST_F(MockErrorHandlerFixture, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<float, 1, 2, MockErrorHandlerProxy>();
	
	auto errorValue = float();
	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1, 0)).WillOnce(testing::ReturnRef(errorValue));
		auto& value = storage.get(1, 0);
		EXPECT_EQ(&errorValue, &value);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0, 2)).WillOnce(testing::ReturnRef(errorValue));
		auto& value = storage.get(0, 2);
		EXPECT_EQ(&errorValue, &value);
	}

}

TEST(ArrayStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<float, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0, 0)));
}

TEST(ArrayStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<float, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0, 0)));
}
