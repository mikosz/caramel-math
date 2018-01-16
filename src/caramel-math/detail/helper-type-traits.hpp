#ifndef CARAMELMATH_DETAIL_HELPERTYPETRAITS_HPP__
#define CARAMELMATH_DETAIL_HELPERTYPETRAITS_HPP__

namespace caramel_math::detail {

template <class To, class... From>
struct AllConvertible {
};

template <class To, class From, class... Tail>
struct AllConvertible<To, From, Tail...> {
	enum { VALUE = std::is_convertible_v<From, To> && AllConvertible<To, Tail...>::VALUE };
};

template <class To>
struct AllConvertible<To> {
	enum { VALUE = true };
};

template <class To, class... From>
constexpr auto AllConvertibleV = AllConvertible<To, From...>::VALUE;

} // namespace caramel_math::detail

#endif /* CARAMELMATH_DETAIL_HELPERTYPETRAITS_HPP__ */
