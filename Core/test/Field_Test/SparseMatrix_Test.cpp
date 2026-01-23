#include "../pch.h"

#include <Core/Field/SparseMatrix.hpp>

#include <memory>
#include <tuple>
#include <ranges>
#include <map>

using namespace fstim;

namespace SparseMatrix_Tests
{
    // Sample Coefficents
    // [10., 0., 0.2, 0., 0.4., 0., 0. ...]
    // [0., 11., 0., 0.2, 0., 0.4, 0. ...]
    // [-0.2, 0., 12., 0., 0.2, 0., 0.4 ...]
    // [0., -0.2, 0., 13., 0., 0.2, 0. ...]
    // [-0.4, 0., -0.2, 0., 14., 0., 0.2 ...]
    // ....

	class SparseMatrix_Fixture : public ::testing::TestWithParam<int>
	{
	protected:
		void SetUp() override
		{
			nRows = GetParam();
			matrix = std::make_unique<SparseMatrix<double>>(nRows);
			inputMatrix = std::make_unique<std::map<int, double>[]>(nRows);
            for (int rowId = 0; rowId < nRows; rowId++)
            {
                for (int offset = -4; offset <= 4; offset++)
                {
                    int offIndex = rowId + offset;
                    if (offIndex < 0. || offIndex >= nRows) continue;

                    double newValue = (offset == 0) 
                        ? (double)rowId + 10. 
                        : (double)offset / 10.;
                    (*matrix)(rowId, rowId) = newValue;
                    inputMatrix[rowId][rowId] = newValue; 
                }
            }
        }
		std::unique_ptr<SparseMatrix<double>> matrix;
		std::unique_ptr<std::map<int, double>[]>  inputMatrix;
        int nRows; 
	};

    class SparseMatrix_CorrectKeys_F : public SparseMatrix_Fixture {};
    TEST_P(SparseMatrix_CorrectKeys_F, SparseMatirx_AssignsCorrectKeys)
    {
        for (std::size_t rowId = 0; rowId < nRows; rowId++)
        {
            auto expectedIds = std::views::keys(inputMatrix[rowId]);
            auto storedIds = matrix->getColumnIds(rowId);
            ASSERT_EQ(storedIds.size(), std::ranges::size(storedIds));
            for (auto id : expectedIds)
            {
                ASSERT_TRUE(storedIds.contains(id));
            }
        }
    }

    INSTANTIATE_TEST_SUITE_P(SparseMatrix_AssignCorrectKeys, 
        SparseMatrix_CorrectKeys_F, 
        testing::Values(10, 100, 1000, 10000));

    class SparseMatrix_CorrectValues_F : public SparseMatrix_Fixture {};
    TEST_P(SparseMatrix_CorrectValues_F, SparseMatirx_AssignsCorrectValues)
    {
        for (std::size_t rowId = 0; rowId < nRows; rowId++)
        {
            for (auto [columnId, expectedValue] : inputMatrix[rowId])
            {
                ASSERT_EQ(expectedValue, (*matrix)(rowId, columnId));
            }
        }
    }

    INSTANTIATE_TEST_SUITE_P(SparseMatrix_AssignCorrectValues, 
        SparseMatrix_CorrectValues_F, 
        testing::Values(10, 100, 1000, 10000));
    
    class SparseMatrix_AddToValues_F : public SparseMatrix_Fixture {};
    TEST_P(SparseMatrix_AddToValues_F, SparseMatirx_AddToValues)
    {
        for (int rowId = 0; rowId < nRows; rowId++)
        {
            for (auto [columnId, oldValue] : inputMatrix[rowId])
            {
                double offset = 50.;
                double expectedValue = oldValue + offset;
                (*matrix)(rowId, columnId) += offset;
                ASSERT_EQ(expectedValue, (*matrix)(rowId, columnId));
            }
        }
    }

    INSTANTIATE_TEST_SUITE_P(SparseMatrix_AddToValues, 
        SparseMatrix_AddToValues_F, 
        testing::Values(10, 100, 1000, 10000));

    class SparseMatrix_OverwriteValues_F : public SparseMatrix_Fixture {};
    TEST_P(SparseMatrix_OverwriteValues_F, SparseMatirx_OverwriteValues)
    {
        for (int rowId = 0; rowId < nRows; rowId++)
        {
            for (auto [columnId, oldValue] : inputMatrix[rowId])
            {
                double newValue = 50.;
                (*matrix)(rowId, columnId) = newValue;
                ASSERT_EQ(newValue, (*matrix)(rowId, columnId));
            }
        }
    }

    INSTANTIATE_TEST_SUITE_P(SparseMatrix_OverwriteValues, 
        SparseMatrix_OverwriteValues_F, 
        testing::Values(10, 100, 1000, 10000));

    class SparseMatrix_Initialise2dDomain_F : public SparseMatrix_Fixture {};
    TEST_P(SparseMatrix_Initialise2dDomain_F, SparseMatirx_Initialise2dDomain)
    {

    }

    INSTANTIATE_TEST_SUITE_P(SparseMatrix_Initialise2dDomain, 
        SparseMatrix_CorrectValues_F, 
        testing::Values(10, 100, 1000, 10000));

}