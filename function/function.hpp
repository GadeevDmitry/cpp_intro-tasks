#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <functional>

//==================================================================================================

namespace my_std
{
    template <class>
    class function;

    template <class Return, class... Args>
    class function<Return(Args...)>
    {
    // member functions
    public:
        function(std::nullptr_t functor = nullptr);
        function(const function & that);
        function(      function &&that);

        template <class F>
        function(F f);

        function &operator =(const function & that);
        function &operator =(      function &&that);

        ~function();

        Return operator ()  (Args&&... args) const;
               operator bool()               const;

    // types
    private:
        class functor_api
        {
        // member functions
        public:
            virtual             ~functor_api()                         {}
            virtual Return       operator   ()(Args&&... args) const = 0;
            virtual functor_api *copy       ()                 const = 0;
        };

        template <class F>
        class functor_t: public functor_api
        {
        // member functions
        public:
            functor_t();
            functor_t(F target);

            virtual             ~functor_t()                       override {}
            virtual Return       operator ()(Args&&... args) const override;
            virtual functor_api *copy     ()                 const override;

        // member data
        private:
            F target;
        };

    // member data
    private:
        functor_api *functor;
    };

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    function<Return(Args...)>::function(std::nullptr_t functor_):
    functor(functor_)
    {}

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    function<Return(Args...)>::function(const function &that):
    functor(that.functor->copy())
    {}

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    function<Return(Args...)>::function(function &&that):
    functor(that.functor)
    {
        that.functor = nullptr;
    }

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    template <class F>
    function<Return(Args...)>::function(F f):
    functor(new functor_t<F>(f))
    {}

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    function<Return(Args...)> &function<Return(Args...)>::operator =(const function<Return(Args...)> &that)
    {
        ~function();
        functor = that.functor->copy();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    function<Return(Args...)> &function<Return(Args...)>::operator =(function<Return(Args...)> &&that)
    {
        std::swap(functor, that.functor);
        return *this;
    }

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    function<Return(Args...)>::~function()
    {
        delete functor;
    }

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    Return function<Return(Args...)>::operator()(Args&&... args) const
    {
        return (*functor)(std::forward<Args>(args)...);
    }

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    function<Return(Args...)>::operator bool() const
    {
        return functor;
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class Return, class... Args>
    template <class F>
    function<Return(Args...)>::functor_t<F>::functor_t():
    target()
    {}

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    template <class F>
    function<Return(Args...)>::functor_t<F>::functor_t(F target_):
    target(target_)
    {}

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    template <class F>
    Return function<Return(Args...)>::functor_t<F>::operator() (Args&&... args) const
    {
        return std::invoke(target, args...);
    }

    //--------------------------------------------------------------------------------------------------

    template <class Return, class... Args>
    template <class F>
    typename function<Return(Args...)>::functor_api *function<Return(Args...)>::functor_t<F>::copy() const
    {
        return new functor_t<F>(target);
    }
}

#endif // FUNCTION_HPP
