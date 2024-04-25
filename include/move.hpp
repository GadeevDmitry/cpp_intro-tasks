#ifndef MOVE_HPP
#define MOVE_HPP

#include "type_traits.hpp"

//==================================================================================================

namespace my_std
{
    template <class T>
    remove_reference_t<T>&& move(T &&arg) noexcept
    {
        return static_cast<remove_reference_t<T>&&>(arg);
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    T&& forward(remove_reference_t<T> &arg) noexcept
    {
        return static_cast<T&&>(arg);
    }

    template <class T>
    T&& forward(remove_reference_t<T> &&arg) noexcept
    {
        return static_cast<T&&>(arg);
    }
}

//==================================================================================================

#endif // MOVE_HPP
