#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cstdio>

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
        explicit inline control_block(Args... args);
    };

// friends
    template <class... Args>
    friend shared_ptr<T> make_shared(Args... args)
    {
        control_block *data = new control_block(args...);
        return shared_ptr<T>(data);
    }

// member functions
public:
      inline ~shared_ptr();
    T inline *get();

private:
    explicit inline shared_ptr(control_block *data);

// member data
private:
    control_block *data;
};

//--------------------------------------------------------------------------------------------------

template <class T>
template <class... Args>
inline shared_ptr<T>::control_block::control_block(Args... args):
counter(1),
object (args...)
{}

//--------------------------------------------------------------------------------------------------

template <class T>
inline shared_ptr<T>::shared_ptr(control_block *data_):
data(data_)
{}

//--------------------------------------------------------------------------------------------------

template <class T>
inline shared_ptr<T>::~shared_ptr()
{
    if ((--data->counter) == 0)
        delete data;
}

//--------------------------------------------------------------------------------------------------

template <class T>
inline T *shared_ptr<T>::get()
{
    return data->object;
}

#endif // SHARED_PTR_HPP
