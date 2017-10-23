#include "data.h"

#include "global.h"

/**
 * @brief      Constructs the Data object.
 * 
 * This fills in the default weight values for all the clauses. Other members
 * are left uninitialized and are filled in by the Parser.
 */
Data::Data() {
    highLevelVarWeights.resize(Global::FIELD_COUNT, 1);
    existingAssignmentWeights.resize(Global::FIELD_COUNT, 1);
    predefinedClausesWeights.resize(Global::PREDEFINED_CLAUSES_COUNT, -1);
    existingAssignmentWeights[FieldType::isMinor] = -1;
    existingAssignmentWeights[FieldType::program] = -1;
    existingAssignmentWeights[FieldType::instructor] = -1;
    predefinedClausesWeights[PredefinedClauses::coreInMorningTime] = 1;
}