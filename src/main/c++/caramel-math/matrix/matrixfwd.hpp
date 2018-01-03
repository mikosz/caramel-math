#ifndef CARAMELMATH_MATRIX_MATRIXFWD_HPP__
#define CARAMELMATH_MATRIX_MATRIXFWD_HPP__

#include "matrixfwd.hpp"

namespace caramel_math::matrix {

template <class StorageType>
class Matrix;

template <
	class ScalarType,
	size_t ROWS_VALUE,
	size_t COLUMNS_VALUE,
	template <class> class ErrorHandlerType
	>
class ArrayStorage;

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIXFWD_HPP__ */
