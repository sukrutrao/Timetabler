#ifndef GLOBAL_H
#define GLOBAL_H

enum FieldType {
    classroom = 0,
    instructor = 1,
    isMinor = 2,
    program = 3,
    segment = 4,
    slot = 5
};

enum class ClauseType {
    hard,
    soft
};

class Global {
public:
    static const int FIELD_COUNT = 6;
};

#endif

