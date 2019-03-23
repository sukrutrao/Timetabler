#include "custom_parser.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tao/pegtl.hpp>
#include <vector>
#include "clauses.h"
#include "global.h"
#include "utils.h"

/**
 * @brief      Makes an antecedent.
 *
 * @param      obj     The object
 * @param[in]  course  The course
 *
 * @return     Clauses corresponding to the antecedent
 */
Clauses makeAntecedent(Object &obj, int course) {
  Clauses ante, clause;
  if (obj.instructorValues.size() > 0) {
    clause = obj.constraintEncoder->hasFieldTypeListedValues(
        course, FieldType::instructor, obj.instructorValues);
    ante = ante & clause;
  }
  if (obj.programValues.size() > 0) {
    clause = obj.constraintEncoder->hasFieldTypeListedValues(
        course, FieldType::program, obj.programValues);
    ante = ante & clause;
  }
  if (obj.segmentValues.size() > 0) {
    clause = obj.constraintEncoder->hasFieldTypeListedValues(
        course, FieldType::segment, obj.segmentValues);
    ante = ante & clause;
  }
  if (obj.isMinorValues.size() > 0) {
    clause = obj.constraintEncoder->hasFieldTypeListedValues(
        course, FieldType::isMinor, obj.isMinorValues);
    ante = ante & clause;
  }
  return ante;
}

/**
 * @brief      Makes a consequent.
 *
 * @param      obj     The object
 * @param[in]  course  The course
 * @param[in]  i       Index of course in courseValues
 *
 * @return     Clauses corresponding to the consequent
 */
Clauses makeConsequent(Object &obj, int course, int i) {
  Clauses cons, clause;
  if (obj.classSame) {
    for (unsigned j = i + 1; j < obj.courseValues.size(); j++) {
      Clauses a = makeAntecedent(obj, obj.courseValues[j]);
      Clauses b = obj.constraintEncoder->hasSameFieldTypeAndValue(
          course, obj.courseValues[j], FieldType::classroom);
      a = a >> b;
      cons = cons & a;
    }
  }
  if (obj.classNotSame) {
    for (unsigned j = i + 1; j < obj.courseValues.size(); j++) {
      Clauses a = makeAntecedent(obj, obj.courseValues[j]);
      Clauses b = obj.constraintEncoder->hasSameFieldTypeAndValue(
          course, obj.courseValues[j], FieldType::classroom);
      a = a >> (~b);
      cons = cons & a;
    }
  }
  if (obj.slotSame) {
    for (unsigned j = i + 1; j < obj.courseValues.size(); j++) {
      Clauses a = makeAntecedent(obj, obj.courseValues[j]);
      Clauses b = obj.constraintEncoder->hasSameFieldTypeAndValue(
          course, obj.courseValues[j], FieldType::slot);
      a = a >> b;
      cons = cons & a;
    }
  }
  if (obj.slotNotSame) {
    for (unsigned j = i + 1; j < obj.courseValues.size(); j++) {
      Clauses a = makeAntecedent(obj, obj.courseValues[j]);
      Clauses b = obj.constraintEncoder->hasSameFieldTypeAndValue(
          course, obj.courseValues[j], FieldType::slot);
      a = a >> (~b);
      cons = cons & a;
    }
  }
  if (obj.classValues.size() > 0) {
    clause = obj.constraintEncoder->hasFieldTypeListedValues(
        course, FieldType::classroom, obj.classValues);
    cons = cons & clause;
  }
  if (obj.slotValues.size() > 0) {
    clause = obj.constraintEncoder->hasFieldTypeListedValues(
        course, FieldType::slot, obj.slotValues);
    cons = cons & clause;
  }
  return cons;
}

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

namespace custom_constraint_grammar {

template <typename Rule>
struct action : pegtl::nothing<Rule> {};

/**
 * @brief      Parse integer: Store the integer in the object
 */
struct integer
    : pegtl::seq<pegtl::opt<pegtl::one<'-'>>, pegtl::plus<pegtl::digit>> {};
template <>
struct action<integer> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.integer = std::stoi(in.string());
  }
};

