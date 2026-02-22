#pragma once

#include <Core/Domain/Mesh.h>

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>

#include <memory>
#include <map>
#include <set>
#include <ranges>
#include <span>
#include <vector>
#include <format>
#include <exception>
#include <iostream>

namespace fstim
{
	template <typename T>
	struct SparseRow
	{
		std::span<const std::size_t> columnIds;
		std::span<T> values;
		T& diagValue;

		SparseRow(T& diagIn) : diagValue(diagIn) {};
	};

	template <typename T>
	class SparseMatrix
	{
	public:
		const std::size_t nRows;

		inline T& operator()(std::size_t rowId, std::size_t columnId)
		{
			auto rowBegin = m_columnIds.begin() + m_rowPositions[rowId];
			auto rowEnd = m_columnIds.begin() + m_rowPositions[rowId + 1];
			auto location = std::find( rowBegin, rowEnd, columnId);

			if (location == rowEnd)
				return this->m_retrieveNewColumnValue(rowId, columnId);

			std::size_t idx = static_cast<std::size_t>(location - m_columnIds.begin());
			return m_values[idx];
			//return m_values[*location];
		}

		inline const T& operator()(std::size_t rowId, std::size_t columnId) const
		{
			auto rowBegin = m_columnIds.begin() + m_rowPositions[rowId];
			auto rowEnd = m_columnIds.begin() + m_rowPositions[rowId + 1];
			auto location =  std::find( rowBegin, rowEnd, columnId);
			if (location == rowEnd)
				throw std::exception();

			std::size_t idx = static_cast<std::size_t>(location - m_columnIds.begin());
			return m_values[idx];
			//return m_values[*location];
		}

		inline std::span<const std::size_t> getColumnIds(std::size_t rowId) const
		{
			std::size_t rowBegin = this->m_rowPositions[rowId];
			std::size_t rowEnd = this->m_rowPositions[rowId + 1];
			return std::span<const std::size_t>(
				this->m_columnIds.data() + rowBegin,
				rowEnd - rowBegin
			);
		}

		inline SparseRow<T> getRow(std::size_t rowId)
		{
			std::size_t rowBegin = this->m_rowPositions[rowId] + 1;
			std::size_t length = this->m_rowPositions[rowId + 1] - rowBegin;
			
			SparseRow<T> row(this->m_values[rowBegin - 1]);
			row.columnIds = std::span<const std::size_t>(
				this->m_columnIds.data() + rowBegin, length);
			row.values = std::span<T>(
				this->m_values.data() + rowBegin, length);
			return row;
		}

		inline SparseRow<const T> getRow(std::size_t rowId) const
		{
			std::size_t rowBegin = this->m_rowPositions[rowId] + 1;
			std::size_t length = this->m_rowPositions[rowId + 1] - rowBegin;
			
			SparseRow<const T> row(this->m_values[rowBegin - 1]);
			row.columnIds = std::span<const std::size_t>(
				this->m_columnIds.data() + rowBegin, length);
			row.values = std::span<const T>(
				this->m_values.data() + rowBegin, length);
			return row;
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
			this->freeze();
		}

		inline void clear()
		{
			std::ranges::fill(this->m_values, T());
			for (std::size_t rowId = 0; rowId < this->nRows; rowId++) 
			{
				this->m_newValues[rowId].clear();
			}
			this->m_numNewValues = 0;
		}

		inline void freeze()
		{
			if (this->m_numNewValues == 0)
				return;

			std::size_t newSize = this->m_columnIds.size() + this->m_numNewValues;
			
			std::unique_ptr<std::size_t[]> newRowPositions = std::make_unique<std::size_t[]>(nRows + 1);
			std::vector<T> newValues(newSize);
			std::vector<std::size_t> newColumnIds(newSize);

			std::size_t currentLocation = 0;
			auto newValuesItr = newValues.begin();
			auto newColumnItr = newColumnIds.begin();
			auto valuesItr = this->m_values.begin();
			auto columnsItr = this->m_columnIds.begin();
			
			for (std::size_t rowId = 0; rowId < this->nRows; rowId++)
			{
				std::size_t rowStart = this->m_rowPositions[rowId];
				std::size_t rowEnd = this->m_rowPositions[rowId + 1];

				newRowPositions[rowId] = currentLocation;

				for (std::size_t index = rowStart; index < rowEnd; index++)
				{
					*newColumnItr++ = *columnsItr++;
					*newValuesItr++ = *valuesItr++;
					currentLocation++;
				}

				for (auto& [newId, newValue] : this->m_newValues[rowId])
				{
					*newColumnItr++ = newId;
					*newValuesItr++ = newValue;
					currentLocation++;
				}

				this->m_newValues[rowId].clear();
			}

			newRowPositions[nRows] = currentLocation;
			this->m_numNewValues = 0;

			this->m_rowPositions.swap(newRowPositions);
			this->m_columnIds.swap(newColumnIds);
			this->m_values.swap(newValues);
			
		}


		SparseMatrix(std::size_t nRowsIn) :
			nRows(nRowsIn),
			m_rowPositions(std::make_unique<std::size_t[]>(nRowsIn + 1)),
			m_newValues(std::make_unique<std::vector<std::pair<std::size_t, T>>[]>(nRowsIn))
		{
			this->m_columnIds.resize(nRowsIn);
			this->m_values.resize(nRowsIn);
			for (int rowId = 0; rowId < nRowsIn; rowId++)
			{
				m_rowPositions[rowId] = rowId;
				m_columnIds[rowId] = rowId;
				m_values[rowId] = 0.;
			}
			m_rowPositions[nRowsIn] = nRowsIn;
		}

		~SparseMatrix() {}

	private:
		std::unique_ptr<std::size_t[]> m_rowPositions;
		std::vector<std::size_t> m_columnIds;
		std::vector<T> m_values;
		std::size_t m_numNewValues = 0;

		std::unique_ptr<std::vector<std::pair<std::size_t, T>>[]> 
			m_newValues; 

		inline T& m_retrieveNewColumnValue(std::size_t rowId, 
			std::size_t columnId)
		{
			auto& row = this->m_newValues[rowId];
			auto currentValue = std::ranges::find(
				row, columnId,
				&std::pair<std::size_t, T>::first 
			);
			if (currentValue != row.end())
				return std::get<1>(*currentValue);
			row.push_back(std::make_pair<>(columnId, T()));
			this->m_numNewValues++;
			return std::get<1>(row.back());
		}

	};

	using SparseMatrix2d = SparseMatrix<vecp::Vec2d>;
	using SparseMatrix3d = SparseMatrix<vecp::Vec3d>;
	using SparseMatrixScalar = SparseMatrix<double>;
}