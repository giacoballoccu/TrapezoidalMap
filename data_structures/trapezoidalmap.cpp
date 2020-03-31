#include "trapezoidalmap.h"
#include "QRandomGenerator"
TrapezoidalMap::TrapezoidalMap(){

};

TrapezoidalMap::TrapezoidalMap(std::vector<cg3::Segment2d> segmentList){
    this->setSegmentList(segmentList);
};

void TrapezoidalMap::addSegment(cg3::Segment2d segment){
    segmentList.resize(segmentList.size() + 1);
    segmentList.push_back(segment);
};

void TrapezoidalMap::permuteSegmentList(){
   std::random_shuffle(segmentList.begin(), segmentList.end());
};

size_t TrapezoidalMap::getSegmentListSize() const{
    return segmentList.size();
};

void TrapezoidalMap::setDag(Dag dag){
    this->dag = dag;
}

void TrapezoidalMap::setSegmentList(std::vector<cg3::Segment2d> sl){
    segmentList = sl;
}
void TrapezoidalMap::setLeftMostTrapezoid(Trapezoid *t){
  leftMostTrapezoid = t;
};
Dag TrapezoidalMap::getDag() const{
    return dag;
}
std::vector<cg3::Segment2d> TrapezoidalMap::getSegmentList() const{
    return segmentList;
};

Trapezoid *TrapezoidalMap::getLeftMostTrapezoid() const{
    return leftMostTrapezoid;
};

void TrapezoidalMap::setTrapezoidSet(std::set<Trapezoid*> trapezoidSet){
    this->trapezoidSet = trapezoidSet;
};




void TrapezoidalMap::getAllRightNeighbors(Trapezoid *t, std::set<Trapezoid*>& result) {
    if (t->getUpperRightNeighbor() == nullptr and t->getLowerRightNeighbor() == nullptr){
        return;
    }
    if(t->getUpperRightNeighbor() != nullptr and t->getLowerLeftNeighbor() == nullptr){
        result.insert(t->getUpperRightNeighbor());
        getAllRightNeighbors(t->getUpperRightNeighbor(), result);
    }
    if(t->getUpperRightNeighbor() == nullptr and t->getLowerLeftNeighbor() != nullptr){
        result.insert(t->getLowerRightNeighbor());
        getAllRightNeighbors(t->getLowerRightNeighbor(), result);
    }else{
        result.insert(t->getUpperRightNeighbor());
        result.insert(t->getLowerRightNeighbor());
        getAllRightNeighbors(t->getLowerRightNeighbor(), result);
        getAllRightNeighbors(t->getUpperRightNeighbor(), result);
    }
}



void TrapezoidalMap::clear(){
    //dag clear
    segmentList.~vector();
    delete this;
}
