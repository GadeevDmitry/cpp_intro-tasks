#ifndef TEST
#define TEST(type)
#endif

#ifndef TEST_CV
#define TEST_CV(type)
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// primary type categories
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TEST_CV(void)

//--------------------------------------------------------------------------------------------------

TEST_CV(bool)

TEST_CV(char)
TEST_CV(signed char)
TEST_CV(unsigned char)

TEST_CV(char16_t)
TEST_CV(char32_t)
TEST_CV(wchar_t)

TEST_CV(short)
TEST_CV(signed short)
TEST_CV(unsigned short)

TEST_CV(int)
TEST_CV(signed int)
TEST_CV(unsigned int)

TEST_CV(long)
TEST_CV(signed long)
TEST_CV(unsigned long)

TEST_CV(long long)
TEST_CV(signed long long)
TEST_CV(unsigned long long)

//--------------------------------------------------------------------------------------------------

TEST_CV(float)
TEST_CV(double)
TEST_CV(long double)

//--------------------------------------------------------------------------------------------------

TEST(int[])
TEST(int[][10])
TEST(int[][10][20])
TEST(int[][10][20][30])
TEST(int[][10][20][30][40])
TEST(int[][10][20][30][40][50])
