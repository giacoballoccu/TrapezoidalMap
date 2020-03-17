#include "trapezoid.h"

Trapezoid::Trapezoid()
{

}


void Trapezoid::clear(){
    top.~Segment();
    bottom.~Segment();
    rightp.~Point2();
    leftp.~Point2();
    delete this;
};
/*

Getter

*/
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

/*

Setter

*/
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

