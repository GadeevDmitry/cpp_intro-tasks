#ifndef ALLOCATOR_TRAITS_HPP
#define ALLOCATOR_TRAITS_HPP

#include "type_traits.hpp"
#include "move.hpp"

//==================================================================================================

namespace my_std
{
    namespace detail
    {
        template <class Default, template<class...> class Preferred, class... Args>
        struct try_to_apply
        {
            using type = Default;
        };

        template <class Default, template<class...> class Preferred, class... Args>
        requires requires() { typename Preferred<Args...>;}
        struct try_to_apply<Default, Preferred, Args...>
        {
            using type = Preferred<Args...>;
        };

        template <class Default, template<class...> class Preferred, class... Args>
        using try_to_apply_t = typename try_to_apply<Default, Preferred, Args...>::type;

        //--------------------------------------------------------------------------------------------------

        struct allocator_base
        {
        protected:
            #define ALLOCATOR_TYPE_ALIAS(alias_name)    \
            template <class Alloc>                      \
            using alias_name = typename Alloc::alias_name;

            ALLOCATOR_TYPE_ALIAS(pointer)
            ALLOCATOR_TYPE_ALIAS(const_pointer)
            ALLOCATOR_TYPE_ALIAS(void_pointer)
            ALLOCATOR_TYPE_ALIAS(const_void_pointer)
            ALLOCATOR_TYPE_ALIAS(difference_type)
            ALLOCATOR_TYPE_ALIAS(size_type)
            ALLOCATOR_TYPE_ALIAS(propagate_on_container_copy_assignment)
            ALLOCATOR_TYPE_ALIAS(propagate_on_container_move_assignment)
            ALLOCATOR_TYPE_ALIAS(propagate_on_container_swap)
            ALLOCATOR_TYPE_ALIAS(is_always_equal)

            #undef ALLOCATOR_TYPE_ALIAS
        };
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class Alloc>
    struct allocator_traits: detail::allocator_base
    {
    // types
    public:
        using allocator_type = Alloc;
        using value_type     = typename Alloc::value_type;

        #define ALLOCATOR_TYPE_ALIAS(alias_name, default_type)  \
        using alias_name = detail::try_to_apply_t<default_type, alias_name, Alloc>;

        ALLOCATOR_TYPE_ALIAS(pointer                               , value_type*)

        ALLOCATOR_TYPE_ALIAS(const_pointer                         , typename std::pointer_traits<pointer>::template rebind<const value_type>)
        ALLOCATOR_TYPE_ALIAS(void_pointer                          , typename std::pointer_traits<pointer>::template rebind<void>)
        ALLOCATOR_TYPE_ALIAS(const_void_pointer                    , typename std::pointer_traits<pointer>::template rebind<const void>)

        ALLOCATOR_TYPE_ALIAS(difference_type                       , typename std::pointer_traits<pointer>::difference_type)
        ALLOCATOR_TYPE_ALIAS(size_type                             , typename make_unsigned<difference_type>::type)
        ALLOCATOR_TYPE_ALIAS(is_always_equal                       , typename is_empty<Alloc>::type)

        ALLOCATOR_TYPE_ALIAS(propagate_on_container_copy_assignment, false_type)
        ALLOCATOR_TYPE_ALIAS(propagate_on_container_move_assignment, false_type)
        ALLOCATOR_TYPE_ALIAS(propagate_on_container_swap           , false_type)

        #undef ALLOCATOR_TYPE_ALIAS

    // static
    private:
        static constexpr pointer test_hint_allocate(Alloc &a, size_type n, const_void_pointer hint)
        requires requires() { a.allocate(n, hint); }
        {
            std::cout << "ALLOCATE BY ALLOCATOR WITH HINT\n";
            return a.allocate(n, hint);
        }

        static constexpr pointer test_hint_allocate(Alloc &a, size_type n, const_void_pointer)
        {
            std::cout << "DEFAULT ALLOCATE WITH NO HINT\n";
            return a.allocate(n);
        }

        //--------------------------------------------------------------------------------------------------

        template <class T, class... Args>
        static constexpr void test_construct(Alloc &a, T *p, Args&&... args)
        requires requires() { a.construct(p, forward<Args>(args)...); }
        {
            std::cout << "CONSTRUCT BY ALLOCATOR\n";
            return a.construct(p, forward<Args>(args)...);
        }

        template <class T, class... Args>
        static constexpr void test_construct(Alloc &a, T *p, Args&&... args)
        {
            std::cout << "DEFAULT CONSTRUCT\n";
            std::construct_at(p, forward<Args>(args)...);
        }

        //--------------------------------------------------------------------------------------------------

        template <class T>
        static constexpr auto test_destroy(Alloc &a, T *p)
        requires requires() { a.destroy(p); }
        {
            std::cout << "DESTROY BY ALLOCATOR\n";
            a.destroy(p);
        }

        template <class T>
        static constexpr void test_destroy(Alloc &a, T *p, ...)
        {
            std::cout << "DEFAULT DESTROY\n";
            std::destroy_at(p);
        }

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    public:
        static constexpr pointer allocate(Alloc &a, size_type n)
        {
            return a.alloc(n);
        }

        static constexpr pointer allocate(Alloc &a, size_type n, const_void_pointer hint)
        {
            return test_hint_allocate(a, n, hint);
        }

        static constexpr void deallocate(Alloc &a, pointer p, size_type n)
        {
            return a.deallocate(p, n);
        }

        template <class T, class... Args>
        static constexpr void construct(Alloc &a, T *p, Args&&... args)
        {
            test_construct(a, p, forward<Args>(args)...);
        }

        template <class T>
        static constexpr void destroy(Alloc &a, T *p)
        {
            test_destroy(a, p);
        }
    };
}

//==================================================================================================

#endif // ALLOCATOR_TRAITS_HPP
