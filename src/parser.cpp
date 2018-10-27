#include "parser.h"

#include <cstdlib>
#include <iostream>

/**
 * @brief      Constructs the Parser object.
 *
 * @param      timetabler  The time tabler
 */
Parser::Parser(Timetabler *timetabler) { this->timetabler = timetabler; }

/**
 * @brief      Parse the fields given in a file.
 *
 * @param[in]  file  The file containing the fields
 */
void Parser::parseFields(std::string file) {
  YAML::Node config = YAML::LoadFile(file);

  YAML::Node instructorsConfig = config["instructors"];
  for (YAML::Node instructorNode : instructorsConfig) {
    Instructor instructor(instructorNode.as<std::string>());
    timetabler->data.instructors.push_back(instructor);
  }

  YAML::Node classroomsConfig = config["classrooms"];
  for (YAML::Node classroomNode : classroomsConfig) {
    Classroom classroom(classroomNode["number"].as<std::string>(),
                        classroomNode["size"].as<unsigned>());
    timetabler->data.classrooms.push_back(classroom);
  }

  YAML::Node segmentsConfig = config["segments"];
  unsigned segmentStart = segmentsConfig["start"].as<unsigned>();
  unsigned segmentEnd = segmentsConfig["end"].as<unsigned>();
  for (unsigned i = segmentStart; i <= segmentEnd; ++i) {
    for (unsigned j = i; j <= segmentEnd; ++j) {
      timetabler->data.segments.push_back(Segment(i, j));
    }
  }

  YAML::Node slotsConfig = config["slots"];
  for (YAML::Node slotNode : slotsConfig) {
    std::vector<SlotElement> slotElements;
    IsMinor isMinor = (slotNode["is_minor"].as<bool>())
                          ? MinorType::isMinorCourse
                          : MinorType::isNotMinorCourse;
    for (YAML::Node periodNode : slotNode["time_periods"]) {
      Time start(periodNode["start"].as<std::string>());
      Time end(periodNode["end"].as<std::string>());
      Day day = getDayFromString(periodNode["day"].as<std::string>());
      slotElements.push_back(SlotElement(start, end, day));
    }
    timetabler->data.slots.push_back(
        Slot(slotNode["name"].as<std::string>(), isMinor, slotElements));
  }

  timetabler->data.isMinors.push_back(IsMinor(MinorType::isMinorCourse));
  timetabler->data.isMinors.push_back(IsMinor(MinorType::isNotMinorCourse));

  YAML::Node programsConfig = config["programs"];
  for (YAML::Node programNode : programsConfig) {
    std::string name = programNode.as<std::string>();
    timetabler->data.programs.push_back(Program(name, CourseType::core));
    timetabler->data.programs.push_back(Program(name, CourseType::elective));
  }

  YAML::Node weightsConfig = config["weights"];
  timetabler->data.existingAssignmentWeights[FieldType::instructor] =
      weightsConfig["instructor"][0].as<int>();
  timetabler->data.highLevelVarWeights[FieldType::instructor] =
      2 * weightsConfig["instructor"][1].as<int>();
  timetabler->data.existingAssignmentWeights[FieldType::segment] =
      weightsConfig["segment"][0].as<int>();
  timetabler->data.highLevelVarWeights[FieldType::segment] =
      2 * weightsConfig["segment"][1].as<int>();
  timetabler->data.existingAssignmentWeights[FieldType::isMinor] =
      weightsConfig["is_minor"][0].as<int>();
  timetabler->data.highLevelVarWeights[FieldType::isMinor] =
      2 * weightsConfig["is_minor"][1].as<int>();
  timetabler->data.existingAssignmentWeights[FieldType::program] =
      weightsConfig["program"].as<int>();
  timetabler->data.existingAssignmentWeights[FieldType::classroom] =
      weightsConfig["classroom"][0].as<int>();
  timetabler->data.highLevelVarWeights[FieldType::classroom] =
      2 * weightsConfig["classroom"][1].as<int>();
  timetabler->data.existingAssignmentWeights[FieldType::slot] =
      weightsConfig["slot"][0].as<int>();
  timetabler->data.highLevelVarWeights[FieldType::slot] =
      2 * weightsConfig["slot"][1].as<int>();

  YAML::Node predefinedWeightsConfig = config["predefined_weights"];
  for (YAML::Node predefinedWeightNode : predefinedWeightsConfig) {
    unsigned clauseNo = predefinedWeightNode["clause"].as<int>();
    int weight = predefinedWeightNode["weight"].as<int>();
    timetabler->data.predefinedClausesWeights[clauseNo] = weight;
  }
}

