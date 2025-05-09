#include "../pch.h"
#include <algorithm>

#include <Core/Domain/MeshFactory2dStructured.h>
#include <Core/Solver/FaceGradients.h>
#include <Core/Domain/FaceSetFactory.h>

using namespace fstim;

namespace FaceGradients_Tests
{
    class FaceGradients_Fixture : public ::testing::TestWithParam<vecp::Vec2d>
    {
    protected:
        void SetUp() override 
        {
            // Set mesh properties and build mesh.
            vecp::Vec2i size {3, 4};
            cellWidth =  GetParam();
            vecp::Vec2d length = size.toDouble() * cellWidth;
            MeshFactory2dStructured factory = MeshFactory2dStructured();
            mesh = factory(vecp::Vec2i(3, 4), length);

            // Create field with specified cell centre values.
            std::vector<double> test_values = {
                1.2, 2.4, 3.6, 4.8, 5.2, 6.4,
                7.6, 8.8, 9.2, 1.4, 2.6, 3.8
            };
            field = std::make_unique<Field<double>>(mesh->nCells);
            double* values = field->writeValues();
            std::copy(test_values.begin(), test_values.end(), values);
        }
        std::unique_ptr<Mesh2d> mesh = nullptr;
        std::unique_ptr<Field<double>> field = nullptr;
        vecp::Vec2d cellWidth;
    };

    class FaceGradientsZeroGradient_F : public FaceGradients_Fixture {};
    TEST_P(FaceGradientsZeroGradient_F, CorrectFaceGradients)
    {
        //std::set<int> boundaryIds = {
        //    0, 3, 4, 7, 8, 11, 12, 15, 16, 17, 18, 28, 29, 30
        //};
        
        std::vector<vecp::Vec2d> expected = {
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(1.2, 0.0), 
            vecp::Vec2d(1.2, 0.0), vecp::Vec2d(0.0, 0.0),
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(0.4, 0.0),
            vecp::Vec2d(1.2, 0.0), vecp::Vec2d(0.0, 0.0),
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(1.2, 0.0),
            vecp::Vec2d(0.4, 0.0), vecp::Vec2d(0.0, 0.0),
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(1.2, 0.0),
            vecp::Vec2d(1.2, 0.0), vecp::Vec2d(0.0, 0.0),
    
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(0.0, 0.0),
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(0.0, 3.6),
            vecp::Vec2d(0.0, 2.8), vecp::Vec2d(0.0, 2.8),
            vecp::Vec2d(0.0, 2.8), vecp::Vec2d(0.0, 3.6),
            vecp::Vec2d(0.0, 2.8), vecp::Vec2d(0.0, -6.2),
            vecp::Vec2d(0.0, -6.2), vecp::Vec2d(0.0, -5.4),
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(0.0, 0.0),
            vecp::Vec2d(0.0, 0.0)
        };
    
        std::unique_ptr<vecp::Vec2d[]> output = FaceGradients<double>::calculate(*field.get(), *mesh.get());

        for (int id = 0; id < 31; id++)
        {
            ASSERT_NEAR(expected[id].x / cellWidth.x, output[id].x, 1e-06);
            ASSERT_NEAR(expected[id].y / cellWidth.y, output[id].y, 1e-06);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectFaceGradients, FaceGradientsZeroGradient_F, testing::Values(
        vecp::Vec2d(1., 1.), vecp::Vec2d(4., 4.), vecp::Vec2d(0.2, 0.4)
    ));


    class FaceGradientsFixedValueBC_F : public FaceGradients_Fixture {};
    TEST_P(FaceGradientsFixedValueBC_F, CorrectFaceGradientsFixedValueBC)
    {
        std::set<int> boundaryIds = {
            0, 3, 4, 7, 8, 11, 12, 15, 16, 17, 18, 28, 29, 30
        };
        
        std::vector<vecp::Vec2d> expected = {
            vecp::Vec2d(-4.6, 0.0), vecp::Vec2d(1.2, 0.0), 
            vecp::Vec2d(1.2, 0.0), vecp::Vec2d(0.0, 0.0),
            
            vecp::Vec2d(2.6, 0.0), vecp::Vec2d(0.4, 0.0),
            vecp::Vec2d(1.2, 0.0), vecp::Vec2d(0.0, 0.0),
            
            vecp::Vec2d(8.2, 0.0), vecp::Vec2d(1.2, 0.0),
            vecp::Vec2d(0.4, 0.0), vecp::Vec2d(0.0, 0.0),
            
            vecp::Vec2d(-4.2, 0.0), vecp::Vec2d(1.2, 0.0),
            vecp::Vec2d(1.2, 0.0), vecp::Vec2d(0.0, 0.0),
    
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(0.0, 0.0),
            vecp::Vec2d(0.0, 0.0), vecp::Vec2d(0.0, 3.6),
            vecp::Vec2d(0.0, 2.8), vecp::Vec2d(0.0, 2.8),
            vecp::Vec2d(0.0, 2.8), vecp::Vec2d(0.0, 3.6),
            vecp::Vec2d(0.0, 2.8), vecp::Vec2d(0.0, -6.2),
            vecp::Vec2d(0.0, -6.2), vecp::Vec2d(0.0, -5.4),
            vecp::Vec2d(0.0, -12.8), vecp::Vec2d(0.0, -15.2),
            vecp::Vec2d(0.0, -17.6)
        };
    
        FaceSetFactory::defineNewFaceSetByCompass(*mesh.get(), Compass::NORTH);
        FaceSetFactory::defineNewFaceSetByCompass(*mesh.get(), Compass::WEST);
        field->addBc(BcType::FIXEDVALUE, -5.); // north
        field->addBc(BcType::FIXEDVALUE, 3.5); // west

        std::unique_ptr<vecp::Vec2d[]> output = FaceGradients<double>::calculate(*field.get(), *mesh.get());

        for (int id = 0; id < 31; id++)
        {
            ASSERT_NEAR(expected[id].x / cellWidth.x, output[id].x, 1e-06);
            ASSERT_NEAR(expected[id].y / cellWidth.y, output[id].y, 1e-06);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectFaceGradientsFixedValueBC, FaceGradientsFixedValueBC_F, testing::Values(
        vecp::Vec2d(1., 1.), vecp::Vec2d(4., 4.), vecp::Vec2d(0.2, 0.4)
    ));


}