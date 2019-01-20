#ifndef CARAMELMATH_VECTOR_VECTOR_IMPLEMENTATION_HPP__
#define CARAMELMATH_VECTOR_VECTOR_IMPLEMENTATION_HPP__

#include <iosfwd>
#include <optional>

#include "Vector.template.hpp"

namespace caramel_math::vector {

// -- const variants initialisation

namespace detail {

template <class StorageType>
Vector<StorageType> zeroVector() {
	auto zero = Vector<StorageType>();
	for (auto rowIdx = Row(0); rowIdx.value() < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = Column(0); columnIdx.value() < StorageType::COLUMNS; ++columnIdx) {
			zero.set(rowIdx, columnIdx, StorageType::ScalarTraits::ZERO);
		}
	}
	return zero;
}

} // namespace detail

template <class StorageType>
Vector<StorageType> Vector<StorageType>::ZERO = detail::zeroVector<StorageType>();

// -- operators

template <class LHSStorageType, class RHSStorageType>
inline [[nodiscard]] bool operator==(
	const Vector<LHSStorageType>& lhs,
	const Vector<RHSStorageType>& rhs
	)
	noexcept(noexcept(lhs.get(0)) && noexcept(rhs.get(0)))
{
	static_assert(
		LHSStorageType::SIZE == RHSStorageType::SIZE,
		"Incompatible vector sizes for equality test"
	);

	for (auto elementIdx = 0; elementIdx < LHSStorageType::SIZE; ++elementIdx) {
		using ScalarTraits = typename LHSStorageType::ScalarTraits;
		if (!ScalarTraits::equal(lhs.get(elementIdx), rhs.get(elementIdx))) {
			return false;
		}
	}

	return true;
}

template <class LHSStorageType, class RHSStorageType>
inline [[nodiscard]] bool operator!=(
	const Vector<LHSStorageType>& lhs,
	const Vector<RHSStorageType>& rhs
	)
	noexcept(noexcept(lhs == rhs))
{
	return !(lhs == rhs);
}

template <class StorageType>
inline Vector<StorageType>& operator*=(Vector<StorageType>& vector, typename StorageType::Scalar scalar)
	noexcept(noexcept(Vector.get(0)))
{
	for (auto elementIdx = 0; elementIdx < StorageType::SIZE; ++elementIdx) {
		vector.set(elementIdx, vector.get(elementIdx) * scalar);
	}

	return vector;
}

template <class StorageType>
inline [[nodiscard]] Vector<StorageType> operator*(const Vector<StorageType>& vector, typename StorageType::Scalar scalar)
	noexcept(noexcept(std::declval<Vector<StorageType>&>() *= scalar))
{
	auto result = vector;
	result *= scalar;
	return result;
}

template <class StorageType>
inline [[nodiscard]] Vector<StorageType> operator*(typename StorageType::Scalar scalar, const Vector<StorageType>& vector)
	noexcept(noexcept(std::declval<Vector<StorageType>&>() *= scalar))
{
	auto result = vector;
	result *= scalar;
	return result;
}

template <class StorageType>
inline Vector<StorageType>& operator/=(Vector<StorageType>& vector, typename StorageType::Scalar scalar) noexcept(
	noexcept(Vector.get(Row(0), Column(0))))
{
	for (auto elementIdx = 0; elementIdx < StorageType::SIZE; ++elementIdx) {
		vector.set(elementIdx, vector.get(elementIdx) / scalar);
	}

	return vector;
}

template <class StorageType>
inline [[nodiscard]] Vector<StorageType> operator/(const Vector<StorageType>& vector, typename StorageType::Scalar scalar)
	noexcept(noexcept(std::declval<Vector<StorageType>&>() /= scalar))
{
	auto result = vector;
	result /= scalar;
	return result;
}

template <class StorageType>
inline [[nodiscard]] Vector<StorageType> operator/(typename StorageType::Scalar scalar, const Vector<StorageType>& vector)
	noexcept(noexcept(std::declval<Vector<StorageType>&>() /= scalar))
{
	auto result = vector;
	result /= scalar;
	return result;
}

template <class StorageType>
inline std::ostream& operator<<(std::ostream& os, const Vector<StorageType>& vector) noexcept {
	os << "{ ";
	
	for (auto elementIdx = 0; elementIdx.value() < vector<StorageType>::SIZE; ++elementIdx) {
		os << vector.get(elementIdx);

		if (elementIdx.value() != vector<StorageType>::ROWS - 1) {
			os << ", ";
		}
	}

	os << " }";

	return os;
}

// -- free functions



} // namespace caramel_math::vector

#endif /* CARAMELMATH_VECTOR_VECTOR_IMPLEMENTATION_HPP__ */
