#include <Core/Field/Field.h>

namespace fstim
{
    template <typename T>
    std::tuple<BcType, T> Field<T>::getBc(int faceSetId)
    {
        if(faceSetId > this->m_boundaries.size() - 1)
        {
            return std::make_tuple(BcType::NONE, T());
        }
        return this->m_boundaries[faceSetId];
    }

    template<typename T>
    int Field<T>::addBc(BcType type, T value)
    {
        this->m_boundaries.push_back(std::make_tuple(type, value));
        return m_boundaries.size() - 1;
    }

    template<typename T>
    bool Field<T>::updateBc(int faceSetId, T newValue)
    {
        if (faceSetId > this->m_boundaries.size() - 1)
        {
            return false;
        }
        BcType type = std::get<0>(this->m_boundaries[faceSetId]);
        this->m_boundaries[faceSetId] = std::make_tuple(type, newValue);
        return true;
    }

    template class Field<double>;

    template class Field<vecp::Vec2d>;
}