/**
 * @brief      Gets the day from the string as a member of the Day enum.
 *
 * For example, the input "Monday" returns Day::Monday.
 *
 * @param[in]  day   The day as a string
 *
 * @return     A member of the Day enum, corresponding the the day the string
 * represented
 */
Day Parser::getDayFromString(std::string day) {
  if (day == "Monday") return Day::Monday;
  if (day == "Tuesday") return Day::Tuesday;
  if (day == "Wednesday") return Day::Wednesday;
  if (day == "Thursday") return Day::Thursday;
  if (day == "Friday") return Day::Friday;
  if (day == "Saturday") return Day::Saturday;
  if (day == "Sunday") return Day::Sunday;
  assert(false && "Incorrect day");
  return Day::Monday;
}

/**
 * @brief      Parses the input given in a file.
 *
 * @param[in]  file  The file containig the input
 */
void Parser::parseInput(std::string file) {
  csv::Parser parser(file);
  timetabler->data.existingAssignmentVars.clear();
  for (unsigned i = 0; i < parser.rowCount(); ++i) {
    std::vector<std::vector<lbool>> assignmentsThisCourse(Global::FIELD_COUNT);

    std::string name = parser[i]["name"];
    std::string classSizeStr = parser[i]["class_size"];
    unsigned classSize = unsigned(std::stoi(classSizeStr));

    std::string instructorStr = parser[i]["instructor"];
    int instructor = -1;
    for (unsigned j = 0; j < timetabler->data.instructors.size(); j++) {
      if (timetabler->data.instructors[j].getName() == instructorStr) {
        instructor = j;
        assignmentsThisCourse[FieldType::instructor].push_back(l_True);
        continue;
      }
      assignmentsThisCourse[FieldType::instructor].push_back(l_False);
    }
    if (instructor == -1) {
      std::cout << "Input contains invalid Instructor name" << std::endl;
      exit(1);
    }
    std::string segmentStr = parser[i]["segment"];
    int segment = -1;
    for (unsigned j = 0; j < timetabler->data.segments.size(); j++) {
      if (timetabler->data.segments[j].getName() == segmentStr) {
        segment = j;
        assignmentsThisCourse[FieldType::segment].push_back(l_True);
        continue;
      }
      assignmentsThisCourse[FieldType::segment].push_back(l_False);
    }
    if (segment == -1) {
      std::cout << "Input contains invalid Segment name" << std::endl;
      exit(1);
    }
    std::string isMinorStr = parser[i]["is_minor"];
    int isMinor;
    if (isMinorStr == "Yes") {
      isMinor = 0;
      assignmentsThisCourse[FieldType::isMinor].push_back(l_True);
    } else if (isMinorStr == "No") {
      isMinor = 1;
      assignmentsThisCourse[FieldType::isMinor].push_back(l_False);
    } else {
      std::cout << "Input contains invalid IsMinor value (should be "
                   "'Yes' or 'No')"
                << std::endl;
      exit(1);
    }
    Course course(name, classSize, instructor, segment, isMinor);

    for (unsigned j = 0; j < timetabler->data.programs.size(); j += 2) {
      std::string s = timetabler->data.programs[j].getName();
      if (parser[i][s] == "Core") {
        course.addProgram(j);
        assignmentsThisCourse[FieldType::program].push_back(l_True);
        assignmentsThisCourse[FieldType::program].push_back(l_False);
      } else if (parser[i][s] == "Elective") {
        course.addProgram(j + 1);
        assignmentsThisCourse[FieldType::program].push_back(l_False);
        assignmentsThisCourse[FieldType::program].push_back(l_True);
      } else if (parser[i][s] == "No") {
        assignmentsThisCourse[FieldType::program].push_back(l_False);
        assignmentsThisCourse[FieldType::program].push_back(l_False);
      } else {
        std::cout << "Input contains invalid Program type (should be "
                     "'Core', 'Elective', or 'No')"
                  << std::endl;
        exit(1);
      }
    }

    std::string classroomStr = parser[i]["classroom"];
    std::string slotStr = parser[i]["slot"];
    bool foundClassroom = false;
    bool foundSlot = false;
    assignmentsThisCourse[FieldType::classroom].resize(
        timetabler->data.classrooms.size(), l_Undef);
    assignmentsThisCourse[FieldType::slot].resize(timetabler->data.slots.size(),
                                                  l_Undef);
    if (classroomStr != "") {
      for (unsigned j = 0; j < timetabler->data.classrooms.size(); j++) {
        if (timetabler->data.classrooms[j].getName() == classroomStr) {
          assignmentsThisCourse[FieldType::classroom][j] = l_True;
          foundClassroom = true;
          course.addClassroom(j);
          continue;
        }
        assignmentsThisCourse[FieldType::classroom][j] = l_False;
      }
      if (!foundClassroom) {
        std::cout << "Input contains invalid Classroom name" << std::endl;
        exit(1);
      }
    }
    if (slotStr != "") {
      for (unsigned j = 0; j < timetabler->data.slots.size(); j++) {
        if (timetabler->data.slots[j].getName() == slotStr) {
          assignmentsThisCourse[FieldType::slot][j] = l_True;
          foundSlot = true;
          course.addSlot(j);
          continue;
        }
        assignmentsThisCourse[FieldType::slot][j] = l_False;
      }
      if (!foundSlot) {
        std::cout << "Input contains invalid Slot name" << std::endl;
        exit(1);
      }
    }
    timetabler->data.courses.push_back(course);
    timetabler->data.existingAssignmentVars.push_back(assignmentsThisCourse);
  }
}

