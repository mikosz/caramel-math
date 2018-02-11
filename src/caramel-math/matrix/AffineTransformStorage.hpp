#ifndef CARAMELMATH_MATRIX_AFFINETRANSFORMSTORAGE_HPP__
#define CARAMELMATH_MATRIX_AFFINETRANSFORMSTORAGE_HPP__

#include <array>

#include "../setup.hpp"

namespace caramel_math::matrix {

template <
	class ScalarTraitsType,
	class ErrorHandlerType
	>
class AffineTransformStorage {
public:

	using ScalarTraits = ScalarTraitsType;

	using ErrorHandler = ErrorHandlerType;

	using Scalar = typename ScalarTraits::Scalar;

	static constexpr auto ROWS = 4;

	static constexpr auto COLUMNS = 4;

	using GetReturnType = std::conditional_t<
		std::is_arithmetic_v<Scalar>,
		std::add_const_t<Scalar>, 
		std::add_const_t<Scalar&>
		>;

	AffineTransformStorage() = default;

	explicit AffineTransformStorage(
		Scalar s00,
		Scalar s01,
		Scalar s02,
		Scalar s03,
		Scalar s10,
		Scalar s11,
		Scalar s12,
		Scalar s13,
		Scalar s20,
		Scalar s21,
		Scalar s22,
		Scalar s23
		) noexcept :
		data_{
			std::move(s00),
			std::move(s01),
			std::move(s02),
			std::move(s03),
			std::move(s10),
			std::move(s11),
			std::move(s12),
			std::move(s13),
			std::move(s20),
			std::move(s21),
			std::move(s22),
			std::move(s23)
		}
	{
	}

	GetReturnType get(Row row, Column column) const noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				return ErrorHandler::invalidAccess<GetReturnType>(row, column);
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

	void set(Row row, Column column, Scalar scalar) noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				ErrorHandler::invalidAccess<GetReturnType>(row, column);
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

	std::array<Scalar, (ROWS - 1) * COLUMNS> data_;

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_AFFINETRANSFORMSTORAGE_HPP__ */
