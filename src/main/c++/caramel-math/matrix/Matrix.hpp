#ifndef CARAMELMATH_MATRIX_HPP__
#define CARAMELMATH_MATRIX_HPP__

#include <type_traits>

namespace caramel_math {

template <
	class ScalarType,
	size_t ROWS,
	size_t COLUMNS,
	class StorageType
	>
class Matrix {
public:

	ScalarType& get(size_t row, size_t column) noexcept(noexcept(StorageType::get(0, 0))) {
		return data_.get(row, column);
	}

	const ScalarType& get(size_t row, size_t column) const noexcept(noexcept(StorageType::get(0, 0))) {
		return const_cast<MatrixArrayStorage&>(*this).get(row, column);
	}

private:

	StorageType data_;

};

} // namespace caramel_math

#endif /* CARAMELMATH_MATRIX_HPP__ */
