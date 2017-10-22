#include "custom_parser.h"
#include <tao/pegtl.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "clauses.h"
#include "global.h"

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

template <typename Rule>
struct action : pegtl::nothing<Rule> {};

struct integer : pegtl::seq<pegtl::opt<pegtl::one<'-'>>, pegtl::plus<pegtl::digit>> {};
template <>
struct action<integer> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.integer = std::stoi(in.string());
    }
};

struct instr : TAOCPP_PEGTL_KEYWORD("IN") {};

struct notstr : TAOCPP_PEGTL_KEYWORD("NOT") {};
template <>
struct action<notstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.isNot = true;
    }
};

struct andstr : TAOCPP_PEGTL_KEYWORD("AND") {};

struct orstr : TAOCPP_PEGTL_KEYWORD("OR") {};

struct classroomstr : TAOCPP_PEGTL_KEYWORD("CLASSROOM") {};
template <>
struct action<classroomstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.takeFieldValues = false;
        obj.takeClassValues = true;
        obj.takeSlotValues = false;
    }
};

struct slotstr : TAOCPP_PEGTL_KEYWORD("SLOT") {};
template <>
struct action<slotstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.takeFieldValues = false;
        obj.takeClassValues = false;
        obj.takeSlotValues = true;
    }
};

struct fieldtype : pegtl::identifier {};
template <>
struct action<fieldtype> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = in.string();
        obj.takeFieldValues = true;
        obj.takeClassValues = false;
        obj.takeSlotValues = false;
        obj.isNot = false;
    }
};

struct value : pegtl::plus<pegtl::sor<pegtl::range<'a','z'>,pegtl::range<'A','Z'>,pegtl::digit,pegtl::one<'.'>,pegtl::space>> {};
template <>
struct action<value> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::string val = in.string();
        int index = -1;
        if (obj.takeFieldValues) {
            if (obj.fieldType == "INSTRUCTOR") {
                for (int i=0; i<obj.timeTabler->data.instructors.size(); i++) {
                    if (obj.timeTabler->data.instructors[i].getName() == val) {
                        index = i;
                        break;
                    }
                }
            } else if (obj.fieldType == "COURSE") {
                for (int i=0; i<obj.timeTabler->data.courses.size(); i++) {
                    if (obj.timeTabler->data.courses[i].getName() == val) {
                        index = i;
                        break;
                    }
                }
            } else if (obj.fieldType == "SEGMENT") {
                for (int i=0; i<obj.timeTabler->data.segments.size(); i++) {
                    if (obj.timeTabler->data.segments[i].getName() == val) {
                        index = i;
                        break;
                    }
                }
            }
            obj.fieldValues.push_back(index);
        } else if (obj.takeClassValues) {
            for (int i=0; i<obj.timeTabler->data.classrooms.size(); i++) {
                if (obj.timeTabler->data.classrooms[i].getName() == val) {
                    index = i;
                    break;
                }
            }
            obj.classValues.push_back(index);
        } else if (obj.takeSlotValues) {
            for (int i=0; i<obj.timeTabler->data.slots.size(); i++) {
                if (obj.timeTabler->data.slots[i].getName() == val) {
                    index = i;
                    break;
                }
            }
            obj.classValues.push_back(index);
        }
    }
};

struct values : pegtl::seq<pegtl::pad<pegtl::one<'{'>,
    pegtl::space>,pegtl::list<value,pegtl::one<','>,pegtl::space>,
    pegtl::pad<pegtl::one<'}'>,pegtl::space>> {};

struct classroomdecl : pegtl::seq<pegtl::pad<classroomstr, pegtl::space>, values> {};

struct slotdecl : pegtl::seq<pegtl::pad<slotstr, pegtl::space>, values> {};

struct constraint_expr : pegtl::seq<pegtl::pad<fieldtype,pegtl::space>,
    values,pegtl::opt<notstr>,pegtl::pad<instr,pegtl::space>,pegtl::sor<classroomdecl,slotdecl>> {};
