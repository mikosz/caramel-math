#ifndef CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__
#define CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__

#include <array>

#include "../setup.hpp"

namespace caramel_math::matrix {

template <
	class ScalarTraitsType,
	size_t ROWS_VALUE,
	size_t COLUMNS_VALUE,
	class ErrorHandlerType
	>
class ArrayStorage {
public:

	using ScalarTraits = ScalarTraitsType;

	using ErrorHandler = ErrorHandlerType;

	using Scalar = typename ScalarTraits::Scalar;

	static constexpr auto ROWS = ROWS_VALUE;

	static constexpr auto COLUMNS = COLUMNS_VALUE;

	using GetReturnType = std::conditional_t<
		std::is_arithmetic_v<Scalar>,
		std::add_const_t<Scalar>,
		std::add_const_t<Scalar&>
		>;

	ArrayStorage() = default;

	template <class... CompatibleValues>
	explicit ArrayStorage(CompatibleValues&&... values) noexcept :
		data_{ std::forward<CompatibleValues>(values)... }
	{
		static_assert(sizeof...(values) == ROWS * COLUMNS);
	}

	GetReturnType get(size_t row, size_t column) const noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				return ErrorHandler::invalidAccess<GetReturnType>(row, column);
			}
		}
		return data_[row * COLUMNS + column];
	}

	void set(size_t row, size_t column, Scalar scalar) noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				ErrorHandler::invalidAccess<GetReturnType>(row, column);
				return;
			}
		}
		data_[row * COLUMNS + column] = std::move(scalar);
	}

private:

	std::array<Scalar, ROWS * COLUMNS> data_;

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__ */
