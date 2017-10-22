#ifndef CUSTOM_PARSER_H
#define CUSTOM_PARSER_H

#include <tao/pegtl.hpp>
#include <iostream>

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

// using namespace pegtl;

struct Object {

};

template <typename Rule>
struct action : pegtl::nothing<Rule> {};

struct integer : pegtl::seq<pegtl::range<'1', '9'>, pegtl::star<pegtl::digit>> {};
template <>
struct action<integer> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "integer " << in.string() << std::endl;
    }
};

struct instr : TAOCPP_PEGTL_KEYWORD("IN") {};
template <>
struct action<instr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "instr " << in.string() << std::endl;
    }
};

struct notstr : TAOCPP_PEGTL_KEYWORD("NOT") {};
template <>
struct action<notstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "notstr " << in.string() << std::endl;
    }
};

struct andstr : TAOCPP_PEGTL_KEYWORD("AND") {};
template <>
struct action<andstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "andstr " << in.string() << std::endl;
    }
};

struct orstr : TAOCPP_PEGTL_KEYWORD("OR") {};
template <>
struct action<orstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "orstr " << in.string() << std::endl;
    }
};

struct classroomstr : TAOCPP_PEGTL_KEYWORD("CLASSROOM") {};
template <>
struct action<classroomstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "classroomstr " << in.string() << std::endl;
    }
};

struct slotstr : TAOCPP_PEGTL_KEYWORD("SLOT") {};
template <>
struct action<slotstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "slotstr " << in.string() << std::endl;
    }
};

struct fieldtype : pegtl::identifier {};
template <>
struct action<fieldtype> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "fieldtype " << in.string() << std::endl;
    }
};

struct fieldvalues : pegtl::seq<pegtl::pad<pegtl::one<'{'>, pegtl::space>,
    pegtl::list<pegtl::identifier, pegtl::one<','>, pegtl::space>,
    pegtl::pad<pegtl::one<'{'>, pegtl::space>> {};
template <>
struct action<fieldvalues> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "fieldvalues " << in.string() << std::endl;
    }
};

struct classvalues : pegtl::seq<pegtl::pad<pegtl::one<'{'>, pegtl::space>,
    pegtl::list<pegtl::identifier, pegtl::one<','>, pegtl::space>,
    pegtl::pad<pegtl::one<'{'>, pegtl::space>> {};
template <>
struct action<classvalues> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "classvalues " << in.string() << std::endl;
    }
};

struct slotvalues : pegtl::seq<pegtl::pad<pegtl::one<'{'>, pegtl::space>,
    pegtl::list<pegtl::identifier, pegtl::one<','>, pegtl::space>,
    pegtl::pad<pegtl::one<'{'>, pegtl::space>> {};
template <>
struct action<slotvalues> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "slotvalues " << in.string() << std::endl;
    }
};

struct classroomdecl : pegtl::seq<pegtl::pad<classroomstr, pegtl::space>, classvalues> {};
template <>
struct action<classroomdecl> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "classroomdecl " << in.string() << std::endl;
    }
};

struct slotdecl : pegtl::seq<pegtl::pad<slotstr, pegtl::space>, slotvalues> {};
template <>
struct action<slotdecl> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "slotdecl " << in.string() << std::endl;
    }
};

struct constraint_expr : pegtl::seq<fieldtype, pegtl::star<pegtl::space>, fieldvalues, pegtl::star<pegtl::space>,
    pegtl::opt<notstr>, pegtl::star<pegtl::space>, instr, pegtl::star<pegtl::space>,
    pegtl::sor<classroomdecl, slotdecl>> {};
template <>
struct action<constraint_expr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "constraint_expr " << in.string() << std::endl;
    }
};

struct constraint_or;
struct constraint_braced : pegtl::seq<pegtl::opt<notstr>, pegtl::if_must<pegtl::one<'('>, pegtl::pad<constraint_or, pegtl::space>, pegtl::one<')'>>> {};
struct constraint_val : pegtl::sor <constraint_expr, constraint_braced> {};
struct constraint_and : pegtl::list<constraint_val, andstr, pegtl::space> {};
struct constraint_or : pegtl::list<constraint_and, orstr, pegtl::space> {};

struct wconstraint : pegtl::seq<constraint_or, pegtl::star<pegtl::space>, pegtl::pad<TAOCPP_PEGTL_KEYWORD("WEIGHT"), pegtl::space>, pegtl::pad<integer, pegtl::space>> {};
struct grammar : pegtl::plus<wconstraint> {};


int main(int argc, char const *argv[]) {
    pegtl::file_input<> in("test.txt");
    Object obj;
    pegtl::parse<grammar, action>(in, obj);
    return 0;
}

#endif