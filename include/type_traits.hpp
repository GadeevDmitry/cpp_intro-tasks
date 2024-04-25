#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <iostream>

//==================================================================================================

namespace my_std
{
    //==================================================================================================

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // integral_constant
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // primary type categories
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class>
    struct is_function;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // composite type categories
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class>
    struct is_reference;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // other type properties
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class>
    struct is_const;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // test for specific operations
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // relationships between types
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // type construction
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class>
    struct remove_cv;

    template <class T>
    using remove_cv_t = typename remove_cv<T>::type;

    template <class T>
    struct add_lvalue_reference;

    template <class T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    template <class T>
    struct add_rvalue_reference;

    template <class T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

    template <class T>
    struct remove_extent;

    template <class T>
    using remove_extent_t = typename remove_extent<T>::type;

    template <class T>
    struct remove_all_extents;

    template <class T>
    using remove_all_extents_t = typename remove_all_extents<T>::type;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // other traits
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <bool Cond, class True, class False>
    struct conditional;

    template <bool Cond, class True, class False>
    using conditional_t = typename conditional<Cond, True, False>::type;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // combining type traits
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // other utilities
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class...>
    using void_t = void;

    template <class T>
    add_rvalue_reference_t<T> declval() noexcept;

    //==================================================================================================

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // integral_constant
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T, T val>
    struct integral_constant
    {
        using value_type = T;
        using type       = integral_constant<T, val>;

        static const T value = val;

        operator value_type()   const noexcept { return value; }
        value_type operator()() const noexcept { return value; }
    };

    template <bool B>
    using bool_constant = integral_constant<bool, B>;

    using  true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // primary type categories
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T>
    struct is_void: public false_type {};

    #define IS_VOID(proper_type)    \
    template <>                     \
    struct is_void<proper_type>: public true_type {};

    IS_VOID(void)
    IS_VOID(const void)
    IS_VOID(volatile void)
    IS_VOID(const volatile void)

    #undef IS_VOID

    template <class T>
    constexpr bool is_void_v = is_void<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_integral_helper: public false_type {};

        #define IS_INTEGRAL_HELPER(proper_type) \
        template <>                             \
        struct is_integral_helper<proper_type>: public true_type {};

        IS_INTEGRAL_HELPER(bool)
        IS_INTEGRAL_HELPER(char)
        IS_INTEGRAL_HELPER(signed char)
        IS_INTEGRAL_HELPER(unsigned char)
        IS_INTEGRAL_HELPER(wchar_t)
        IS_INTEGRAL_HELPER(char8_t)
        IS_INTEGRAL_HELPER(char16_t)
        IS_INTEGRAL_HELPER(char32_t)
        IS_INTEGRAL_HELPER(short)
        IS_INTEGRAL_HELPER(unsigned short)
        IS_INTEGRAL_HELPER(int)
        IS_INTEGRAL_HELPER(unsigned int)
        IS_INTEGRAL_HELPER(long)
        IS_INTEGRAL_HELPER(unsigned long)
        IS_INTEGRAL_HELPER(long long)
        IS_INTEGRAL_HELPER(unsigned long long)

