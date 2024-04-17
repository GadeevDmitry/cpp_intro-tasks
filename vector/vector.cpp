#include "vector.hpp"
#include <vector>
#include <algorithm>

//==================================================================================================

static void test_ctors();
static void test_assign_operators();
static void test_assign();
static void test_reserve();
static void test_clear();
static void test_insert_emplace_erase();
static void test_push_emplace_pop_back();
static void test_resize();
static void test_iterators();

int main()
{
    test_ctors();
    test_assign_operators();
    test_assign();
    test_reserve();
    test_clear();
    test_insert_emplace_erase();
    test_push_emplace_pop_back();
    test_resize();
    test_iterators();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class heavy_t
{
public:
    heavy_t():
    data_(nullptr),
    size_(0)
    {}

    heavy_t(size_t size):
    data_(new int[size]),
    size_(size)
    {}

    heavy_t(int *data, size_t size):
    data_(data),
    size_(size)
    {}

    heavy_t(const heavy_t &that):
    data_(new int[that.size_]),
    size_(that.size_)
    {
        for (size_t idx = 0; idx < size_; ++idx)
            data_[idx] = that.data_[idx];
    }

    heavy_t(heavy_t &&that):
    data_(that.data_),
    size_(that.size_)
    {
        that.data_ = nullptr;
        that.size_ = 0;
    }

    heavy_t(std::initializer_list<int> init_list):
    data_(new int[init_list.size()]),
    size_(init_list.size())
    {
        size_t idx = 0;
        for (auto it = init_list.begin(); it != init_list.end(); ++it, ++idx)
            data_[idx] = *it;
    }

    ~heavy_t()
    {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
    }

    heavy_t &operator =(const heavy_t &that)
    {
        if (this != &that)
        {
            this->~heavy_t();

            data_ = new int[that.size_];
            size_ = that.size_;
            for (size_t idx = 0; idx < size_; ++idx)
                data_[idx] = that.data_[idx];
        }
        return *this;
    }

    heavy_t &operator =(heavy_t &&that)
    {
        if (this != &that)
        {
            std::swap(data_, that.data_);
            std::swap(size_, that.size_);
        }
        return *this;
    }

    friend std::ostream &operator <<(std::ostream &output, const heavy_t &object);

private:
    int   *data_;
    size_t size_;
};

std::ostream &operator <<(std::ostream &output, const heavy_t &object)
{
    output << "size = " << object.size_ << " { ";

    if (object.data_)
        for (size_t i = 0; i < object.size_; ++i)
            output << object.data_[i] << " ";

    output << "}\n";

    return output;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static const char *bold_separator = "======================";
static const char *thin_separator = "----------------------";

static void print_bold_separator()
{
    std::cout << bold_separator << '\n';
}

static void print_thin_separator()
{
    std::cout << thin_separator << '\n';
}

static void print_test_header(const char *header)
{
    std::cout <<
        bold_separator << '\n' <<
        header << '\n' <<
        bold_separator << '\n';
}

static void print_subtest_header(const char *header)
{
    std::cout <<
        thin_separator << '\n' <<
        header << '\n' <<
        thin_separator << '\n';
}

#define PRINT_TEST_HEADER print_test_header(__PRETTY_FUNCTION__)

//--------------------------------------------------------------------------------------------------

static void test_ctors()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec;
        print_subtest_header("vector()");
        std::cout << vec;
    }
    {
        my_std::vector<heavy_t> vec(3, heavy_t{1, 2, 3, 4, 5});
        print_subtest_header("vector(size_type count, const_reference value)");
        std::cout << vec;
    }
    {
        my_std::vector<heavy_t> vec(4);
        print_subtest_header("vector(size_type count)");
        std::cout << vec;
    }
    {
        std::vector<heavy_t> std_vec{
            {1},
            {1, 2},
            {1, 2, 3},
            {1, 2, 3, 4},
            {1, 2, 3, 4, 5}};

        my_std::vector<heavy_t> vec_original(std_vec.begin(), std_vec.end());
        print_subtest_header("vector(InputIt first, InputIt last)");
        std::cout << vec_original;

        my_std::vector<heavy_t> vec_copy(vec_original);
        print_subtest_header("vector(const vector &that)");
        std::cout << "original " << vec_original;
        print_thin_separator();
        std::cout << "copy " << vec_copy;

        my_std::vector<heavy_t> vec_move(std::move(vec_original));
        print_subtest_header("vector(vector &&that)");
        std::cout << "original " << vec_original;
        print_thin_separator();
        std::cout << "move " << vec_move;
    }
    {
        my_std::vector<heavy_t> vec{
            {1, 2, 3, 4, 5, 6},
            {1, 2, 3, 4, 5, 6, 7},
            {1, 2, 3, 4, 5, 6, 7, 8}};
        print_subtest_header("vector(init_list)");
        std::cout << vec;
    }
}

