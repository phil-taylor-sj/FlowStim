#include "../pch.h"

#include <Core/Field/Field.h>

using namespace fstim;

namespace Field_Tests
{

    // ***********************************************
    // Parent class: FieldTolerance
    // ***********************************************
    
    const double DEFAULT_ABS = 0.0001;
    const double DEFAULT_REL = 0.01;

    // Test that the default constructor sets the tolerance to the default values.
    TEST(Field_FieldTolerance_Tests, verify_constructor_sets_default_tolerances_values)
    {
        Field<double> field{12};
        ASSERT_NEAR(DEFAULT_ABS, field.getTolerance().absolute, 1E-08);
        ASSERT_NEAR(DEFAULT_REL, field.getTolerance().relative, 1E-08);
    }

    class Field_FieldTolerance_Fixture : public testing::TestWithParam<std::tuple<Tolerance<double>, Tolerance<double>>>
    {
    protected:
        void SetUp() override
        {
            input = std::get<0>(GetParam());
            field.setTolerance(input);
            expected = std::get<1>(GetParam());
        }
        Tolerance<double> input, expected;
        Field<double> field{12};
    };

    // Test that the setTolerance method correctly sets the tolerance to the provided values
    // if the provided values are valid.
    class Field_FieldTolerance_ValidF : public Field_FieldTolerance_Fixture {};
    TEST_P(Field_FieldTolerance_ValidF, Field_FieldTolerance_Valid)
    {
        ASSERT_NEAR(expected.absolute, field.getTolerance().absolute, 1E-08);
        ASSERT_NEAR(expected.relative, field.getTolerance().relative, 1E-08);
    }

    INSTANTIATE_TEST_SUITE_P(Field_FieldTolerance_Valid, Field_FieldTolerance_ValidF, testing::Values(
        std::make_tuple(Tolerance<double>(1E-08, 0.1), Tolerance<double>(1E-08, 0.1)),
        std::make_tuple(Tolerance<double>(0.1, 0.), Tolerance<double>(0.1, 0.)),
        std::make_tuple(Tolerance<double>(1., 1.), Tolerance<double>(1., 1.))
    ));

    // Test that the setTolerance method correctly sets the tolerance to the default values
    // is that value if the provided tolerance is invalid.
    class Field_FieldTolerance_InvalidF : public Field_FieldTolerance_Fixture {};
    TEST_P(Field_FieldTolerance_InvalidF, Field_FieldTolerance_Invalid)
    {
        ASSERT_NEAR(expected.absolute, field.getTolerance().absolute, 1E-08);
        ASSERT_NEAR(expected.relative, field.getTolerance().relative, 1E-08);
    }

    INSTANTIATE_TEST_SUITE_P(Field_FieldTolerance_Invalid, Field_FieldTolerance_InvalidF, testing::Values(
        //std::make_tuple(Tolerance<double>(1.1, 1.1), Tolerance<double>(DEFAULT_ABS, DEFAULT_REL)),
        std::make_tuple(Tolerance<double>(-0.001, -0.1), Tolerance<double>(DEFAULT_ABS, DEFAULT_REL)),
        std::make_tuple(Tolerance<double>(-1., -1.1), Tolerance<double>(DEFAULT_ABS, DEFAULT_REL))
    ));


    // ***********************************************
    // Parent class: FieldRelaxation
    // ***********************************************

    const double DEFAULT_RELAXATION = 0.9;

    // Test that the default constructor sets the under-relaxation to the default value. 
    TEST(Field_FieldRelaxation_Tests, verify_constructor_sets_default_under_relaxation_values)
    {
        Field<double> field{12};
        ASSERT_NEAR(DEFAULT_RELAXATION, field.getRelaxation(), 1E-08);
    }

    // Test that the setRelaxation method correctly sets the under-relaxation to the provided value
    // if the provided value is valid.
    TEST(FieldRelaxation_Tests, verify_setTolerance_sets_valid_values)
    {
        Field<double> field{12};
        field.setRelaxation(0.8);
        ASSERT_NEAR(0.8, field.getRelaxation(), 1E-08);
        
        field.setRelaxation(1.);
        ASSERT_NEAR(1., field.getRelaxation(), 1E-08);
    }

    // Test that the setRelaxation method correctly sets the under-relaxation to the default value
    // is that value if the provided value is invalid.
    TEST(Field_FieldRelaxation_Tests, verify_setTolerance_sets_default_value_for_invalid_values)
    {
        Field<double> field{12};
        field.setRelaxation(0.);
        ASSERT_NEAR(DEFAULT_RELAXATION, field.getRelaxation(), 1E-08);

        field.setRelaxation(1.1);
        ASSERT_NEAR(1., field.getRelaxation(), 1E-08);
        
        field.setRelaxation(-0.5);
        ASSERT_NEAR(DEFAULT_RELAXATION, field.getRelaxation(), 1E-08);
    }

    // Test that the activateRelaxation method suppresses and unsuppresses the under-relaxation factor
    // which is returned by the getRelaxaton method.
    TEST(Field_FieldRelaxation_Tests, verify_activateRelaxation_suppreses_set_value)
    {
        Field<double> field{12};
        field.setRelaxation(0.5);
        ASSERT_NEAR(0.5, field.getRelaxation(), 1E-08);

        field.activateRelaxation(false);
        ASSERT_NEAR(1., field.getRelaxation(), 1E-08);

        field.activateRelaxation(true);
        ASSERT_NEAR(0.5, field.getRelaxation(), 1E-08);
    }
}