#ifndef CARAMELMATH_MATRIXARRAYSTORAGE_HPP__
#define CARAMELMATH_MATRIXARRAYSTORAGE_HPP__

#include <cassert>
#include <array>

#include "../setup.hpp"

namespace caramel_math {

template <
	class ScalarType,
	size_t ROWS,
	size_t COLUMNS,
	class ErrorHandlerType
	>
class MatrixArrayStorage {
public:

	ScalarType& get(size_t row, size_t column) noexcept(ErrorHandlerType::THROWS_EXCEPTIONS) {
		if constexpr (RUNTIME_CHECKS) {
			if (row < ROWS && column < COLUMNS) {
				ErrorHandlerType::invalidAccess(row, column);
			}
		}
		return data_[row][column];
	}

private:

	std::array<ScalarType, ROWS * COLUMNS> data_;

};

} // namespace caramel_math

#endif /* CARAMELMATH_MATRIXARRAYSTORAGE_HPP__ */