bool Parser::verify() {
  bool result = true;
  for (auto course1 : timetabler->data.courses) {
    if (course1.getIsMinor() && course1.getSlot() != -1) {
      if (timetabler->data.slots[course1.getSlot()].isMinorSlot()) {
        std::cout << course1.getName()
                  << " which is minor course is scheduled in non minor slot."
                  << std::endl;
        result = false;
      }
    }
    for (auto course2 : timetabler->data.courses) {
      if (course1.getName() == course2.getName()) continue;
      bool segementIntersecting =
          timetabler->data.segments[course1.getSegment()].isIntersecting(
              timetabler->data.segments[course2.getSegment()]);
      bool classroomSame = (course1.getClassroom() != -1 &&
                            course1.getClassroom() == course2.getClassroom());
      // bool slotSame =
      //     (course1.getSlot() != -1 && course1.getSlot() ==
      //     course2.getSlot());
      bool slotIntersecting =
          (course1.getSlot() != -1 && course2.getSlot() != -1)
              ? (timetabler->data.slots[course1.getSlot()].isIntersecting(
                    timetabler->data.slots[course2.getSlot()]))
              : false;
      if (segementIntersecting && slotIntersecting) {
        if (course1.getInstructor() == course2.getInstructor()) {
          std::cout << course1.getName() << " and " << course2.getName()
                    << " having same instructor clash." << std::endl;
          result = false;
        }
        if (classroomSame) {
          std::cout << course1.getName() << " and " << course2.getName()
                    << " having same classroom clash." << std::endl;
          result = false;
        }
        for (auto program1 : course1.getPrograms()) {
          for (auto program2 : course2.getPrograms()) {
            if (program1 == program2) {
              if (timetabler->data.programs[program1].isCoreProgram() &&
                  timetabler->data.programs[program2].isCoreProgram()) {
                std::cout << course1.getName() << " and " << course2.getName()
                          << " which have common core program "
                          << timetabler->data.programs[program1].getName()
                          << " clash." << std::endl;
                result = false;
              }
            }
          }
        }
      }
    }
  }
  return result;
}

/**
 * @brief      Requests for variables to be added to the solver and stores the
 * data.
 */
void Parser::addVars() {
  for (Course c : timetabler->data.courses) {
    std::vector<std::vector<Var>> courseVars;
    courseVars.resize(Global::FIELD_COUNT);
    for (Classroom cr : timetabler->data.classrooms) {
      Var v = timetabler->newVar();
      courseVars[FieldType::classroom].push_back(v);
    }
    for (Instructor i : timetabler->data.instructors) {
      Var v = timetabler->newVar();
      courseVars[FieldType::instructor].push_back(v);
    }
    for (IsMinor i : timetabler->data.isMinors) {
      Var v = timetabler->newVar();
      courseVars[FieldType::isMinor].push_back(v);
    }
    for (Program p : timetabler->data.programs) {
      Var v = timetabler->newVar();
      courseVars[FieldType::program].push_back(v);
    }
    for (Segment s : timetabler->data.segments) {
      Var v = timetabler->newVar();
      courseVars[FieldType::segment].push_back(v);
    }
    for (Slot s : timetabler->data.slots) {
      Var v = timetabler->newVar();
      courseVars[FieldType::slot].push_back(v);
    }
    timetabler->data.fieldValueVars.push_back(courseVars);

    std::vector<Var> highLevelCourseVars;
    for (unsigned i = 0; i < Global::FIELD_COUNT; ++i) {
      Var v = timetabler->newVar();
      highLevelCourseVars.push_back(v);
    }
    timetabler->data.highLevelVars.push_back(highLevelCourseVars);
  }
  for (unsigned i = 0; i < Global::PREDEFINED_CLAUSES_COUNT; i++) {
    Var v = timetabler->newVar();
    timetabler->data.predefinedConstraintVars.push_back(v);
  }
}