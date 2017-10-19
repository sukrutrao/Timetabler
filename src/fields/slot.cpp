#include "fields/slot.h"

#include <string>
#include <vector>
#include <iostream>
#include "global.h"
#include "fields/is_minor.h"

Time::Time(unsigned hours, unsigned minutes) {
    this->hours = hours;
    this->minutes = minutes;
}

Time::Time(std::string time) {
    this->hours = std::stoul(time.substr(0, 2));
    this->minutes = std::stoul(time.substr(3, 5));
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

std::string Time::getTimeString() {
    return std::to_string(hours)+":"+std::to_string(minutes);
}

SlotElement::SlotElement(Time &startTime, Time &endTime, Day day) 
    : startTime(startTime), endTime(endTime) {
    this->day = day;
}

bool SlotElement::isIntersecting(SlotElement &other) {
    std::cout << "TT : " << startTime.getTimeString() << "-" << endTime.getTimeString() << std::endl;
    std::cout << "OT : " << other.startTime.getTimeString() << "-" << other.endTime.getTimeString() << std::endl;
    if(this->day != other.day) {
        std::cout << "0" << std::endl;
        return false;
    }
    if(this->startTime < other.startTime) {
        std::cout << !(this->endTime <= other.startTime) << std::endl;
        return !(this->endTime <= other.startTime);    
    }
    else if(this->startTime > other.startTime) {
        std::cout << !(this->startTime >= other.endTime) << std::endl;
        return !(this->startTime >= other.endTime);
    }
    return true;
}


Slot::Slot(std::string name, IsMinor isMinor, std::vector<SlotElement> slotElements)
    : isMinor(isMinor) {
    this->name = name;
    this->slotElements = slotElements;
}

bool Slot::operator==(const Slot &other) {
    return (this->name == other.name);
}

bool Slot::isIntersecting(Slot &other) {
    for(int i = 0; i < slotElements.size(); i++) {
        for(int j = 0; j < other.slotElements.size(); j++) {
            if(slotElements[i].isIntersecting(other.slotElements[j])) {
                return true;
            }
        }
    }
    return false;
}

void Slot::addSlotElements(SlotElement slotElement) {
    slotElements.push_back(slotElement);
}

FieldType Slot::getType() {
    return FieldType::slot;
}

bool Slot::isMinorSlot() {
    return isMinor.getMinorType() == MinorType::isMinorCourse;
}

std::string Slot::getTypeName() {
    return "Slot";
}

std::string Slot::getName() {
    return name;
}