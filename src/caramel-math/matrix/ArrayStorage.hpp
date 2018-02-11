#ifndef CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__
#define CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__

#include <array>

#include "../detail/helper-type-traits.hpp"
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

	template <
		class... CompatibleValues,
		typename = std::enable_if_t<caramel_math::detail::AllConvertibleV<Scalar, CompatibleValues...>>
		>
	explicit ArrayStorage(CompatibleValues&&... values) noexcept :
		data_{ std::forward<CompatibleValues>(values)... }
	{
		static_assert(sizeof...(values) == ROWS * COLUMNS);
	}

	GetReturnType get(Row row, Column column) const noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row.value() >= ROWS || column.value() >= COLUMNS) {
				return ErrorHandler::invalidAccess<GetReturnType>(row, column);
			}
		}
		return data_[row.value() * COLUMNS + column.value()];
	}

	void set(Row row, Column column, Scalar scalar) noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row.value() >= ROWS || column.value() >= COLUMNS) {
				ErrorHandler::invalidAccess<GetReturnType>(row, column);
				return;
			}
		}
		data_[row.value() * COLUMNS + column.value()] = std::move(scalar);
	}

private:

	std::array<Scalar, ROWS * COLUMNS> data_;

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__ */
