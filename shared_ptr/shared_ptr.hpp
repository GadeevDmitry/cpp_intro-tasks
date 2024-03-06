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
    struct control_block
    {
        size_t counter;
        T       object;

        template <class... Args>
        explicit control_block(Args... args);
    };

    template <class = std::enable_if_t<std::is_pointer_v<T>>>
    using T_elem = std::remove_pointer_t<T>;

// friends
    template <class U, class... Args>
    friend shared_ptr<U> make_shared(Args... args);

// member functions
public:
     shared_ptr(std::nullptr_t value = nullptr);
     shared_ptr(const shared_ptr & that);
     shared_ptr(      shared_ptr &&that);
    ~shared_ptr();

    shared_ptr &operator =(const shared_ptr & that);
    shared_ptr &operator =(      shared_ptr &&that);

    T      *get()       const;
    bool    unique()    const;
    size_t  use_count() const;

    T &operator *   () const;
    T *operator ->  () const;
       operator bool() const;

    template <class = std::enable_if_t<std::is_pointer_v<T>>>
    T_elem<> &operator[](size_t idx) const;

private:
    explicit shared_ptr(control_block *data);

// member data
private:
    control_block *data;
};

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T>::shared_ptr(control_block *data_):
data(data_)
{}

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T>::shared_ptr(std::nullptr_t value):
data(value)
{}

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T>::shared_ptr(const shared_ptr<T> &that):
data(that.data)
{
    if (data) data->counter++;
}

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T>::shared_ptr(shared_ptr<T> &&that):
data(that.data)
{
    that.data = nullptr;
}

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T> &shared_ptr<T>::operator =(const shared_ptr<T> &that)
{
    ~shared_ptr();
    shared_ptr(that.data);
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
    if (data && (--data->counter) == 0)
        delete data;
}

//--------------------------------------------------------------------------------------------------

template <class T>
T *shared_ptr<T>::get() const
{
    if (!data) return nullptr;
    return &data->object;
}

//--------------------------------------------------------------------------------------------------

template <class T>
bool shared_ptr<T>::unique() const
{
    if (!data) return false;
    return data->counter == 1;
}

//--------------------------------------------------------------------------------------------------

template <class T>
size_t shared_ptr<T>::use_count() const
{
    if (!data) return 0;
    return data->counter;
}

//--------------------------------------------------------------------------------------------------

template <class T>
T &shared_ptr<T>::operator *() const
{
    return data->object;
}

//--------------------------------------------------------------------------------------------------

template <class T>
T *shared_ptr<T>::operator ->() const
{
    if (!data) return nullptr;
    return &data->object;
}

//--------------------------------------------------------------------------------------------------

template <class T>
shared_ptr<T>::operator bool() const
{
    return data;
}

//--------------------------------------------------------------------------------------------------

template <class T>
template <class>
typename shared_ptr<T>::template T_elem<> &shared_ptr<T>::operator[](const size_t idx) const
{
    return data->object[idx];
}

//--------------------------------------------------------------------------------------------------

template <class T>
template <class... Args>
shared_ptr<T>::control_block::control_block(Args... args):
counter(1),
object (args...)
{}

//--------------------------------------------------------------------------------------------------

template <class T, class... Args>
shared_ptr<T> make_shared(Args... args)
{
    typename shared_ptr<T>::control_block *data = new typename shared_ptr<T>::control_block(args...);
    return shared_ptr<T>(data);
}

#endif // SHARED_PTR_HPP
