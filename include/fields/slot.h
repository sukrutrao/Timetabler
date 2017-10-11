#ifndef SLOT_H
#define SLOT_H

#include <string>
#include "fields/field.h"
#include "global.h"

enum class Day {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

class Time {
private:
    unsigned hours;
    unsigned minutes;
public:
    Time(unsigned, unsigned);
    Time(std::string &);
    Time& operator=(const Time&);
    bool operator==(const Time&);
    bool operator<(const Time&);
    bool operator<=(const Time&);  
    bool operator>=(const Time&);
    bool operator>(const Time&);
};

class SlotElement {
private:
    Time startTime, endTime;
    Day day;
public:
    SlotElement(Time&, Time&, Day);
    bool isIntersecting(const SlotElement &other);
};

class Slot : public Field {
private:
    std::string name;
    IsMinor isMinor
    std::vector<SlotElement> slotElements;
public:
    Slot(std::string, IsMinor, std::vector<SlotElement>);
    bool operator==(const Slot &other);
    bool isIntersecting(const Slot &other);
    void addSlotElements(SlotElement);
    FieldType getType();
};

#endif