template <>
struct action<constraint_expr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << in.string() << std::endl;
        Clauses clauses;
        FieldType RHSType;
        FieldType fieldType;
        if(obj.takeClassValues) {
            RHSType = FieldType::classroom;
        }
        else {
            RHSType = FieldType::slot;
        }
        if (obj.fieldType == "COURSE") {
            clauses = obj.constraintAdder->customConstraint(obj.fieldValues, RHSType, obj.classValues, obj.isNot);
        }
        else {
            if (obj.fieldType == "INSTRUCTOR") {
                fieldType = FieldType::instructor;
            } else if (obj.fieldType == "PROGRAM") {
                fieldType = FieldType::program;
            } else if (obj.fieldType == "SEGMENT") {
                fieldType = FieldType::segment;
            } else if (obj.fieldType == "ISMINOR") {
                fieldType = FieldType::isMinor;
            }
            clauses = obj.constraintAdder->customConstraint(fieldType, obj.fieldValues, RHSType, obj.classValues, obj.isNot);
        }
        obj.constraint = clauses;
        obj.fieldValues.clear();
        obj.classValues.clear();
        obj.slotValues.clear();
    }
};

struct constraint_or;
struct constraint_braced : pegtl::seq<pegtl::opt<notstr>,
    pegtl::if_must<pegtl::pad<pegtl::one<'('>,
    pegtl::space>,constraint_or,pegtl::pad<pegtl::one<')'>,pegtl::space>>> {};

struct constraint_not : pegtl::seq<pegtl::pad<notstr, pegtl::space>, constraint_braced> {};
template <>
struct action<constraint_not> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        Clauses clauses = obj.constraint;
        obj.constraint = ~clauses;
    }
};

struct constraint_val : pegtl::sor<constraint_expr, constraint_not, constraint_braced> {};
template <>
struct action<constraint_val> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.constraintVals.push_back(obj.constraint);
    }
};

struct constraint_and : pegtl::list<constraint_val, andstr, pegtl::space> {};
template <>
struct action<constraint_and> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        Clauses clauses = obj.constraintVals[0];
        for (unsigned i = 1; i < obj.constraintVals.size(); i++) {
            clauses = clauses & obj.constraintVals[i];
        }
        obj.constraintVals.clear();
        obj.constraintAnds.push_back(clauses);
    }
};

struct constraint_or : pegtl::list<constraint_and, orstr, pegtl::space> {};
template <>
struct action<constraint_or> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        Clauses clauses = obj.constraintAnds[0];
        for (unsigned i = 1; i < obj.constraintAnds.size(); i++) {
            clauses = clauses | obj.constraintAnds[i];
        }
        obj.constraintAnds.clear();
        obj.constraint = clauses;
        std::cout << in.string() << std::endl;
    }
};

struct wconstraint : pegtl::seq<pegtl::pad<constraint_or, pegtl::space>,
    pegtl::pad<TAOCPP_PEGTL_KEYWORD("WEIGHT"), pegtl::space>, pegtl::pad<integer, pegtl::space>> {};
template <>
struct action<wconstraint> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.timeTabler->addClauses(obj.constraint, obj.integer);
        std::cout << in.string() << std::endl;
    }
};

struct grammar : pegtl::try_catch<pegtl::must<pegtl::star<wconstraint>, pegtl::eof>> {};
template <>
struct action<grammar> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << "CUSTOM CONSTRAINTS SUCCESSFULLY PARSED" <<std::endl;
        std::cout << in.string() << std::endl;
    }
};

template <typename Rule>
struct control : pegtl::normal<Rule> {
    template <typename Input, typename... States>
    static void raise(const Input& in, States&&...) {
        std::cout << in.position() << " Error parsing custom constraints" << std::endl;
        // throw pegtl::parse_error( "parse error matching " + pegtl::internal::demangle< Rule >(), in );
        exit(1);
    }
};

void parseCustomConstraints(ConstraintAdder* constraintAdder, TimeTabler* timeTabler) {
    Object obj;
    obj.constraintAdder = constraintAdder;
    obj.timeTabler = timeTabler;
    pegtl::file_input<> in("config/custom.txt");
    pegtl::parse<grammar, action, control>(in, obj);
}