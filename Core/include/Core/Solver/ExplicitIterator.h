#pragma once

#include <Core/Field/Field.h>

namespace fstim
{
    template <typename T>
    class ExplicitIterator
    {
    public:

        void operator()(Field<T>& field);

        ExplicitIterator();
    };
}

