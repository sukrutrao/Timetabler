#include "custom_parser.h"

template <> struct action<integer> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.integer = std::stoi(in.string());
    }
};

template <> struct action<instr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};

template <> struct action<notstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.isNot = true;
    }
};

template <> struct action<andstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};

template <> struct action<orstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};

template <> struct action<classroomstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.takeFieldValues = false;
        obj.takeClassValues = true;
        obj.taleSlotValues = false;
    }
};

template <> struct action<slotstr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.takeFieldValues = false;
        obj.takeClassValues = false;
        obj.taleSlotValues = true;
    }
};

template <> struct action<fieldtype> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.fieldType = in.string();
        obj.takeFieldValues = true;
        obj.takeClassValues = false;
        obj.taleSlotValues = false;
        obj.isNot = false;
    }
};

template <> struct action<value> {
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
                    if (obj.timeTabler->data.segments[i].toString() == val) {
                        index = i;
                        break;
                    }
                }
            }
            obj.fieldValues.push_back(index);
        } else if (obj.takeClassValues) {
            for (int i=0; i<obj.timeTabler->data.classrooms.size(); i++) {
                if (obj.timeTabler->data.classrooms[i].getNumber() == val) {
                    index = i;
                    break;
                }
            }
            obj.classValues.push_back(index);
        } else if (obj.takeSlotValues) {
            for (int i=0; i<obj.timeTabler->data.slots.size(); i++) {
                if (obj.timeTabler->data.slots[i].getNumber() == val) {
                    index = i;
                    break;
                }
            }
            obj.slotValues.push_back(index);
        }
    }
};

template <> struct action<values> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};

template <> struct action<classroomdecl> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};

template <> struct action<slotdecl> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};

template <> struct action<constraint_expr> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        Clauses clauses;
        if (obj.fieldType == "INSTRUCTOR") {
            
        } else if (obj.fieldType == "COURSE") {

        } else if (obj.fieldType == "SEGMENT") {
            
        }
        obj.constraint = clauses;
        obj.fieldValues.clear();
        obj.classValues.clear();
        obj.slotValues.clear();
    }
};

template <> struct action<constraint_braced> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};

template <> struct action<constraint_not> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        Clauses clauses = obj.constraint;
        obj.constraint = ~clauses;
    }
};

template <> struct action<constraint_val> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        obj.constraintVals.push_back(obj.constraint);
    }
};

template <> struct action<constraint_and> {
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

template <> struct action<constraint_or> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        Clauses clauses = obj.constraintAnds[0];
        for (unsigned i = 1; i < obj.constraintAnds.size(); i++) {
            clauses = clauses | obj.constraintAnds[i];
        }
        obj.constraintAnds.clear();
        obj.constraint = clauses;
    }
};

template <> struct action<wconstraint> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {
        // TODO
    }
};

template <> struct action<grammar> {
    template <typename Input>
    static void apply(const Input& in, Object &obj) {}
};
