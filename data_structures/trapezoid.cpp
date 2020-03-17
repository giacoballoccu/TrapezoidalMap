#include "trapezoid.h"

Trapezoid::Trapezoid(){
    topNeighbor = nullptr;
    botNeighbor = nullptr;
    rightNeighbor = nullptr;
    leftNeighbor = nullptr;
}

Trapezoid::Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom){
    this->top = top;
    this->bottom = bottom;
    this->leftp = top.p1();
    this->rightp = bottom.p2();
    topNeighbor = nullptr;
    botNeighbor = nullptr;
    rightNeighbor = nullptr;
    leftNeighbor = nullptr;
}


void Trapezoid::clear(){
    top.~Segment();
    bottom.~Segment();
    rightp.~Point2();
    leftp.~Point2();
    topNeighbor = nullptr;
    botNeighbor = nullptr;
    leftNeighbor = nullptr;
    rightNeighbor = nullptr;
    delete this;
};

cg3::Polygon2 Trapezoid::computeTrapezoid() const{
  cg3::Polygon2 p = cg3::Polygon2();

  /*Metto i punti clockwise(To check) in un polygono per calcolare il bounding box*/
  /*TO CHECK: SBAGLIATO serve un albero per vedere sia x che y*/
  p.pushBack(std::max(top.p1().y(), leftp.y()));
  p.pushBack(std::max(top.p2().x(), rightp.x()));
  p.pushBack(std::min(top.p1().x(), leftp.x()));
  p.pushBack(std::max(top.p2().x(), rightp.x()));
  p.updateBoundingBox();
  return p;
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
Trapezoid* Trapezoid::getTopNeighbor() const{
    return topNeighbor;
};
Trapezoid* Trapezoid::getBotNeighbor() const{
    return botNeighbor;
};
Trapezoid* Trapezoid::getLeftNeighbor() const{
    return leftNeighbor;
};
Trapezoid* Trapezoid::getRightNeighbor() const{
    return rightNeighbor;
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
void Trapezoid::setTopNeighbor(Trapezoid *t){
    topNeighbor = t;
};
void Trapezoid::setBotNeighbor(Trapezoid *t){
     botNeighbor = t;
};
void Trapezoid::setLeftNeighbor(Trapezoid *t){
     leftNeighbor = t;
};
void Trapezoid::setRightNeighbor(Trapezoid *t){
     rightNeighbor = t;
};


