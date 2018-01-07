#ifndef CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__
#define CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__

#include <string>
#include <stdexcept>

namespace caramel_math::matrix {

class InvalidMatrixDataAccess final : public std::logic_error {
public:

	InvalidMatrixDataAccess(size_t row, size_t column) noexcept :
		std::logic_error(
			"Invalid matrix data access at row " + std::to_string(row) + ", column " + std::to_string(column)),
		row_(row),
		column_(column)
	{
	}

	size_t row() const noexcept {
		return row_;
	}

	size_t column() const noexcept {
		return column_;
	}

private:

	size_t row_;

	size_t column_;

};

template <class ScalarType>
class InvalidMatrixDataValue final : public std::logic_error {
public:

	using Scalar = ScalarType;

	InvalidMatrixDataValue(size_t row, size_t column, Scalar actualValue, Scalar expectedValue) noexcept :
		std::logic_error(
			"Invalid matrix data value at row " + std::to_string(row) + ", column " + std::to_string(column) +
			" - got " + std::to_string(actualValue) + ", expected " + std::to_string(expectedValue)
			),
		row_(row),
		column_(column),
		actualValue_(std::move(actualValue)),
		expectedValue_(std::move(expectedValue))
	{
	}

	size_t row() const noexcept {
		return row_;
	}

	size_t column() const noexcept {
		return column_;
	}

	Scalar actualValue() const noexcept {
		return actualValue_;
	}

	Scalar expectedValue() const noexcept {
		return expectedValue_;
	}

private:

	size_t row_;

	size_t column_;

	Scalar actualValue_;

	Scalar expectedValue_;

};

struct ThrowingErrorHandler final {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t row, size_t column) {
		throw InvalidMatrixDataAccess(row, column);
	}

	template <class ScalarType>
	static void invalidValue(
		size_t row,
		size_t column,
		ScalarType actualValue,
		ScalarType expectedValue
		)
	{
		throw InvalidMatrixDataValue<ScalarType>(row, column, actualValue, expectedValue);
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__ */