/**
 * @brief      Parse "IN"
 */
struct instr : TAO_PEGTL_KEYWORD("IN") {};

/**
 * @brief      Parse "NOT": Store that NOT keyword is present in the custom
 * constraint
 */
struct notstr : TAO_PEGTL_KEYWORD("NOT") {};
template <>
struct action<notstr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.isNot = true;
  }
};

/**
 * @brief      Parse "NOT"
 */
struct andstr : TAO_PEGTL_KEYWORD("AND") {};

/**
 * @brief      Parse "OR"
 */
struct orstr : TAO_PEGTL_KEYWORD("OR") {};

/**
 * @brief      Parse "EXCEPT"
 */
struct exceptstr : TAO_PEGTL_KEYWORD("EXCEPT") {};

/**
 * @brief      Parse "CLASSROOM": Store the field type to be classroom
 * which will be used while forming the custom consraint
 */
struct classroomstr : TAO_PEGTL_KEYWORD("CLASSROOM") {};
template <>
struct action<classroomstr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.fieldType = FieldValuesType::CLASSROOM;
  }
};

/**
 * @brief      Parse "SLOT": Similar to classroom
 */
struct slotstr : TAO_PEGTL_KEYWORD("SLOT") {};
template <>
struct action<slotstr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.fieldType = FieldValuesType::SLOT;
  }
};

/**
 * @brief      Parse "COURSE": Similar to classroom
 */
struct coursestr : TAO_PEGTL_KEYWORD("COURSE") {};
template <>
struct action<coursestr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.fieldType = FieldValuesType::COURSE;
  }
};

/**
 * @brief      Parse "INSTRUCTOR": Similar to classroom
 */
struct instructorstr : TAO_PEGTL_KEYWORD("INSTRUCTOR") {};
template <>
struct action<instructorstr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.fieldType = FieldValuesType::INSTRUCTOR;
  }
};

/**
 * @brief      Parse "SEGMENT": Similar to classroom
 */
struct segmentstr : TAO_PEGTL_KEYWORD("SEGMENT") {};
template <>
struct action<segmentstr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.fieldType = FieldValuesType::SEGMENT;
  }
};

/**
 * @brief      Parse "ISMNOR": Similar to classroom
 */
struct isminorstr : TAO_PEGTL_KEYWORD("ISMINOR") {};
template <>
struct action<isminorstr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.fieldType = FieldValuesType::ISMINOR;
  }
};

/**
 * @brief      Parse "PROGRAM": Similar to classroom
 */
struct programstr : TAO_PEGTL_KEYWORD("PROGRAM") {};
template <>
struct action<programstr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.fieldType = FieldValuesType::PROGRAM;
  }
};

/**
 * @brief      Parse "UNBUNDLE"
 */
struct unbundlestr : TAO_PEGTL_KEYWORD("UNBUNDLE") {};

/**
 * @brief      Parse "WEIGHT"
 */
struct weightstr : TAO_PEGTL_KEYWORD("WEIGHT") {};

/**
 * @brief      Constraint is on one of the instructor, segment, isminor,
 * program. isNot, classSame, slotSame, classNotSame, slotNotSame are reset.
 */
struct fieldtype
    : pegtl::sor<instructorstr, segmentstr, isminorstr, programstr> {};
template <>
struct action<fieldtype> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.isNot = false;
    obj.classSame = false;
    obj.slotSame = false;
    obj.classNotSame = false;
    obj.slotNotSame = false;
  }
};

/**
 * @brief      Parse a value of the field that is specified in the constraint
 */
struct value
    : pegtl::plus<pegtl::sor<pegtl::range<'a', 'z'>, pegtl::range<'A', 'Z'>,
                             pegtl::digit, pegtl::one<'.'>, pegtl::one<'-'>,
                             pegtl::one<'@'>, pegtl::one<'<'>, pegtl::one<'>'>,
                             pegtl::space>> {};
