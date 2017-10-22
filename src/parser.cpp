#include "parser.h"

#include <iostream>

Parser::Parser(TimeTabler* timeTabler) {
    this->timeTabler = timeTabler;
}

void Parser::parseFields(std::string file) {
    YAML::Node config = YAML::LoadFile(file);

    YAML::Node instructorsConfig = config["instructors"];
    for (YAML::Node instructorNode : instructorsConfig) {
        Instructor instructor(instructorNode.as<std::string>());
        timeTabler->data.instructors.push_back(instructor);
    }

    YAML::Node classroomsConfig = config["classrooms"];
    for (YAML::Node classroomNode : classroomsConfig) {
        Classroom classroom(classroomNode["number"].as<std::string>(), classroomNode["size"].as<unsigned>());
        timeTabler->data.classrooms.push_back(classroom);
    }

    YAML::Node segmentsConfig = config["segments"];
    unsigned segmentStart = segmentsConfig["start"].as<unsigned>();
    unsigned segmentEnd = segmentsConfig["end"].as<unsigned>();
    for (unsigned i = segmentStart; i<=segmentEnd; ++i) {
        for (unsigned j = i; j<=segmentEnd; ++j) {
            timeTabler->data.segments.push_back(Segment(i, j));
        }
    }

    YAML::Node slotsConfig = config["slots"];
    for (YAML::Node slotNode : slotsConfig) {
        std::vector<SlotElement> slotElements;
        IsMinor isMinor = (slotNode["is_minor"].as<bool>()) ? MinorType::isMinorCourse : MinorType::isNotMinorCourse;
        for (YAML::Node periodNode : slotNode["time_periods"]) {
            Time start(periodNode["start"].as<std::string>());
            Time end(periodNode["end"].as<std::string>());
            Day day = getDayFromString(periodNode["day"].as<std::string>());
            slotElements.push_back(SlotElement(start, end, day));
        }
        timeTabler->data.slots.push_back(Slot(slotNode["name"].as<std::string>(), isMinor, slotElements));
    }

    timeTabler->data.isMinors.push_back(IsMinor(MinorType::isMinorCourse));
    timeTabler->data.isMinors.push_back(IsMinor(MinorType::isNotMinorCourse));

    YAML::Node programsConfig = config["programs"];
    for (YAML::Node programNode : programsConfig) {
        std::string name = programNode.as<std::string>();
        timeTabler->data.programs.push_back(Program(name, CourseType::core));
        timeTabler->data.programs.push_back(Program(name, CourseType::elective));
    }

    YAML::Node weightsConfig = config["weights"];
    timeTabler->data.existingAssignmentWeights[FieldType::instructor] = weightsConfig["instructor"][0].as<int>();
    timeTabler->data.highLevelVarWeights[FieldType::instructor] = weightsConfig["instructor"][1].as<int>();
    timeTabler->data.existingAssignmentWeights[FieldType::segment] = weightsConfig["segment"][0].as<int>();
    timeTabler->data.highLevelVarWeights[FieldType::segment] = weightsConfig["segment"][1].as<int>();
    timeTabler->data.existingAssignmentWeights[FieldType::isMinor] = weightsConfig["is_minor"][0].as<int>();
    timeTabler->data.highLevelVarWeights[FieldType::isMinor] = weightsConfig["is_minor"][1].as<int>();
    timeTabler->data.existingAssignmentWeights[FieldType::program] = weightsConfig["program"][0].as<int>();
    timeTabler->data.existingAssignmentWeights[FieldType::classroom] = weightsConfig["classroom"][0].as<int>();
    timeTabler->data.highLevelVarWeights[FieldType::classroom] = weightsConfig["classroom"][1].as<int>();
    timeTabler->data.existingAssignmentWeights[FieldType::slot] = weightsConfig["slot"][0].as<int>();
    timeTabler->data.highLevelVarWeights[FieldType::slot] = weightsConfig["slot"][1].as<int>();

}

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

