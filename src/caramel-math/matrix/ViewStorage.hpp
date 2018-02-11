#ifndef CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__
#define CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__

#include <tuple>

#include "../setup.hpp"
#include "Matrix.hpp"

namespace caramel_math::matrix {

template <
	class ViewedMatrixType,
	class ModifierFuncType
	>
class ViewStorage {
public:

	using ViewedMatrix = ViewedMatrixType;

	using ModifierFunc = ModifierFuncType;

	using Scalar = typename ViewedMatrix::Scalar;

	using ErrorHandler = typename ViewedMatrix::Storage::ErrorHandler;

	static constexpr auto ROWS = ModifierFuncType::rows(ViewedMatrix::ROWS, ViewedMatrix::COLUMNS);

	static constexpr auto COLUMNS = ModifierFuncType::columns(ViewedMatrix::ROWS, ViewedMatrix::COLUMNS);

	ViewStorage(ViewedMatrix& viewedMatrix, ModifierFunc modifierFunc = ModifierFunc()) :
		viewedMatrix_(viewedMatrix),
		modifierFunc_(std::move(modifierFunc))
	{
	}

	decltype(auto) get(size_t row, size_t column) const noexcept(
		noexcept(viewedMatrix_.get(row, column)))
	{
		const auto modifiedCoords = modifierFunc_(row, column);
		return viewedMatrix_.get(std::get<0>(modifiedCoords), std::get<1>(modifiedCoords));
	}

	void set(size_t row, size_t column, Scalar scalar) noexcept(
		noexcept(viewedMatrix_.set(row, column, scalar)))
	{
		auto modifiedRow = size_t();
		auto modifiedColumn = size_t();
		std::tie(modifiedRow, modifiedColumn) = modifierFunc_(row, column);
		return viewedMatrix_.set(modifiedRow, modifiedColumn, std::move(scalar));
	}

	ViewedMatrix& viewedMatrix() noexcept {
		return viewedMatrix_;
	}

	const ViewedMatrix& viewedMatrix() const noexcept {
		return viewedMatrix_;
	}

private:

	ViewedMatrix& viewedMatrix_;

	ModifierFunc modifierFunc_;

};

namespace detail {

struct TransposedModifierFunc {

	static constexpr size_t rows([[maybe_unused]] size_t rows, size_t columns) noexcept {
		return columns;
	}

	static constexpr size_t columns(size_t rows, [[maybe_unused]] size_t columns) noexcept {
		return rows;
	}

	std::tuple<size_t, size_t> operator()(size_t row, size_t column) const noexcept {
		return { column, row };
	}

};

class SubmatrixModifierFunc {
public:

	static constexpr size_t rows(size_t rows, [[maybe_unused]] size_t columns) noexcept {
		return rows - 1;
	}

	static constexpr size_t columns([[maybe_unused]] size_t rows, size_t columns) noexcept {
		return columns - 1;
	}

	SubmatrixModifierFunc(size_t excludedRow, size_t excludedColumn) :
		excludedRow_(excludedRow),
		excludedColumn_(excludedColumn)
	{
	}

	std::tuple<size_t, size_t> operator()(size_t row, size_t column) const noexcept {
		return
			{
				((row < excludedRow_) ? row : row + 1),
				((column < excludedColumn_) ? column : column + 1)
			}
			;
	}

private:

	size_t excludedRow_;

	size_t excludedColumn_;

};

} // namespace detail

template <class ViewedMatrixType>
using TransposedViewStorage = ViewStorage<ViewedMatrixType, detail::TransposedModifierFunc>;

template <class ViewedMatrixType>
inline auto viewTransposed(ViewedMatrixType& matrix) noexcept {
	return Matrix<TransposedViewStorage<ViewedMatrixType>>(matrix);
}

template <class ViewedMatrixType>
using SubmatrixViewStorage = ViewStorage<ViewedMatrixType, detail::SubmatrixModifierFunc>;

template <class ViewedMatrixType>
inline auto viewSubmatrix(ViewedMatrixType& matrix, size_t excludedRow, size_t excludedColumn) noexcept {
	if constexpr (RUNTIME_CHECKS) {
		if (excludedRow >= ViewedMatrixType::ROWS || excludedColumn >= ViewedMatrixType::COLUMNS) {
			using ErrorHandler = typename ViewedMatrixType::Storage::ErrorHandler;
			ErrorHandler::invalidAccess<typename ViewedMatrixType::Scalar>(
				excludedRow,
				excludedColumn
				);
		}
	}

	return Matrix<SubmatrixViewStorage<ViewedMatrixType>>(
		matrix,
		detail::SubmatrixModifierFunc(excludedRow, excludedColumn)
		);
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__ */
