#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(){};

TrapezoidalMap::TrapezoidalMap(cg3::Segment2d top, cg3::Segment2d bottom, cg3::Point2d leftp,cg3::Point2d rightp){
    this->top = top;
    this->bottom = bottom;
    this->leftp = leftp;
    this->rightp = rightp;
};

void TrapezoidalMap::setTop(cg3::Segment2d top){
    this->top = top;
}

void TrapezoidalMap::setBottom(cg3::Segment2d bottom){
    this->bottom = bottom;
}

void TrapezoidalMap::setLeftp(cg3::Point2d leftp){
    this->leftp = leftp;
}

void TrapezoidalMap::setRightp(cg3::Point2d rightp){
    this->rightp = rightp;
}

cg3::Segment2d TrapezoidalMap::getTop() const{
    return top;
}

cg3::Segment2d TrapezoidalMap::getBottom() const{
    return bottom;
}

cg3::Point2d TrapezoidalMap::getLeftp() const{
    return leftp;
}

cg3::Point2d TrapezoidalMap::getRightp() const{
    return rightp;
}

void TrapezoidalMap::clear(){
    top.~Segment();
    bottom.~Segment();
    leftp.~Point2();
    rightp.~Point2();
    delete this;
}
