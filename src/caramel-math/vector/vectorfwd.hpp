#ifndef CARAMELMATH_VECTOR_VECTORFWD_HPP__
#define CARAMELMATH_VECTOR_VECTORFWD_HPP__

namespace caramel_math::vector {

template <class StorageType>
class Vector;

template <
	class ScalarTraitsType,
	size_t SIZE_VALUE,
	class ErrorHandlerType
	>
class ArrayStorage;

//template <
//	class ScalarTraitsType,
//	class ErrorHandlerType
//	>
//class AffineTransformStorage;
//
//template <
//	class ViewedMatrixType,
//	class ModifierFuncType
//	>
//class ViewStorage;
//
//template <class ScalarTraitsType, class ErrorHandlerType>
//class SimdStorage;

} // namespace caramel_math::vector

#endif /* CARAMELMATH_VECTOR_VECTORFWD_HPP__ */
