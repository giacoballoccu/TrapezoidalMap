#include "trapezoidalmap.h"
#include "QRandomGenerator"
TrapezoidalMap::TrapezoidalMap(){

};

TrapezoidalMap::TrapezoidalMap(std::vector<cg3::Segment2d> segmentList){
    this->setSegmentList(segmentList);
};

void TrapezoidalMap::addSegment(cg3::Segment2d segment){
    segmentList.push_back(segment);
};

void TrapezoidalMap::permuteSegmentList(){
   std::random_shuffle(segmentList.begin(), segmentList.end());
};

size_t TrapezoidalMap::getSegmentListSize() const{
    return segmentList.size();
};


void TrapezoidalMap::setSegmentList(std::vector<cg3::Segment2d> sl){
    segmentList = sl;
}
void TrapezoidalMap::setLeftMostTrapezoid(Trapezoid *t){
    this->leftMostTrapezoid = nullptr;
    leftMostTrapezoid = t;
};

std::vector<cg3::Segment2d> TrapezoidalMap::getSegmentList() const{
    return segmentList;
};

Trapezoid *TrapezoidalMap::getLeftMostTrapezoid() const{
    return leftMostTrapezoid;
};



void TrapezoidalMap::addTrapezoid(Trapezoid* t){
    trapezoidSet.insert(t);
};

void TrapezoidalMap::removeTrapezoid(Trapezoid* t){
    trapezoidSet.erase(t);
};



std::set<Trapezoid*> TrapezoidalMap::getAllRightNeighbors() const{
    std::set<Trapezoid*> result = std::set<Trapezoid*>();
    result.insert(leftMostTrapezoid);
    getAllRightNeighborsHelper(leftMostTrapezoid, result);
    return result;
}

void TrapezoidalMap::getAllRightNeighborsHelper(Trapezoid *t, std::set<Trapezoid*>& result) const{
   if(t == nullptr){
       return;
    }else{
       if (!t->neighborExist("upperRightNeighbor") and !t->neighborExist("lowerRightNeighbor")){
           result.insert(t);
           return;
       }
       if(t->neighborExist("upperRightNeighbor") and !t->neighborExist("lowerRightNeighbor")){
           result.insert(t->getUpperRightNeighbor());
           getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
           return;
       }
       if(!t->neighborExist("upperRightNeighbor") and t->neighborExist("lowerRightNeighbor")){
           result.insert(t->getLowerRightNeighbor());
           getAllRightNeighborsHelper(t->getLowerRightNeighbor(), result);
           return;
       }
       if (t->neighborExist("upperRightNeighbor") and t->neighborExist("lowerRightNeighbor")){
           if(t->getUpperRightNeighbor() == t->getLowerRightNeighbor()){
               result.insert(t->getUpperRightNeighbor());
               getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
               return;
           }else{
               result.insert(t->getUpperRightNeighbor());
               result.insert(t->getLowerRightNeighbor());
               getAllRightNeighborsHelper(t->getLowerRightNeighbor(), result);
               getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
               return;
           }
       }
   }
    /*if (t == nullptr){
        return;
    }else{
        if (!t->neighborExist("upperRightNeighbor") and !t->neighborExist("lowerRightNeighbor")){
            result.insert(t);
        }
        if(t->neighborExist("upperRightNeighbor") and !t->neighborExist("lowerRightNeighbor")){
            result.insert(t->getUpperRightNeighbor());
            getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
        }
        if(!t->neighborExist("upperRightNeighbor") and t->neighborExist("lowerRightNeighbor")){
            result.insert(t->getLowerRightNeighbor());
            getAllRightNeighborsHelper(t->getLowerRightNeighbor(), result);
        }
        if (t->neighborExist("upperRightNeighbor") and t->neighborExist("lowerRightNeighbor")){
            result.insert(t->getUpperRightNeighbor());
            result.insert(t->getLowerRightNeighbor());
            getAllRightNeighborsHelper(t->getLowerRightNeighbor(), result);
            getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
        }
    }
*/
}

std::set<Trapezoid*> TrapezoidalMap::getTrapezoidSet() const{
    return trapezoidSet;
};

void TrapezoidalMap::setTrapezoidSet(std::set<Trapezoid*> trapezoidSet){
    this->trapezoidSet.clear();
    this->trapezoidSet = trapezoidSet;
};

void TrapezoidalMap::clear(){
    //dag clear
    segmentList.~vector();
    delete this;
}
