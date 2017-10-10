#include "functions.h"

static bool Function::hasSameFieldTypeAndValue(Variable &var1, Variable &var2, FieldType fieldType) {
    if(!(var1.getType() == fieldType) || !(var2.getType() == fieldType)) {
        return false;
    }
    else if(var1.hasSameCourse(var2)) {
        return false;
    }
    else if(!var1.hasSameFieldValue(var2)) {
        return false;
    }
    return true;
}

static bool Function::isSameCourse(Variable &var1, Variable &var2) {
    return var1.hasSameCourse(var2);
}

static bool Function::hasIntersectingTime(Variable &var1, Variable &var2) {
    
}