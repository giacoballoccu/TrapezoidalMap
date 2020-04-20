#include "trapezoid.h"
#define BOUNDINGBOX 1e+6

Trapezoid::Trapezoid(){
    cg3::Point2d BBx1y1 = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx2y1 = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx1y2 = cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX);
    cg3::Point2d BBx2y2 = cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX);
    setTop(cg3::Segment2d(BBx1y1, BBx2y1));
    setBottom(cg3::Segment2d(BBx1y2, BBx2y2));
    setLeftp(this->top.p1());
    setRightp(this->top.p2());

    upperLeftNeighbor = nullptr;
    lowerLeftNeighbor = nullptr;
    upperRightNeighbor = nullptr;
    lowerRightNeighbor = nullptr;

}


void Trapezoid::clear(){
    top.~Segment();
    bottom.~Segment();
    rightp.~Point2();
    leftp.~Point2();
    upperLeftNeighbor = nullptr;
    lowerLeftNeighbor = nullptr;
    upperRightNeighbor = nullptr;
    lowerRightNeighbor = nullptr;
};



cg3::Segment2d Trapezoid::getLeftEdge() const{
    return cg3::Segment2d(top.p1(), bottom.p1());
}

cg3::Segment2d Trapezoid::getRightEdge() const{
    return cg3::Segment2d(top.p2(), bottom.p2());
}


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


cg3::Segment2d& Trapezoid::getTopRfr(){
    return top;
};
cg3::Segment2d& Trapezoid::getBottomRfr(){
    return bottom;
};
cg3::Point2d& Trapezoid::getLeftpRfr(){
    return leftp;
};
cg3::Point2d& Trapezoid::getRightpRfr(){
    return rightp;
};

Trapezoid* Trapezoid::getLowerRightNeighbor() const{
    return lowerRightNeighbor;
};
Trapezoid* Trapezoid::getLowerLeftNeighbor() const{
    return lowerLeftNeighbor;
};
Trapezoid* Trapezoid::getUpperLeftNeighbor() const{
    return upperLeftNeighbor;
};
Trapezoid* Trapezoid::getUpperRightNeighbor() const{
    return upperRightNeighbor;
};

std::vector<cg3::Point2d> Trapezoid::getPoints() const{
    std::vector<cg3::Point2d> vertices = {top.p1(), top.p2(),  bottom.p2(), bottom.p1()};
    return vertices;
};



/*

Setter

*/
void Trapezoid::setTop(cg3::Segment2d s){
    top = s;
};
void Trapezoid::setBottom(cg3::Segment2d s){
    bottom = s;
};
void Trapezoid::setLeftp(cg3::Point2d p){
    leftp = p;
};
void Trapezoid::setRightp(cg3::Point2d p){
    rightp = p;
};
void Trapezoid::setUpperRightNeighbor(Trapezoid *t){
     upperRightNeighbor = t;
};
void Trapezoid::setUpperLeftNeighbor(Trapezoid *t){
     upperLeftNeighbor = t;
};
void Trapezoid::setLowerRightNeighbor(Trapezoid *t){
     lowerLeftNeighbor = t;
};
void Trapezoid::setLowerLeftNeighbor(Trapezoid *t){
     lowerRightNeighbor = t;
};

void Trapezoid::updateLeftNeighbors(Trapezoid& shared){
    upperLeftNeighbor = &shared;
    lowerLeftNeighbor = &shared;
};
void Trapezoid::updateRightNeighbors(Trapezoid& shared){
    upperRightNeighbor = &shared;
    lowerRightNeighbor = &shared;
};
void Trapezoid::updateLeftNeighbors(Trapezoid& upperLeft, Trapezoid& lowerLeft){
    upperLeftNeighbor = &upperLeft;
    lowerLeftNeighbor = &lowerLeft;
};
void Trapezoid::updateRightNeighbors(Trapezoid& upperRight, Trapezoid& lowerRight){
    upperRightNeighbor = &upperRight;
    lowerRightNeighbor = &lowerRight;
};
void Trapezoid::updateLeftNeighbors(const Trapezoid& old, Trapezoid& upperLeft, Trapezoid& lowerLeft){
    upperRightNeighbor = old.upperRightNeighbor;
    lowerRightNeighbor = old.lowerRightNeighbor;
    upperLeftNeighbor = &upperLeft;
    lowerLeftNeighbor = &lowerLeft;
};
void Trapezoid::updateRightNeighbors(const Trapezoid& old, Trapezoid& upperRight, Trapezoid& lowerRight){
    upperLeftNeighbor = old.upperLeftNeighbor;
    lowerLeftNeighbor = old.lowerLeftNeighbor;
    upperRightNeighbor = &upperRight;
    lowerRightNeighbor = &lowerRight;
};
void Trapezoid::updateNeighbors(Trapezoid& upperLeft, Trapezoid& lowerLeft, Trapezoid& upperRight, Trapezoid& lowerRight){
    upperLeftNeighbor = &upperLeft;
    lowerLeftNeighbor = &lowerLeft;
    upperRightNeighbor = &upperRight;
    lowerRightNeighbor = &lowerRight;
};


void Trapezoid::updateNeighbors(Trapezoid& sharedLeft, Trapezoid& sharedRight){
    upperLeftNeighbor = &sharedLeft;
    lowerLeftNeighbor = &sharedLeft;
    upperRightNeighbor = &sharedRight;
    lowerRightNeighbor = &sharedRight;
};

void Trapezoid::updateLeftNeighbors(const Trapezoid& old){
    upperLeftNeighbor = old.upperLeftNeighbor;
    lowerLeftNeighbor = old.lowerLeftNeighbor;
};
void Trapezoid::updateRightNeighbors(const Trapezoid& old){
    upperRightNeighbor = old.upperRightNeighbor;
    lowerRightNeighbor = old.lowerRightNeighbor;
};
