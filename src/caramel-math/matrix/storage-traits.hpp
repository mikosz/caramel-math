#ifndef CARAMELMATH_MATRIX_STORAGETRAITS_HPP__
#define CARAMELMATH_MATRIX_STORAGETRAITS_HPP__

#include <type_traits>

#include "matrixfwd.hpp"

namespace caramel_math::matrix {

namespace detail {

template <class StorageType>
struct EffectiveStorageType {
	using Type = StorageType;
};

template <
	class ViewedMatrixType,
	class ModifierFuncType
	>
struct EffectiveStorageType<ViewStorage<ViewedMatrixType, ModifierFuncType>> {
	using Type = typename EffectiveStorageType<typename ViewedMatrixType::Storage>::Type;
};

template <class LHSStorageType, class RHSStorageType, size_t ROWS, size_t COLUMNS, class = void>
struct BinaryOperatorResultType;

template <class StorageType, size_t ROWS, size_t COLUMNS>
struct BinaryOperatorResultType<StorageType, StorageType, ROWS, COLUMNS> {
	static_assert(StorageType::ROWS == ROWS);
	static_assert(StorageType::COLUMNS == COLUMNS);

	using Type = StorageType;
};

template <
	class LHSScalarTraitsType,
	size_t LHS_ROWS,
	size_t LHS_COLUMNS,
	class LHSErrorHandlerType,
	class RHSStorageType,
	size_t ROWS,
	size_t COLUMNS
	>
struct BinaryOperatorResultType<
	ArrayStorage<LHSScalarTraitsType, LHS_ROWS, LHS_COLUMNS, LHSErrorHandlerType>,
	RHSStorageType,
	ROWS,
	COLUMNS,
	std::enable_if_t<
		!std::is_same_v<ArrayStorage<LHSScalarTraitsType, LHS_ROWS, LHS_COLUMNS, LHSErrorHandlerType>, RHSStorageType>
		>
	>
{
	using Type = ArrayStorage<LHSScalarTraitsType, ROWS, COLUMNS, LHSErrorHandlerType>;
};

template <class StorageType>
struct IsArrayStorage {
	enum { VALUE = false };
};

template <
	class ScalarTraitsType,
	size_t ROWS,
	size_t COLUMNS,
	class ErrorHandlerType
	>
struct IsArrayStorage<ArrayStorage<ScalarTraitsType, ROWS, COLUMNS, ErrorHandlerType>> {
	enum { VALUE = true };
};

template <
	class LHSStorageType,
	class RHSScalarTraitsType,
	size_t RHS_ROWS,
	size_t RHS_COLUMNS,
	class RHSErrorHandlerType,
	size_t ROWS,
	size_t COLUMNS
	>
struct BinaryOperatorResultType<
	LHSStorageType,
	ArrayStorage<RHSScalarTraitsType, RHS_ROWS, RHS_COLUMNS, RHSErrorHandlerType>,
	ROWS,
	COLUMNS,
	std::enable_if_t<
		!IsArrayStorage<LHSStorageType>::VALUE &&
		!std::is_same_v<LHSStorageType, ArrayStorage<RHSScalarTraitsType, RHS_ROWS, RHS_COLUMNS, RHSErrorHandlerType>>
		>
	>
{
	using Type = ArrayStorage<RHSScalarTraitsType, ROWS, COLUMNS, RHSErrorHandlerType>;
};

} // namespace detail

template <class StorageType>
using EffectiveStorageType = typename detail::EffectiveStorageType<StorageType>::Type;

template <class LHSStorageType, class RHSStorageType, size_t ROWS, size_t COLUMNS>
using BinaryOperatorResultType = typename detail::BinaryOperatorResultType<
	EffectiveStorageType<LHSStorageType>,
	EffectiveStorageType<RHSStorageType>,
	ROWS,
	COLUMNS
	>::Type;

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_STORAGETRAITS_HPP__ */