void Parser::parseInput(std::string file) {
    csv::Parser parser(file);
    timeTabler->data.existingAssignmentVars.clear();
    for (unsigned i=0; i<parser.rowCount(); ++i) {
        std::vector<std::vector<lbool>> assignmentsThisCourse(Global::FIELD_COUNT);

        std::string name = parser[i]["name"];
        std::string classSizeStr = parser[i]["class_size"];
        unsigned classSize = unsigned(std::stoi(classSizeStr));
        
        std::string instructorStr = parser[i]["instructor"];
        int instructor = -1;
        for (unsigned i = 0; i < timeTabler->data.instructors.size(); i++) {
            if (timeTabler->data.instructors[i].getName() == instructorStr) {
                instructor = i;
                assignmentsThisCourse[FieldType::instructor].push_back(l_True);
                continue;
            }
            assignmentsThisCourse[FieldType::instructor].push_back(l_False);
        }
        if (instructor == -1) {
            // TODO Error and exit
        }
        std::string segmentStr = parser[i]["segment"];
        int segment = -1;
        for (unsigned i = 0; i < timeTabler->data.segments.size(); i++) {
            if (timeTabler->data.segments[i].getName() == segmentStr) {
                segment = i;
                assignmentsThisCourse[FieldType::segment].push_back(l_True);
                continue;
            }
            std::cout << "SEG : " << timeTabler->data.segments[i].getName() << " " << segmentStr << std::endl;
            assignmentsThisCourse[FieldType::segment].push_back(l_False);
        }
        if (segment == -1) {
            // TODO Error and exit
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
            // TODO Error and exit
        }
        Course course(name, classSize, instructor, segment, isMinor);

        for (unsigned j = 0; j < timeTabler->data.programs.size(); j+=2) {
            std::string s = timeTabler->data.programs[j].getName();
            if (parser[i][s] == "Core") {
                course.addProgram(j);
                assignmentsThisCourse[FieldType::program].push_back(l_True);
                assignmentsThisCourse[FieldType::program].push_back(l_False);
            } else if (parser[i][s] == "Elective") {
                course.addProgram(j+1);
                assignmentsThisCourse[FieldType::program].push_back(l_False);
                assignmentsThisCourse[FieldType::program].push_back(l_True);
            } else if (parser[i][s] == "No") {
                assignmentsThisCourse[FieldType::program].push_back(l_False);
                assignmentsThisCourse[FieldType::program].push_back(l_False);
            } else {
                // TODO Error and exit
            }
        }

        std::string classroomStr = parser[i]["classroom"];
        std::string slotStr = parser[i]["slot"];
        assignmentsThisCourse[FieldType::classroom].resize(timeTabler->data.classrooms.size(), l_Undef);
        assignmentsThisCourse[FieldType::slot].resize(timeTabler->data.slots.size(), l_Undef);
        timeTabler->data.courses.push_back(course);
        timeTabler->data.existingAssignmentVars.push_back(assignmentsThisCourse);
    }
}

void Parser::addVars() {
    for (Course c : timeTabler->data.courses) {
        std::vector<std::vector<Var>> courseVars;
        courseVars.resize(Global::FIELD_COUNT);
        for (Classroom cr : timeTabler->data.classrooms) {
            Var v = timeTabler->newVar(); // TODO create vars using solver
            courseVars[FieldType::classroom].push_back(v);
        }
        for (Instructor i : timeTabler->data.instructors) {
            Var v = timeTabler->newVar();
            courseVars[FieldType::instructor].push_back(v);
        }
        for (IsMinor i : timeTabler->data.isMinors) {
            Var v = timeTabler->newVar();
            courseVars[FieldType::isMinor].push_back(v);
        }
        for (Program p : timeTabler->data.programs) {
            Var v = timeTabler->newVar();
            courseVars[FieldType::program].push_back(v);
        }
        for (Segment s : timeTabler->data.segments) {
            Var v = timeTabler->newVar();
            courseVars[FieldType::segment].push_back(v);
        }
        for (Slot s : timeTabler->data.slots) {
            Var v = timeTabler->newVar();
            courseVars[FieldType::slot].push_back(v);
        }
        timeTabler->data.fieldValueVars.push_back(courseVars);

        std::vector<Var> highLevelCourseVars;
        for (unsigned i = 0; i < Global::FIELD_COUNT; ++i) {
            Var v = timeTabler->newVar();
            highLevelCourseVars.push_back(v);
        }
        timeTabler->data.highLevelVars.push_back(highLevelCourseVars);
    }
}