#include "fields/segment.h"

#include <cassert>
#include <iostream>
#include <string>
#include "global.h"

/**
 * @brief      Constructs the Segment object.
 *
 * @param[in]  startSegment  The start segment
 * @param[in]  endSegment    The end segment
 */
Segment::Segment(int startSegment, int endSegment) {
  assert((startSegment <= endSegment) && "Start Segment after End Segment!");
  this->startSegment = startSegment;
  this->endSegment = endSegment;
}

/**
 * @brief      Checks if two segment objects are identical, which is if their
 *             start and end segments are identical.
 *
 * @param[in]  other  The Segment object with which the comparison is being made
 *
 * @return     True if identical, False otherwise
 */
bool Segment::operator==(const Segment &other) {
  return ((this->startSegment == other.startSegment) &&
          (this->endSegment == other.endSegment));
}

/**
 * @brief      Gets the length of the Segment, which is the number
 *             of segment units it represents.
 *             For example, the length of the Segment 16 is 6.
 *
 * @return     The length
 */
int Segment::length() { return (endSegment - startSegment + 1); }

/**
 * @brief      Determines if two Segments are intersecting.
 *             Two segments are said to be intersecting if they contain a common
 *             segment unit. For example, 14 and 36 are intersecting, while 12
 *             and 36 are not.
 *
 * @param[in]  other  The Segment object with which the comparison is being made
 *
 * @return     True if intersecting, False otherwise.
 */
bool Segment::isIntersecting(const Segment &other) {
  if (this->startSegment < other.startSegment) {
    return !(this->endSegment < other.startSegment);
  } else if (this->startSegment > other.startSegment) {
    return !(this->startSegment > other.endSegment);
  }
  return true;
}

/**
 * @brief      Gets the type under the FieldType enum.
 *
 * @return     A member of the FieldType enum, which is FieldType::segment
 */
FieldType Segment::getType() { return FieldType::segment; }

/**
 * @brief      Gets the name of the Segment.
 *             The name is a string that concatenates the start and end segment
 *             values. For example, the name of 16 is "16".
 *
 * @return     The name
 */
std::string Segment::getName() {
  return std::to_string(startSegment) + std::to_string(endSegment);
}

/**
 * @brief      Gets the type name, which is "Segment".
 *
 * @return     The string "Segment"
 */
std::string Segment::getTypeName() { return "Segment"; }