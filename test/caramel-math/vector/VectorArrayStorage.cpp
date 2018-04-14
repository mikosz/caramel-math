#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/vector/ArrayStorage.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"
#include "VectorMockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::vector;
using namespace caramel_math::scalar;
using namespace caramel_math::vector::test;

namespace /* anonymous */ {

class VectorArrayStorageTest : public MockErrorHandlerFixtureTest {
};

TEST_F(VectorArrayStorageTest, IsDefaultConstructible) {
	auto storage = ArrayStorage<BasicScalarTraits<int>, 2, MockErrorHandlerProxy>();
}

TEST_F(VectorArrayStorageTest, IsConstructibleWithListOfValues) {
	using Storage = ArrayStorage<BasicScalarTraits<int>, 2, MockErrorHandlerProxy>;
	auto storage = Storage(0, 1);

	EXPECT_EQ(storage.get(0), 0);
	EXPECT_EQ(storage.get(1), 1);
}

TEST_F(VectorArrayStorageTest, ArrayStorageIsCopyable) {
	using Storage = ArrayStorage<BasicScalarTraits<int>, 2, MockErrorHandlerProxy>;
	auto storage = Storage(0, 1);
	const auto copy = storage;

	EXPECT_EQ(copy.get(0), 0);
	EXPECT_EQ(copy.get(1), 1);
}

TEST_F(VectorArrayStorageTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = ArrayStorage<BasicScalarTraits<int>, 2, MockErrorHandlerProxy>();
	storage.set(0, 42);
	storage.set(1, 666);

	EXPECT_EQ(storage.get(0), 42);
	EXPECT_EQ(storage.get(1), 666);
}

TEST_F(VectorArrayStorageTest, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<int>, 2, MockErrorHandlerProxy>();
	
	const auto errorValue = -42;

	EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(2)).WillOnce(testing::Return(errorValue));
	const auto value = storage.get(2);
	EXPECT_EQ(errorValue, value);
}

TEST_F(VectorArrayStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0)));
}

TEST_F(VectorArrayStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0)));
}

TEST_F(VectorArrayStorageTest, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<int>, 2, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(2));
	storage.set(2, 0);
}

TEST_F(VectorArrayStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0, 0.0f)));
}

TEST_F(VectorArrayStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0, 0.0f)));
}

} // anonymous namespace
