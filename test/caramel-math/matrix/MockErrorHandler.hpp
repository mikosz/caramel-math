#ifndef CARAMELMATH_TEST_MATRIX_MOCKERRORHANDLER_HPP__
#define CARAMELMATH_TEST_MATRIX_MOCKERRORHANDLER_HPP__

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/matrix-coordinates.hpp"

namespace caramel_math::matrix::test {

struct NoexceptErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(Row row, Column column) noexcept;

	template <class ScalarType>
	static void invalidValue(Row row, Column column, ScalarType got, ScalarType expected) noexcept;

};

struct PotentiallyThrowingErrorHandler {

	template <class ReturnType>
	static ReturnType invalidAccess(Row row, Column column);

	template <class ScalarType>
	static void invalidValue(Row row, Column column, ScalarType got, ScalarType expected);

};

struct MockErrorHandler {

	static MockErrorHandler* instance;

	MOCK_METHOD2(invalidAccess, int (Row, Column));

	MOCK_METHOD4(invalidValue, void (Row, Column, int, int));

};

struct MockErrorHandlerProxy {

	template <class ReturnType>
	static ReturnType invalidAccess(Row row, Column column) noexcept {
		return static_cast<ReturnType>(MockErrorHandler::instance->invalidAccess(row, column));
	}

	template <class ScalarType>
	static void invalidValue(Row row, Column column, ScalarType got, ScalarType expected) noexcept {
		MockErrorHandler::instance->invalidValue(row, column, static_cast<int>(got), static_cast<int>(expected));
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
