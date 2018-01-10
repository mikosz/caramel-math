#ifndef CARAMELMATH_SIMD_FLOAT4_HPP__
#define CARAMELMATH_SIMD_FLOAT4_HPP__

#include <array>

#include "detail/VisualCppFloat4.hpp"

namespace caramel_math::simd {

class Float4 {
public:

	Float4() = default;

	Float4(float xyzw[4]) noexcept {
		detail::load(data_, xyzw);
	}

	std::array<float, 4> xyzw() const noexcept {
		auto data = std::array<float, 4>();
		detail::store(data.data(), data_);
		return std::move(data);
	}

	friend Float4 operator+(const Float4& lhs, const Float4& rhs) noexcept {
		auto result = Float4();
		result.data_ = detail::add(lhs, rhs);
		return result;
	}

	Float4& operator+=(const Float4& other) noexcept {
		data_ = (*this + other).data_;
		return *this;
	}

private:

	detail::Float4 data_;

};

} // namespace caramel_math::simd

#endif /* CARAMELMATH_SIMD_FLOAT4_HPP__ */
