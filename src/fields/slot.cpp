#include "fields/slot.h"

#include "fields/is_minor.h"
#include "global.h"
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief      Constructs the Time object.
 *
 * @param[in]  hours    The hours
 * @param[in]  minutes  The minutes
 */
Time::Time(unsigned hours, unsigned minutes) {
    this->hours = hours;
    this->minutes = minutes;
}

/**
 * @brief      Constructs the Time object.
 *
 * @param[in]  time  The time represented as a string in the 24 hour format.
 *                   A valid input would be "12:30".
 */
Time::Time(std::string time) {
    this->hours = std::stoul(time.substr(0, 2));
    this->minutes = std::stoul(time.substr(3, 5));
}

/**
 * @brief      Assigns a time object to this.
 *
 * @param[in]  other  The object that is assigned
 *
 * @return     this Time object
 */
Time &Time::operator=(const Time &other) {
    this->hours = other.hours;
    this->minutes = other.minutes;
    return *this;
}

/**
 * @brief      Checks if two Time objects are identical, which is whether they
 *             represent the same hours and minutes.
 *
 * @param[in]  other  The Time object with which the comparison is being made
 *
 * @return     True if identical, False otherwise
 */
bool Time::operator==(const Time &other) {
    return (this->hours == other.hours) && (this->minutes == other.minutes);
}

/**
 * @brief      Checks if a Time is strictly before another.
 *
 * @param[in]  other  The Time object with which the comparison is being made
 *
 * @return     True if the Time of this object is strictly before the other
 * object, and False otherwise
 */
bool Time::operator<(const Time &other) {
    if (this->hours == other.hours) {
        return this->minutes < other.minutes;
    } else {
        return this->hours < other.hours;
    }
}

/**
 * @brief      Checks if a Time is before or identical to another.
 *
 * @param[in]  other  The Time object with which the comparison is being made
 *
 * @return     True if this is before or identical to the other object, False
 *             otherwise
 */
bool Time::operator<=(const Time &other) {
    return (*this < other) || (*this == other);
}

/**
 * @brief      Checks if a Time is after or identical to another.
 *
 * @param[in]  other  The Time object with which the comparison is being made
 *
 * @return     True if this is after or identical to the other object, False
 *             otherwise
 */
bool Time::operator>=(const Time &other) { return !(*this < other); }

/**
 * @brief      Checks if a Time is strictly after another.
 *
 * @param[in]  other  The Time object with which the comparison is being made
 *
 * @return     True if the Time of this object is strictly after the other
 * object, and False otherwise
 */
bool Time::operator>(const Time &other) { return !(*this <= other); }

/**
 * @brief      Gets the time as a string.
 *
 *             For example, 10 hours and 30 minutes is represented as "10:30".
 *
 * @return     The time string.
 */
std::string Time::getTimeString() {
    return std::to_string(hours) + ":" + std::to_string(minutes);
}

/**
 * @brief      Determines if the Time is a morning time.
 *
 * A Time is said to be a morning time if its hours value lies in the range
 * [0,12].
 *
 * @return     True if morning time, False otherwise.
 */
bool Time::isMorningTime() {
    if (hours >= 0 && hours < 13) {
        return true;
    }
    return false;
}

/**
 * @brief      Constructs the SlotElement object.
 *
 * @param      startTime  The start time
 * @param      endTime    The end time
 * @param[in]  day        The day
 */
SlotElement::SlotElement(Time &startTime, Time &endTime, Day day)
    : startTime(startTime), endTime(endTime) {
    this->day = day;
}

/**
 * @brief      Determines if two slot elements are intersecting.
 *
 * Two slot elements are said to be intersecting if the Day is
 * identical and there is an intersection in the start and end Times.
 *
 * @param      other  The SlotElement with which the comparison is being made
 *
 * @return     True if intersecting, False otherwise.
 */
bool SlotElement::isIntersecting(SlotElement &other) {
    if (this->day != other.day) {
        return false;
    }
    if (this->startTime < other.startTime) {
        return !(this->endTime <= other.startTime);
    } else if (this->startTime > other.startTime) {
        return !(this->startTime >= other.endTime);
    }
    return true;
}

/**
 * @brief      Determines if the SlotElement is a morning slot element.
 *
 * A SlotElement is a morning SlotElement if the start time is in the morning.
 *
 * @return     True if morning slot element, False otherwise.
 */
bool SlotElement::isMorningSlotElement() { return startTime.isMorningTime(); }

/**
 * @brief      Constructs the Slot object.
 *
 * @param[in]  name          The name of the Slot
 * @param[in]  isMinor       Indicates if it is a minor Slot
 * @param[in]  slotElements  The slot elements in the Slot
 */
Slot::Slot(std::string name, IsMinor isMinor,
           std::vector<SlotElement> slotElements)
    : isMinor(isMinor) {
    this->name = name;
    this->slotElements = slotElements;
}

/**
 * @brief      Checks if two Slot objects are identical, which is if they have
 * the same name.
 *
 * @param[in]  other  The Slot object with which the comparison is being made
 *
 * @return     True if identical, False otherwise
 */
bool Slot::operator==(const Slot &other) { return (this->name == other.name); }

/**
 * @brief      Determines if two Slots are intersecting.
 *
 * Two Slots are said to be intersecting if there exists a pair of SlotElements,
 * one from each Slot, such that the SlotElements are intersecting.
 *
 * @param      other  The Slot with which the comparison is being made
 *
 * @return     True if intersecting, False otherwise.
 */
bool Slot::isIntersecting(Slot &other) {
    for (int i = 0; i < slotElements.size(); i++) {
        for (int j = 0; j < other.slotElements.size(); j++) {
            if (slotElements[i].isIntersecting(other.slotElements[j])) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief      Adds a slot element to the Slot.
 *
 * @param[in]  slotElement  The slot element to be added.
 */
void Slot::addSlotElements(SlotElement slotElement) {
    slotElements.push_back(slotElement);
}

/**
 * @brief      Gets the type under the FieldType enum.
 *
 * @return     A member of the FieldType enum, which is FieldType::slot
 */
FieldType Slot::getType() { return FieldType::slot; }

/**
 * @brief      Determines if the Slot is a minor Slot.
 *
 * @return     True if minor slot, False otherwise.
 */
bool Slot::isMinorSlot() {
    return isMinor.getMinorType() == MinorType::isMinorCourse;
}

/**
 * @brief      Gets the type name, which is "Slot".
 *
 * @return     The string "Slot"
 */
std::string Slot::getTypeName() { return "Slot"; }

/**
 * @brief      Gets the name of the Slot.
 *
 * @return     The name.
 */
std::string Slot::getName() { return name; }

/**
 * @brief      Determines if the Slot is a morning Slot.
 *
 * A Slot is said to be a morning Slot if all its SlotElements
 * are morning SlotElements.
 *
 * @return     True if morning slot, False otherwise.
 */
bool Slot::isMorningSlot() {
    for (int i = 0; i < slotElements.size(); i++) {
        if (!slotElements[i].isMorningSlotElement()) {
            return false;
        }
    }
    return true;
}