template <>
struct action<value> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    std::string val = in.string();
    bool found = false;
    if (obj.fieldType == FieldValuesType::INSTRUCTOR) {
      for (unsigned i = 0; i < obj.timetabler->data.instructors.size(); i++) {
        if (obj.timetabler->data.instructors[i].getName() == val) {
          found = true;
          obj.instructorValues.push_back(i);
          break;
        }
      }
      if (!found) {
        LOG(ERROR) << "Instructor " << val << " does not exist.";
      }
      found = false;
    } else if (obj.fieldType == FieldValuesType::COURSE) {
      for (unsigned i = 0; i < obj.timetabler->data.courses.size(); i++) {
        if (obj.timetabler->data.courses[i].getName() == val) {
          found = true;
          obj.courseValues.push_back(i);
          break;
        }
      }
      if (!found) {
        LOG(ERROR) << "Course " << val << " does not exist.";
      }
      found = false;
    } else if (obj.fieldType == FieldValuesType::SEGMENT) {
      for (unsigned i = 0; i < obj.timetabler->data.segments.size(); i++) {
        if (obj.timetabler->data.segments[i].getName() == val) {
          found = true;
          obj.segmentValues.push_back(i);
          break;
        }
      }
      if (!found) {
        LOG(ERROR) << "Segment " << val << " does not exist.";
      }
      found = false;
    } else if (obj.fieldType == FieldValuesType::PROGRAM) {
      for (unsigned i = 0; i < obj.timetabler->data.programs.size(); i++) {
        if (obj.timetabler->data.programs[i].getNameWithType() == val) {
          found = true;
          obj.programValues.push_back(i);
          break;
        }
      }
      if (!found) {
        LOG(ERROR) << "Program " << val << " does not exist.";
      }
      found = false;
    } else if (obj.fieldType == FieldValuesType::ISMINOR) {
      for (unsigned i = 0; i < obj.timetabler->data.isMinors.size(); i++) {
        if (obj.timetabler->data.isMinors[i].getName() == val) {
          found = true;
          obj.isMinorValues.push_back(i);
          break;
        }
      }
      if (!found) {
        LOG(ERROR) << "IsMinor " << val << " does not exist.";
      }
      found = false;
    } else if (obj.fieldType == FieldValuesType::CLASSROOM) {
      for (unsigned i = 0; i < obj.timetabler->data.classrooms.size(); i++) {
        if (obj.timetabler->data.classrooms[i].getName() == val) {
          found = true;
          obj.classValues.push_back(i);
          break;
        }
      }
      if (!found) {
        LOG(ERROR) << "Classroom " << val << " does not exist.";
      }
      found = false;
    } else if (obj.fieldType == FieldValuesType::SLOT) {
      for (unsigned i = 0; i < obj.timetabler->data.slots.size(); i++) {
        if (obj.timetabler->data.slots[i].getName() == val) {
          found = true;
          obj.slotValues.push_back(i);
          break;
        }
      }
      if (!found) {
        LOG(ERROR) << "Slot " << val << " does not exist.";
      }
      found = false;
    }
  }
};

/**
 * @brief      Parse * as all values of the specified field
 */
