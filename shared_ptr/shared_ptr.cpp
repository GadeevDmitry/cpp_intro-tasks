#include "shared_ptr.hpp"

//==================================================================================================

struct shared_t
{
    int *arr;

    shared_t():
    arr(new int[10])
    {
        for (int i = 0; i < 10; ++i) arr[i] = i;
    }

    ~shared_t()
    {
        delete[] arr;
    }
};

int main()
{
    shared_ptr<shared_t[]> ptr(new shared_t[5]);

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 10; ++j)
            printf("%d ", ptr[i].arr[j]);

        printf("\n");
    }
}
