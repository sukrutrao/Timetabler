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

/**
 * @brief      Constructor for the Logger.
 *
 * @param[in]  severity     The severity level to log with
 * @param[in]  isDebug      Indicates if in debug mode
 * @param[in]  lineWidth    The width to fix per line, 0 for no limit
 * @param[in]  indentWidth  The width by which output is to be indented
 */
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

/**
 * @brief      Displays output and destroys object.
 */
Log::~Log() {
  if (isDebug) {
#ifdef DEBUG
    std::cerr << "[DEBUG]";
    displayOutput(std::cerr);
#endif
  } else {
    displayOutput(std::cout);
  }
}

/**
 * @brief      Gets the colour for the severity level as a member of the
 * DisplayColour enum.
 *
 * @return     The colour corresponding to the severity level
 */
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

/**
 * @brief      Gets the name of the severity level ("INFO" for Severity::INFO,
 * and so on).
 *
 * @return     The name of the severity level as a string
 */
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

/**
 * @brief      Displays the output to a specified output stream (such as
 * std::cout).
 *
 * @param      out   The output stream to send the output to
 */
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

/**
 * @brief      Formats a string according to the parameters specified. Handles
 * transformations to conform with fixed line width and fixed indent width.
 *
 * @param[in]  str   The string to format
 *
 * @return     The formatted string
 */
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

/**
 * @brief      Adds spaces to a string at a given position based on the indent
 * width specified.
 *
 * @param[in]  str       The string to format
 * @param[in]  position  The position to add spaces to
 *
 * @return     The formatted string
 */
std::string Log::applyIndent(std::string str, int position) {
  int indentToApply = indentWidth;
  if (severity != Severity::EMPTY) {
    indentToApply += metaWidth;
  }
  std::string strAfterThisPos = str.substr(position + 1);
  std::string strBeforeThisPos = str.substr(0, position + 1);
  return strBeforeThisPos + std::string(indentToApply, ' ') + strAfterThisPos;
}

/**
 * @brief      Sets the verbosity level for logging. (0 - EMPTY, 1 - ERROR, 2 -
 * WARNING, 3 - INFO). All messages of levels in and below the current verbosity
 * level are displayed in the output.
 *
 * @param[in]  verb  The verbosity level
 */
void Log::setVerbosity(int verb) { verbosity = verb; }

int Log::verbosity = 3;

}  // namespace Utils