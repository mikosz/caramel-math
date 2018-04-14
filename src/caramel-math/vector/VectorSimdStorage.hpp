#ifndef CARAMELMATH_VECTOR_SIMDSTORAGE_HPP__
#define CARAMELMATH_VECTOR_SIMDSTORAGE_HPP__

#include <array>

#include "../detail/helper-type-traits.hpp"
#include "../simd/Float4.hpp"
#include "../setup.hpp"
#include "vectorfwd.hpp"

namespace caramel_math::vector {

template <class ScalarTraitsType, class ErrorHandlerType>
class SimdStorage {
public:

	using ScalarTraits = ScalarTraitsType;

	static_assert(std::is_same_v<typename ScalarTraits::Scalar, float>, "Non-float scalar type provided");

	using Scalar = float;

	using GetReturnType = float;

	using ErrorHandler = ErrorHandlerType;

	static constexpr auto SIZE = 4;

	SimdStorage() = default;

	template <
		class... CompatibleValues,
		typename = std::enable_if_t<caramel_math::detail::AllConvertibleV<Scalar, CompatibleValues...>>
		>
	explicit SimdStorage(CompatibleValues&&... values) noexcept
	{
		static_assert(sizeof...(values) == SIZE, "Bad number of values provided");
		init_(std::forward<CompatibleValues>(values)...);
		to chyba nie tak??
	}

	float get(size_t elementIdx) const noexcept(
		noexcept(ErrorHandler::invalidAccess<float>(elementIdx)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (elementIdx >= SIZE) {
				return ErrorHandler::invalidAccess<float>(elementIdx);
			}
		}
		return data_.xyzw()[elementIdx];
	}

	void set(size_t elementIdx, Scalar scalar) noexcept(
		noexcept(ErrorHandler::invalidAccess<float>(elementIdx)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (elementIdx >= SIZE) {
				ErrorHandler::invalidAccess<float>(elementIdx);
				return;
			}
		}
		auto xyzw = data_.xyzw();
		xyzw[elementIdx] = std::move(scalar);
		data_ = simd::Float4(xyzw);
	}

	simd::Float4 get() const noexcept {
		return data_;
	}

	void set(simd::Float4 value) noexcept {
		data_ = std::move(value);
	}

private:

	simd::Float4 data_;

	template <class... Tail>
	void init_(Tail&&... tail) noexcept {
		const auto rawData = std::array<float, ROWS * COLUMNS>{ std::forward<Tail>(tail)... };
		for (auto columnIdx = 0u; columnIdx < COLUMNS; ++columnIdx) {
			columns_[columnIdx] = simd::Float4({
				rawData[0 * COLUMNS + columnIdx],
				rawData[1 * COLUMNS + columnIdx],
				rawData[2 * COLUMNS + columnIdx],
				rawData[3 * COLUMNS + columnIdx]
				});
		}
	}

};

template <
	class LHSScalarTraitsType,
	class LHSErrorHandlerType,
	class RHSScalarTraitsType,
	class RHSErrorHandlerType
	>
inline auto operator*(
	const Vector<SimdStorage<LHSScalarTraitsType, LHSErrorHandlerType>>& lhs,
	const Vector<SimdStorage<RHSScalarTraitsType, RHSErrorHandlerType>>& rhs
	) noexcept(noexcept(lhs.get(Row(0), Column(0))) && noexcept(rhs.get(Row(0), Column(0)))) // TODO
{
	static_assert(SimdStorage<LHSScalarTraitsType, LHSErrorHandlerType>::COLUMNS == 4);

	auto result = Vector<SimdStorage<LHSScalarTraitsType, LHSErrorHandlerType>>();

	for (auto columnIdx = Column(0); columnIdx.value() < 4; ++columnIdx) {
		const auto rhsColumnXyzw = rhs.get(columnIdx).xyzw();

		auto column = lhs.get(Column(0)) * simd::Float4(rhsColumnXyzw[0]);
		column += lhs.get(Column(1)) * simd::Float4(rhsColumnXyzw[1]);
		column += lhs.get(Column(2)) * simd::Float4(rhsColumnXyzw[2]);
		column += lhs.get(Column(3)) * simd::Float4(rhsColumnXyzw[3]);

		result.set(columnIdx, column);
	}

	return result;
}

} // namespace caramel_math::vector

#endif /* CARAMELMATH_VECTOR_SIMDSTORAGE_HPP__ */
