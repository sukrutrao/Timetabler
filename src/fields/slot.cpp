#include "fields/slot.h"

Time::Time(unsigned hours, unsigned minutes) {
    this->hours = hours;
    this->minutes = minutes;
}

Time& Time::operator=(const Time &other) {
    this->hours = other.hours;
    this->minutes = other.minutes;
}

bool Time::operator==(const Time &other) {
    return (this->hours == other.hours) && (this->minutes == other.minutes);
}

bool Time::operator<(const Time &other) {
    if(this->hours == other.hours) {
        return this->minutes < other.minutes;
    }
    else {
        return this->hours < other.hours;
    }
}

bool Time::operator<=(const Time &other) {
    return (*this < other) || (*this == other);
}

bool Time::operator>=(const Time &other) {
    return !(*this < other);
}

bool Time::operator>(const Time &other) {
    return !(*this <= other);
}

Slot::Slot(std::string name, Time &startTime, Time &endTime) {
    this->name = name;
    this->startTime = startTime;
    this->endTime = endTime;
}

bool Slot::operator==(const Slot &other) {
    return (this->name == other.name);
}

bool Slot::isIntersecting(const Slot &other) {
    if(this->startTime < other.startTime) {
        return !(this->endTime < other.startTime);    
    }
    else if(this->startTime > other.startTime) {
        return !(this->startTime > other.endTime);
    }
    return true;
}

FieldType Slot::getType() {
    return FieldType::slot;
}
