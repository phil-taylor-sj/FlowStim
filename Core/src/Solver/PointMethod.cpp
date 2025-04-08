#include <Core/Solver/PointMethod.h>

namespace fstim
{
    template <typename T>
    PointMethod<T>::~PointMethod() {}

    template class PointMethod<double>;

    template class PointMethod<vecp::Vec2d>;
}