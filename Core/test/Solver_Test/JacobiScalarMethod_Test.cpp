#include "../pch.h"

#include <Core/Solver/JacobiScalarMethod.h>

using namespace fstim;

namespace Solver_Tests
{
    class SmallFieldF : public ::testing::Test
    {
    protected:
        void SetUp() override 
        {
            field = std::make_unique<Field<double>>(12);

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
        std::unique_ptr<Field<double>> field;
    };

    TEST_F(SmallFieldF, test_sets_correctly_solves_for_values)
    {
        double* initialValues = field->writeValues();
        for (int cellId = 0; cellId < 12; cellId++)
        {
            initialValues[cellId] = 0.1;
        }
        
        field->setTolerance(Tolerance<double>(1E-06, 0.));
        field->activateRelaxation(false);
        JacobiScalarMethod<double> solver{};
        solver(*(field.get()));

        const std::map<int, double>* lhs = field->readLeft();
        const double* values = field->readValues();
        
        // Calculate the dot product of the left hand side coefficients
        // and the converged values.
        std::vector<double> output(12);
        for (int cellId = 0; cellId < 12; cellId++)
        {
            for (const std::pair<int, double> pair : lhs[cellId])
            {
                output[cellId] += pair.second * values[pair.first];
            }
        }
    
        const double* expected = field->readRight();
        for (int cellId = 0; cellId < 12; cellId++)
        {
            ASSERT_NEAR(expected[cellId], output[cellId], 1E-04);
        }
    }

}