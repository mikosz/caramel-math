#ifndef CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__
#define CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__

#include <string>
#include <stdexcept>

#include "matrix-coordinates.hpp"

namespace caramel_math::matrix {

class InvalidMatrixDataAccess final : public std::logic_error {
public:

	InvalidMatrixDataAccess(Row row, Column column) noexcept :
		std::logic_error(
			"Invalid matrix data access at row " +
			std::to_string(row.value()) +
			", column " +
			std::to_string(column.value())
			),
		row_(row),
		column_(column)
	{
	}

	Row row() const noexcept {
		return row_;
	}

	Column column() const noexcept {
		return column_;
	}

private:

	Row row_;

	Column column_;

};

template <class ScalarType>
class InvalidMatrixDataValue final : public std::logic_error {
public:

	using Scalar = ScalarType;

	InvalidMatrixDataValue(Row row, Column column, Scalar actualValue, Scalar expectedValue) noexcept :
		std::logic_error(
			"Invalid matrix data value at row " +
			std::to_string(row.value()) +
			", column " +
			std::to_string(column.value()) +
			" - got " +
			std::to_string(actualValue) +
			", expected " +
			std::to_string(expectedValue)
			),
		row_(row),
		column_(column),
		actualValue_(std::move(actualValue)),
		expectedValue_(std::move(expectedValue))
	{
	}

	Row row() const noexcept {
		return row_;
	}

	Column column() const noexcept {
		return column_;
	}

	Scalar actualValue() const noexcept {
		return actualValue_;
	}

	Scalar expectedValue() const noexcept {
		return expectedValue_;
	}

private:

	Row row_;

	Column column_;

	Scalar actualValue_;

	Scalar expectedValue_;

};

struct ThrowingErrorHandler final {

	template <class ReturnType>
	static ReturnType invalidAccess(Row row, Column column) {
		throw InvalidMatrixDataAccess(row, column);
	}

	template <class ScalarType>
	static void invalidValue(
		Row row,
		Column column,
		ScalarType actualValue,
		ScalarType expectedValue
		)
	{
		throw InvalidMatrixDataValue<ScalarType>(row, column, actualValue, expectedValue);
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__ */
