#include "type_traits.hpp"
#include <type_traits>
#include <cassert>

//==================================================================================================

class test_manager_t
{
// types
private:
    class test_scope_t
    {
    private:
        const char    *   header_;
        const char    *separator_;
        size_t      order_number_;
        size_t   ok_tests_number_;
        size_t fail_tests_number_;

    public:
        test_scope_t():
        header_           (nullptr),
        separator_        (nullptr),
        order_number_     (0),
        ok_tests_number_  (0),
        fail_tests_number_(0)
        {}

        void set_separator(const char *separator)
        {
            assert(separator);
            separator_ = separator;
        }

        void open(const char *header)
        {
            header_ = header;
            ++order_number_;

            assert(header_);
            assert(separator_);
            assert(ok_tests_number_ == 0);
            assert(fail_tests_number_ == 0);

            std::cout <<
                separator_                              << '\n' <<
                "(" << order_number_ << ") " << header_ << '\n' <<
                separator_                              << '\n';
        }

        void new_test(bool test_result, size_t src_line)
        {
            if (test_result) ++  ok_tests_number_;
            else             ++fail_tests_number_;

            std::cout <<
                "(" << ok_tests_number_ + fail_tests_number_ << ") " <<
                "[" << src_line                              << "] " <<
                (test_result ? "OK\n" : "FAILED\n");
        }

        void close()
        {
            if (!ok_tests_number_ && !fail_tests_number_)
                return;

            assert(header_);
            assert(separator_);

            std::cout <<
                separator_                                              << '\n' <<
                "(" << order_number_ << ") " << header_                 << '\n' <<
                "    TOTAL : " << ok_tests_number_ + fail_tests_number_ << '\n' <<
                "    OK    : " << ok_tests_number_                      << '\n' <<
                "    FAILED: " <<                    fail_tests_number_ << '\n' <<
                separator_                                              << '\n';

            ok_tests_number_   = 0;
            fail_tests_number_ = 0;
        }
    };

// static
private:
    static constexpr const char *separators[] =
    {
        "======================",
        "++++++++++++++++++++++",
        "----------------------",
    };

    static constexpr unsigned char scope_lvls_number = sizeof(separators) / sizeof(const char *);

// member functions
public:
    test_manager_t():
    scopes   (),
    cur_scope(0)
    {
        for (size_t i = 0; i < scope_lvls_number; ++i)
            scopes[i].set_separator(separators[i]);
    }

    ~test_manager_t()
    {
        while (cur_scope)
        {
            scopes[cur_scope].close();
            --cur_scope;
        }
        scopes[cur_scope].close();
    }

    void create_test_scope(const char *test_scope_header)
    {
        assert(test_scope_header);
        assert(cur_scope < scope_lvls_number);

        scopes[cur_scope].close();
        scopes[cur_scope].open(test_scope_header);
    }

    bool enter_test_scope()
    {
        assert(cur_scope < scope_lvls_number);

        if (cur_scope + 1 == scope_lvls_number)
            return false;

        ++cur_scope;
        return true;
    }

    bool leave_test_scope()
    {
        assert(cur_scope < scope_lvls_number);

        scopes[cur_scope].close();

        if (cur_scope == 0)
            return false;

        --cur_scope;
        return true;
    }

    void new_test(bool test_answer, bool right_answer, size_t src_line)
    {
        scopes[cur_scope].new_test(test_answer == right_answer, src_line);
    }

// member data
private:
    test_scope_t  scopes[scope_lvls_number];
    unsigned char cur_scope;
};

//==================================================================================================

static void test_type_traits                (test_manager_t &manager);

static void test_primary_type_categories    (test_manager_t &manager);
static void test_composite_type_categories  (test_manager_t &manager);
static void test_other_type_properties      (test_manager_t &manager);
static void test_specific_operations        (test_manager_t &manager);
static void test_relationships_between_types(test_manager_t &manager);
static void test_type_construction          (test_manager_t &manager);
static void test_other_traits               (test_manager_t &manager);
static void test_combining_type_traits      (test_manager_t &manager);

//==================================================================================================

int main()
{
    test_manager_t manager;
    test_type_traits(manager);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define TEST_TRAIT(trait_type)                              \
template <class Tested_type_1, class... Tested_types>       \
void test_##trait_type##_trait(test_manager_t &manager)     \
{                                                           \
    manager.new_test(                                       \
        my_std::trait_type<Tested_type_1>::value,           \
           std::trait_type<Tested_type_1>::value, __LINE__  \
    );                                                      \
                                                            \
    if constexpr (sizeof...(Tested_types))                  \
        test_##trait_type##_trait<Tested_type...>(manager); \
}

//--------------------------------------------------------------------------------------------------

static void test_type_traits(test_manager_t &manager)
{
    manager.create_test_scope("TYPE TRAITS");
    manager.enter_test_scope();

    test_primary_type_categories    (manager);
    test_composite_type_categories  (manager);
    test_other_type_properties      (manager);
    test_specific_operations        (manager);
    test_relationships_between_types(manager);
    test_type_construction          (manager);
    test_other_traits               (manager);
    test_combining_type_traits      (manager);

    manager.leave_test_scope();
}

//--------------------------------------------------------------------------------------------------

static void test_primary_type_categories(test_manager_t &manager)
{
    manager.create_test_scope("PRIMARY CATEGORIES");
    manager.enter_test_scope();

    manager.leave_test_scope();
}

static void test_composite_type_categories(test_manager_t &manager)
{
    manager.create_test_scope("COMPOSITE CATEGORIES");
    manager.enter_test_scope();

    manager.leave_test_scope();
}

static void test_other_type_properties(test_manager_t &manager)
{
    manager.create_test_scope("OTHER PROPERTIES");
    manager.enter_test_scope();

    manager.leave_test_scope();
}

static void test_specific_operations(test_manager_t &manager)
{
    manager.create_test_scope("SPECIFIC OPERATIONS");
    manager.enter_test_scope();

    manager.leave_test_scope();
}

static void test_relationships_between_types(test_manager_t &manager)
{
    manager.create_test_scope("RELATIONSHIPS BETWEEN TYPES");
    manager.enter_test_scope();

    manager.leave_test_scope();
}

static void test_type_construction(test_manager_t &manager)
{
    manager.create_test_scope("TYPE CONSTRUCTION");
    manager.enter_test_scope();

    manager.leave_test_scope();
}

static void test_other_traits(test_manager_t &manager)
{
    manager.create_test_scope("OTHER TRAITS");
    manager.enter_test_scope();

    manager.leave_test_scope();
}

static void test_combining_type_traits(test_manager_t &manager)
{
    manager.create_test_scope("COMBINING TRAITS");
    manager.enter_test_scope();

    manager.leave_test_scope();
}
