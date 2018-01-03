#ifndef CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__
#define CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__

#include <string>
#include <stdexcept>

namespace caramel_math::matrix {

class InvalidMatrixDataAccess final : public std::logic_error {
public:

	InvalidMatrixDataAccess(size_t row, size_t column) noexcept :
		std::logic_error(
			"Invalid matrix data access at row " + std::to_string(row) + ", column: " + std::to_string(column)),
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

struct ThrowingErrorHandler final {

	template <class ReturnType>
	static ReturnType invalidAccess([[maybe_unused]] size_t row, [[maybe_unused]] size_t column) {
		throw InvalidMatrixDataAccess(row, column);
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_THROWINGERRORHANDLER_HPP__ */