//--------------------------------------------------------------------------------------------------

static void test_assign_operators()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec_zero;

        my_std::vector<heavy_t> vec_first{
            {1},
            {1, 2, 3, 4, 5}};

        my_std::vector<heavy_t> vec_second{
            {2},
            {1, 2, 3, 4, 5},
            {6, 7, 8}};

        my_std::vector<heavy_t> vec_third{
            {3},
            {1, 2, 3, 4, 5},
            {6, 7, 8},
            {9, 10}};

        std::cout << "zero " << vec_zero;
        print_thin_separator();
        std::cout << "first " << vec_first;
        print_thin_separator();
        std::cout << "second " << vec_second;
        print_thin_separator();
        std::cout << "third " << vec_third;

        vec_second = vec_first;
        print_subtest_header("operator =(const vector &that)");
        std::cout << "first (source) " << vec_first;
        print_thin_separator();
        std::cout << "second (destination) " << vec_second;

        vec_third = std::move(vec_first);
        print_subtest_header("operator =(vector &&that)");
        std::cout << "first (source) " << vec_first;
        print_thin_separator();
        std::cout << "third (destination) " << vec_third;

        vec_first = {
            {4},
            {11, 12, 13, 14, 15},
            {16, 17, 18, 19, 20}};
        print_subtest_header("operator =(init_list)");
        std::cout << "first (destination) " << vec_first;

        vec_second = vec_zero;
        print_subtest_header("operator =(const vector &zero)");
        std::cout << "zero (source) " << vec_zero;
        print_thin_separator();
        std::cout << "second (destination) " << vec_second;

        vec_third = std::move(vec_zero);
        print_subtest_header("operator =(vector &&zero)");
        std::cout << "zero (source) " << vec_zero;
        print_thin_separator();
        std::cout << "third (destination) " << vec_third;
    }
}

//--------------------------------------------------------------------------------------------------

static void test_assign()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec_original;
        my_std::vector<heavy_t> vec_duplicate(6, heavy_t{1, 2, 3, 4});

        std::cout << "original " << vec_original;
        print_thin_separator();
        std::cout << "duplicate " << vec_duplicate;

        vec_original.assign(5, heavy_t{0, 5, 10, 15, 20, 25});
        print_subtest_header("assign(size_type count, const_reference value)");
        std::cout << vec_original;

        vec_duplicate.assign(vec_original.begin(), vec_original.begin() + 2);
        print_subtest_header("assign(InputIt first, InputIt last)");
        std::cout << "duplicate " << vec_duplicate;

        vec_original.assign({
            {1},
            {1, 4},
            {1, 4, 9},
            {1, 4, 9, 16},
            {1, 4, 9, 16, 25}});
        print_subtest_header("assign(init_list)");
        std::cout << "original " << vec_original;
    }
}

//--------------------------------------------------------------------------------------------------

