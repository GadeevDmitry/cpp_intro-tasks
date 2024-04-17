#include "function.hpp"

struct tested_t
{
private:
    int value = 5;
public:
    int get() { return value; }
};

int main()
{
    my_std::function<int(tested_t *)> member(&tested_t::get);
    tested_t aboba;
    printf("member function: %d\n", member(&aboba));

    my_std::function<int(int, int)> lambda([](int a, int b) { return a + b;});
    printf("lambda function: (%d, %d) -> %d\n", 20, 30, lambda(20, 30));
}
