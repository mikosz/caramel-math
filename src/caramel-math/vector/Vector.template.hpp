#ifndef CARAMELMATH_VECTOR_VECTOR_TEMPLATE_HPP__
#define CARAMELMATH_VECTOR_VECTOR_TEMPLATE_HPP__

#include "storage-traits.hpp"
#include "vectorfwd.hpp"

namespace caramel_math::vector {

template <class StorageType>
class Vector final : StorageType {
public:

	using Storage = StorageType;

	using typename StorageType::Scalar;

	using StorageType::SIZE;

	static Vector ZERO;

	Vector() = default;
	Vector(const Vector& other) = default;
	Vector(Vector&& other) = default;
	Vector& operator=(const Vector& other) = default;
	Vector& operator=(Vector&& other) = default;

	using StorageType::StorageType;

	// Conversion from compatible vector types
	// TODO: this should probably be implemented in storage also (not in here)
	template <class OtherStorageType>
	explicit Vector(const Vector<OtherStorageType>& other) {
		*this = other;
	}

	template <class OtherStorageType>
	Vector& operator=(const Vector<OtherStorageType>& other) {
		static_assert(SIZE == Vector<OtherStorageType>::SIZE);

		for (auto elementIdx = 0u; elementIdx < SIZE; ++elementIdx) {
			set(elementIdx, other.get(elementIdx));
		}
		
		return *this;
	}

	using StorageType::get;

	typename StorageType::GetReturnType get(size_t elementIdx) const
		noexcept(noexcept(get(elementIdx)))
	{
		return get(elementIdx);
	}

	using StorageType::set;

	void set(size_t elementIidx, Scalar scalar) noexcept(noexcept(set(elementIdx, scalar))) {
		return set(elementIdx, std::move(scalar));
	}

	const Storage& storage() const noexcept {
		return static_cast<const StorageType&>(*this);
	}

};

} // namespace caramel_math::vector

#endif /* CARAMELMATH_VECTOR_VECTOR_TEMPLATE_HPP__ */
