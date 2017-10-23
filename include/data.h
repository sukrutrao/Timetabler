#ifndef DATA_H
#define DATA_H

#include "fields/course.h"
#include "fields/instructor.h"
#include "fields/classroom.h"
#include "fields/program.h"
#include "fields/segment.h"
#include "fields/slot.h"
#include "core/Solver.h"
#include <vector>
#include <functional>
#include <string>
#include <utility>
#include <map>

using namespace Minisat;

class Data {
public:
    std::vector<Course> courses;
    std::vector<Instructor> instructors;
    std::vector<Classroom> classrooms;
    std::vector<Program> programs;
    std::vector<Segment> segments;
    std::vector<Slot> slots;
    std::vector<IsMinor> isMinors;
    std::vector<std::vector<std::vector<Var>>> fieldValueVars;
    std::vector<std::vector<Var>> highLevelVars;
    std::vector<std::vector<std::vector<lbool>>> existingAssignmentVars;
    std::vector<int> highLevelVarWeights;
    std::vector<int> existingAssignmentWeights;
    std::vector<int> predefinedClausesWeights;
    // TODO - this is only a temporary measure - fix this
    Data() {
        highLevelVarWeights.resize(6, 1);
        existingAssignmentWeights.resize(6, 1);
        predefinedClausesWeights.resize(11, -1);
        existingAssignmentWeights[FieldType::isMinor] = -1;
        existingAssignmentWeights[FieldType::program] = -1;
        predefinedClausesWeights[PredefinedClauses::coreInMorningTime] = 1;
        predefinedClausesWeights[PredefinedClauses::programSingleCoreCourseAtATime] = -1;
    }
};

#endif