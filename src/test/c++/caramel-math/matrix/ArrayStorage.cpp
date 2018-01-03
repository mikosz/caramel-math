#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

struct NoexceptErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t row, size_t column) noexcept;

};

struct PotentiallyThrowingErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t row, size_t column);

};

struct MockErrorHandler {

	static MockErrorHandler* instance;

	MOCK_METHOD2(invalidAccess, float (size_t, size_t));

};

MockErrorHandler* MockErrorHandler::instance = nullptr;

struct MockErrorHandlerProxy {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t row, size_t column) noexcept {
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

TEST_F(MockErrorHandlerFixture, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, MockErrorHandlerProxy>();
	storage.set(0, 0, 42.0f);
	storage.set(0, 1, 666.0f);

	EXPECT_EQ(storage.get(0, 0), 42.0f);
	EXPECT_EQ(storage.get(0, 1), 666.0f);
}

TEST_F(MockErrorHandlerFixture, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, MockErrorHandlerProxy>();
	
	const auto errorValue = -42.0f;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1, 0)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(1, 0);
		EXPECT_EQ(errorValue, value);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0, 2)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(0, 2);
		EXPECT_EQ(errorValue, value);
	}

}

TEST(ArrayStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0, 0)));
}

TEST(ArrayStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0, 0)));
}

TEST_F(MockErrorHandlerFixture, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, MockErrorHandlerProxy>();

	const auto errorValue = -42.0f;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1, 0)).WillOnce(testing::Return(errorValue));
		storage.set(1, 0, 0.0f);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0, 2)).WillOnce(testing::Return(errorValue));
		storage.set(0, 2, 0.0f);
	}

}

TEST(ArrayStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0, 0, 0.0f)));
}

TEST(ArrayStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0, 0, 0.0f)));
}
