#pragma once

#include <Core/Domain/Mesh.h>

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>

#include <memory>
#include <map>
#include <set>
#include <ranges>

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

		inline std::set<int> getColumnIds(std::size_t rowId) const
		{
			return std::set<int>(
				std::views::keys(m_coeffs[rowId]).begin(),
				std::views::keys(m_coeffs[rowId]).end()
			);
		}

		inline void initialiseForMesh(const Mesh2d& mesh)
		{
			if (mesh.nCells != this->nRows) return;

			for (int cellId = 0; cellId < mesh.nCells; cellId++)
			{
				(*this)(cellId, cellId) = T();

				const Cell2d& cell = mesh.cells[cellId];
				for (const auto& faceId : cell.faceId)
				{
					const Face2d& face = mesh.faces[faceId];
					if (face.neighId < 0) continue;
					int neighId = (face.ownerId == cellId)
						? face.neighId
						: face.ownerId;
					(*this)(cellId, neighId) = T();
				}
			}
		}

		inline void clear()
		{
			std::for_each(this->m_coeffs.get(), this->m_coeffs.get() + nRows, 
				[](auto& coeffs) {
					std::ranges::for_each(coeffs, [](auto& pair) {pair.second = T();});
				}
			);
		}

		SparseMatrix(std::size_t nRowsIn) :
			nRows(nRowsIn),
			m_coeffs(std::make_unique<std::map<int, T>[]>(nRowsIn)) {}

		~SparseMatrix() {}

	private:
		std::unique_ptr<std::map<int, T>[]> m_coeffs;
	};

	using SparseMatrix2d = SparseMatrix<vecp::Vec2d>;
	using SparseMatrix3d = SparseMatrix<vecp::Vec3d>;
	using SparseMatrixScalar = SparseMatrix<double>;
}