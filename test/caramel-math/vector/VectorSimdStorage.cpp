#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/vector/ThrowingErrorHandler.hpp"
#include "caramel-math/vector/SimdStorage.hpp"
#include "caramel-math/vector/Vector.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"
#include "VectorMockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::scalar;
using namespace caramel_math::vector;
using namespace caramel_math::vector::test;

namespace /* anonymous */ {

class VectorSimdStorageTest : public MockErrorHandlerFixtureTest {
};

TEST_F(VectorSimdStorageTest, IsDefaultConstructible) {
	auto storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>();
}

TEST_F(VectorSimdStorageTest, IsConstructibleWithListOfValues) {
	using Storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>;
	auto storage = Storage(0.0f, 1.0f, 2.0f, 3.0f);

	EXPECT_FLOAT_EQ(storage.get(0), 0.0f);
	EXPECT_FLOAT_EQ(storage.get(1), 1.0f);
	EXPECT_FLOAT_EQ(storage.get(2), 2.0f);
	EXPECT_FLOAT_EQ(storage.get(3), 3.0f);
}

TEST_F(VectorSimdStorageTest, ArrayStorageIsCopyable) {
	using Storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>;
	const auto storage = Storage(0.0f, 1.0f, 2.0f, 3.0f);
	const auto copy = storage;

	EXPECT_FLOAT_EQ(copy.get(0), 0.0f);
	EXPECT_FLOAT_EQ(copy.get(1), 1.0f);
	EXPECT_FLOAT_EQ(copy.get(2), 2.0f);
	EXPECT_FLOAT_EQ(copy.get(3), 3.0f);
}

TEST_F(VectorSimdStorageTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>();
	storage.set(0, 42.0f);
	storage.set(1, 666.0f);

	EXPECT_FLOAT_EQ(storage.get(0), 42.0f);
	EXPECT_FLOAT_EQ(storage.get(1), 666.0f);
}

TEST_F(VectorSimdStorageTest, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = SimdStorage<BasicScalarTraits<float>, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(4)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(4);
		EXPECT_EQ(errorValue, value);
	}
}

TEST_F(VectorSimdStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = SimdStorage<BasicScalarTraits<float>, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0)));
}

TEST_F(VectorSimdStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = SimdStorage<BasicScalarTraits<float>, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0)));
}

TEST_F(VectorSimdStorageTest, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = SimdStorage<BasicScalarTraits<float>, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(4)).WillOnce(testing::Return(errorValue));
		storage.set(4, 0.0f);
	}
}

TEST_F(VectorSimdStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = SimdStorage<BasicScalarTraits<float>, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0, 0.0f)));
}

TEST_F(VectorSimdStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = SimdStorage<BasicScalarTraits<float>, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0, 0.0f)));
}

} // anonymous namespace
