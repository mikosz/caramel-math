#pragma once

#include <iosfwd>

namespace caramel_math::scalar {

template <class Subclass, class ScalarType>
class Value {
public:

	constexpr Value() = default;

	constexpr explicit Value(ScalarType value) :
		value_(std::move(value))
	{
	}

	constexpr ScalarType value() const noexcept {
		return value_;
	}

	constexpr friend bool operator==(Subclass lhs, Subclass rhs) noexcept {
		return lhs.value() == rhs.value();
	}

	constexpr friend bool operator!=(Subclass lhs, Subclass rhs) noexcept {
		return lhs.value() != rhs.value();
	}

	constexpr friend bool operator<(Subclass lhs, Subclass rhs) noexcept {
		return lhs.value() < rhs.value();
	}

	constexpr friend bool operator<=(Subclass lhs, Subclass rhs) noexcept {
		return lhs.value() <= rhs.value();
	}

	constexpr friend bool operator>(Subclass lhs, Subclass rhs) noexcept {
		return lhs.value() > rhs.value();
	}

	constexpr friend bool operator>=(Subclass lhs, Subclass rhs) noexcept {
		return lhs.value() >= rhs.value();
	}

	constexpr friend Subclass operator+(Subclass lhs, Subclass rhs) noexcept {
		return Subclass(lhs.value() + rhs.value());
	}

	friend Subclass& operator+=(Subclass& lhs, Subclass rhs) noexcept {
		lhs = Subclass(lhs.value() + rhs.value());
		return lhs;
	}

	friend Subclass& operator++(Subclass& v) noexcept {
		v = Subclass(v.value() + 1);
		return v;
	}

	friend Subclass operator++(Subclass& v, int) noexcept {
		const auto currentValue = v.value();
		v = Subclass(currentValue + 1);
		return Subclass(currentValue);
	}

	friend Subclass& operator--(Subclass& v) noexcept {
		v = Subclass(v.value() - 1);
		return v;
	}

	friend Subclass operator--(Subclass& v, int) noexcept {
		const auto currentValue = v.value();
		v = Subclass(currentValue - 1);
		return Subclass(currentValue);
	}

	constexpr friend Subclass operator-(Subclass lhs, Subclass rhs) noexcept {
		return Subclass(lhs.value() - rhs.value());
	}

	friend Subclass& operator-=(Subclass& lhs, Subclass rhs) noexcept {
		lhs = Subclass(lhs.value() - rhs.value());
		return lhs;
	}

	constexpr friend Subclass operator*(ScalarType lhs, Subclass rhs) noexcept {
		return Subclass(lhs * rhs.value());
	}

	constexpr friend Subclass operator*(Subclass lhs, ScalarType rhs) noexcept {
		return Subclass(lhs.value() * rhs);
	}

	friend Subclass& operator*=(Subclass& lhs, ScalarType rhs) noexcept {
		lhs = Subclass(lhs.value() * rhs);
		return lhs;
	}

	constexpr friend Subclass operator/(Subclass lhs, ScalarType rhs) noexcept {
		return Subclass(lhs.value() / rhs);
	}

	friend Subclass& operator/=(Subclass& lhs, ScalarType rhs) noexcept {
		lhs = Subclass(lhs.value() / rhs);
		return lhs;
	}

	constexpr friend Subclass operator%(Subclass lhs, ScalarType rhs) noexcept {
		return Subclass(lhs.value() % rhs);
	}

	friend Subclass& operator%=(Subclass& lhs, ScalarType rhs) noexcept {
		lhs = Subclass(lhs.value() % rhs);
		return lhs;
	}

	friend std::ostream& operator<<(std::ostream& os, Subclass s) noexcept {
		return os << typeid(Subclass).name() << "(" << s.value() << ")";
	}

private:

	ScalarType value_;

};

} // namespace caramel_math::scalar
