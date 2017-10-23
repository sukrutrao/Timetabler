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
        obj.fieldType = FieldValuesType::CLASSROOM;
    }
};

struct slotstr : TAOCPP_PEGTL_KEYWORD("SLOT") {};
template <>
struct action<slotstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = FieldValuesType::SLOT;
    }
};

struct coursestr : TAOCPP_PEGTL_KEYWORD("COURSE") {};
template <>
struct action<coursestr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = FieldValuesType::COURSE;
    }
};

struct instructorstr : TAOCPP_PEGTL_KEYWORD("INSTRUCTOR") {};
template <>
struct action<instructorstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = FieldValuesType::INSTRUCTOR;
    }
};

struct segmentstr : TAOCPP_PEGTL_KEYWORD("SEGMENT") {};
template <>
struct action<segmentstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = FieldValuesType::SEGMENT;
    }
};

struct isminorstr : TAOCPP_PEGTL_KEYWORD("ISMINOR") {};
template <>
struct action<isminorstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = FieldValuesType::ISMINOR;
    }
};

struct programstr : TAOCPP_PEGTL_KEYWORD("PROGRAM") {};
template <>
struct action<programstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = FieldValuesType::PROGRAM;
    }
};

struct fieldtype : pegtl::sor<instructorstr,segmentstr,isminorstr,programstr> {};
template <>
struct action<fieldtype> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.isNot = false;
    }
};

struct value : pegtl::plus<pegtl::sor<pegtl::range<'a','z'>,pegtl::range<'A','Z'>,
    pegtl::digit,pegtl::one<'.'>,pegtl::one<'-'>,pegtl::one<'@'>,pegtl::space>> {};
template <>
struct action<value> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::string val = in.string();
        if (obj.fieldType == FieldValuesType::INSTRUCTOR) {
            for (int i=0; i<obj.timeTabler->data.instructors.size(); i++) {
                if (obj.timeTabler->data.instructors[i].getName() == val) {
                    obj.instructorValues.push_back(i);
                    break;
                }
            }
        } else if (obj.fieldType == FieldValuesType::COURSE) {
            for (int i=0; i<obj.timeTabler->data.courses.size(); i++) {
                if (obj.timeTabler->data.courses[i].getName() == val) {
                    obj.courseValues.push_back(i);
                    break;
                }
            }
        } else if (obj.fieldType == FieldValuesType::SEGMENT) {
            for (int i=0; i<obj.timeTabler->data.segments.size(); i++) {
                if (obj.timeTabler->data.segments[i].getName() == val) {
                    obj.segmentValues.push_back(i);
                    break;
                }
            }
        } else if (obj.fieldType == FieldValuesType::PROGRAM) {
            for (int i=0; i<obj.timeTabler->data.programs.size(); i++) {
                if (obj.timeTabler->data.programs[i].getName() == val) {
                    obj.programValues.push_back(i);
                    break;
                }
            }
        } else if (obj.fieldType == FieldValuesType::ISMINOR) {
            for (int i=0; i<obj.timeTabler->data.isMinors.size(); i++) {
                if (obj.timeTabler->data.isMinors[i].getName() == val) {
                    obj.isMinorValues.push_back(i);
                    break;
                }
            }
        } else if (obj.fieldType == FieldValuesType::CLASSROOM) {
            for (int i=0; i<obj.timeTabler->data.classrooms.size(); i++) {
                if (obj.timeTabler->data.classrooms[i].getName() == val) {
                    obj.classValues.push_back(i);
                    break;
                }
            }            
        } else if (obj.fieldType == FieldValuesType::SLOT) {
            for (int i=0; i<obj.timeTabler->data.slots.size(); i++) {
                if (obj.timeTabler->data.slots[i].getName() == val) {
                    obj.slotValues.push_back(i);
                    break;
                }
            }            
        }
    }
};

