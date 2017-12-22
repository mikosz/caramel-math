#ifndef CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__
#define CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__

#include <array>

#include "../setup.hpp"

namespace caramel_math::matrix {

template <
	class ScalarType,
	size_t ROWS,
	size_t COLUMNS,
	template <class> class ErrorHandlerType
	>
class ArrayStorage {
public:

	using ErrorHandler = ErrorHandlerType<ScalarType>;

	ScalarType& get(size_t row, size_t column) noexcept(noexcept(ErrorHandler::invalidAccess(0, 0))) {
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				return ErrorHandler::invalidAccess(row, column);
			}
		}
		return data_[row * COLUMNS + column];
	}

private:

	std::array<ScalarType, ROWS * COLUMNS> data_;

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_ARRAYSTORAGE_HPP__ */
