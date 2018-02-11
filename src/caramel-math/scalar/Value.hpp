#pragma once

#include <iosfwd>

namespace caramel_math::scalar {

template <class Subclass, class ScalarType>
class Value {
public:

	constexpr Value() :
		value_(ScalarType(0))
	{
	}

	constexpr explicit Value(ScalarType value) :
		value_(std::move(value))
	{
	}

	constexpr ScalarType value() const noexcept {
		return value_;
	}

	constexpr friend bool operator==(const Subclass& lhs, const Subclass& rhs) noexcept {
		return lhs.value() == rhs.value();
	}

	constexpr friend bool operator!=(const Subclass& lhs, const Subclass& rhs) noexcept {
		return lhs.value() != rhs.value();
	}

	constexpr friend bool operator<(const Subclass& lhs, const Subclass& rhs) noexcept {
		return lhs.value() < rhs.value();
	}

	constexpr friend bool operator<=(const Subclass& lhs, const Subclass& rhs) noexcept {
		return lhs.value() <= rhs.value();
	}

	constexpr friend bool operator>(const Subclass& lhs, const Subclass& rhs) noexcept {
		return lhs.value() > rhs.value();
	}

	constexpr friend bool operator>=(const Subclass& lhs, const Subclass& rhs) noexcept {
		return lhs.value() >= rhs.value();
	}

	constexpr friend Subclass operator+(const Subclass& lhs, const Subclass& rhs) noexcept {
		return Subclass(lhs.value() + rhs.value());
	}

	friend Subclass& operator+=(Subclass& lhs, const Subclass& rhs) noexcept {
		lhs = Subclass(lhs.value() + rhs.value());
		return lhs;
	}

	constexpr friend Subclass operator-(const Subclass& lhs, const Subclass& rhs) noexcept {
		return Subclass(lhs.value() - rhs.value());
	}

	friend Subclass& operator-=(Subclass& lhs, const Subclass& rhs) noexcept {
		lhs = Subclass(lhs.value() - rhs.value());
		return lhs;
	}

	constexpr friend Subclass operator*(const Subclass& lhs, const Subclass& rhs) noexcept {
		return Subclass(lhs.value() * rhs.value());
	}

	friend Subclass& operator*=(Subclass& lhs, const Subclass& rhs) noexcept {
		lhs = Subclass(lhs.value() * rhs.value());
		return lhs;
	}

	constexpr friend Subclass operator/(const Subclass& lhs, const Subclass& rhs) noexcept {
		return Subclass(lhs.value() / rhs.value());
	}

	friend Subclass& operator/=(Subclass& lhs, const Subclass& rhs) noexcept {
		lhs = Subclass(lhs.value() / rhs.value());
		return lhs;
	}

	friend std::ostream& operator<<(std::ostream& os, const Subclass& s) noexcept {
		return os << typeid(Subclass).name() << "(" << s.value() << ")";
	}

private:

	ScalarType value_;

};

} // namespace caramel_math::scalar
