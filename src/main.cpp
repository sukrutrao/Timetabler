#include "parser.h"
#include "constraint_encoder.h"
#include "core/Solver.h"
#include "constraint_adder.h"
#include "mtl/Vec.h"
#include "global.h"
#include "custom_parser.h"
#include <iostream>

TimeTabler *Global::timeTabler = nullptr;


int main(int argc, char const *argv[]) {
    TimeTabler *timeTabler = new TimeTabler();
    Global::timeTabler = timeTabler;
    Parser parser(timeTabler);
    parser.parseFields("config/testfields.yml");
    parser.parseInput("config/testinput.csv");
    parser.addVars();

    std::cout << "FieldValueVars" << std::endl;
    for (int i=0; i<timeTabler->data.fieldValueVars.size(); i++) {
        std::cout << "Course : " << timeTabler->data.courses[i].getName() << std::endl;
        for (int j=0; j<timeTabler->data.fieldValueVars[i][FieldType::classroom].size(); j++)
            std::cout << "Classroom : " << timeTabler->data.classrooms[j].getName()
                << " " << timeTabler->data.fieldValueVars[i][FieldType::classroom][j] << std::endl;

        for (int j=0; j<timeTabler->data.fieldValueVars[i][FieldType::instructor].size(); j++)
            std::cout << "Instructor : " << timeTabler->data.instructors[j].getName()
                << " " << timeTabler->data.fieldValueVars[i][FieldType::instructor][j] << std::endl;

        for (int j=0; j<timeTabler->data.fieldValueVars[i][FieldType::isMinor].size(); j++)
            std::cout << "IsMinor : " << timeTabler->data.isMinors[j].getMinorType()
                << " " << timeTabler->data.fieldValueVars[i][FieldType::isMinor][j] << std::endl;

        for (int j=0; j<timeTabler->data.fieldValueVars[i][FieldType::program].size(); j++)
            std::cout << "Program : " << timeTabler->data.programs[j].getName()
                << " " << timeTabler->data.fieldValueVars[i][FieldType::program][j] << std::endl;

        for (int j=0; j<timeTabler->data.fieldValueVars[i][FieldType::segment].size(); j++)
            std::cout << "Segment : " << timeTabler->data.segments[j].getName()
                << " " << timeTabler->data.fieldValueVars[i][FieldType::segment][j] << std::endl;

        for (int j=0; j<timeTabler->data.fieldValueVars[i][FieldType::slot].size(); j++)
            std::cout << "Slot : " << timeTabler->data.slots[j].getName()
                << " " << timeTabler->data.fieldValueVars[i][FieldType::slot][j] << std::endl;

    /*    for (int j=0; j<timeTabler->data.highLevelVars.size(); j++) {
            for(int k = 0; k < timeTabler->data.highLevelVars[j].size(); k++)
                std::cout << "HLV : Course : " << j << " Field : " << k << " " << timeTabler->data.highLevelVars[j][k] << std::endl;
        } */
            

        std::cout << std::endl;
    }

    ConstraintEncoder encoder(timeTabler);
    ConstraintAdder constraintAdder(&encoder, timeTabler);
    constraintAdder.addConstraints();
    parseCustomConstraints(&encoder, timeTabler);
//    timeTabler->addClauses(constraintAdder.addConstraints());
  //  timeTabler->addSoftClauses(constraintAdder.softConstraints());
    timeTabler->addHighLevelClauses();
    timeTabler->addExistingAssignments();
    // std::cout << "START\n";
    // timeTabler->addClauses(constraintAdder.classroomSingleCourseAtATime());
    // std::cout << "END\n";
    bool solved = timeTabler->solve();
    if (solved) {
        std::cout << "Solved" << std::endl;
        std::cout << timeTabler->checkAllTrue(timeTabler->data.highLevelVars[0]) << std::endl;
        std::cout << timeTabler->checkAllTrue(timeTabler->data.highLevelVars[1]) << std::endl;
    } else {
        std::cout << "Not Solved" << std::endl;
    }
    timeTabler->printResult();
    timeTabler->writeOutput("output.csv");
    delete timeTabler;
    return 0;
}
