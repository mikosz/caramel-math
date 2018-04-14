#ifndef CARAMELMATH_VECTOR_ARRAYSTORAGE_HPP__
#define CARAMELMATH_VECTOR_ARRAYSTORAGE_HPP__

#include <array>

#include "../detail/helper-type-traits.hpp"
#include "../setup.hpp"

namespace caramel_math::vector {

template <
	class ScalarTraitsType,
	size_t SIZE_VALUE,
	class ErrorHandlerType
	>
class ArrayStorage {
public:

	using ScalarTraits = ScalarTraitsType;

	using ErrorHandler = ErrorHandlerType;

	using Scalar = typename ScalarTraits::Scalar;

	static constexpr auto SIZE = SIZE_VALUE;

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
		static_assert(sizeof...(values) == SIZE);
	}

	GetReturnType get(size_t elementIdx) const noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(elementIdx)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (elementIdx >= SIZE) {
				return ErrorHandler::invalidAccess<GetReturnType>(elementIdx);
			}
		}
		return data_[elementIdx];
	}

	void set(size_t elementIdx, Scalar scalar) noexcept(
		noexcept(ErrorHandler::invalidAccess<GetReturnType>(elementIdx)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (elementIdx >= SIZE) {
				ErrorHandler::invalidAccess<GetReturnType>(elementIdx);
				return;
			}
		}
		data_[elementIdx] = std::move(scalar);
	}

private:

	std::array<Scalar, SIZE> data_;

};

} // namespace caramel_math::vector

#endif /* CARAMELMATH_VECTOR_ARRAYSTORAGE_HPP__ */
