#include "utils.h"

#include <iomanip>
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

Log::Log(Severity severity) { this->severity = severity; }

Log::~Log() {
  if (severity == Severity::EMPTY) {
    std::cout << ss.str() << std::endl;
    return;
  }
  std::cout << "\033[" << getSeverityCode(severity) << "m";
  std::cout << std::setw(10) << std::left
            << "[" + getSeverityIdentifier(severity) + "]";
  std::cout << ss.str() << std::endl;
  std::cout << "\033[" << 0 << "m";
  if (severity == Severity::ERROR) {
    exit(1);
  }
}

int Log::getSeverityCode(Severity severity) {
  if (severity == Severity::EMPTY) return 0;
  if (severity == Severity::INFO)
    return 0;
  else if (severity == Severity::WARNING)
    return 33;
  else if (severity == Severity::ERROR)
    return 31;
  assert(false && "Severity code not defined!");
  return -1;
}

std::string Log::getSeverityIdentifier(Severity severity) {
  if (severity == Severity::EMPTY) return "";
  if (severity == Severity::INFO)
    return "INFO";
  else if (severity == Severity::WARNING)
    return "WARNING";
  else if (severity == Severity::ERROR)
    return "ERROR";
  assert(false && "Severity name not defined!");
  return "Invalid Type";
}

}  // namespace Utils