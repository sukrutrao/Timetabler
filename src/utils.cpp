#include "utils.h"

#include "data.h"

namespace Utils {

/**
 * @brief      Gets the field type name as a string.
 *
 * For example, the name of FieldType::instructor is "Instructor".
 *
 * @param[in]  fieldType  The FieldType member
 *
 * @return     The field type name.
 */
std::string getFieldTypeName(FieldType fieldType) {
    if (fieldType == FieldType::classroom)
        return "Classroom";
    else if (fieldType == FieldType::instructor)
        return "Instructor";
    else if (fieldType == FieldType::isMinor)
        return "Minor Type";
    else if (fieldType == FieldType::program)
        return "Programs";
    else if (fieldType == FieldType::segment)
        return "Segment";
    else if (fieldType == FieldType::slot)
        return "Slot";
    assert(false && "Invalid field type!");
    return "Invalid Type";
}

std::string getPredefinedConstraintName(PredefinedClauses clauseType) {
    if (clauseType == instructorSingleCourseAtATime)
        return "instructorSingleCourseAtATime";
    if (clauseType == classroomSingleCourseAtATime)
        return "classroomSingleCourseAtATime";
    if (clauseType == programSingleCoreCourseAtATime)
        return "programSingleCoreCourseAtATime";
    if (clauseType == minorInMinorTime)
        return "minorInMinorTime";
    if (clauseType == exactlyOneSlotPerCourse)
        return "exactlyOneSlotPerCourse";
    if (clauseType == exactlyOneInstructorPerCourse)
        return "exactlyOneInstructorPerCourse";
    if (clauseType == exactlyOneSegmentPerCourse)
        return "exactlyOneSegmentPerCourse";
    if (clauseType == exactlyOneIsMinorPerCourse)
        return "exactlyOneIsMinorPerCourse";
    if (clauseType == exactlyOneClassroomPerCourse)
        return "exactlyOneClassroomPerCourse";
    if (clauseType == coreInMorningTime)
        return "coreInMorningTime";
    if (clauseType == electiveInNonMorningTime)
        return "electiveInNonMorningTime";
    if (clauseType == programAtMostOneOfCoreOrElective)
        return "programAtMostOneOfCoreOrElective";
    assert(false && "Invalid predefined constraint type!");
    return "Invalid Type";
}

/**
 * @brief      Gets the field name in the Data of a given FieldType at a
 *             given index
 *
 * @param[in]  fieldType  The FieldType member
 * @param[in]  index      The index
 * @param      data       The Data object
 *
 * @return     The field name as a string
 */
std::string getFieldName(FieldType fieldType, int index, Data &data) {
    if (fieldType == FieldType::classroom)
        return data.classrooms[index].getName();
    if (fieldType == FieldType::instructor)
        return data.instructors[index].getName();
    if (fieldType == FieldType::isMinor)
        return data.isMinors[index].getName();
    if (fieldType == FieldType::program)
        return data.programs[index].getName();
    if (fieldType == FieldType::segment)
        return data.segments[index].getName();
    if (fieldType == FieldType::slot)
        return data.slots[index].getName();
    assert(false && "Invalid field type!");
    return "Invalid Type";
}

} // namespace Utils