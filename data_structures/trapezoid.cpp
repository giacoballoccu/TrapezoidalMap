#include "trapezoid.h"

Trapezoid::Trapezoid(Node *dagNode){
    this->dagNode = dagNode;
};

Trapezoid::Trapezoid(Node *dagNode, cg3::Segment2d top, cg3::Segment2d bottom, cg3::Point2d leftp,cg3::Point2d rightp){
    this->dagNode = dagNode;
    this->top = top;
    this->bottom = bottom;
    this->leftp = leftp;
    this->rightp = rightp;
};

void Trapezoid::setTop(cg3::Segment2d top){
    this->top = top;
}

void Trapezoid::setBottom(cg3::Segment2d bottom){
    this->bottom = bottom;
}

void Trapezoid::setLeftp(cg3::Point2d leftp){
    this->leftp = leftp;
}

void Trapezoid::setRightp(cg3::Point2d rightp){
    this->rightp = rightp;
}

cg3::Segment2d Trapezoid::getTop() const{
    return top;
}

cg3::Segment2d Trapezoid::getBottom() const{
    return bottom;
}

cg3::Point2d Trapezoid::getLeftp() const{
    return leftp;
}

cg3::Point2d Trapezoid::getRightp() const{
    return rightp;
}

void Trapezoid::clear(){
    top.~Segment();
    bottom.~Segment();
    leftp.~Point2();
    rightp.~Point2();
    delete this;
}
