#include "../pch.h"

#include <Core/Field/FieldTolerance.h>

using namespace fstim;

namespace FieldTolerance_Tests
{
    const double DEFAULT_ABS = 0.0001;
    const double DEFAULT_REL = 0.01;

    // Test that the default constructor sets the tolerance to the default values.
    TEST(FieldTolerance_Tests, verify_constructor_sets_default_tolerances_values)
    {
        FieldTolerance field{};
        ASSERT_NEAR(DEFAULT_ABS, field.getTolerance().absolute, 1E-08);
        ASSERT_NEAR(DEFAULT_REL, field.getTolerance().relative, 1E-08);
    }

    class FieldTolerance_Fixture : public testing::TestWithParam<std::tuple<Tolerance<double>, Tolerance<double>>>
    {
    protected:
        void SetUp() override
        {
            input = std::get<0>(GetParam());
            field.setTolerance(input);
            expected = std::get<1>(GetParam());
        }
        Tolerance<double> input, expected;
        FieldTolerance field{};
    };

    // Test that the setTolerance method correctly sets the tolerance to the provided values
    // if the provided values are valid.
    class FieldTolerance_ValidF : public FieldTolerance_Fixture {};
    TEST_P(FieldTolerance_ValidF, FieldTolerance_Valid)
    {
        ASSERT_NEAR(expected.absolute, field.getTolerance().absolute, 1E-08);
        ASSERT_NEAR(expected.relative, field.getTolerance().relative, 1E-08);
    }

    INSTANTIATE_TEST_SUITE_P(FieldTolerance_Valid, FieldTolerance_ValidF, testing::Values(
        std::make_tuple(Tolerance<double>(1E-08, 0.1), Tolerance<double>(1E-08, 0.1)),
        std::make_tuple(Tolerance<double>(0.1, 0.), Tolerance<double>(0.1, 0.)),
        std::make_tuple(Tolerance<double>(1., 1.), Tolerance<double>(1., 1.))
    ));

    // Test that the setTolerance method correctly sets the tolerance to the default values
    // is that value if the provided tolerance is invalid.
    
    class FieldTolerance_InvalidF : public FieldTolerance_Fixture {};
    TEST_P(FieldTolerance_InvalidF, FieldTolerance_Invalid)
    {
        ASSERT_NEAR(expected.absolute, field.getTolerance().absolute, 1E-08);
        ASSERT_NEAR(expected.relative, field.getTolerance().relative, 1E-08);
    }

    INSTANTIATE_TEST_SUITE_P(FieldTolerance_Invalid, FieldTolerance_InvalidF, testing::Values(
        //std::make_tuple(Tolerance<double>(1.1, 1.1), Tolerance<double>(DEFAULT_ABS, DEFAULT_REL)),
        std::make_tuple(Tolerance<double>(-0.001, -0.1), Tolerance<double>(DEFAULT_ABS, DEFAULT_REL)),
        std::make_tuple(Tolerance<double>(-1., -1.1), Tolerance<double>(DEFAULT_ABS, DEFAULT_REL))
    ));
}