struct allvalues : pegtl::pad<pegtl::one<'*'>, pegtl::space> {};
template <>
struct action<allvalues> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    std::string val = in.string();
    if (obj.fieldType == FieldValuesType::INSTRUCTOR) {
      for (unsigned i = 0; i < obj.timetabler->data.instructors.size(); i++) {
        obj.instructorValues.push_back(i);
      }
    } else if (obj.fieldType == FieldValuesType::COURSE) {
      for (unsigned i = 0; i < obj.timetabler->data.courses.size(); i++) {
        obj.courseValues.push_back(i);
      }
    } else if (obj.fieldType == FieldValuesType::SEGMENT) {
      for (unsigned i = 0; i < obj.timetabler->data.segments.size(); i++) {
        obj.segmentValues.push_back(i);
      }
    } else if (obj.fieldType == FieldValuesType::PROGRAM) {
      for (unsigned i = 0; i < obj.timetabler->data.programs.size(); i++) {
        obj.programValues.push_back(i);
      }
    } else if (obj.fieldType == FieldValuesType::ISMINOR) {
      for (unsigned i = 0; i < obj.timetabler->data.isMinors.size(); i++) {
        obj.isMinorValues.push_back(i);
      }
    } else if (obj.fieldType == FieldValuesType::CLASSROOM) {
      for (unsigned i = 0; i < obj.timetabler->data.classrooms.size(); i++) {
        obj.classValues.push_back(i);
      }
    } else if (obj.fieldType == FieldValuesType::SLOT) {
      for (unsigned i = 0; i < obj.timetabler->data.slots.size(); i++) {
        obj.slotValues.push_back(i);
      }
    }
  }
};

/**
 * @brief      Parse "SAME": Used to specify constraints on courses with same
 * field values
 */
struct sameval : pegtl::pad<TAO_PEGTL_KEYWORD("SAME"), pegtl::space> {};
template <>
struct action<sameval> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    if (obj.fieldType == FieldValuesType::CLASSROOM) {
      obj.classSame = true;
    } else if (obj.fieldType == FieldValuesType::SLOT) {
      obj.slotSame = true;
    }
  }
};

/**
 * @brief      Parse "NOTSAME": Used to specify constraints on courses with
 * different field values
 */
struct notsameval : pegtl::pad<TAO_PEGTL_KEYWORD("NOTSAME"), pegtl::space> {};
template <>
struct action<notsameval> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    if (obj.fieldType == FieldValuesType::CLASSROOM) {
      obj.classNotSame = true;
    } else if (obj.fieldType == FieldValuesType::SLOT) {
      obj.slotNotSame = true;
    }
  }
};

/**
 * @brief      Parse list of values of a field specified in the constraint
 */
struct listvalues
    : pegtl::seq<pegtl::pad<pegtl::one<'{'>, pegtl::space>,
                 pegtl::list<value, pegtl::one<','>, pegtl::space>,
                 pegtl::pad<pegtl::one<'}'>, pegtl::space>> {};

/**
 * @brief      Parse values
 */
struct values : pegtl::sor<allvalues, listvalues, sameval, notsameval> {};

/**
 * @brief      Parse classrooms
 */
struct classroomdecl
    : pegtl::seq<pegtl::pad<classroomstr, pegtl::space>, values> {};

/**
 * @brief      Parse slots
 */
struct slotdecl : pegtl::seq<pegtl::pad<slotstr, pegtl::space>, values> {};

/**
 * @brief      Parse courses declaration without except keyword
 */
struct coursenoexceptdecl
    : pegtl::seq<pegtl::pad<coursestr, pegtl::space>, values> {};
template <>
struct action<coursenoexceptdecl> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.courseExcept = false;
  }
};

/**
 * @brief      Parse courses declaration with except keyword
 */
struct courseexceptdecl
    : pegtl::seq<pegtl::pad<coursestr, pegtl::space>,
                 pegtl::pad<exceptstr, pegtl::space>, values> {};
template <>
struct action<courseexceptdecl> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.courseExcept = true;
  }
};

/**
 * @brief      Parse courses
 */
struct coursedecl : pegtl::sor<coursenoexceptdecl, courseexceptdecl> {};
template <>
struct action<coursedecl> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.isNot = false;
  }
};

/**
 * @brief      Parse single decl in consequent of the constraint
 */
struct decl : pegtl::sor<slotdecl, classroomdecl> {};

/**
 * @brief      Parse multi decls in consequent of the constraint
 */
struct decls : pegtl::list<decl, andstr, pegtl::space> {};

/**
 * @brief      Parse fieldd decl in antecedent of the constraint
 */
struct fielddecl : pegtl::seq<pegtl::pad<fieldtype, pegtl::space>, values> {};

