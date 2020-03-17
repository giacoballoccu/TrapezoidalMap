#include "trapezoidalmap.h"
#include "QRandomGenerator"

TrapezoidalMap::TrapezoidalMap(std::list<cg3::Segment2d> segmentList){
    this->segmentList = segmentList;
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

void TrapezoidalMap::setSegmentList(std::list<cg3::Segment2d> sl){
    segmentList = sl;
}

Dag TrapezoidalMap::getDag() const{
    return dag;
}
std::list<cg3::Segment2d> TrapezoidalMap::getSegmentList() const{
    return segmentList;
};


void TrapezoidalMap::clear(){
    //dag clear
    segmentList.~list();
    delete this;
}
