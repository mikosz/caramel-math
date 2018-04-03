#ifndef CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__
#define CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__

#include <cassert>
#include <type_traits>

#include "matrix-coordinates.hpp"

namespace caramel_math::matrix {

struct AssertErrorHandler final {
	
	template <class ReturnType>
	static ReturnType invalidAccess([[maybe_unused]] Row row, [[maybe_unused]] Column column) noexcept {
		assert(!"Invalid matrix data access");
		
		static auto returnValue = std::decay_t<ReturnType>();
		return returnValue;
	}

	template <class ScalarType>
	static void invalidValue(
		[[maybe_unused]] Row row,
		[[maybe_unused]] Column column,
		[[maybe_unused]] ScalarType actualValue,
		[[maybe_unused]] ScalarType expectedValue
		) noexcept
	{
		assert(!"Invalid matrix data value");
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__ */
