#ifndef CARAMELMATH_TEST_MATRIX_MOCKERRORHANDLER_HPP__
#define CARAMELMATH_TEST_MATRIX_MOCKERRORHANDLER_HPP__

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace caramel_math::matrix::test {

struct NoexceptErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t row, size_t column) noexcept;

	template <class ScalarType>
	static void invalidValue(size_t row, size_t column, ScalarType got, ScalarType expected) noexcept;

};

struct PotentiallyThrowingErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t row, size_t column);

	template <class ScalarType>
	static void invalidValue(size_t row, size_t column, ScalarType got, ScalarType expected);

};

struct MockErrorHandler {

	static MockErrorHandler* instance;

	MOCK_METHOD2(invalidAccess, float (size_t, size_t));

	MOCK_METHOD4(invalidValue, void (size_t, size_t, int, int));

};

MockErrorHandler* MockErrorHandler::instance = nullptr;

struct MockErrorHandlerProxy {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t row, size_t column) noexcept {
		return MockErrorHandler::instance->invalidAccess(row, column);
	}

	template <class ScalarType>
	static void invalidValue(size_t row, size_t column, ScalarType got, ScalarType expected) noexcept {
		MockErrorHandler::instance->invalidValue(row, column, got, expected);
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

} // namespace caramel_math::matrix::test

#endif /* CARAMELMATH_TEST_MATRIX_MOCKERRORHANDLER_HPP__ */
