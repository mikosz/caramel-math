#ifndef CARAMELMATH_MATRIX_AFFINETRANSFORMSTORAGE_HPP__
#define CARAMELMATH_MATRIX_AFFINETRANSFORMSTORAGE_HPP__

#include <array>

#include "../setup.hpp"

namespace caramel_math::matrix {

template <
	class ScalarType,
	template <class> class ErrorHandlerType,
	class ScalarTraitsType = ScalarTraits<ScalarType>
	>
class AffineTransformStorage {
public:

	using ErrorHandler = ErrorHandlerType<ScalarType>;

	using Scalar = ScalarType;

	static constexpr auto ROWS = 4;

	static constexpr auto COLUMNS = 4;

	using GetReturnType = std::conditional_t<
		std::is_arithmetic_v<Scalar>,
		std::add_const_t<Scalar>, 
		std::add_const_t<Scalar&>
		>;

	using ScalarTraits = ScalarTraitsType;

	GetReturnType get(size_t row, size_t column) const noexcept(
		noexcept(ErrorHandler::invalidAccess(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				return ErrorHandler::invalidAccess(row, column);
			}
		}

		if (row == ROWS - 1) {
			if (column == COLUMNS - 1) {
				return ScalarTraits::ONE;
			} else {
				return ScalarTraits::ZERO;
			}
		}

		return data_[row * COLUMNS + column];
	}

	void set(size_t row, size_t column, Scalar scalar) noexcept(
		noexcept(ErrorHandler::invalidAccess(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				ErrorHandler::invalidAccess(row, column);
				return;
			}
		}

		if (row == ROWS - 1) {
			if constexpr (RUNTIME_CHECKS) {
				if (column == COLUMNS - 1 && !ScalarTraits::equal(scalar, ScalarTraits::ONE)) {
					ErrorHandler::invalidValue(row, column, scalar, ScalarTraits::ONE);
				} else if (column < COLUMNS - 1 && !ScalarTraits::equal(scalar, ScalarTraits::ZERO)) {
					ErrorHandler::invalidValue(row, column, scalar, ScalarTraits::ZERO);
				}
			}

			return;
		}

		data_[row * COLUMNS + column] = std::move(scalar);
	}

private:

	std::array<ScalarType, ROWS * COLUMNS> data_;

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_AFFINETRANSFORMSTORAGE_HPP__ */
