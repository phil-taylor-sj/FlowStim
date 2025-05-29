#include "../pch.h"

#include <Core/Solver/JacobiVectorMethod.h>

using namespace fstim;

namespace VectorSolver_Tests
{
    class JacobiVector_Fixture : public ::testing::TestWithParam<std::tuple<int, Tolerance<double>>>
    {
    protected:
        void SetUp() override 
        {
            int nCells = std::get<0>(GetParam());
            tolerance = std::get<1>(GetParam()); 

            field = std::make_unique<Field<vecp::Vec2d>>(std::get<0>(GetParam()));

            std::map<int, vecp::Vec2d>* lhs = field->writeLeft();
            vecp::Vec2d* rhs = field->writeRight();

            for (int cellId = 0; cellId < field->nCells; cellId++)
            {
                int minIndex = std::max(cellId - 2, 0);
                int maxIndex = std::min(cellId + 2, field->nCells - 1);
                for (int index = minIndex; index <= maxIndex; index++)
                {
                    double value = (double)cellId;
                    lhs[cellId][index] = (cellId == index) 
                        ? vecp::Vec2d(value + 10., -(value + 10))
                        : vecp::Vec2d(-1., -1.);
                }
                double rhs_value = 2. * ((double)cellId + 1.);
                rhs[cellId] = vecp::Vec2d(rhs_value, -rhs_value);
            }
        }
        Tolerance<double> tolerance{};
        std::unique_ptr<Field<vecp::Vec2d>> field;
    };

    class JacobiVector_Vanilla_F : public JacobiVector_Fixture {}; 
    TEST_P(JacobiVector_Vanilla_F, JacobiVector_Vanilla)
    {
        vecp::Vec2d* initialValues = this->field->writeValues();
        for (int cellId = 0; cellId < this->field->nCells; cellId++)
        {
            initialValues[cellId] = 0.1;
        }
        
        this->field->setTolerance(this->tolerance);
        this->field->activateRelaxation(false);
        JacobiVectorMethod<vecp::Vec2d> solver{};
        solver(*(this->field.get()));

        const std::map<int, vecp::Vec2d>* lhs = this->field->readLeft();
        const vecp::Vec2d* values = this->field->readValues();
        
        // Calculate the dot product of the left hand side coefficients
        // and the converged values.
        std::vector<vecp::Vec2d> output(this->field->nCells);
        for (int cellId = 0; cellId < this->field->nCells; cellId++)
        {
            for (const std::pair<int, vecp::Vec2d> pair : lhs[cellId])
            {
                output[cellId] += pair.second * values[pair.first];
            }
        }

        const vecp::Vec2d* expected = this->field->readRight();
        for (int cellId = 0; cellId < this->field->nCells; cellId++)
        {
            //std::cout << expected[cellId].x << "   "  << output[cellId].x << " " << this->tolerance.absolute << std::endl;
            ASSERT_NEAR(expected[cellId].x, output[cellId].x, this->tolerance.absolute * 10);
            ASSERT_NEAR(expected[cellId].y, output[cellId].y, this->tolerance.absolute * 10);
        }
    }

    INSTANTIATE_TEST_SUITE_P(JacobiVector_Vanilla, JacobiVector_Vanilla_F, testing::Values(
        std::make_tuple(12, Tolerance<double>{1E-06, 0.}),
        std::make_tuple(200, Tolerance<double>{1E-06, 0.}),
        std::make_tuple(2000, Tolerance<double>{1E-06, 0.}),
        std::make_tuple(2000, Tolerance<double>{1E-06, 0.})
    ));
}