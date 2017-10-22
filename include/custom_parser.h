#ifndef CUSTOM_PARSER_H
#define CUSTOM_PARSER_H

#include <tao/pegtl.hpp>

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

using namespace pegtl;

struct integer : seq<range<'1', '9'>, star<digit>> {};
struct instr : TAOCPP_PEGTL_KEYWORD("IN") {};
struct notstr : TAOCPP_PEGTL_KEYWORD("NOT") {};
struct andstr : TAOCPP_PEGTL_KEYWORD<"AND"> {};
struct orstr : TAOCPP_PEGTL_KEYWORD<"OR"> {};

struct fieldtype : identifier {};
struct fieldvalues : list_must<identifier, one<','>, space> {};
struct classvalues : list_must<identifier, one<','>, space> {};
struct slotvalues : list_must<identifier, one<','>, space> {};
struct classroomdecl : seq<pad<TAOCPP_PEGTL_KEYWORD("CLASSROOM"), space>, classvalues> {};
struct slotdecl : seq<pad<TAOCPP_PEGTL_KEYWORD("SLOT"), space>, slotvalues> {};
struct constraint_expr : seq<fieldtype, fieldvalues, opt<notstr>, instr, sor<classroomdecl, slotdecl>> {};

struct constraint;
struct constraint_braced : seq<opt<notstr>, if_must<one<'('>, pad<constraint, space>, one<')'>>> {};
struct constraint_val : sor <constraint_expr, constraint_braced> {};
struct constraint_and : list_must<constraint_val, andstr, space> {};
struct constraint_or : list_must<constraint_and, orstr, space> {};

struct wconstraint : seq<constraint_or, pad<TAOCPP_PEGTL_KEYWORD("WEIGHT"), space>, pad<integer, space>> {};
struct grammar : plus<wconstraint> {};

template <typename Rule>
struct action : nothing<Rule> {};

struct Object {

};

template <>
struct action<grammar> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::out << in.string() << std::endl;
    }
};

#endif