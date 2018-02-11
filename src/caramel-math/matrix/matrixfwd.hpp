#ifndef CARAMELMATH_MATRIX_MATRIXFWD_HPP__
#define CARAMELMATH_MATRIX_MATRIXFWD_HPP__

namespace caramel_math::matrix {

template <class StorageType>
class Matrix;

template <
	class ScalarTraitsType,
	size_t ROWS_VALUE,
	size_t COLUMNS_VALUE,
	class ErrorHandlerType
	>
class ArrayStorage;

template <
	class ScalarTraitsType,
	class ErrorHandlerType
	>
class AffineTransformStorage;

template <
	class ViewedMatrixType,
	class ModifierFuncType
	>
class ViewStorage;

template <class ScalarTraitsType, class ErrorHandlerType>
class SimdStorage;

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIXFWD_HPP__ */
