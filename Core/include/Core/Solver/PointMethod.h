#pragma once
#include <Core/Field/Field.h>

namespace fstim
{
    template <typename T>
    class PointMethod
    {
    public:
        void virtual operator()(Field<T>& field, const T* source = nullptr) = 0;
    
        PointMethod() {};

        virtual ~PointMethod() = 0;
    };

    //extern template class PointMethod<double>;

    //extern template class PointMethod<vecp::Vec2d>; 
}