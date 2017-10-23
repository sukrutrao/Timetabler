#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include "fields/field.h"
#include "global.h"

/**
 * @brief      Class for segment.
 */
class Segment : public Field{
private:
	/**
	 * The start segment ID
	 */
    int startSegment;
    /**
     * The end segment ID
     */
    int endSegment;
public:
    Segment(int, int);
    bool operator==(const Segment &other);
    int length();
    bool isIntersecting(const Segment &other);
    FieldType getType();
    std::string getName();
    std::string getTypeName();
};

#endif
