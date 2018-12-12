/** @file */

#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include "fields/instructor.h"
#include "fields/is_minor.h"
#include "fields/program.h"
#include "fields/segment.h"

/**
 * @brief      Class for a course.
 */
class Course {
 private:
  /**
   * The name of the course, typically the course code.
   * This uniquely identifies a given course.
   */
  std::string name;
  /**
   * The class strength of the course. This is the
   * number of students registering for the course.
   */
  unsigned classSize;
  /**
   * The instructor of the course. This is the index of
   * the instructor in the list of possible instructors.
   */
  int instructor;
  /**
   * The programs for which this course is applicable.
   * This is the index of the programs in the list of
   * possible programs.
   */
  std::vector<int> programs;
  /**
   * The segment in which the course runs. This is the
   * index of the segment in the list of possible segments.
   */
  int segment;
  /**
   * Whether the course is a minor course or not. This is
   * the index of the type in the list of possible isMinor
   * values.
   */
  MinorType isMinor;

  int classroom = -1;

  int slot = -1;

 public:
  Course(std::string, unsigned, int, int, MinorType);
  Course(std::string, unsigned, int, int, MinorType, std::vector<int>);
  void setPrograms(std::vector<int>);
  void addProgram(int);
  void addClassroom(int);
  void addSlot(int);
  bool operator==(const Course &other);
  std::string getName();
  int getInstructor();
  std::vector<int> getPrograms();
  int getSegment();
  MinorType getIsMinor();
  int getClassroom();
  int getSlot();
  unsigned getClassSize();
};

#endif
