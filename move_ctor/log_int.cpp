#include "log_int.h"

#define NORM_OBJ_COLOR "lime"
#define TEMP_OBJ_COLOR "darkorange1"
#define COPY_OBJ_COLOR "red"
#define REPL_OBJ_COLOR "purple"

#define  DEFAULT_COLOR "black"

//==================================================================================================

unsigned log_int::id_counter = 1;

FILE    *log_int::dot_file =   nullptr;
unsigned log_int::dot_obj_counter  = 0;
unsigned log_int::dot_oper_counter = 0;

//==================================================================================================

void log_int::create_dot_obj_node(const log_int &obj, const char *color)
{
    fprintf(
        dot_file, "obj_%u[shape=record, style=\"rounded, filled\", fillcolor=\"%s\", label=\"id: %u | %d\"];\n",
        obj.dot_cnt, color, obj.id, obj.value);
}

//--------------------------------------------------------------------------------------------------

void log_int::create_dot_oper_node(const char *oper)
{
    fprintf(
        dot_file, "oper%u[shape=record, style=\"rounded, filled\", fillcolor=\"aqua\", label=\"%s\"];\n",
        dot_oper_counter++, oper);
}

//--------------------------------------------------------------------------------------------------

void log_int::create_dot_obj_obj_arrow(
    const unsigned obj_from_cnt, const unsigned obj_to_cnt,
    const char *label, const char *style, const char *color)
{
    fprintf(
        dot_file, "obj_%u->obj_%u[style=\"%s\", arrowhead=\"normal\", label=\"%s\", color=\"%s\"];\n",
        obj_from_cnt, obj_to_cnt, style, label, color);
}

//--------------------------------------------------------------------------------------------------

void log_int::create_dot_obj_oper_arrow(
    const unsigned obj_from_cnt, const unsigned oper_to_cnt,
    const char *label, const char *style, const char *color)
{
    fprintf(
        dot_file, "obj_%u->oper%u[style=\"%s\", arrowhead=\"normal\", label=\"%s\", color=\"%s\"];\n",
        obj_from_cnt, oper_to_cnt, style, label, color);
}

//--------------------------------------------------------------------------------------------------

void log_int::create_dot_oper_obj_arrow(
    const unsigned oper_from_cnt, const unsigned obj_to_cnt,
    const char *label, const char *style, const char *color)
{
    fprintf(
        dot_file, "oper%u->obj_%u[style=\"%s\", arrowhead=\"normal\", label=\"%s\", color=\"%s\"];\n",
        oper_from_cnt, obj_to_cnt, style, label, color);
}

//--------------------------------------------------------------------------------------------------

log_int::log_int(int value_):
id     (0),
dot_cnt(dot_obj_counter++),
value  (value_)
{
    create_dot_obj_node(*this, TEMP_OBJ_COLOR);
}

//--------------------------------------------------------------------------------------------------

log_int::log_int(int value_, int real):
id     (id_counter++),
dot_cnt(dot_obj_counter++),
value  (value_)
{
    create_dot_obj_node(*this, NORM_OBJ_COLOR);
}

//--------------------------------------------------------------------------------------------------

log_int::log_int(const log_int &init):
id     (0),
dot_cnt(dot_obj_counter++),
value  (init.value)
{
    create_dot_obj_node(*this, COPY_OBJ_COLOR);
    create_dot_obj_obj_arrow(init.dot_cnt, dot_cnt, "&", "", COPY_OBJ_COLOR);
}

//--------------------------------------------------------------------------------------------------

log_int::log_int(log_int &&init):
id     (id_counter++),
dot_cnt(dot_obj_counter++),
value  (init.value)
{
    create_dot_obj_node(*this, REPL_OBJ_COLOR);
    create_dot_obj_obj_arrow(init.dot_cnt, dot_cnt, "&&", "", REPL_OBJ_COLOR);
}

//--------------------------------------------------------------------------------------------------

