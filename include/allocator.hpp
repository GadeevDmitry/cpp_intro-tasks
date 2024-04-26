#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <limits>
#include "move.hpp"
#include "type_traits.hpp"

//==================================================================================================

namespace my_std
{
    template <class T>
    struct allocator
    {
    // types
    public:
        using value_type                             = T;
        using size_type                              = size_t;
        using difference_type                        = ptrdiff_t;
        using propagate_on_container_move_assignment = true_type;

    // member functions
    public:
        constexpr allocator() noexcept
        {}

        constexpr allocator(const allocator &that) noexcept
        {}

        template <class U>
        constexpr allocator(const allocator<U> &that) noexcept
        {}

        constexpr T *allocate(size_t n)
        {
            if (std::numeric_limits<size_t>::max() / sizeof(T) < n)
                throw std::bad_array_new_length();

            return (T*) ::operator new(sizeof(T) * n);
        }

        constexpr void deallocate(T *p, size_t n)
        {
            ::operator delete(p);
        }

        size_type max_size() const noexcept
        {
            return std::numeric_limits<size_type>::max() / sizeof(value_type);
        }

        template <class U, class... Args>
        void construct(U *p, Args... args)
        {
            :: new((void *) p) U(forward<Args>(args)...);
        }

        template <class U>
        void destroy(U *p)
        {
            p->~U();
        }
    };

    template <class T1, class T2>
    constexpr bool operator ==(const allocator<T1>&, const allocator<T2>&) noexcept
    {
        return true;
    }

    template <class T1, class T2>
    constexpr bool operator !=(const allocator<T1>&, const allocator<T2>&) noexcept
    {
        return false;
    }
}

//==================================================================================================

#endif // ALLOCATOR_HPP
