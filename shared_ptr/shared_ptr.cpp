#include "shared_ptr.hpp"

//==================================================================================================

int main()
{
    shared_ptr<int> ptr = make_shared<int>(5);
    printf("%p\n", &ptr);
}