/**
 * @brief      Parse multi field decls in antecedent of the constraint
 */
struct fielddecls : pegtl::opt<pegtl::list<fielddecl, andstr, pegtl::space>> {};

/**
 * @brief      Parse a constraint
 */
struct constraint_expr : pegtl::seq<coursedecl, fielddecls, pegtl::opt<notstr>,
                                    pegtl::pad<instr, pegtl::space>, decls> {};
template <>
struct action<constraint_expr> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    Clauses clauses;
    if (obj.courseExcept) {
      std::vector<int> courseVals;
      for (unsigned i = 0; i < obj.timetabler->data.courses.size(); i++) {
        if (std::find(obj.courseValues.begin(), obj.courseValues.end(), i) ==
            obj.courseValues.end()) {
          courseVals.push_back(i);
        }
      }
      obj.courseValues = courseVals;
    }
    for (unsigned i = 0; i < obj.courseValues.size(); i++) {
      int course = obj.courseValues[i];
      Clauses ante, cons, clause;
      ante = makeAntecedent(obj, course);
      cons = makeConsequent(obj, course, i);
      if (obj.isNot) {
        cons = ~cons;
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
    obj.classValues.clear();
    obj.slotValues.clear();
    obj.isNot = false;
    obj.classSame = false;
    obj.slotSame = false;
    obj.classNotSame = false;
    obj.slotNotSame = false;
  }
};

struct constraint_or;

/**
 * @brief      Parse constraint enclosed in braces
 */
struct constraint_braced
    : pegtl::seq<pegtl::if_must<pegtl::pad<pegtl::one<'('>, pegtl::space>,
                                constraint_or,
                                pegtl::pad<pegtl::one<')'>, pegtl::space>>> {};

/**
 * @brief      Parse negation of a constraint
 */
struct constraint_not
    : pegtl::seq<pegtl::pad<notstr, pegtl::space>, constraint_braced> {};
template <>
struct action<constraint_not> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    Clauses clauses = obj.constraint;
    obj.constraint = ~clauses;
  }
};

/**
 * @brief      Parse a constraint: Constraint expression or negation of some
 * constraint expression or a constraint enclosed in parantheses
 */
struct constraint_val
    : pegtl::sor<constraint_expr, constraint_not, constraint_braced> {};
template <>
struct action<constraint_val> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.constraintVals.push_back(obj.constraint);
  }
};

/**
 * @brief      Parse conjunction of constraints
 * Add all the constraints to obj.constraintAdds
 */
struct constraint_and : pegtl::list<constraint_val, andstr, pegtl::space> {};
template <>
struct action<constraint_and> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    Clauses clauses = obj.constraintVals[0];
    for (unsigned i = 1; i < obj.constraintVals.size(); i++) {
      clauses = clauses & obj.constraintVals[i];
    }
    obj.constraintVals.clear();
    obj.constraintAnds.push_back(clauses);
  }
};

/**
 * @brief      Parse disjunction of constraints
 * The combined clauses for all the constraints are stored in obj.constraint
 */
struct constraint_or : pegtl::list<constraint_and, orstr, pegtl::space> {};
template <>
struct action<constraint_or> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    Clauses clauses = obj.constraintAnds[0];
    for (unsigned i = 1; i < obj.constraintAnds.size(); i++) {
      clauses = clauses | obj.constraintAnds[i];
    }
    obj.constraintAnds.clear();
    obj.constraint = clauses;
  }
};

/**
 * @brief      Parse a constraint with unbundle keyword
 */
struct constraint_unbundle
    : pegtl::seq<coursedecl, pegtl::pad<unbundlestr, pegtl::space>, fielddecls,
                 pegtl::opt<notstr>, pegtl::pad<instr, pegtl::space>, decl,
                 pegtl::pad<weightstr, pegtl::space>,
                 pegtl::pad<integer, pegtl::space>> {};