struct allvalues : pegtl::pad<pegtl::one<'*'>, pegtl::space> {};
template <>
struct action<allvalues> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::string val = in.string();
        if (obj.fieldType == FieldValuesType::INSTRUCTOR) {
            for (int i=0; i<obj.timeTabler->data.instructors.size(); i++) {
                obj.instructorValues.push_back(i);
            }
        } else if (obj.fieldType == FieldValuesType::COURSE) {
            for (int i=0; i<obj.timeTabler->data.courses.size(); i++) {
                obj.courseValues.push_back(i);
            }
        } else if (obj.fieldType == FieldValuesType::SEGMENT) {
            for (int i=0; i<obj.timeTabler->data.segments.size(); i++) {
                obj.segmentValues.push_back(i);
            }
        } else if (obj.fieldType == FieldValuesType::PROGRAM) {
            for (int i=0; i<obj.timeTabler->data.programs.size(); i++) {
                obj.programValues.push_back(i);
            }
        } else if (obj.fieldType == FieldValuesType::ISMINOR) {
            for (int i=0; i<obj.timeTabler->data.isMinors.size(); i++) {
                obj.isMinorValues.push_back(i);
            }
        } else if (obj.fieldType == FieldValuesType::CLASSROOM) {
            for (int i=0; i<obj.timeTabler->data.classrooms.size(); i++) {
                obj.classValues.push_back(i);
            }            
        } else if (obj.fieldType == FieldValuesType::SLOT) {
            for (int i=0; i<obj.timeTabler->data.slots.size(); i++) {
                obj.slotValues.push_back(i);
            }
        }
    }
};

struct listvalues : pegtl::seq<pegtl::pad<pegtl::one<'{'>,
    pegtl::space>,pegtl::list<value,pegtl::one<','>,pegtl::space>,
    pegtl::pad<pegtl::one<'}'>,pegtl::space>> {};

struct values : pegtl::sor<allvalues, listvalues> {};

struct classroomdecl : pegtl::seq<pegtl::pad<classroomstr, pegtl::space>, values> {};

struct slotdecl : pegtl::seq<pegtl::pad<slotstr, pegtl::space>, values> {};

struct coursedecl : pegtl::seq<pegtl::pad<coursestr, pegtl::space>, values> {};

struct decl : pegtl::sor<slotdecl, classroomdecl> {};

struct decls : pegtl::list<decl,andstr,pegtl::space> {};

struct fielddecl : pegtl::seq<pegtl::pad<fieldtype,pegtl::space>,values> {};

struct fielddecls : pegtl::list<fielddecl,andstr,pegtl::space> {};

struct constraint_expr : pegtl::seq<coursedecl,fielddecls,pegtl::opt<notstr>,pegtl::pad<instr,pegtl::space>,decls> {};
template <>
struct action<constraint_expr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        std::cout << in.string() << std::endl;
        Clauses clauses;
        for (int course : obj.courseValues) {
            Clauses ante, cons, clause;
            if (obj.instructorValues.size() > 0) {
                clause = obj.constraintEncoder->hasFieldTypeListedValues(course, FieldType::instructor, obj.instructorValues);
                ante = ante & clause;
            }
            if (obj.programValues.size() > 0) {
                clause = obj.constraintEncoder->hasFieldTypeListedValues(course, FieldType::program, obj.programValues);
                ante = ante & clause;
            }
            if (obj.segmentValues.size() > 0) {
                clause = obj.constraintEncoder->hasFieldTypeListedValues(course, FieldType::segment, obj.segmentValues);
                ante = ante & clause;
            }
            if (obj.isMinorValues.size() > 0) {
                clause = obj.constraintEncoder->hasFieldTypeListedValues(course, FieldType::isMinor, obj.isMinorValues);
                ante = ante & clause;
            }
            if (obj.classValues.size() > 0) {
                clause = obj.constraintEncoder->hasFieldTypeListedValues(course, FieldType::classroom, obj.classValues);
                cons = cons & clause;
            }
            if (obj.slotValues.size() > 0) {
                clause = obj.constraintEncoder->hasFieldTypeListedValues(course, FieldType::slot, obj.slotValues);
                cons = cons & clause;
            }
            clause = ante >> cons;
            clauses = clauses & clause;
        }
        obj.constraint = clauses;
        obj.courseValues.clear();
        obj.instructorValues.clear();
        obj.isMinorValues.clear();
        obj.programValues.clear();
        obj.segmentValues.clear();
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

void parseCustomConstraints(ConstraintEncoder* constraintEncoder , ConstraintAdder* constraintAdder, TimeTabler* timeTabler) {
    Object obj;
    obj.constraintEncoder = constraintEncoder;
    obj.constraintAdder = constraintAdder;
    obj.timeTabler = timeTabler;
    pegtl::file_input<> in("config/custom.txt");
    pegtl::parse<grammar, action, control>(in, obj);
}