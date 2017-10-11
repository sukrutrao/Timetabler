#include "parser.h"

Parser::Parser(TimeTabler* timeTabler) {
    this->timeTabler = timeTabler;
}

void Parser::parseFields(std::string &file) {
    YAML::Node config = YAML::LoadFile(file);

    YAML::Node instructorsConfig = config["instructors"];
    for (YAML::Node& instructorNode : instructorsConfig) {
        Instructor instructor(instructorNode.as<std::string>());
        timeTabler->data.instructors.push_back(instructor);
    }

    YAML::Node classroomsConfig = config["classrooms"];
    for (YAML::Node& classroomNode : classroomsConfig) {
        Classroom classroom(classroomNode["number"].as<std::string>(), classroomNode["size"].as<unsigned>());
        timeTabler->data.classrooms.push_back(classroom);
    }

    YAML::Node segmentsConfig = config["segments"];
    unsigned segmentStart = segmentsConfig["start"].as<unsigned>();
    unsigned segmentEnd = segmentsConfig["end"].as<unsigned>();
    for (unsigned i = segmentStart; i<segmentEnd; ++i) {
        for (unsigned j = i; j<segmentEnd; ++j) {
            timeTabler->data.segments.push_back(Segment(i, j));
        }
    }

    YAML::Node slotsConfig = config["slots"];
    for (YAML::Node& slotNode : slotsConfig) {
        std::vector<SlotElement> slotElements;
        IsMinor isMinor = (slotNode["is_minor"].as<bool>()) ? IsMinor::isMinor : IsMinor::isNotMinor;
        for (YAML::Node& periodNode : slotElements["time_periods"]) {
            Time start(periodNode["start"].as<std::string>());
            Time end(periodNode["end"].as<std::string>())
            Day day = getDayFromString(periodNode["day"].as<std::string>());
            slotElements.push_back(SlotElement(start, end, day));
        }
        timeTabler->data.slots.push_back(Slot(slotNode["name"].as<std::string>(), isMinor, slotElements));
    }

    

}

Day Parser::getDayFromString(std::string &day) {
    if (day == "Monday") return Day::Monday;
    if (day == "Tuesday") return Day::Tuesday;
    if (day == "Wednesday") return Day::Wednesday;
    if (day == "Thursady") return Day::Thursady;
    if (day == "Friday") return Day::Friday;
    if (day == "Saturday") return Day::Saturday;
    if (day == "Sunday") return Day::Sunday;
    assert(false && "Incorrect day");
    return Day::Monday;
}

void Parser::parseInput(std::string &file) {
    csv::Parser parser(file);
    for (unsigned i=0; i<parser.rowCount; ++i) {
        // TODO
    }
}