        #undef IS_INTEGRAL_HELPER
    }

    template <class T>
    struct is_integral: public detail::is_integral_helper<remove_cv_t<T>>::type {};

    template <class T>
    constexpr bool is_integral_v = is_integral<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_floating_point_helper: public false_type {};

        #define IS_FLOATING_POINT_HELPER(proper_type)   \
        template <>                                     \
        struct is_floating_point_helper<proper_type>: public true_type {};

        IS_FLOATING_POINT_HELPER(float)
        IS_FLOATING_POINT_HELPER(double)
        IS_FLOATING_POINT_HELPER(long double)

        #undef IS_FLOATING_POINT_HELPER
    }

    template <class T>
    struct is_floating_point: public detail::is_floating_point_helper<remove_cv_t<T>>::type {};

    template <class T>
    constexpr bool is_floating_point_v = is_floating_point<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_array_unknown_bounds: public false_type {};

        template <class T>
        struct is_array_unknown_bounds<T[]>: public true_type {};
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_array: public false_type {};

    template <class T>
    struct is_array<T[]>: public true_type {};

    template <class T, size_t N>
    struct is_array<T[N]>: public true_type {};

    template <class T>
    constexpr bool is_array_v = is_array<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_pointer_helper: public false_type {};

        template <class T>
        struct is_pointer_helper<T *>: public true_type {};
    }

    template <class T>
    struct is_pointer: public detail::is_pointer_helper<remove_cv_t<T>>::type {}; 

    template <class T>
    constexpr bool is_pointer_v = is_pointer<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_null_pointer_helper: public false_type {};

        template <>
        struct is_null_pointer_helper<std::nullptr_t>: public true_type {};
    }

    template <class T>
    struct is_null_pointer: public detail::is_null_pointer_helper<remove_cv_t<T>>::type {};

    template <class T>
    constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_member_object_pointer_helper: public false_type {};

        template <class T, class C>
        struct is_member_object_pointer_helper<T C::*>: public bool_constant<!is_function<T>::value>::type {};
    }

    template <class T>
    struct is_member_object_pointer: public detail::is_member_object_pointer_helper<remove_cv_t<T>>::type {};

    template <class T>
    constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

    namespace detail
    {
        template <class T>
        struct is_member_function_pointer_helper: public false_type {};

        template <class T, class C>
        struct is_member_function_pointer_helper<T C::*>: public is_function<T>::type {};
    }

    template <class T>
    struct is_member_function_pointer: public detail::is_member_function_pointer_helper<remove_cv_t<T>>::type {};

    template <class T>
    constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_lvalue_reference: public false_type {};

    template <class T>
    struct is_lvalue_reference<T&>: public true_type {};

    template <class T>
    constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

    template <class T>
    struct is_rvalue_reference: public false_type {};

    template <class T>
    struct is_rvalue_reference<T&&>: public true_type {};

    template <class T>
    constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_enum: public bool_constant<__is_enum(T)> {};

    template <class T>
    constexpr bool is_enum_v = is_enum<T>::value;

    template <class T>
    struct is_class: public bool_constant<__is_class(T)> {};

    template <class T>
    constexpr bool is_class_v = is_class<T>::value;

    template <class T>
    struct is_union: public bool_constant<__is_union(T)> {};

    template <class T>
    constexpr bool is_union_v = is_union<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_function: public bool_constant<!is_const<const T>::value && !is_reference<T>::value> {};

    template <class T>
    constexpr bool is_function_v = is_function<T>::value;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // composite type categories
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T>
    struct is_reference: public false_type {};

    #define IS_REFERENCE(proper_type)   \
    template <class T>                  \
    struct is_reference<proper_type>: public true_type {};

    IS_REFERENCE(T&)
    IS_REFERENCE(T&&)

    #undef IS_REFERENCE

    template <class T>
    constexpr bool is_reference_v = is_reference<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_member_pointer_helper: public false_type {};

        template <class T, class C>
        struct is_member_pointer_helper<T C::*>: public true_type {};
    }

    template <class T>
    struct is_member_pointer: public detail::is_member_pointer_helper<remove_cv_t<T>>::type {};

    template <class T>
    constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_arithmetic: public bool_constant<
        is_integral      <T>::value ||
        is_floating_point<T>::value> {};

    template <class T>
    constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_fundamental: public bool_constant<
        is_arithmetic  <T>::value ||
        is_void        <T>::value ||
        is_null_pointer<T>::value> {};

    template <class T>
    constexpr bool is_fundamental_v = is_fundamental<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_scalar: public bool_constant<
        is_arithmetic    <T>::value ||
        is_enum          <T>::value ||
        is_pointer       <T>::value ||
        is_member_pointer<T>::value ||
        is_null_pointer  <T>::value> {};

    template <class T>
    constexpr bool is_scalar_v = is_scalar<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_object: public bool_constant<!(
        is_function <T>::value ||
        is_reference<T>::value ||
        is_void     <T>::value)> {};

    template <class T>
    constexpr bool is_object_v = is_object<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_compound: public bool_constant<!
        is_fundamental<T>::value> {};

    template <class T>
    constexpr bool is_compound_v = is_compound<T>::value;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // other type properties
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    namespace detail
    {
        template <class T, bool = is_arithmetic<T>::value>
        struct is_signed_helper: public false_type {};

        template <class T>
        struct is_signed_helper<T, true>: public bool_constant<T(-1) < T(0)> {};
    }

    template <class T>
    struct is_signed: public detail::is_signed_helper<T>::type {};

    template <class T>
    constexpr bool is_signed_v = is_signed<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_unsigned: public bool_constant<
        is_arithmetic<T>::value &&
       !is_signed    <T>::value>::type {};

    template <class T>
    constexpr bool is_unsigned_v = is_unsigned<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_const: public false_type {};

    template <class T>
    struct is_const<const T>: public true_type {};

    template <class T>
    constexpr bool is_const_v = is_const<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_volatile: public false_type {};

    template <class T>
    struct is_volatile<volatile T>: public true_type {};

    template <class T>
    constexpr bool is_volatile_v = is_volatile<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_aggregate: public bool_constant<__is_aggregate(remove_cv_t<T>)> {};

    template <class T>
    constexpr bool is_aggregate_v = is_aggregate<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_trivial: public bool_constant<__is_trivial(T)> {};

    template <class T>
    constexpr bool is_trivial_v = is_trivial<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_trivially_copyable: public bool_constant<__is_trivially_copyable(T)> {};

    template <class T>
    constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_standard_layout: public bool_constant<__is_standard_layout(T)> {};

    template <class T>
    constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_empty: public bool_constant<__is_empty(T)> {};

    template <class T>
    constexpr bool is_empty_v = is_empty<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_polymorphic: public bool_constant<__is_polymorphic(T)> {};

    template <class T>
    constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_abstract: public bool_constant<__is_abstract(T)> {};

    template <class T>
    constexpr bool is_abstract_v = is_abstract<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct is_final: public bool_constant<__is_final(T)> {};

    template <class T>
    constexpr bool is_final_v = is_final<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct has_virtual_destructor: public bool_constant<__has_virtual_destructor(T)> {};

    template <class T>
    constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct has_unique_object_representations: public bool_constant<__has_unique_object_representations(remove_cv_t<remove_all_extents_t<T>>)> {};

    template <class T>
    constexpr bool has_unique_object_representations_v = has_unique_object_representations<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct alignment_of: public integral_constant<size_t, alignof(T)> {};

    template <class T>
    constexpr bool alignment_of_v = alignment_of<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct rank: public integral_constant<size_t, 0> {};

    template <class T>
    struct rank<T[]>: public integral_constant<size_t, rank<T>::value + 1> {};

    template <class T, size_t N>
    struct rank<T[N]>: public integral_constant<size_t, rank<T>::value + 1> {};

    template <class T>
    constexpr bool rank_v = rank<T>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T, size_t IDX = 0>
    struct extent: public integral_constant<size_t, 0> {};

    template <class T, size_t IDX>
    struct extent<T[], IDX>: public extent<T, IDX - 1>::type {};

    template <class T, size_t N, size_t IDX>
    struct extent<T[N], IDX>: public extent<T, IDX - 1>::type {};

    template <class T>
    struct extent<T[], 0>: public integral_constant<size_t, 0> {};

    template <class T, size_t N>
    struct extent<T[N], 0>: public integral_constant<size_t, N> {};

    template <class T, size_t IDX = 0>
    constexpr bool extent_v = extent<T, IDX>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T, bool = is_enum<T>::value>
        struct underlying_type_helper {};

        template <class T>
        struct underlying_type_helper<T, true>
        {
            using type = __underlying_type(T);
        };
    }

    template <class T>
    struct underlying_type: public detail::underlying_type_helper<T> {};

    template <class T>
    using underlying_type_t = typename underlying_type<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T, class... Args>
    using is_invocable = std::is_invocable<T, Args...>;

    template <class T, class... Args>
    constexpr bool is_invocable_v = is_invocable<T, Args...>::value;

    template <class T, class... Args>
    using is_nothrow_invocable = std::is_nothrow_invocable<T, Args...>;

    template <class T, class... Args>
    constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<T, Args...>::value;

    //--------------------------------------------------------------------------------------------------

    template <class Ret, class T, class... Args>
    using is_invocable_r = std::is_invocable_r<Ret, T, Args...>;

    template <class Ret, class T, class... Args>
    constexpr bool is_invocable_r_v = is_invocable_r<Ret, T, Args...>::value;

    template <class Ret, class T, class... Args>
    using is_nothrow_invocable_r = std::is_nothrow_invocable_r<Ret, T, Args...>;

    template <class Ret, class T, class... Args>
    constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<Ret, T, Args...>::value;

    //--------------------------------------------------------------------------------------------------

    template <class T, class... Args>
    using invoke_result = std::invoke_result<T, Args...>;

    template <class T, class... Args>
    using invoke_result_t = typename invoke_result<T, Args...>::type;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // test for specific operations
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    namespace detail
    {
        template <class T, class, class... Args>
        struct is_constructible_helper: public false_type {};

        template <class T, class... Args>
        struct is_constructible_helper<T, void_t<decltype(T(declval<Args>()...))>, Args...>: public true_type {};

        //--------------------------------------------------------------------------------------------------

        template <class T, class, class... Args>
        struct is_trivially_constructible_helper: public bool_constant<__is_trivially_constructible(T, Args...)> {};

        //--------------------------------------------------------------------------------------------------

        template <class T, class, class... Args>
        struct is_nothrow_constructible_helper: public false_type {};

        template <class T, class... Args>
        struct is_nothrow_constructible_helper<T, void_t<decltype(T(declval<Args>()...))>, Args...>:
            public bool_constant<noexcept(T(declval<Args>()...))> {};
    }

    //--------------------------------------------------------------------------------------------------

    #define IS_CONSTRUCTIBLE(trait)                                                                         \
    template <class T, class... Args>                                                                       \
    struct trait##_constructible: public detail::trait##_constructible_helper<T, void, Args...>::type {};   \
                                                                                                            \
    template <class T, class... Args>                                                                       \
    constexpr bool trait##_constructible_v = trait##_constructible<T, Args...>::value;

    IS_CONSTRUCTIBLE(is)
    IS_CONSTRUCTIBLE(is_trivially)
    IS_CONSTRUCTIBLE(is_nothrow)

    #undef IS_CONSTRUCTIBLE

    //--------------------------------------------------------------------------------------------------

    #define IS_DEFAULT_CONSTRUCTIBLE(trait)                                                                 \
    template <class T>                                                                                      \
    struct trait##_default_constructible: public detail::trait##_constructible_helper<T, void>::type {};    \
                                                                                                            \
    template <class T>                                                                                      \
    constexpr bool trait##_default_constructible_v = trait##_default_constructible<T>::value;

    IS_DEFAULT_CONSTRUCTIBLE(is)
    IS_DEFAULT_CONSTRUCTIBLE(is_trivially)
    IS_DEFAULT_CONSTRUCTIBLE(is_nothrow)

    #undef IS_DEFAULT_CONSTRUCTIBLE

    //--------------------------------------------------------------------------------------------------

    #define IS_COPY_CONSTRUCTIBLE(trait)                                                                                                \
    template <class T>                                                                                                                  \
    struct trait##_copy_constructible: public detail::trait##_constructible_helper<T, void, add_lvalue_reference_t<const T>>::type {};  \
                                                                                                                                        \
    template <class T>                                                                                                                  \
    constexpr bool trait##_copy_constructible_v = trait##_copy_constructible<T>::value;

    IS_COPY_CONSTRUCTIBLE(is)
    IS_COPY_CONSTRUCTIBLE(is_trivially)
    IS_COPY_CONSTRUCTIBLE(is_nothrow)

    #undef IS_COPY_CONSTRUCTIBLE

    //--------------------------------------------------------------------------------------------------

    #define IS_MOVE_CONSTRUCTIBLE(trait)                                                                                            \
    template <class T>                                                                                                              \
    struct trait##_move_constructible: public detail::trait##_constructible_helper<T, void, add_rvalue_reference_t<T>>::type {};    \
                                                                                                                                    \
    template <class T>                                                                                                              \
    constexpr bool trait##_move_constructible_v = trait##_move_constructible<T>::value;

    IS_MOVE_CONSTRUCTIBLE(is)
    IS_MOVE_CONSTRUCTIBLE(is_trivially)
    IS_MOVE_CONSTRUCTIBLE(is_nothrow)

    #undef IS_MOVE_CONSTRUCTIBLE

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    namespace detail
    {
        template <class To, class From, class = void>
        struct is_assignable_helper: public false_type {};

        template <class To, class From>
        struct is_assignable_helper<To, From, void_t<decltype(declval<To>() = declval<From>())>>: public true_type {};

        //--------------------------------------------------------------------------------------------------

        template <class To, class From>
        struct is_trivially_assignable_helper: public bool_constant<__is_trivially_assignable(To, From)> {};

        //--------------------------------------------------------------------------------------------------

        template <class To, class From, class = void>
        struct is_nothrow_assignable_helper: public false_type {};

        template <class To, class From>
        struct is_nothrow_assignable_helper<To, From, void_t<decltype(declval<To>() = declval<From>())>>:
            public bool_constant<noexcept(declval<To>() = declval<From>())> {};
    }

    //--------------------------------------------------------------------------------------------------

    #define IS_ASSIGNABLE(trait)                                                            \
    template <class To, class From>                                                         \
    struct trait##_assignable: public detail::trait##_assignable_helper<To, From>::type {}; \
                                                                                            \
    template <class To, class From>                                                         \
    constexpr bool trait##_assignable_v = trait##_assignable<To, From>::value;

    IS_ASSIGNABLE(is)
    IS_ASSIGNABLE(is_trivially)
    IS_ASSIGNABLE(is_nothrow)

    #undef IS_ASSIGNABLE

    //--------------------------------------------------------------------------------------------------

    #define IS_COPY_ASSIGNABLE(trait)                                                                                                               \
    template <class T>                                                                                                                              \
    struct trait##_copy_assignable: public detail::trait##_assignable_helper<add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>>::type {};  \
                                                                                                                                                    \
    template <class T>                                                                                                                              \
    constexpr bool trait##_copy_assignable_v = trait##_copy_assignable<T>::value;

    IS_COPY_ASSIGNABLE(is)
    IS_COPY_ASSIGNABLE(is_trivially)
    IS_COPY_ASSIGNABLE(is_nothrow)

    #undef IS_COPY_ASSIGNABLE

    //--------------------------------------------------------------------------------------------------

    #define IS_MOVE_ASSIGNABLE(trait)                                                                                                           \
    template <class T>                                                                                                                          \
    struct trait##_move_assignable: public detail::trait##_assignable_helper<add_lvalue_reference_t<T>, add_rvalue_reference_t<T>>::type {};    \
                                                                                                                                                \
    template <class T>                                                                                                                          \
    constexpr bool trait##_move_assignable_v = trait##_move_assignable<T>::value;

    IS_MOVE_ASSIGNABLE(is)
    IS_MOVE_ASSIGNABLE(is_trivially)
    IS_MOVE_ASSIGNABLE(is_nothrow)

    #undef IS_MOVE_ASSIGNABLE

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    namespace detail
    {
        template <class T>
        struct is_destructible_implementation
        {
        private:
            template <class U, class = decltype(declval<U&>().~U())>
            static true_type test(void *);

            template <class>
            static false_type test(...);

        public:
            using type = decltype(test<T>(nullptr));
        };

        template <
            class T,
            bool = is_void<T>::value || detail::is_array_unknown_bounds<T>::value || is_function<T>::value,
            bool = is_reference<T>::value || is_scalar<T>::value>
        struct is_destructible_helper;

        template <class T>
        struct is_destructible_helper<T, false, false>: public is_destructible_implementation<remove_all_extents_t<T>>::type {};

        template <class T>
        struct is_destructible_helper<T, false, true>: public true_type {};

        template <class T>
        struct is_destructible_helper<T, true, false>: public false_type {};

        //--------------------------------------------------------------------------------------------------

        template <class T>
        struct is_trivially_destructible_helper: public bool_constant<__is_trivially_destructible(T)> {};

        //--------------------------------------------------------------------------------------------------

        template <class T>
        struct is_nothrow_destructible_implementation
        {
        private:
            template <class U>
            static bool_constant<noexcept(declval<U&>().~U())> test(void *);

            template <class>
            static false_type test(...);

        public:
            using type = decltype(test<T>(nullptr));
        };

        template <
            class T,
            bool = is_void<T>::value || detail::is_array_unknown_bounds<T>::value || is_function<T>::value,
            bool = is_reference<T>::value || is_scalar<T>::value>
        struct is_nothrow_destructible_helper;

        template <class T>
        struct is_nothrow_destructible_helper<T, false, false>: public is_nothrow_destructible_implementation<remove_all_extents_t<T>>::type {};

        template <class T>
        struct is_nothrow_destructible_helper<T, false, true>: public true_type {};

        template <class T>
        struct is_nothrow_destructible_helper<T, true, false>: public false_type {};
    }

    //--------------------------------------------------------------------------------------------------

    #define IS_DESTRUCTIBLE(trait)                                                          \
    template <class T>                                                                      \
    struct trait##_destructible: public detail::trait##_destructible_helper<T>::type {};    \
                                                                                            \
    template <class T>                                                                      \
    constexpr bool trait##_destructible_v = trait##_destructible<T>::value;

    IS_DESTRUCTIBLE(is)
    IS_DESTRUCTIBLE(is_trivially)
    IS_DESTRUCTIBLE(is_nothrow)

    #undef IS_DESTRUCTIBLE

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T1, class T2>
        struct is_swappable_with_helper
        {
        private:
            template <
                class U1, class U2,
                class = decltype(std::swap(declval<U1>(), declval<U2>())),
                class = decltype(std::swap(declval<U2>(), declval<U1>()))>
            static true_type test(void *);

            template <class, class>
            static false_type test(...);

        public:
            using type = decltype(test<T1, T2>(nullptr));
        };
    }

    template <class T1, class T2>
    struct is_swappable_with: public detail::is_swappable_with_helper<T1, T2>::type {};

    template <class T1, class T2>
    constexpr bool is_swappable_with_v = is_swappable_with<T1, T2>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T1, class T2>
        struct is_nothrow_swappable_with_helper
        {
        private:
            template <
                class U1, class U2,
                class = decltype(std::swap(declval<U1>(), declval<U2>())),
                class = decltype(std::swap(declval<U2>(), declval<U1>()))>
            static bool_constant<
                noexcept(std::swap(declval<U1>(), declval<U2>())) &&
                noexcept(std::swap(declval<U2>(), declval<U1>()))> test(void *);

            template <class, class>
            static false_type test(...);

        public:
            using type = decltype(test<T1, T2>(nullptr));
        };
    }

    template <class T1, class T2>
    struct is_nothrow_swappable_with: public detail::is_nothrow_swappable_with_helper<T1, T2>::type {};

    template <class T1, class T2>
    constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T1, T2>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_swappable_helper
        {
        private:
            template <class U, class = decltype(std::swap(declval<T&>(), declval<T&>()))>
            static true_type test(void *);

            template <class>
            static false_type test(...);

        public:
            using type = decltype(test<T>(nullptr));
        };
    }

    template <class T>
    struct is_swappable: public detail::is_swappable_helper<T>::type {};

    template <class T>
    constexpr bool is_swappable_v = is_swappable<T>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct is_nothrow_swappable_helper
        {
        private:
            template <class U, class = decltype(std::swap(declval<T&>(), declval<T&>()))>
            static bool_constant<noexcept(std::swap(declval<T&>(), declval<T&>()))> test(void *);

            template <class>
            static false_type test(...);

        public:
            using type = decltype(test<T>(nullptr));
        };
    }

    template <class T>
    struct is_nothrow_swappable: public detail::is_nothrow_swappable_helper<T>::type {};

    template <class T>
    constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // relationships between types
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T1, class T2>
    struct is_same: false_type {};

    template <class T>
    struct is_same<T, T>: true_type {};

    template <class T1, class T2>
    constexpr bool is_same_v = is_same<T1, T2>::value;

    //--------------------------------------------------------------------------------------------------

    template <class Base, class Derived>
    struct is_base_of: public bool_constant<__is_base_of(Base, Derived)> {};

    template <class Base, class Derived>
    constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class From, class To, bool = is_void<To>::value || is_array<To>::value || is_function<To>::value>
        struct is_convertible_helper: public bool_constant<is_void<To>::value> {};

        template <class From, class To>
        struct is_convertible_helper<From, To, false>
        {
        private:
            static void aux(To);

            template <class F, class = decltype(aux(declval<F>()))>
            static true_type test(void *);

            template <class>
            static false_type test(...);

        public:
            using type = decltype(test<From>(nullptr));
        };
    }

    template <class From, class To>
    struct is_convertible: public detail::is_convertible_helper<From, To>::type {};

    template <class From, class To>
    constexpr bool is_convertible_v = is_convertible<From, To>::value;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // type construction
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T>
    struct remove_const
    {
        using type = T;
    };

    template <class T>
    struct remove_const<const T>
    {
        using type = T;
    };

    template <class T>
    using remove_const_t = typename remove_const<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct remove_volatile
    {
        using type = T;
    };

    template <class T>
    struct remove_volatile<volatile T>
    {
        using type = T;
    };

    template <class T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct remove_cv
    {
        using type = T;
    };

    template <class T>
    struct remove_cv<const T>
    {
        using type = T;
    };

    template <class T>
    struct remove_cv<volatile T>
    {
        using type = T;
    };

    template <class T>
    struct remove_cv<const volatile T>
    {
        using type = T;
    };

    template <class T>
    using remove_cv_t = typename remove_cv<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct add_const
    {
        using type = const T;
    };

    template <class T>
    using add_const_t = typename add_const<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct add_volatile
    {
        using type = volatile T;
    };

    template <class T>
    using add_volatile_t = typename add_volatile<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct add_cv
    {
        using type = const volatile T;
    };

    template <class T>
    using add_cv_t = typename add_cv<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    using make_signed = std::make_signed<T>;

    template <class T>
    using make_signed_t = typename make_signed<T>::type;

    template <class T>
    using make_unsigned = std::make_unsigned<T>;

    template <class T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct remove_reference
    {
        using type = T;
    };

    template <class T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    template <class T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    template <class T>
    using remove_reference_t = typename remove_reference<T>::type;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T, class = void>
        struct add_lvalue_reference_helper
        {
            using type = T;
        };

        template <class T>
        struct add_lvalue_reference_helper<T, void_t<T&>>
        {
            using type = T&;
        };

        template <class T>
        using add_lvalue_reference_helper_t = typename add_lvalue_reference_helper<T>::type;
    }

    template <class T>
    struct add_lvalue_reference
    {
        using type = detail::add_lvalue_reference_helper_t<T>;
    };

    template <class T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T, class = void>
        struct add_rvalue_reference_helper
        {
            using type = T;
        };

        template <class T>
        struct add_rvalue_reference_helper<T, void_t<T&&>>
        {
            using type = T&&;
        };

        template <class T>
        using add_rvalue_reference_helper_t = typename add_rvalue_reference_helper<T>::type;
    }

    template <class T>
    struct add_rvalue_reference
    {
        using type = detail::add_rvalue_reference_helper_t<T>;
    };

    template <class T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T, class>
        struct remove_pointer_helper
        {
            using type = T;
        };

        template <class T, class U>
        struct remove_pointer_helper<T, U*>
        {
            using type = U;
        };

        template <class T, class U>
        using remove_pointer_helper_t = typename remove_pointer_helper<T, U>::type;
    }

    template <class T>
    struct remove_pointer
    {
        using type = detail::remove_pointer_helper_t<T, remove_cv_t<T>>;
    };

    template <class T>
    using remove_pointer_t = typename remove_pointer<T>::type;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T, class = void>
        struct add_pointer_helper
        {
            using type = T;
        };

        template <class T>
        struct add_pointer_helper<T, void_t<T*>>
        {
            using type = T*;
        };

        template <class T>
        using add_pointer_helper_t = typename add_pointer_helper<T>::type;
    }

    template <class T>
    struct add_pointer
    {
        using type = detail::add_pointer_helper_t<T>;
    };

    template <class T>
    struct add_pointer<T&>
    {
        using type = T*;
    };

    template <class T>
    struct add_pointer<T&&>
    {
        using type = T*;
    };

    template <class T>
    using add_pointer_t = typename add_pointer<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct remove_extent
    {
        using type = T;
    };

    template <class T>
    struct remove_extent<T[]>
    {
        using type = T;
    };

    template <class T, size_t N>
    struct remove_extent<T[N]>
    {
        using type = T;
    };

    template <class T>
    using remove_extent_t = typename remove_extent<T>::type;

    //--------------------------------------------------------------------------------------------------

    template <class T>
    struct remove_all_extents
    {
        using type = T;
    };

    template <class T>
    struct remove_all_extents<T[]>
    {
        using type = typename remove_all_extents<T>::type;
    };

    template <class T, size_t N>
    struct remove_all_extents<T[N]>
    {
        using type = typename remove_all_extents<T>::type;
    };

    template <class T>
    using remove_all_extents_t = typename remove_all_extents<T>::type;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class T>
        struct decay_helper: public conditional<is_const<const T>::value, remove_cv<T>, add_pointer<T>>::type {};

        template <class T>
        struct decay_helper<T[]>
        {
            using type = T*;
        };

        template <class T, size_t N>
        struct decay_helper<T[N]>
        {
            using type = T*;
        };
    }

    template <class T>
    struct decay
    {
        using type = typename detail::decay_helper<T>::type;
    };

    template <class T>
    struct decay<T&>
    {
        using type = typename decay<T>::type;
    };

    template <class T>
    struct decay<T&&>
    {
        using type = typename decay<T>::type;
    };

    template <class T>
    using decay_t = typename decay<T>::type;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // other traits
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <bool Cond, class T = void>
    struct enable_if {};

    template <class T>
    struct enable_if<true, T>
    {
        using type = T;
    };

    template <bool Cond, class T = void>
    using enable_if_t = typename enable_if<Cond, T>::type;

    //--------------------------------------------------------------------------------------------------

    template <bool Cond, class True, class False>
    struct conditional
    {
        using type = True;
    };

    template <class True, class False>
    struct conditional<false, True, False>
    {
        using type = False;
    };

    template <bool Cond, class True, class False>
    using conditional_t = typename conditional<Cond, True, False>::type;

    //--------------------------------------------------------------------------------------------------

    template <class... Ts>
    using common_type = std::common_type<Ts...>;

    template <class... Ts>
    using common_type_t = typename std::common_type<Ts...>::type;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <size_t Len>
        struct aligned_storage_helper
        {
            union type
            {
                unsigned char data[Len];
                struct __attribute__((__aligned__)) {} align;
            };
        };
    }

    template <size_t Len, size_t Align = alignof(typename detail::aligned_storage_helper<Len>::type)>
    struct aligned_storage
    {
        union type
        {
            unsigned char data[Len];
            struct __attribute__((__aligned__((Align)))) {} align;
        };
    };

    template <size_t Len, size_t Align = alignof(typename detail::aligned_storage_helper<Len>::tpye)>
    using aligned_storage_t = typename aligned_storage<Len, Align>::type;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class... Ts>
        struct aligned_union_helper
        {
            static const size_t size            = 0;
            static const size_t alignment_value = 0;
        };

        template <class T1, class... Ts>
        struct aligned_union_helper<T1, Ts...>
        {
            static const size_t size = (
                sizeof(T1) > aligned_union_helper<Ts...>::size) ?
                sizeof(T1) : aligned_union_helper<Ts...>::size;

            static const size_t alignment_value = (
                alignof(T1) > aligned_union_helper<Ts...>::alignment_value) ?
                alignof(T1) : aligned_union_helper<Ts...>::alignment_value;
        };
    }

    template <size_t Min_len, class... Ts>
    struct aligned_union
    {
    private:
        static const size_t Len = (
            Min_len > detail::aligned_union_helper<Ts...>::size) ?
            Min_len : detail::aligned_union_helper<Ts...>::size;
    public:
        static const size_t alignment_value = detail::aligned_union_helper<Ts...>::alignment_value;
        using type = aligned_storage_t<Len, alignment_value>;
    };

    template <size_t Min_len, class... Ts>
    using aligned_union_t = typename aligned_union<Min_len, Ts...>::type;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // combining type traits
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    namespace detail
    {
        template <class, class B1, class... Bs>
        struct conjunction_helper
        {
            using type = B1;
        };

        template <class B1, class B2, class... Bs>
        struct conjunction_helper<enable_if_t<B1::value>, B1, B2, Bs...>
        {
            using type = typename conjunction_helper<void, B2, Bs...>::type;
        };
    }

    template <class... Bs>
    struct conjunction: public detail::conjunction_helper<void, Bs...>::type {};

    template <>
    struct conjunction<>: public true_type {};

    template <class... Bs>
    constexpr bool conjunction_v = conjunction<Bs...>::value;

    //--------------------------------------------------------------------------------------------------

    namespace detail
    {
        template <class, class B1, class... Bs>
        struct disjunction_helper
        {
            using type = B1;
        };

        template <class B1, class B2, class... Bs>
        struct disjunction_helper<enable_if_t<!B1::value>, B1, B2, Bs...>
        {
            using type = typename disjunction_helper<void, B2, Bs...>::type;
        };
    }

    template <class... Bs>
    struct disjunction: public detail::disjunction_helper<void, Bs...>::type {};

    template <>
    struct disjunction<>: public false_type {};

    template <class... Bs>
    constexpr bool disjunction_v = disjunction<Bs...>::value;

    //--------------------------------------------------------------------------------------------------

    template <class B>
    struct negation: public bool_constant<!B::value> {};

    template <class B>
    constexpr bool negation_v = negation<B>::value;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // other utilities
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

//==================================================================================================

#endif // TYPE_TRAITS_HPP
