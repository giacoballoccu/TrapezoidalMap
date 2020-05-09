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

    upperLeftNeighbor = SIZE_MAX;
    lowerLeftNeighbor = SIZE_MAX;
    upperRightNeighbor = SIZE_MAX;
    lowerRightNeighbor = SIZE_MAX;
    node = nullptr;
}

void Trapezoid::clear(){
    top.~Segment();
    bottom.~Segment();
    rightp.~Point2();
    leftp.~Point2();
    node->~Node();
    upperLeftNeighbor =  SIZE_MAX;
    lowerLeftNeighbor =  SIZE_MAX;
    upperRightNeighbor = SIZE_MAX;
    lowerRightNeighbor = SIZE_MAX;
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

size_t Trapezoid::getLowerRightNeighbor() const{
    return lowerRightNeighbor;
};
size_t Trapezoid::getLowerLeftNeighbor() const{
    return lowerLeftNeighbor;
};
size_t Trapezoid::getUpperLeftNeighbor() const{
    return upperLeftNeighbor;
};
size_t Trapezoid::getUpperRightNeighbor() const{
    return upperRightNeighbor;
};

const std::array<cg3::Point2d, 4> Trapezoid::getPoints() const{
    std::array<cg3::Point2d, 4> vertices = {top.p1(), top.p2(),  bottom.p2(), bottom.p1()};
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

void Trapezoid::setUpperLeftNeighbor(const size_t& id){
    upperLeftNeighbor = id;
};
void Trapezoid::setUpperRightNeighbor(const size_t& id){
    upperRightNeighbor = id;
};
void Trapezoid::setLowerLeftNeighbor(const size_t& id){
    lowerLeftNeighbor = id;
};
void Trapezoid::setLowerRightNeighbor(const size_t& id){
    lowerRightNeighbor = id;
};

void Trapezoid::updateLLNeighbor(const size_t& idLowerLeft){
    lowerLeftNeighbor = idLowerLeft;
}
void Trapezoid::updateULNeighbor(const size_t& idUpperLeft){
    upperLeftNeighbor = idUpperLeft;
}

void Trapezoid::updateLRNeighbor(const size_t& idLowerRight){
    lowerRightNeighbor = idLowerRight;
}
void Trapezoid::updateURNeighbor(const size_t& idUpperRight){
    upperRightNeighbor = idUpperRight;
}
void Trapezoid::updateLeftNeighbors(const size_t& idShared){
    upperLeftNeighbor = idShared;
    lowerLeftNeighbor = idShared;
};
void Trapezoid::updateRightNeighbors(const size_t& idShared){
    upperRightNeighbor = idShared;
    lowerRightNeighbor = idShared;
};
void Trapezoid::updateLeftNeighbors(const size_t& idUpperLeft, const size_t& idLowerLeft){
    upperLeftNeighbor = idUpperLeft;
    lowerLeftNeighbor = idLowerLeft;
};
void Trapezoid::updateRightNeighbors(const size_t& idUpperRight, const size_t& idLowerRight){
    upperRightNeighbor = idUpperRight;
    lowerRightNeighbor = idLowerRight;
};

void Trapezoid::updateLeftNeighborsOld(Trapezoid& old){
    upperLeftNeighbor = old.upperLeftNeighbor;
    lowerLeftNeighbor = old.lowerLeftNeighbor;
};
void Trapezoid::updateRightNeighborsOld(Trapezoid& old){
    upperRightNeighbor = old.upperRightNeighborRfr();
    lowerRightNeighbor = old.lowerRightNeighborRfr();
};

void Trapezoid::updateNeighborsLeft(const Trapezoid& old, const size_t& idUpperLeft, const size_t& idLowerLeft){
     upperRightNeighbor = old.upperRightNeighbor;
     lowerRightNeighbor = old.lowerRightNeighbor;
     upperLeftNeighbor = idUpperLeft;
     lowerLeftNeighbor = idLowerLeft;
};
void Trapezoid::updateNeighborsRight(const Trapezoid& old, const size_t& idUpperRight, const size_t& idLowerRight){
    upperLeftNeighbor = old.upperLeftNeighbor;
    lowerLeftNeighbor = old.lowerLeftNeighbor;
    upperRightNeighbor = idUpperRight;
    lowerRightNeighbor = idLowerRight;
};

void Trapezoid::updateNeighbors(const size_t idSharedLeft, const size_t idSharedRight){
    upperLeftNeighbor = idSharedLeft;
    lowerLeftNeighbor = idSharedLeft;
    upperRightNeighbor = idSharedRight;
    lowerRightNeighbor = idSharedRight;
}
/*
void Trapezoid::indirectUpdateLeftNeighbors(const size_t& idNewT){
    if(lowerLeftNeighbor != SIZE_MAX){
        if(lowerLeftNeighbor->lowerRightNeighbor == this){
            lowerLeftNeighbor->lowerRightNeighbor = &newT;
        }
        if(lowerLeftNeighbor->upperRightNeighbor == this){
            lowerLeftNeighbor->upperRightNeighbor = &newT;
        }
    }
    if(upperLeftNeighbor != nullptr){
        if(upperLeftNeighbor->lowerRightNeighbor == this){
            upperLeftNeighbor->lowerRightNeighbor = &newT;
        }
        if(upperLeftNeighbor->upperRightNeighbor == this){
            upperLeftNeighbor->upperRightNeighbor = &newT;
        }
    }
}

void Trapezoid::indirectUpdateRightNeighbors(Trapezoid& newT){
    if(lowerRightNeighbor != -1){
        if(lowerRightNeighbor->lowerLeftNeighbor == this){
            lowerRightNeighbor->lowerLeftNeighbor = &newT;
        }
        if(lowerRightNeighbor->upperLeftNeighbor == this){
            lowerRightNeighbor->upperLeftNeighbor = &newT;
        }
    }
    if(upperRightNeighbor != nullptr){
        if(upperRightNeighbor->lowerLeftNeighbor == this){
            upperRightNeighbor->lowerLeftNeighbor = &newT;
        }
        if(upperRightNeighbor->upperLeftNeighbor == this){
            upperRightNeighbor->upperLeftNeighbor = &newT;
        }
    }
}*/
