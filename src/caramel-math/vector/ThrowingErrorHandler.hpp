#ifndef CARAMELMATH_VECTOR_THROWINGERRORHANDLER_HPP__
#define CARAMELMATH_VECTOR_THROWINGERRORHANDLER_HPP__

#include <string>
#include <stdexcept>

namespace caramel_math::vector {

class InvalidVectorDataAccess final : public std::logic_error {
public:

	InvalidVectorDataAccess(size_t elementIdx) noexcept :
		std::logic_error("Invalid vector data access at index " + std::to_string(elementIdx)),
		elementIdx_(elementIdx)
	{
	}

	size_t elementIdx() const noexcept {
		return elementIdx_;
	}

private:

	size_t elementIdx_;

};

struct ThrowingErrorHandler final {

	template <class ReturnType>
	static ReturnType invalidAccess(size_t elementIdx) {
		throw InvalidVectorDataAccess(elementIdx);
	}

};

} // namespace caramel_math::vector

#endif /* CARAMELMATH_VECTOR_THROWINGERRORHANDLER_HPP__ */
