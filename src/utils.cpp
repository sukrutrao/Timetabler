#include "utils.h"

#include <iostream>
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
  if (fieldType == FieldType::classroom) return "Classroom";
  if (fieldType == FieldType::instructor) return "Instructor";
  if (fieldType == FieldType::isMinor) return "Minor Type";
  if (fieldType == FieldType::program) return "Programs";
  if (fieldType == FieldType::segment) return "Segment";
  if (fieldType == FieldType::slot) return "Slot";
  assert(false && "Invalid field type!");
  return "Invalid Type";
}

/**
 * @brief      Gets the predefined constraint name as a string.
 *
 * For example, the name of PredefinedClauses::instructorSingleCourseAtATime is
 * "instructorSingleCourseAtATime".
 *
 * @param[in]  clauseType  The PredefinedClauses member
 *
 * @return     The predefined constraint name.
 */
std::string getPredefinedConstraintName(const PredefinedClauses clauseType) {
  if (clauseType == PredefinedClauses::instructorSingleCourseAtATime)
    return "instructorSingleCourseAtATime";
  if (clauseType == PredefinedClauses::classroomSingleCourseAtATime)
    return "classroomSingleCourseAtATime";
  if (clauseType == PredefinedClauses::programSingleCoreCourseAtATime)
    return "programSingleCoreCourseAtATime";
  if (clauseType == PredefinedClauses::minorInMinorTime)
    return "minorInMinorTime";
  if (clauseType == PredefinedClauses::exactlyOneSlotPerCourse)
    return "exactlyOneSlotPerCourse";
  if (clauseType == PredefinedClauses::exactlyOneInstructorPerCourse)
    return "exactlyOneInstructorPerCourse";
  if (clauseType == PredefinedClauses::exactlyOneSegmentPerCourse)
    return "exactlyOneSegmentPerCourse";
  if (clauseType == PredefinedClauses::exactlyOneIsMinorPerCourse)
    return "exactlyOneIsMinorPerCourse";
  if (clauseType == PredefinedClauses::exactlyOneClassroomPerCourse)
    return "exactlyOneClassroomPerCourse";
  if (clauseType == PredefinedClauses::coreInMorningTime)
    return "coreInMorningTime";
  if (clauseType == PredefinedClauses::electiveInNonMorningTime)
    return "electiveInNonMorningTime";
  if (clauseType == PredefinedClauses::programAtMostOneOfCoreOrElective)
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
  if (fieldType == FieldType::isMinor) return data.isMinors[index].getName();
  if (fieldType == FieldType::program) return data.programs[index].getName();
  if (fieldType == FieldType::segment) return data.segments[index].getName();
  if (fieldType == FieldType::slot) return data.slots[index].getName();
  assert(false && "Invalid field type!");
  return "Invalid Type";
}

}  // namespace Utils

Log::Log() {}

Log::~Log() { std::cout << ss.str() << std::endl; }

template <class T>
Log &Log::operator<<(const T &input) {
  ss << input;
  return *this;
}