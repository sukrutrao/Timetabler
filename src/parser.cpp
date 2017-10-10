#include "parse.h"

void Parser::parseFields(std::string &file) {
    YAML::Node config = YAML::LoadFile(file);

    YAML::Node instructorsConfig = config["instructors"];
    for (YAML::Node& instructorNode : instructorsConfig) {
        Instructor instructor(instructorNode.as<std::string>());
        instructors.push_back(instructor);
    }

    YAML::Node classroomsConfig = config["classrooms"];
    for (YAML::Node& classroomNode : classroomsConfig) {
        Classroom classroom(classroomNode["number"].as<std::string>(), classroomNode["size"].as<unsigned>());
        classrooms.push_back(classroom);
    }

    YAML::Node segmentsConfig = config["segments"];
    unsigned segmentStart = segmentsConfig["start"].as<unsigned>();
    unsigned segmentEnd = segmentsConfig["end"].as<unsigned>();
    for (unsigned i = segmentStart; i<segmentEnd; ++i) {
        for (unsigned j = i; j<segmentEnd; ++j) {
            segments.push_back(Segment(i, j));
        }
    }

    YAML::Node slotsConfig = config["slots"];
    // TODO

}

void Parser::parseInput(std::string &file) {
    csv::Parser parser(file);
    for (unsigned i=0; i<parser.rowCount; ++i) {
        // TODO
    }
}