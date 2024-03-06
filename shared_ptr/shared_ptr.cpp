#include "shared_ptr.hpp"

//==================================================================================================

int main()
{
    shared_ptr<int> ptr = shared_ptr<int>::make_shared(5);
    printf("%p\n", &ptr);
}
