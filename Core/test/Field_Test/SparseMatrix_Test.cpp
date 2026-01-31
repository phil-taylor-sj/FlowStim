#include "../pch.h"

#include <Core/Field/SparseMatrix.hpp>
#include <Core/Domain/Mesh.h>
#include <Core/Domain/Mesh2dStructuredFactory.h>

#include <VecPlus/Vec2.h>

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

    class SparseMatrix_Initialise2dMesh_Fixture : public ::testing::TestWithParam<std::pair<int, int>>
    {
    protected:
        void SetUp() override
        {
            this->meshSize = vecp::Vec2i(
                std::get<0>(GetParam()),
                std::get<1>(GetParam())
            );
 
            Mesh2dStructuredFactory meshFactory {};
            this->mesh = meshFactory(meshSize, vecp::Vec2d(1., 1.));
            this->nCells = this->mesh->nCells;
            this->matrix = std::make_unique<SparseMatrix2d>(this->nCells);
        }
        std::unique_ptr<SparseMatrix2d> matrix;
        std::unique_ptr<Mesh2d> mesh;
        vecp::Vec2i meshSize;
        int nCells;
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

    class SparseMatrix_ClearValues_F : public SparseMatrix_Fixture {};
    TEST_P(SparseMatrix_ClearValues_F, SparseMatirx_ClearValues)
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
        matrix->clear();
        for (int rowId = 0; rowId < nRows; rowId++)
        {
            auto expectedIds = std::views::keys(inputMatrix[rowId]);
            auto columnIds = matrix->getColumnIds(rowId);

           ASSERT_EQ(columnIds.size(), std::ranges::size(columnIds));
            for (auto id : expectedIds)
            {
                ASSERT_TRUE(columnIds.contains(id));
                ASSERT_EQ(0., (*matrix)(rowId, id));
            }
        }
    }

    INSTANTIATE_TEST_SUITE_P(SparseMatrix_ClearValues, 
        SparseMatrix_ClearValues_F, 
        testing::Values(10, 100, 1000, 10000));

    class SparseMatrix_Initialise2dDomain_F : public SparseMatrix_Initialise2dMesh_Fixture {};
    TEST_P(SparseMatrix_Initialise2dDomain_F, SparseMatirx_Initialise2dDomain)
    {
        matrix->initialiseForMesh(*mesh);

        for (int cellId = 0; cellId < nCells; cellId++)
        {
            auto columnIds = matrix->getColumnIds(cellId);          
            
            std::vector<int> expectedIds = mesh->cells[cellId].faceId
                | std::views::filter([&](int faceId) { 
                    return mesh->faces[faceId].neighId >= 0; })
                | std::views::transform([&](int faceId) { 
                    const Face2d& face = mesh->faces[faceId];
                    return (face.ownerId == cellId) ? face.neighId : face.ownerId;})
                | std::ranges::to<std::vector>();

            expectedIds.emplace_back(cellId);
            ASSERT_EQ(expectedIds.size(), columnIds.size());
            for(auto id : expectedIds)
            {
                ASSERT_TRUE(std::ranges::contains(columnIds, id));
            }
        }
    }

    INSTANTIATE_TEST_SUITE_P(SparseMatrix_Initialise2dDomain, 
        SparseMatrix_Initialise2dDomain_F, testing::Values(
            std::make_pair<int, int>(3, 4)
        ));

}