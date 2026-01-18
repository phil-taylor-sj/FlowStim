#include "../pch.h"

#include <Core/Field/SparseMatrix.hpp>

#include <memory>
#include <tuple>

using namespace fstim;

namespace SparseMatrix_Tests
{
	class SparseMatrix_Fixture : public ::testing::TestWithParam<int>
	{
	protected:
		void SetUp() override
		{
			int nCells = GetParam();
			matrix = std::make_unique<SparseMatrix<vecp::Vec2d>>(nCells);
			inputMatrix = std::make_unique<std::map<int, double>[]>(nCells);
            for (int cellId = 0; cellId < nCells; cellId++)
            {
                // Sample Coefficents
                // [10., 0., -1., 0., -1., 0., 0. ...]
                // [0., 11., 0., -1., 0., -1., 0. ...]
                // [-1., 0., 12., 0., -1., 0., -1. ...]
                // [-1., 0., -1., 13., 0., -1., 0. ...]
                // [-1., 0., -1., 0., 14., 0., -1. ...]
                // ....
                for (int offset : std::views::iota(-4, 5) | std::views::stride(2))
                {
                    int offIndex = cellId + offset;
                    if (offIndex < 0. || offIndex >= nCells) continue;

                    double newValue = (offset == 0) ? (double)cellId + 10. : -1.;
                    (*matrix)(cellId, cellId) = newValue;
                    inputMatrix[cellId][cellId] = newValue; 
                }
            }
        }
		std::unique_ptr<SparseMatrix<vecp::Vec2d>> matrix;
		std::unique_ptr<std::map<int, double>[]>  inputMatrix;
	};

}