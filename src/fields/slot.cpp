#include "fields/slot.h"

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
