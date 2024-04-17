#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <iostream>
#include <type_traits>

//==================================================================================================

namespace my_detail
{
    template <class T>
    struct default_deleter_t
    {
        void operator() (T *elem) { delete elem; }
    };

    template <class T>
    struct default_deleter_t<T[]>
    {
        void operator() (T *elem) { delete[] elem; }
    };

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T>
    using elem_t = std::remove_extent_t<T>;

    template <class T>
    class control_block_api
    {
    // member functions
    public:
        control_block_api():
        cnt(1)
        {}

        virtual ~control_block_api() {};
        virtual elem_t<T> *get_data() const = 0;

        void   inc_cnt()       { cnt++; }
        void   dec_cnt()       { cnt--; }
        size_t get_cnt() const { return cnt; }

    // member data
    private:
        size_t cnt;
    };

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T>
    class single_control_block_t: public control_block_api<T>
    {
    // assert
        static_assert(!std::is_array_v<T>);

    // member functions
    public:
        template <class ...Args>
        single_control_block_t(Args&&... args);

        virtual ~single_control_block_t() override;
        virtual elem_t<T> *get_data() const override;

    // member data
    private:
        T data;
    };

    //--------------------------------------------------------------------------------------------------

    template <class T>
    template <class... Args>
    single_control_block_t<T>::single_control_block_t(Args&&... args):
    control_block_api<T>(),
    data(std::forward(args)...)
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T>
    single_control_block_t<T>::~single_control_block_t()
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T>
    elem_t<T> *single_control_block_t<T>::get_data() const
    {
        return (elem_t<T> *) &data;
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class T, template <class> class Deleter = default_deleter_t>
    class separate_control_block_t: public control_block_api<T>
    {
    // member functions
    public:
        separate_control_block_t(elem_t<T> *data);
        separate_control_block_t(elem_t<T> *data, Deleter<T> del);

        virtual ~separate_control_block_t() override;
        virtual elem_t<T> *get_data() const override;

    private:
        elem_t <T> *data;
        Deleter<T>  deleter;
    };

    //--------------------------------------------------------------------------------------------------

    template <class T, template <class> class Deleter>
    separate_control_block_t<T, Deleter>::separate_control_block_t(elem_t<T> *data_):
    control_block_api<T>(),
    data(data_),
    deleter()
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T, template <class> class Deleter>
    separate_control_block_t<T, Deleter>::separate_control_block_t(elem_t<T> *data_, Deleter<T> del):
    control_block_api<T>(),
    data(data_),
    deleter(del)
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T, template <class> class Deleter>
    separate_control_block_t<T, Deleter>::~separate_control_block_t()
    {
        deleter(data);
    }

    //--------------------------------------------------------------------------------------------------

    template <class T, template <class> class Deleter>
    elem_t<T> *separate_control_block_t<T, Deleter>::get_data() const
    {
        return data;
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace my_std
{
    template <class T>
    class shared_ptr
    {
    // friends
        template <class U, class... Args>
        friend shared_ptr<U> make_shared(Args&&... args);

    // member functions
    public:
        shared_ptr(std::nullptr_t data = nullptr);
        shared_ptr(my_detail::elem_t<T> *elem);
        shared_ptr(const shared_ptr & that);
        shared_ptr(      shared_ptr &&that);

        template <template <class> class Deleter = my_detail::default_deleter_t>
        shared_ptr(my_detail::elem_t<T> *elem, Deleter<T> del);

        shared_ptr &operator =(const shared_ptr & that);
        shared_ptr &operator =(      shared_ptr &&that);

        ~shared_ptr();

        my_detail::elem_t<T> *get          ()                 const;
        bool                  unique       ()                 const;
        size_t                use_count    ()                 const;

        my_detail::elem_t<T> &operator *   ()                 const;
        my_detail::elem_t<T> *operator ->  ()                 const;
        my_detail::elem_t<T> &operator []  (const size_t idx) const;
                              operator bool()                 const;
    private:
        explicit shared_ptr(my_detail::control_block_api<T> *data);

    // member data
    private:
        my_detail::control_block_api<T> *data;
    };

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T>::shared_ptr(std::nullptr_t data_):
    data(data_)
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T>::shared_ptr(my_detail::elem_t<T> *elem):
    shared_ptr(elem, my_detail::default_deleter_t<T>())
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T>::shared_ptr(const shared_ptr &that):
    data(that.data)
    {
        data->inc_cnt();
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T>::shared_ptr(shared_ptr &&that):
    data(that.data)
    {
        that.data = nullptr;
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    template <template <class> class Deleter>
    shared_ptr<T>::shared_ptr(my_detail::elem_t<T> *elem, Deleter<T> del):
    data(new my_detail::separate_control_block_t<T, Deleter>(elem, del))
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T> &shared_ptr<T>::operator =(const shared_ptr<T> &that)
    {
        ~shared_ptr();
        data = that.data;
        data->inc_cnt();

        return *this;
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T> &shared_ptr<T>::operator =(shared_ptr<T> &&that)
    {
        std::swap(data, that.data);
        return *this;
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T>::~shared_ptr()
    {
        if (!data) return;

        data->dec_cnt();
        if (!data->get_cnt())
            delete data;
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    my_detail::elem_t<T> *shared_ptr<T>::get() const
    {
        if (!data) return nullptr;
        return data->get_data();
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    bool shared_ptr<T>::unique() const
    {
        if (!data) return false;
        return data->get_cnt() == 1;
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    size_t shared_ptr<T>::use_count() const
    {
        if (!data) return 0;
        return data->get_cnt();
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    my_detail::elem_t<T> &shared_ptr<T>::operator *() const
    {
        return *(data->get_data());
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    my_detail::elem_t<T> *shared_ptr<T>::operator ->() const
    {
        if (!data) return nullptr;
        return data->get_data();
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    my_detail::elem_t<T> &shared_ptr<T>::operator [](const size_t idx) const
    {
        return get()[idx];
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T>::operator bool() const
    {
        return data;
    }

    //--------------------------------------------------------------------------------------------------

    template <class T>
    shared_ptr<T>::shared_ptr(my_detail::control_block_api<T> *data_):
    data(data_)
    {}

    //--------------------------------------------------------------------------------------------------

    template <class T, class... Args>
    shared_ptr<T> make_shared(Args&&... args)
    {
        static_assert(!std::is_array_v<T>);

        my_detail::single_control_block_t<T> *data = new my_detail::single_control_block_t<T>(std::forward(args)...);
        return shared_ptr<T>(data);
    }
}

#endif // SHARED_PTR_HPP
