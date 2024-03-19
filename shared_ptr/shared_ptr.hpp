#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <iostream>
#include <type_traits>

//==================================================================================================

template <class T>
class shared_ptr
{
// types
private:
    template <bool = std::is_array_v<T>>
    struct default_deleter_t;

    template <>
    struct default_deleter_t<false>
    {
        void operator() (T *elem) { delete elem; }
    };

    template <>
    struct default_deleter_t<true>
    {
        void operator() (T *elem) { delete[] elem; }
    };

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    using elem_t = std::remove_extent_t<T>;

    class control_block_api
    {
    // member functions
    public:
        control_block_api():
        cnt(1)
        {}

        virtual ~control_block_api() {};
        virtual elem_t *get_data() const = 0;

        void   inc_cnt()       { cnt++; }
        void   dec_cnt()       { cnt--; }
        size_t get_cnt() const { return cnt; }

    // member data
    private:
        size_t cnt;
    };

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class = std::enable_if<!std::is_array_v<T>>>
    class single_control_block_t: public control_block_api
    {
    // member functions
    public:
        template <class... Args>
        single_control_block_t(Args&&... args):
        control_block_api(),
        data(std::forward(args)...)
        {}

        virtual ~single_control_block_t() override {}
        virtual elem_t *get_data() const  override { return (elem_t *) &data; }

    // member data
    private:
        T data;
    };

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template <class Deleter = default_deleter_t<>>
    class separate_control_block_t: public control_block_api
    {
    // member functions
    public:
        separate_control_block_t(elem_t *data_):
        control_block_api(),
        data(data_),
        deleter()
        {}

        separate_control_block_t(elem_t *data_, Deleter del):
        control_block_api(),
        data(data_),
        deleter(del)
        {}

        virtual ~separate_control_block_t() override { deleter(data); }
        virtual elem_t *get_data() const    override { return data; }

    private:
        elem_t *data;
        Deleter deleter;
    };

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// friends
    template <class U, class... Args>
    friend shared_ptr<U> make_shared(Args&&... args);

// member functions
public:
    shared_ptr(std::nullptr_t data = nullptr);
    shared_ptr(elem_t *elem);
    shared_ptr(const shared_ptr & that);
    shared_ptr(      shared_ptr &&that);

    template <template <class> class Deleter>
    shared_ptr(elem_t *elem, Deleter<T> del);

    shared_ptr &operator =(const shared_ptr & that);
    shared_ptr &operator =(      shared_ptr &&that);

   ~shared_ptr();

    elem_t *get          ()                 const;
    bool    unique       ()                 const;
    size_t  use_count    ()                 const;

    elem_t &operator *   ()                 const;
    elem_t *operator ->  ()                 const;
    elem_t &operator []  (const size_t idx) const;
            operator bool()                 const;

private:
    explicit shared_ptr(control_block_api *data);

// member data
private:
    control_block_api *data;
};

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T>::shared_ptr(std::nullptr_t data_):
data(data_)
{}

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T>::shared_ptr(elem_t *elem):
data(new separate_control_block_t(elem))
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
shared_ptr<T>::shared_ptr(elem_t *elem, Deleter<T> del):
data(new separate_control_block_t<Deleter<T>>(elem, del))
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
typename shared_ptr<T>::elem_t *shared_ptr<T>::get() const
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
typename shared_ptr<T>::elem_t &shared_ptr<T>::operator *() const
{
    return *(data->get_data());
}

//--------------------------------------------------------------------------------------------------

template <class T>
typename shared_ptr<T>::elem_t *shared_ptr<T>::operator ->() const
{
    if (!data) return nullptr;
    return data->get_data();
}

//--------------------------------------------------------------------------------------------------

template <class T>
typename shared_ptr<T>::elem_t &shared_ptr<T>::operator [](const size_t idx) const
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
shared_ptr<T>::shared_ptr(control_block_api *data_):
data(data_)
{}

//--------------------------------------------------------------------------------------------------

template <class T, class... Args>
shared_ptr<T> make_shared(Args&&... args)
{
    static_assert(!std::is_array_v<T>);

    typename shared_ptr<T>::single_control_block_t *data = new typename shared_ptr<T>::single_control_block_t(std::forward(args)...);
    return shared_ptr<T>(data);
}

#endif // SHARED_PTR_HPP
