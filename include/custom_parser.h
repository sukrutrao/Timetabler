#ifndef CUSTOM_PARSER_H
#define CUSTOM_PARSER_H

#include <tao/pegtl.hpp>
#include <string>
#include <vector>
#include "clauses.h"

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

using namespace pegtl;

template <typename Rule>
struct action : nothing<Rule> {};

struct Object {
    bool isNot;
    bool takeFieldValues;
    bool takeClassValues;
    bool takeSlotValues;
    int integer;
    TimeTabler* timeTabler;

    std::string fieldType;
    std::vector<int> fieldValues;
    std::vector<int> classValues;
    std::vector<int> slotValues;

    Clauses constraint;
    std::vector<Clauses> constraintAnds;
    std::vector<Clauses> constraintVals;    
};

struct integer : seq<range<'1', '9'>, star<digit>> {};
template <> struct action<integer> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct instr : TAOCPP_PEGTL_KEYWORD("IN") {};
template <> struct action<instr> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct notstr : TAOCPP_PEGTL_KEYWORD("NOT") {};
template <> struct action<notstr> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct andstr : TAOCPP_PEGTL_KEYWORD("AND") {};
template <> struct action<andstr> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct orstr : TAOCPP_PEGTL_KEYWORD("OR") {};
template <> struct action<orstr> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct classroomstr : TAOCPP_PEGTL_KEYWORD("CLASSROOM") {};
template <> struct action<classroomstr> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct slotstr : TAOCPP_PEGTL_KEYWORD("SLOT") {};
template <> struct action<slotstr> {
    template <typename Input> static void apply(const Input&, Object &);
};

struct fieldtype : identifier {};
template <> struct action<fieldtype> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct value : plus<not_one<','>> {};
template <> struct action<value> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct values : seq<pad<one<'{'>,space>,list<value,one<','>,space>,pad<one<'}'>,space>> {};
template <> struct action<values> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct classroomdecl : seq<pad<classroomstr, space>, values> {};
template <> struct action<classroomdecl> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct slotdecl : seq<pad<slotstr, space>, values> {};
template <> struct action<slotdecl> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct constraint_expr : seq<pad<fieldtype,space>,values,opt<notstr>,pad<instr,space>,sor<classroomdecl,slotdecl>> {};
template <> struct action<constraint_expr> {
    template <typename Input> static void apply(const Input&, Object &);
};

struct constraint_or;
struct constraint_braced : seq<opt<notstr>, if_must<pad<one<'('>,space>,constraint_or,pad<one<')'>,space>>> {};
template <> struct action<constraint_braced> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct constraint_not : seq<pad<notstr, space>, constraint_braced> {};
template <> struct action<constraint_not> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct constraint_val : sor<constraint_expr, constraint_not, constraint_braced> {};
template <> struct action<constraint_val> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct constraint_and : list<constraint_val, andstr, space> {};
template <> struct action<constraint_and> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct constraint_or : list<constraint_and, orstr, space> {};
template <> struct action<constraint_or> {
    template <typename Input> static void apply(const Input&, Object &);
};

struct wconstraint : seq<pad<constraint_or, space>, pad<TAOCPP_PEGTL_KEYWORD("WEIGHT"), space>, pad<integer, space>> {};
template <> struct action<wconstraint> {
    template <typename Input> static void apply(const Input&, Object &);
};
struct grammar : star<wconstraint> {};
template <> struct action<constraint_expr> {
    template <typename Input> static void apply(const Input&, Object &);
};

#endif