static void test_reserve()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec{{1}, {2, 3}, {4, 5, 6}};
        std::cout << vec;

        vec.reserve(3 * vec.capacity());
        print_subtest_header("reserve(3 * capacity)");
        std::cout << vec;

        vec.reserve(vec.capacity() / 2);
        print_subtest_header("reserve(capacity / 2)");
        std::cout << vec;
    }
}

//--------------------------------------------------------------------------------------------------

static void test_clear()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec{{1}, {2, 3}, {4, 5, 6}};
        std::cout << vec;

        vec.clear();
        print_subtest_header("clear()");
        std::cout << vec;
    }
}

//--------------------------------------------------------------------------------------------------

static void test_insert_emplace_erase()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec;
        std::cout << vec;

        vec.insert(vec.begin(), heavy_t{0, 0, 0, 0, 0, 0});
        print_subtest_header("insert(const_iterator pos, T &&value)");
        std::cout << vec;

        vec.insert(vec.end(), *vec.begin());
        print_subtest_header("insert(const_iterator pos, const_reference value)");
        std::cout << vec;

        vec.insert(vec.begin() + 1, 3, heavy_t{1, 1, 1, 1, 1});
        print_subtest_header("insert(const_iterator pos, size_type count, const_reference value)");
        std::cout << vec;

        std::vector<heavy_t> std_vec{{2}, {2}, {2}, {2}};
        vec.insert(vec.begin(), std_vec.begin(), std_vec.end());
        print_subtest_header("insert(const_iterator pos, InputIt first, InputIt last)");
        std::cout << vec;

        vec.insert(vec.begin() + 2, {{3}, {3}, {3}});
        print_subtest_header("insert(const_iterator pos, init_list)");
        std::cout << vec;

        vec.emplace(vec.end() - 1, 2);
        print_subtest_header("emplace(const_iterator pos, size_t size)");
        std::cout << vec;

        vec.erase(vec.begin() + 5);
        print_subtest_header("erase(const_iterator pos = #5)");
        std::cout << vec;

        vec.erase(vec.end() - 1);
        print_subtest_header("erase(const_iterator pos = #-1)");
        std::cout << vec;

        vec.erase(vec.begin() + 1, vec.end() - 3);
        print_subtest_header("erase(const_iterator first = #1, const_iterator last = #-3)");
        std::cout << vec;
    }
}

//--------------------------------------------------------------------------------------------------

static void test_push_emplace_pop_back()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec;
        std::cout << vec;

        for (int i = 0; i < 3; ++i)
        {
            vec.push_back(heavy_t{i});
            print_subtest_header("push_back(heavy_t &&that)");
            std::cout << vec;
        }

        for (int i = 0; i < 1; ++i)
        {
            vec.emplace_back(2);
            print_subtest_header("emplace_back(size_t)");
            std::cout << vec;
        }

        for (int i = 0; i < 4; ++i)
        {
            vec.pop_back();
            print_subtest_header("pop_back()");
            std::cout << vec;
        }
    }
}

//--------------------------------------------------------------------------------------------------

static void test_resize()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec;
        std::cout << vec;

        vec.resize(10);
        print_subtest_header("resize(size_type count > size)");
        std::cout << vec;

        vec.resize(10);
        print_subtest_header("resize(size_type count = size)");
        std::cout << vec;

        vec.resize(5);
        print_subtest_header("resize(size_type count < size)");
        std::cout << vec;

        vec.resize(10, heavy_t{10});
        print_subtest_header("resize(size_type count > size, const_reference)");
        std::cout << vec;
    }
}

//--------------------------------------------------------------------------------------------------

static void test_iterators()
{
    PRINT_TEST_HEADER;

    {
        my_std::vector<heavy_t> vec{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}};
        std::cout << vec;

        auto func = [](heavy_t &obj)
        {
            obj = {100, 200, 300};
        };

        std::for_each(vec.begin(), vec.end() - 1, func);
        print_test_header("std::for_each(...)");
        std::cout << vec;
    }
}
