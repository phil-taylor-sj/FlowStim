#include "../pch.h"

#include <Core/Solver/JacobiMethod.h>

using namespace fstim;

namespace Solver_Tests
{
    class JacobiScalar_Fixture : public ::testing::TestWithParam<std::tuple<int, Tolerance<double>>>
    {
    protected:
        void SetUp() override 
        {
            int nCells = std::get<0>(GetParam());
            tolerance = std::get<1>(GetParam()); 

            field = std::make_unique<Field<double>>(nCells);

            std::map<int, double>* lhs = field->writeLeft();
            double* rhs = field->writeRight();

            for (int cellId = 0; cellId < field->nCells; cellId++)
            {
                // Sample Coefficents
                // [10., -1., -1., 0., 0., ...]
                // [-1., 11., -1., -1., 0., ...]
                // [-1., -1., 12., -1., -1., ...]
                // [0., -1., -1., 13., -1., -1., ...]
                // [0., 0., -1., -1., 14.., -1., ...]
                // ....
                int minIndex = std::max(cellId - 2, 0);
                int maxIndex = std::min(cellId + 2, field->nCells - 1);
                for (int index = minIndex; index <= maxIndex; index++)
                {
                    lhs[cellId][index] = (cellId == index) 
                        ? (double)cellId + 10.
                        : -1.;
                }
                rhs[cellId] = 2. * (double)(cellId + 1);
            }
        }
        Tolerance<double> tolerance{};
        std::unique_ptr<Field<double>> field;
    };

    class JacobiScalar_Vanilla_F : public JacobiScalar_Fixture {}; 
    TEST_P(JacobiScalar_Vanilla_F, JacobiScalar_Vanilla)
    {
        double* initialValues = this->field->writeValues();
        for (int cellId = 0; cellId < this->field->nCells; cellId++)
        {
            initialValues[cellId] = 0.1;
        }
        
        this->field->setTolerance(this->tolerance);
        this->field->activateRelaxation(false);
        JacobiMethod<double> solver{};
        int numLoops = solver(*(this->field.get()));

        const std::map<int, double>* lhs = this->field->readLeft();
        const double* values = this->field->readValues();
        const double* rhs = this->field->readRight();
        // Calculate the dot product of the left hand side coefficients
        // and the converged values.
        double residual = 0.;  
        for (int cellId = 0; cellId < this->field->nCells; cellId++)
        {
            double sum = rhs[cellId];
            for (const std::pair<int, double> pair : lhs[cellId])
            {
                 sum -= pair.second * values[pair.first];
            }
            residual += std::abs(sum);
        }
        residual /= std::sqrt(field->nCells);

        ASSERT_LT(numLoops, 50);
        ASSERT_LE(residual, tolerance.absolute);
    }

    INSTANTIATE_TEST_SUITE_P(JacobiScalar_Vanilla, JacobiScalar_Vanilla_F, testing::Values(
        std::make_tuple(12, Tolerance<double>{1E-06, 0.}),
        std::make_tuple(200, Tolerance<double>{1E-06, 0.}),
        std::make_tuple(2000, Tolerance<double>{1E-06, 0.}),
        std::make_tuple(20000, Tolerance<double>{1E-06, 0.})
    ));
}