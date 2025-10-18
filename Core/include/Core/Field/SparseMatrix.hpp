#pragma once

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>


#include <memory>
#include <map>

namespace fstim
{
	template <typename T>
	class SparseMatrix
	{
	public:
		const std::size_t nRows;

		inline T& operator()(std::size_t rowId, std::size_t columnId)
		{
			return this->m_coeffs[rowId][columnId];
		}

		inline T operator()(std::size_t rowId, std::size_t columnId) const
		{
			return this->m_coeffs[rowId][columnId];
		}

		SparseMatrix(std::size_t nRowsIn) :
			nRows(nRowsIn),
			m_coeffs(std::make_unique<std::map<std::size_t, T>[]>(nRowsIn)) {}

		SparseMatrix() {}

	private:
		std::unique_ptr<std::map<int, T>[]> m_coeffs;
	};

	using SparseMatrix2d = SparseMatrix<vecp::Vec2d>;
	using SparseMatrix3d = SparseMatrix<vecp::Vec3d>;
}