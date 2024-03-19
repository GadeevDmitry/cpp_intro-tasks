#ifndef LOG_INT_H
#define LOG_INT_H

#include <stdio.h>

//==================================================================================================

class log_int
{
// static
public:
    static void inline set_dot_file(FILE *dot_file);

private:
    static unsigned id_counter;

    static FILE    *dot_file;
    static unsigned dot_obj_counter;
    static unsigned dot_oper_counter;

    static void create_dot_obj_node (const log_int &obj, const char *color);
    static void create_dot_oper_node(const char *oper);

    static void create_dot_obj_obj_arrow (const unsigned  obj_from_cnt, const unsigned  obj_to_cnt, const char *label, const char *style, const char *color);
    static void create_dot_obj_oper_arrow(const unsigned  obj_from_cnt, const unsigned oper_to_cnt, const char *label, const char *style, const char *color);
    static void create_dot_oper_obj_arrow(const unsigned oper_from_cnt, const unsigned  obj_to_cnt, const char *label, const char *style, const char *color);

// member functions
public:
    log_int(int value);
    log_int(int value, int real);
    log_int(const log_int & init);
    log_int(      log_int &&init);

    log_int &operator ++();
    log_int &operator --();
    log_int &operator ++(int);
    log_int &operator --(int);

    log_int  operator - () const;
    log_int  operator ~ () const;

    bool     operator ! () const;

    log_int &operator  =(const log_int &sec);
    log_int &operator +=(const log_int &sec);
    log_int &operator -=(const log_int &sec);
    log_int &operator *=(const log_int &sec);
    log_int &operator /=(const log_int &sec);
    log_int &operator %=(const log_int &sec);
    log_int &operator ^=(const log_int &sec);
    log_int &operator &=(const log_int &sec);
    log_int &operator |=(const log_int &sec);

    log_int  operator + (const log_int &sec) const;
    log_int  operator - (const log_int &sec) const;
    log_int  operator * (const log_int &sec) const;
    log_int  operator / (const log_int &sec) const;
    log_int  operator % (const log_int &sec) const;
    log_int  operator ^ (const log_int &sec) const;
    log_int  operator & (const log_int &sec) const;
    log_int  operator | (const log_int &sec) const;

    bool     operator < (const log_int &sec) const;
    bool     operator > (const log_int &sec) const;
    bool     operator <=(const log_int &sec) const;
    bool     operator >=(const log_int &sec) const;
    bool     operator ==(const log_int &sec) const;
    bool     operator !=(const log_int &sec) const;
    bool     operator &&(const log_int &sec) const;
    bool     operator ||(const log_int &sec) const;

// member data
private:
    const unsigned id;
    unsigned  dot_cnt;
    int         value;
};

//--------------------------------------------------------------------------------------------------

inline void log_int::set_dot_file(FILE *dot_file_)
{
    dot_file = dot_file_;
}

#endif // LOG_INT_H
