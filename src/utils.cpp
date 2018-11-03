#include "utils.h"

#include <cctype>
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

Log::Log(Severity severity, bool isDebug, int lineWidth, int indentWidth)
    : metaWidth(10) {
  this->severity = severity;
  this->isDebug = isDebug;
  this->lineWidth = lineWidth - indentWidth;
  this->indentWidth = indentWidth;
  if (severity != Severity::EMPTY) {
    this->lineWidth -= metaWidth;
  }
}

Log::~Log() {
  if (isDebug) {
#ifdef DEBUG
    displayOutput(std::cerr);
#endif
  } else {
    displayOutput(std::cout);
  }
}

int Log::getSeverityCode() {
  if (severity == Severity::EMPTY) return DisplayColour::NORMAL;
  if (severity == Severity::INFO)
    return DisplayColour::NORMAL;
  else if (severity == Severity::WARNING)
    return DisplayColour::YELLOW;
  else if (severity == Severity::ERROR)
    return DisplayColour::RED;
  assert(false && "Severity code not defined!");
  return -1;
}

std::string Log::getSeverityIdentifier() {
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

void Log::displayOutput(std::ostream &out) {
  if (static_cast<int>(severity) <= verbosity) {
    if (severity == Severity::EMPTY) {
      out << std::string(indentWidth, ' ') << formatString(ss.str());
      return;
    }
    out << "\033[" << getSeverityCode() << "m";
    out << std::setw(metaWidth) << std::left
        << "[" + getSeverityIdentifier() + "]";
    out << std::string(indentWidth, ' ') << formatString(ss.str()) << std::endl;
    out << "\033[" << 0 << "m";
    if (severity == Severity::ERROR) {
      exit(1);
    }
  }
}

std::string Log::formatString(std::string str) {
  if (lineWidth <= 0) {
    return str;
  }
  int lastSpacePosition;
  unsigned strPos = 0;
  while (strPos < str.size()) {
    if (static_cast<int>(str.size() - strPos) <= lineWidth) break;
    lastSpacePosition = -1;
    for (int i = 0; i < lineWidth && strPos < str.size(); i++, strPos++) {
      if (std::isspace(str[strPos])) {
        lastSpacePosition = strPos;
      }
    }
    if (lastSpacePosition > 0) {
      str[lastSpacePosition] = '\n';
      if (indentWidth > 0) {
        str = applyIndent(str, lastSpacePosition);
        strPos = lastSpacePosition + indentWidth + 1;
      } else {
        strPos = lastSpacePosition + 1;
      }
    } else {
      while (strPos < str.size() && !std::isspace(str[strPos])) strPos++;
      if (strPos < str.size()) {
        str[strPos] = '\n';
        if (indentWidth > 0) {
          str = applyIndent(str, strPos);
          strPos = strPos + indentWidth;
        }
        strPos++;
      }
    }
  }
  return str;
}

std::string Log::applyIndent(std::string str, int position) {
  int indentToApply = indentWidth;
  if (severity != Severity::EMPTY) {
    indentToApply += metaWidth;
  }
  std::string strAfterThisPos = str.substr(position + 1);
  std::string strBeforeThisPos = str.substr(0, position + 1);
  return strBeforeThisPos + std::string(indentToApply, ' ') + strAfterThisPos;
}

void Log::setVerbosity(int verb) { verbosity = verb; }

int Log::verbosity = 3;

}  // namespace Utils