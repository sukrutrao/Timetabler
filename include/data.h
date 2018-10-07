/** @file */

#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include "core/Solver.h"
#include "fields/classroom.h"
#include "fields/course.h"
#include "fields/instructor.h"
#include "fields/program.h"
#include "fields/segment.h"
#include "fields/slot.h"

using namespace NSPACE;

/**
 * @brief      Class for data.
 *
 * This represents all the data taken from the input.
 * This contains the list of all the allowed field values
 * for each FieldType, the variables for each
 * (Course, FieldType, field value), the high level variables
 * used for providing reasons if a time table could not be created,
 * the existing assignments given in the input, and the weights of
 * every constraint and exisiting assignment. Most of this
 * data is populated by the Parser after parsing fields and inputs.
 */
class Data {
 public:
  /**
   * Stores objects for each Course in the input
   */
  std::vector<Course> courses;
  /**
   * Stores objects for each possible Instructor
   */
  std::vector<Instructor> instructors;
  /**
   * Stores objects for each possible Classroom
   */
  std::vector<Classroom> classrooms;
  /**
   * Stores objects for each possible Program
   */
  std::vector<Program> programs;
  /**
   * Stores objects for each possible Segment
   */
  std::vector<Segment> segments;
  /**
   * Stores objects for each possible Slot
   */
  std::vector<Slot> slots;
  /**
   * Stores objects for each possible IsMinor value
   */
  std::vector<IsMinor> isMinors;
  /**
   * Stores the primary variables used in the solver.
   * The variables are in a 3D vector form.
   * They can be represented as a 3-tuple,
   * (Course, FieldType, field value), where the first
   * two dimensions represent the Course and the FieldType
   * the variable represents, and the third dimension
   * represents which field value of the given FieldType,
   * out of the allowed field values, the variable
   * corresponds to. These variables are added to the
   * solver by the Parser.
   */
  std::vector<std::vector<std::vector<Var>>> fieldValueVars;
  /**
   * Stores the high level variables. It is of the form
   * (Course, FieldType). If an assignment for a given
   * FieldType for the given Course could not be made, this
   * will be False, and is given as output so that the user
   * can make the necessary changes.
   */
  std::vector<std::vector<Var>> highLevelVars;
  /**
   * Stores the existing assignments for every Course and
   * FieldType as given in the input. This is of the form
   * (Course, FieldType, field value), where if assigned true,
   * the element is assigned l_True, and if false, is assigned
   * l_False. If no assignment is given, such as for a
   * Classroom or a Slot, the assigned value is l_Undef.
   */
  std::vector<std::vector<std::vector<lbool>>> existingAssignmentVars;
  /**
   * Stores the weights for the high level variables of each
   * FieldType. This represents the weight that must be given to
   * soft clauses which ensure that a value is found for each
   * FieldType for every Course. Negative weights imply the clause
   * is hard, and a zero weight implies that the input can be ignored
   * completely.
   */
  std::vector<int> highLevelVarWeights;
  /**
   * Stores the weights for the existing assignments given by the
   * user in the input. This represents the weight that must
   * be given in the soft clauses that ask that the value assigned
   * be the value that the user gave as input. Negative weights imply the
   * clause is hard, and a zero weight implies that the input can be ignored
   * completely.
   */
  std::vector<int> existingAssignmentWeights;
  /**
   * Stores the weights for the predefined clauses for the time
   * tabling problem. These have default values -1 for the
   * default hard constraints, and 1 for the soft constraints.
   * Changing these can be used to toggle between hard and soft
   * or to disable certain constraints.
   */
  std::vector<int> predefinedClausesWeights;
  Data();
};

#endif
