#ifndef FIELD_H
#define FIELD_H

#include <string>
#include "global.h"

class Field {
public:
    virtual FieldType getType() = 0; 
    virtual std::string getTypeName() = 0;  
};

#endif
