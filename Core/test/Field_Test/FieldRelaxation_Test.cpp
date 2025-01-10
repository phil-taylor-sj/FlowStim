#include "../pch.h"

#include <Core/Field/FieldRelaxation.h>

using namespace fstim;

namespace FieldTolerance_Tests
{
    const double DEFAULT_RELAXATION = 0.9;

    // Test that the default constructor sets the under-relaxation to the default value. 
    TEST(FieldRelaxation_Tests, verify_constructor_sets_default_under_relaxation_values)
    {
        FieldRelaxation field{};
        ASSERT_NEAR(DEFAULT_RELAXATION, field.getRelaxation(), 1E-08);
    }

    // Test that the setRelaxation method correctly sets the under-relaxation to the provided value
    // if the provided value is valid.
    TEST(FieldRelaxation_Tests, verify_setRelaxaton_sets_valid_values)
    {
        FieldRelaxation field{};
        field.setRelaxation(0.8);
        ASSERT_NEAR(0.8, field.getRelaxation(), 1E-08);
        
        field.setRelaxation(1.);
        ASSERT_NEAR(1., field.getRelaxation(), 1E-08);
    }

    // Test that the setRelaxation method correctly sets the under-relaxation to the default value
    // is that value if the provided value is invalid.
    TEST(FieldRelaxation_Tests, verify_setRelaxation_sets_default_value_for_invalid_values)
    {
        FieldRelaxation field{};
        field.setRelaxation(0.);
        ASSERT_NEAR(DEFAULT_RELAXATION, field.getRelaxation(), 1E-08);

        field.setRelaxation(1.1);
        ASSERT_NEAR(1., field.getRelaxation(), 1E-08);
        
        field.setRelaxation(-0.5);
        ASSERT_NEAR(DEFAULT_RELAXATION, field.getRelaxation(), 1E-08);
    }

    // Test that the activateRelaxation method suppresses and unsuppresses the under-relaxation factor
    // which is returned by the getRelaxaton method.
    TEST(FieldRelaxation_Tests, verify_activateRelaxation_suppreses_set_value)
    {
        FieldRelaxation field{};
        field.setRelaxation(0.5);
        ASSERT_NEAR(0.5, field.getRelaxation(), 1E-08);

        field.activateRelaxation(false);
        ASSERT_NEAR(1., field.getRelaxation(), 1E-08);

        field.activateRelaxation(true);
        ASSERT_NEAR(0.5, field.getRelaxation(), 1E-08);
    }
}