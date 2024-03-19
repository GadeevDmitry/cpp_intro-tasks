#include <stdio.h>
#include <stdlib.h>

#include "log_int.h"

#define REAL 1

typedef log_int (*func_2_arg) (const log_int &, const log_int &);
typedef log_int (*func_3_arg) (const log_int &, const log_int &, const log_int &);

//==================================================================================================

static const char *DOT_FILE_NAME = "log.dot";
static FILE       *DOT_FILE      = nullptr;

static void  open_dot_file();
static void close_dot_file(const char *png_file_name);

static void func_2_arg_handler(func_2_arg call, const char *png_file_name);
static void func_3_arg_handler(func_3_arg call, const char *png_file_name);


static log_int max        (const log_int &a, const log_int &b);
static log_int sum_of_2   (const log_int &a, const log_int &b);
static log_int sum_of_3   (const log_int &a, const log_int &b, const log_int &c);
static log_int sum_by_self(const log_int &a, const log_int &b);

static log_int &the_ladder(log_int &a, log_int &b, log_int &c);


//==================================================================================================

int main()
{

    func_2_arg_handler(max        , "img/max.png");
    func_2_arg_handler(sum_of_2   , "img/sum_of_2.png");
    func_2_arg_handler(sum_by_self, "img/sum_by_self.png");

    func_3_arg_handler(sum_of_3   , "img/sum_of_3.png");


    open_dot_file();

    log_int a(rand() % 10, REAL);
    log_int b(rand() % 10, REAL);
    log_int c(rand() % 10, REAL);

    the_ladder(a, b, c);

    close_dot_file("img/the_ladder.png");
}

//--------------------------------------------------------------------------------------------------

static void func_2_arg_handler(func_2_arg call, const char *png_file_name)
{
    open_dot_file();

    log_int a(rand() % 10, REAL);
    log_int b(rand() % 10, REAL);

    call(a, b);

    close_dot_file(png_file_name);
}

//--------------------------------------------------------------------------------------------------

static void func_3_arg_handler(func_3_arg call, const char *png_file_name)
{
    open_dot_file();

    log_int a(rand() % 10, REAL);
    log_int b(rand() % 10, REAL);
    log_int c(rand() % 10, REAL);

    call(a, b, c);

    close_dot_file(png_file_name);
}

//--------------------------------------------------------------------------------------------------

static void open_dot_file()
{
    DOT_FILE = fopen(DOT_FILE_NAME, "w");
    fprintf(DOT_FILE, "digraph G\n{\nrankdir=LR;\n");

    log_int::set_dot_file(DOT_FILE);
}

//--------------------------------------------------------------------------------------------------

static void close_dot_file(const char *png_file_name)
{
    fprintf(DOT_FILE, "}\n");
    fclose (DOT_FILE);

    char command[256] = "";
    snprintf(command, 255, "dot -Tpng %s > %s", DOT_FILE_NAME, png_file_name);
    system(command);
}

//--------------------------------------------------------------------------------------------------

static log_int max(const log_int &a, const log_int &b)
{
    return (a > b) ? a : b; 
}

//--------------------------------------------------------------------------------------------------

static log_int sum_of_2(const log_int &a, const log_int &b)
{
    return a + b;
}

//--------------------------------------------------------------------------------------------------

static log_int sum_of_3(const log_int &a, const log_int &b, const log_int &c)
{
    return a + b + c;
}

//--------------------------------------------------------------------------------------------------

static log_int self(const log_int &a)
{
    return a;
}

static log_int sum_by_self(const log_int &a, const log_int &b)
{
    return self(a) + self(b);
}

//--------------------------------------------------------------------------------------------------

static log_int &the_ladder(log_int &a, log_int &b, log_int &c)
{
    return a += (b += c++);
}
