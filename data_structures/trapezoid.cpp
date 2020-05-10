#include "trapezoid.h"
#define BOUNDINGBOX 1e+6

Trapezoid::Trapezoid(){
    cg3::Point2d BBx1y1 = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx2y1 = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx1y2 = cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX);
    cg3::Point2d BBx2y2 = cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX);
    setTop(cg3::Segment2d(BBx1y1, BBx2y1));
    setBottom(cg3::Segment2d(BBx1y2, BBx2y2));
    setLeftp(_top.p1());
    setRightp(_top.p2());

    _upperLeftNeighbor = SIZE_MAX;
    _lowerLeftNeighbor = SIZE_MAX;
    _upperRightNeighbor = SIZE_MAX;
    _lowerRightNeighbor = SIZE_MAX;
    _node = SIZE_MAX;
}

Trapezoid::~Trapezoid(){
    _top.~Segment();
    _bottom.~Segment();
    _rightp.~Point2();
    _leftp.~Point2();
};



/*Getter*/
cg3::Segment2d Trapezoid::top() const{
    return _top;
}
cg3::Segment2d Trapezoid::bottom() const{
    return _bottom;
}
cg3::Point2d Trapezoid::leftp() const{
    return _leftp;
}
cg3::Point2d Trapezoid::rightp() const{
    return _rightp;
}

size_t Trapezoid::lowerRightNeighbor() const{
    return _lowerRightNeighbor;
};
size_t Trapezoid::lowerLeftNeighbor() const{
    return _lowerLeftNeighbor;
};
size_t Trapezoid::upperLeftNeighbor() const{
    return _upperLeftNeighbor;
};
size_t Trapezoid::upperRightNeighbor() const{
    return _upperRightNeighbor;
};

size_t Trapezoid::node() const{
    return _node;
}

const std::array<cg3::Point2d, 4> Trapezoid::getVertices() const{
    std::array<cg3::Point2d, 4> vertices = {_top.p1(), _top.p2(),  _bottom.p2(), _bottom.p1()};
    return vertices;
};


/*Setter*/
void Trapezoid::setTop(cg3::Segment2d s){
    _top = s;
};
void Trapezoid::setTopP1(cg3::Point2d p){
    _top.setP1(p);
};
void Trapezoid::setTopP2(cg3::Point2d p){
    _top.setP2(p);
};
void Trapezoid::setBottom(cg3::Segment2d s){
    _bottom = s;
};
void Trapezoid::setBottomP1(cg3::Point2d p){
    _bottom.setP1(p);
};
void Trapezoid::setBottomP2(cg3::Point2d p){
    _bottom.setP2(p);
};
void Trapezoid::setLeftp(cg3::Point2d p){
    _leftp = p;
};
void Trapezoid::setRightp(cg3::Point2d p){
    _rightp = p;
};

void Trapezoid::setUpperLeftNeighbor(const size_t& id){
    _upperLeftNeighbor = id;
};
void Trapezoid::setUpperRightNeighbor(const size_t& id){
    _upperRightNeighbor = id;
};
void Trapezoid::setLowerLeftNeighbor(const size_t& id){
    _lowerLeftNeighbor = id;
};
void Trapezoid::setLowerRightNeighbor(const size_t& id){
    _lowerRightNeighbor = id;
};
void Trapezoid::setNode(const size_t& nodeId){
    _node = nodeId;
}

/*Neighbor update functions*/
void Trapezoid::updateLLNeighbor(const size_t& idLowerLeft){
   _lowerLeftNeighbor = idLowerLeft;
}
void Trapezoid::updateULNeighbor(const size_t& idUpperLeft){
   _upperLeftNeighbor = idUpperLeft;
}

void Trapezoid::updateLRNeighbor(const size_t& idLowerRight){
   _lowerRightNeighbor = idLowerRight;
}
void Trapezoid::updateURNeighbor(const size_t& idUpperRight){
   _upperRightNeighbor = idUpperRight;
}
void Trapezoid::updateLeftNeighbors(const size_t& idShared){
   _upperLeftNeighbor = idShared;
   _lowerLeftNeighbor = idShared;
};
void Trapezoid::updateRightNeighbors(const size_t& idShared){
   _upperRightNeighbor = idShared;
   _lowerRightNeighbor = idShared;
};
void Trapezoid::updateLeftNeighbors(const size_t& idUpperLeft, const size_t& idLowerLeft){
   _upperLeftNeighbor = idUpperLeft;
   _lowerLeftNeighbor = idLowerLeft;
};
void Trapezoid::updateRightNeighbors(const size_t& idUpperRight, const size_t& idLowerRight){
   _upperRightNeighbor = idUpperRight;
   _lowerRightNeighbor = idLowerRight;
};

void Trapezoid::updateLeftNeighborsOld(Trapezoid& old){
   _upperLeftNeighbor = old._upperLeftNeighbor;
   _lowerLeftNeighbor = old._lowerLeftNeighbor;
};
void Trapezoid::updateRightNeighborsOld(Trapezoid& old){
   _upperRightNeighbor = old._upperRightNeighbor;
   _lowerRightNeighbor = old._lowerRightNeighbor;
};

void Trapezoid::updateNeighborsLeft(const Trapezoid& old, const size_t& idUpperLeft, const size_t& idLowerLeft){
    _upperRightNeighbor = old._upperRightNeighbor;
    _lowerRightNeighbor = old._lowerRightNeighbor;
    _upperLeftNeighbor = idUpperLeft;
    _lowerLeftNeighbor = idLowerLeft;
};
void Trapezoid::updateNeighborsRight(const Trapezoid& old, const size_t& idUpperRight, const size_t& idLowerRight){
   _upperLeftNeighbor = old._upperLeftNeighbor;
   _lowerLeftNeighbor = old._lowerLeftNeighbor;
   _upperRightNeighbor = idUpperRight;
   _lowerRightNeighbor = idLowerRight;
};

void Trapezoid::updateNeighbors(const size_t idSharedLeft, const size_t idSharedRight){
   _upperLeftNeighbor = idSharedLeft;
   _lowerLeftNeighbor = idSharedLeft;
   _upperRightNeighbor = idSharedRight;
   _lowerRightNeighbor = idSharedRight;
}
