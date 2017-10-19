#include "fields/segment.h"

#include <cassert>
#include <string>
#include <iostream>
#include "global.h"

Segment::Segment(int startSegment, int endSegment) {
    assert((startSegment <= endSegment) && "Start Segment after End Segment!");
    this->startSegment = startSegment;
    this->endSegment = endSegment;
}

bool Segment::operator==(const Segment &other) {
    return ((this->startSegment == other.startSegment) && (this->endSegment && other.endSegment));
}

int Segment::length() {
    return (endSegment - startSegment + 1);
}

bool Segment::isIntersecting(const Segment &other) {
    std::cout << "II1 : " << this->startSegment << " " << this->endSegment << std::endl;
    std::cout << "II2 : " << other.startSegment << " " << other.endSegment << std::endl;
    if(this->startSegment < other.startSegment) {
        std::cout << "If1 " << !(this->endSegment < other.startSegment) << std::endl;
        return !(this->endSegment < other.startSegment);    
    }
    else if(this->startSegment > other.startSegment) {
        std::cout << "If2 " << !(this->startSegment > other.endSegment) << std::endl;
        return !(this->startSegment > other.endSegment);
    }
    return true;
}

FieldType Segment::getType() {
    return FieldType::segment;
}

std::string Segment::toString() {
    return std::to_string(startSegment) + std::to_string(endSegment);
}

std::string Segment::getTypeName() {
    return "Segment";
}