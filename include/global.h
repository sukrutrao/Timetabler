/** @file */

#ifndef GLOBAL_H
#define GLOBAL_H

/**
 * @brief      Enum that represents all the field types.
 */
enum FieldType { instructor, segment, isMinor, program, classroom, slot };

/**
 * @brief      Enum that represents all the predefined constraints.
 */
enum class PredefinedConstraint {
  instructorSingleCourseAtATime,   // 1
  classroomSingleCourseAtATime,    // 1
  programSingleCoreCourseAtATime,  // 1
  minorInMinorTime,
  exactlyOneSlotPerCourse,
  exactlyOneInstructorPerCourse,
  exactlyOneSegmentPerCourse,
  exactlyOneIsMinorPerCourse,
  exactlyOneClassroomPerCourse,
  coreInMorningTime,
  electiveInNonMorningTime,
  programAtMostOneOfCoreOrElective
};

/**
 * @brief      Class for global values.
 */
class Global {
 public:
  /**
   * The number of field types in the FieldType enumerator
   */
  static const int FIELD_COUNT = 6;
  /**
   * The number of predefined clauses in the PredefinedClauses enumerator
   */
  static const int PREDEFINED_CLAUSES_COUNT = 12;
};

#endif