#define PREFIX_OPERATOR(op)                                                                 \
    log_int &log_int::operator op()                                                         \
    {                                                                                       \
        op value;                                                                           \
                                                                                            \
        unsigned old_dot_cnt = dot_cnt;                                                     \
        dot_cnt = dot_obj_counter++;                                                        \
                                                                                            \
        create_dot_obj_node(*this, NORM_OBJ_COLOR);                                         \
        create_dot_obj_obj_arrow(old_dot_cnt, dot_cnt, "prefix" #op, "", NORM_OBJ_COLOR);   \
                                                                                            \
        return *this;                                                                       \
    }

PREFIX_OPERATOR(++)
PREFIX_OPERATOR(--)

//--------------------------------------------------------------------------------------------------

#define POSTFIX_OPERATOR(op)                                                                \
    log_int &log_int::operator op(int)                                                      \
    {                                                                                       \
        value op;                                                                           \
                                                                                            \
        unsigned old_dot_cnt = dot_cnt;                                                     \
        dot_cnt = dot_obj_counter++;                                                        \
                                                                                            \
        create_dot_obj_node(*this, NORM_OBJ_COLOR);                                         \
        create_dot_obj_obj_arrow(old_dot_cnt, dot_cnt, "postfix" #op, "", NORM_OBJ_COLOR);  \
                                                                                            \
        return *this;                                                                       \
    }

POSTFIX_OPERATOR(++)
POSTFIX_OPERATOR(--)

//--------------------------------------------------------------------------------------------------

#define UNARY_ARITHMETIC_OPERATOR(op)                                                   \
    log_int log_int::operator op() const                                                \
    {                                                                                   \
        log_int res = op value;                                                         \
        create_dot_obj_obj_arrow(dot_cnt, res.dot_cnt, "unary" #op, "", TEMP_OBJ_COLOR);\
        return res;                                                                     \
    }

UNARY_ARITHMETIC_OPERATOR(-)
UNARY_ARITHMETIC_OPERATOR(~)

//--------------------------------------------------------------------------------------------------

#define UNARY_LOGICAL_OPERATOR(op)      \
    bool log_int::operator op() const   \
    {                                   \
        bool res = op value;            \
        return res;                     \
    }

UNARY_LOGICAL_OPERATOR(!)

//--------------------------------------------------------------------------------------------------

#define ASSIGN_OPERATOR(op)                                                                         \
    log_int &log_int::operator op(const log_int &sec)                                               \
    {                                                                                               \
        value op sec.value;                                                                         \
                                                                                                    \
        unsigned old_dot_cnt = dot_cnt;                                                             \
        dot_cnt = dot_obj_counter++;                                                                \
                                                                                                    \
        create_dot_oper_node(#op);                                                                  \
        create_dot_obj_oper_arrow(old_dot_cnt, dot_oper_counter - 1, "", "bold"  , DEFAULT_COLOR);  \
        create_dot_obj_oper_arrow(sec.dot_cnt, dot_oper_counter - 1, "", "dashed", DEFAULT_COLOR);  \
                                                                                                    \
        create_dot_obj_node(*this, NORM_OBJ_COLOR);                                                 \
        create_dot_oper_obj_arrow(dot_oper_counter - 1, dot_cnt, "", "", NORM_OBJ_COLOR);           \
                                                                                                    \
        return *this;                                                                               \
    }

ASSIGN_OPERATOR(=)
ASSIGN_OPERATOR(+=)
ASSIGN_OPERATOR(-=)
ASSIGN_OPERATOR(*=)
ASSIGN_OPERATOR(/=)
ASSIGN_OPERATOR(%=)
ASSIGN_OPERATOR(&=)
ASSIGN_OPERATOR(|=)

//--------------------------------------------------------------------------------------------------

#define BINARY_ARITHMETIC_OPERATOR(op)                                                          \
    log_int log_int::operator op(const log_int &sec) const                                      \
    {                                                                                           \
        log_int res = value op sec.value;                                                       \
                                                                                                \
        create_dot_oper_node(#op);                                                              \
        create_dot_obj_oper_arrow(    dot_cnt, dot_oper_counter - 1, "", "", DEFAULT_COLOR);    \
        create_dot_obj_oper_arrow(sec.dot_cnt, dot_oper_counter - 1, "", "", DEFAULT_COLOR);    \
                                                                                                \
        create_dot_oper_obj_arrow(dot_oper_counter - 1, res.dot_cnt, "", "", TEMP_OBJ_COLOR);   \
                                                                                                \
        return res;                                                                             \
    }

BINARY_ARITHMETIC_OPERATOR(+)
BINARY_ARITHMETIC_OPERATOR(-)
BINARY_ARITHMETIC_OPERATOR(*)
BINARY_ARITHMETIC_OPERATOR(/)
BINARY_ARITHMETIC_OPERATOR(%)
BINARY_ARITHMETIC_OPERATOR(^)
BINARY_ARITHMETIC_OPERATOR(&)
BINARY_ARITHMETIC_OPERATOR(|)

//--------------------------------------------------------------------------------------------------

#define BINARY_LOGICAL_OPERATOR(op)                     \
    bool log_int::operator op(const log_int &sec) const \
    {                                                   \
        bool res = value op sec.value;                  \
        return res;                                     \
    }

BINARY_LOGICAL_OPERATOR(<)
BINARY_LOGICAL_OPERATOR(>)
BINARY_LOGICAL_OPERATOR(<=)
BINARY_LOGICAL_OPERATOR(>=)
BINARY_LOGICAL_OPERATOR(==)
BINARY_LOGICAL_OPERATOR(!=)
BINARY_LOGICAL_OPERATOR(&&)
BINARY_LOGICAL_OPERATOR(||)