template <>
struct action<constraint_unbundle> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    if (obj.courseExcept) {
      std::vector<int> courseVals;
      for (unsigned i = 0; i < obj.timetabler->data.courses.size(); i++) {
        if (std::find(obj.courseValues.begin(), obj.courseValues.end(), i) ==
            obj.courseValues.end()) {
          courseVals.push_back(i);
        }
      }
      obj.courseValues = courseVals;
    }
    for (unsigned i = 0; i < obj.courseValues.size(); i++) {
      int course = obj.courseValues[i];
      Clauses ante, cons, clause;
      ante = makeAntecedent(obj, course);
      cons = makeConsequent(obj, course, i);
      if (obj.isNot) {
        cons = ~cons;
      }
      clause = ante >> cons;
      obj.constraint = clause;
      obj.timetabler->data.customConstraintVars.push_back(
          obj.timetabler->newVar());
      int index = obj.timetabler->data.customConstraintVars.size() - 1;
      if (obj.integer != 0) {
        Clauses hardConsequent =
            CClause(obj.timetabler->data.customConstraintVars[index]) >>
            obj.constraint;
        obj.timetabler->addClauses(hardConsequent, -1);
      }
      obj.timetabler->data.customMap[index] = course;
      obj.timetabler->addHighLevelCustomConstraintClauses(index, obj.integer);
    }
    obj.courseValues.clear();
    obj.instructorValues.clear();
    obj.isMinorValues.clear();
    obj.programValues.clear();
    obj.segmentValues.clear();
    obj.classValues.clear();
    obj.slotValues.clear();
    obj.isNot = false;
    obj.classSame = false;
    obj.slotSame = false;
    obj.classNotSame = false;
    obj.slotNotSame = false;
  }
};

/**
 * @brief      Parse weighted constraint
 * Add the clauses corresponding to the constraint along with its weight
 */
struct wconstraint : pegtl::seq<pegtl::pad<constraint_or, pegtl::space>,
                                pegtl::pad<weightstr, pegtl::space>,
                                pegtl::pad<integer, pegtl::space>> {};
template <>
struct action<wconstraint> {
  template <typename Input>
  static void apply(const Input &in, Object &obj) {
    obj.timetabler->data.customConstraintVars.push_back(
        obj.timetabler->newVar());
    int index = obj.timetabler->data.customConstraintVars.size() - 1;
    if (obj.integer != 0) {
      Clauses hardConsequent =
          CClause(obj.timetabler->data.customConstraintVars[index]) >>
          obj.constraint;
      obj.timetabler->addClauses(hardConsequent, -1);
    }
    obj.timetabler->addHighLevelCustomConstraintClauses(index, obj.integer);
  }
};

/**
 * @brief      Parse constraints from the file, generate error on failure
 */
struct grammar
    : pegtl::try_catch<
          pegtl::must<pegtl::star<pegtl::sor<wconstraint, constraint_unbundle>>,
                      pegtl::eof>> {};

template <typename Rule>
struct control : pegtl::normal<Rule> {
  template <typename Input, typename... States>
  static void raise(const Input &in, States &&...) {
    LOG(ERROR) << in.position() << " Error parsing custom constraints";
  }
};

}  // namespace custom_constraint_grammar

/**
 * @brief      Parses custom constraints given in a file and adds them to the
 * solver.
 *
 * @param[in]  file               The file containing the constraints
 * @param      constraintEncoder  The ConstraintEncoder object
 * @param      timetabler         The Timetabler object
 */
void parseCustomConstraints(std::string file,
                            ConstraintEncoder *constraintEncoder,
                            Timetabler *timetabler) {
  Object obj;
  obj.constraintEncoder = constraintEncoder;
  obj.timetabler = timetabler;
  pegtl::file_input<> in(file);
  pegtl::parse<custom_constraint_grammar::grammar,
               custom_constraint_grammar::action,
               custom_constraint_grammar::control>(in, obj);
}

/**
 * @brief      Initialize object members
 */
Object::Object() {
  isNot = false;
  classSame = false;
  slotSame = false;
  classNotSame = false;
  slotNotSame = false;
}
