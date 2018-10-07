/** @file */

#ifndef SLOT_H
#define SLOT_H

#include <string>
#include <vector>
#include "fields/field.h"
#include "fields/is_minor.h"
#include "global.h"

/**
 * @brief      Enum Class to represent a day of the week.
 */
enum class Day {
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

/**
 * @brief      Class for a time unit.
 *
 * This can represent the hours and minutes as a time unit.
 */
class Time {
 private:
  /**
   * The hours in the 24 hour format. Range should be 0-23.
   */
  unsigned hours;
  /**
   * The minutes. Range should be 0-59.
   */
  unsigned minutes;

 public:
  Time(unsigned, unsigned);
  Time(std::string);
  Time &operator=(const Time &);
  bool operator==(const Time &);
  bool operator<(const Time &);
  bool operator<=(const Time &);
  bool operator>=(const Time &);
  bool operator>(const Time &);
  std::string getTimeString();
  bool isMorningTime();
};

/**
 * @brief      Class for a slot element.
 *
 * A slot element consists of a Day, a starting Time, and an ending Time.
 */
class SlotElement {
 private:
  /**
   * The start and end Time of the slot element
   */
  Time startTime, endTime;
  /**
   * The Day of the slot element
   */
  Day day;

 public:
  SlotElement(Time &, Time &, Day);
  bool isIntersecting(SlotElement &other);
  bool isMorningSlotElement();
};

/**
 * @brief      Class for a slot.
 *
 * A slot is a collection of slot elements. A slot can be designated to be
 * a minor slot, which means it is reserved for minor courses.
 */
class Slot : public Field {
 private:
  /**
   * The name of the Slot, which uniquely identifies it
   */
  std::string name;
  /**
   * Represents whether the Slot is a minor slot.
   * Minor slots are reserved for minor courses, by default.
   */
  IsMinor isMinor;
  /**
   * The slot elements that define this Slot
   */
  std::vector<SlotElement> slotElements;

 public:
  Slot(std::string, IsMinor, std::vector<SlotElement>);
  bool operator==(const Slot &other);
  bool isIntersecting(Slot &other);
  void addSlotElements(SlotElement);
  bool isMinorSlot();
  FieldType getType();
  std::string getTypeName();
  std::string getName();
  bool isMorningSlot();
};

#endif