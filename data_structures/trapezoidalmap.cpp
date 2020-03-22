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
void TrapezoidalMap::setLeftMostTrapezoid(Trapezoid t){
  leftMostTrapezoid = t;
};
Dag TrapezoidalMap::getDag() const{
    return dag;
}
std::list<cg3::Segment2d> TrapezoidalMap::getSegmentList() const{
    return segmentList;
};

std::set<Trapezoid*> TrapezoidalMap::getTrapezoidsIntersected(cg3::Segment2d s, Trapezoid *currentTrapezoid, std::set<Trapezoid*> trapezoids){
     SegmentIntersectionChecker checker = SegmentIntersectionChecker();
     /*Check with left edge of the trapezoid*/
     if (checker.checkSegmentIntersection(s, cg3::Segment2d(currentTrapezoid->getTop().p1(), currentTrapezoid->getBottom().p1()))){
          trapezoids.insert(currentTrapezoid);
          getTrapezoidsIntersected(s, currentTrapezoid->getLeftNeighbor(), trapezoids);
     }
     /*Check with the right edge of the trapezoid*/
     if (checker.checkSegmentIntersection(s, cg3::Segment2d(currentTrapezoid->getTop().p2(), currentTrapezoid->getBottom().p2()))){
          trapezoids.insert(currentTrapezoid);
          getTrapezoidsIntersected(s, currentTrapezoid->getRightNeighbor(), trapezoids);
     }
     /*Check with the top segment of the trapezoid*/
     if (checker.checkSegmentIntersection(s, cg3::Segment2d(currentTrapezoid->getTop()))){
          trapezoids.insert(currentTrapezoid);
          getTrapezoidsIntersected(s, currentTrapezoid->getTopNeighbor(), trapezoids);
     }
     /*Check with the bottom segment of the trapezoid*/
     if (checker.checkSegmentIntersection(s, cg3::Segment2d(currentTrapezoid->getBottom()))){
          trapezoids.insert(currentTrapezoid);
          getTrapezoidsIntersected(s, currentTrapezoid->getBotNeighbor(), trapezoids);
     }

     return trapezoids;
};


void TrapezoidalMap::clear(){
    //dag clear
    segmentList.~list();
    delete this;
}
