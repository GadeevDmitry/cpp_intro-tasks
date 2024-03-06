#include "shared_ptr.hpp"

//==================================================================================================

static bool test_get();
static bool test_unique();
static bool test_use_count();
static bool test_move();
static bool test_nullptr();
static bool test_array();

typedef bool (*test_func_t) ();
test_func_t RUNNING[] =
{
    test_get,
    test_unique,
    test_use_count,
    test_move,
    test_nullptr,
    test_array,
};

//==================================================================================================

int main()
{
    size_t all = sizeof(RUNNING) / sizeof(test_func_t);
    size_t successed = 0;

    for (test_func_t func : RUNNING)
    {
        if (func()) successed++;
    }

    printf("\n"
        "all      : %lu\n"
        "successed: %lu\n", all, successed);
}

//==================================================================================================

static bool check(bool expression, const char *func_name, const int line)
{
    if (!expression)
    {
        printf("FAIL on \"%s\":%d\n", func_name, line);
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------

#define CHECK(expression) { if (!check(expression, __PRETTY_FUNCTION__, __LINE__)) return false; }
#define SUCCESS()         { printf("OK \"%s\"\n", __PRETTY_FUNCTION__); return true; }

//--------------------------------------------------------------------------------------------------

static bool test_get()
{
    int value = rand();
    shared_ptr<int> ptr = make_shared<int>(value);
    CHECK(value == *ptr.get());

    SUCCESS();
}

//--------------------------------------------------------------------------------------------------

static bool test_unique()
{
    shared_ptr<int> ptr1 = make_shared<int>(5);
    CHECK(ptr1.unique());

    {
        shared_ptr<int> ptr2 = ptr1;
        CHECK(!ptr1.unique() && !ptr2.unique());
    }

    CHECK(ptr1.unique());

    SUCCESS();
}

//--------------------------------------------------------------------------------------------------

static bool test_use_count()
{
    shared_ptr<int> ptr1 = make_shared<int>(5);
    CHECK(ptr1.use_count() == 1);

    shared_ptr<int> ptr2 = ptr1;
    shared_ptr<int> ptr3 = ptr2;
    shared_ptr<int> ptr4 = ptr1;

    {
        shared_ptr<int> ptr5 = ptr4;
        CHECK(ptr4.use_count() == 5);
    }

    CHECK(ptr1.use_count() == 4);
    CHECK(ptr2.use_count() == 4);
    CHECK(ptr3.use_count() == 4);
    CHECK(ptr4.use_count() == 4);

    SUCCESS();
}

//--------------------------------------------------------------------------------------------------

static bool test_move()
{
    int value = rand();
    shared_ptr<int> ptr1 = make_shared<int>(value);
    shared_ptr<int> ptr2(std::move(ptr1));

    CHECK(ptr2.unique());
    CHECK(*ptr2.get() == value);

    ptr1 = std::move(ptr2);

    CHECK(ptr1.unique());
    CHECK(*ptr1.get() == value);

    SUCCESS();
}

//--------------------------------------------------------------------------------------------------

static bool test_nullptr()
{
    shared_ptr<int> null_ptr;

    CHECK(!null_ptr.unique());
    CHECK(null_ptr.get() == nullptr);
    CHECK(null_ptr.use_count() == 0);
    CHECK((bool) null_ptr == false);

    SUCCESS();
}

//--------------------------------------------------------------------------------------------------

static bool test_array()
{
    shared_ptr<int *> ptr = make_shared<int *>(new int [10] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    for (int i = 0; i < 10; ++ i)
        CHECK(ptr[i] == i);

    SUCCESS();
}
