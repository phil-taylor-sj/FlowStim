#include "../pch.h"
#include <algorithm>

#include <Core/Domain/Mesh2dStructuredFactory.h>
#include <Core/Solver/FaceValues.h>
#include <Core/Domain/FaceSetFactory.h>

using namespace fstim;

namespace FaceValue_Tests
{
    class FaceValues_Fixture : public ::testing::TestWithParam<vecp::Vec2d>
    {
    protected:
        void SetUp() override 
        {
            // Set mesh properties and build mesh.
            vecp::Vec2i size {3, 4};
            cellWidth =  GetParam();
            vecp::Vec2d length = size.toDouble() * cellWidth;
            Mesh2dStructuredFactory factory = Mesh2dStructuredFactory();
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

    class FaceValuesZeroGradient_F : public FaceValues_Fixture {};
    TEST_P(FaceValuesZeroGradient_F, CorrectFaceValues)
    {
        std::vector<double> expected = {
            1.2, 1.8, 3.0, 3.6,
            4.8, 5.0, 5.8, 6.4,
            7.6, 8.2, 9.0, 9.2,
            1.4, 2.0, 3.2, 3.8,
            1.2, 2.4, 3.6,
            3.0, 3.8, 5.0,
            6.2, 7.0, 7.8,
            4.5, 5.7, 6.5,
            1.4, 2.6, 3.8
        };
        std::unique_ptr<double[]> output = FaceValues<double>::interpolate(*field.get(), *mesh.get());

        for (int id = 0; id < 31; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id], output[id]);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectFaceValues, FaceValuesZeroGradient_F, testing::Values(
        vecp::Vec2d(1., 1.), vecp::Vec2d(4., 4.), vecp::Vec2d(0.2, 0.4)
    ));


    class FaceValuesFixedValueBC_F : public FaceValues_Fixture {};
    TEST_P(FaceValuesFixedValueBC_F, CorrectFaceValuesFixedValueBC)
    {
        FaceSetFactory::defineNewFaceSetByCompass(*mesh.get(), Compass::NORTH);
        FaceSetFactory::defineNewFaceSetByCompass(*mesh.get(), Compass::WEST);
        field->addBc(BcType::FIXEDVALUE, -5.); // north
        field->addBc(BcType::FIXEDVALUE, 3.5); // west

        std::vector<double> expected = {
            3.5, 1.8, 3.0, 3.6,
            3.5, 5.0, 5.8, 6.4,
            3.5, 8.2, 9.0, 9.2,
            3.5, 2.0, 3.2, 3.8,
            1.2, 2.4, 3.6,
            3.0, 3.8, 5.0,
            6.2, 7.0, 7.8,
            4.5, 5.7, 6.5,
            -5., -5., -5.
        };
        std::unique_ptr<double[]> output = FaceValues<double>::interpolate(*field.get(), *mesh.get());

        for (int id = 0; id < 31; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id], output[id]);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectFaceValuesFixedValueBC, FaceValuesFixedValueBC_F, testing::Values(
        vecp::Vec2d(1., 1.), vecp::Vec2d(4., 4.), vecp::Vec2d(0.2, 0.4)
    ));


}