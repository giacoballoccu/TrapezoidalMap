#include "trapezoidalmap.h"
#include "QRandomGenerator"

TrapezoidalMap::TrapezoidalMap(){
    segmentList.resize(1);
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


Dag TrapezoidalMap::getDag() const{
    return dag;
}
std::vector<cg3::Segment2d> TrapezoidalMap::getSegmentList() const{
    return segmentList;
};


void TrapezoidalMap::clear(){
    //dag clear
    segmentList.~vector();
    delete this;
}
