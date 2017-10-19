#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <string>
#include "fields/field.h"
#include "global.h"

class Classroom : public Field {
private:
    std::string number;
    unsigned size;
public:
    Classroom(std::string, unsigned);
    bool operator==(const Classroom &other);
    bool sizeLessThan(const Classroom &other);
    FieldType getType();
    std::string getTypeName();
    std::string getName();
};

#endif
