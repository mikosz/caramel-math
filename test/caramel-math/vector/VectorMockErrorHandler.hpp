#ifndef CARAMELMATH_TEST_VECTOR_MOCKERRORHANDLER_HPP__
#define CARAMELMATH_TEST_VECTOR_MOCKERRORHANDLER_HPP__

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace caramel_math::vector::test {

struct NoexceptErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t elementIdx) noexcept;

	template <class ScalarType>
	static void invalidValue(size_t elementIdx, ScalarType got, ScalarType expected) noexcept;

};

struct PotentiallyThrowingErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t elementIdx);

	template <class ScalarType>
	static void invalidValue(size_t elementIdx, ScalarType got, ScalarType expected);

};

struct MockErrorHandler {

	static MockErrorHandler* instance;

	MOCK_METHOD1(invalidAccess, int (size_t));

	MOCK_METHOD3(invalidValue, void (size_t, int, int));

};

struct MockErrorHandlerProxy {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t elementIdx) noexcept {
		return static_cast<ReturnType>(MockErrorHandler::instance->invalidAccess(elementIdx));
	}

	template <class ScalarType>
	static void invalidValue(size_t elementIdx, ScalarType got, ScalarType expected) noexcept {
		MockErrorHandler::instance->invalidValue(elementIdx, static_cast<int>(got), static_cast<int>(expected));
	}

};

class MockErrorHandlerFixtureTest : public testing::Test {
public:

	MockErrorHandlerFixtureTest() {
		MockErrorHandler::instance = &mockErrorHandler_;
	}

	~MockErrorHandlerFixtureTest() {
		MockErrorHandler::instance = nullptr;
	}

private:

	MockErrorHandler mockErrorHandler_;

};

} // namespace caramel_math::vector::test

#endif /* CARAMELMATH_TEST_VECTOR_MOCKERRORHANDLER_HPP__ */
