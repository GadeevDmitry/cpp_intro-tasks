#include "allocator_traits.hpp"
#include "type_traits.hpp"
#include "move.hpp"

#include <limits>

//==================================================================================================

template <class T>
struct allocator
{
// types
public:
    using value_type = T;

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

    template <class U, class... Args>
    constexpr void construct(U *p, Args... args)
    {
        ::new((void *)p) U(my_std::forward<Args>(args)...);
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

//==================================================================================================

int main()
{
    allocator<int> a;

    int *pointer = my_std::allocator_traits<allocator<int>>::allocate(a, 5, nullptr);
    my_std::allocator_traits<allocator<int>>::construct(a, pointer, 6);

    my_std::allocator_traits<allocator<int>>::destroy(a, pointer);
    my_std::allocator_traits<allocator<int>>::deallocate(a, pointer, 5);
}
