#include "trapezoidalmap.h"
#include "QRandomGenerator"
TrapezoidalMap::TrapezoidalMap(){
    this->trapezoids= std::vector<Trapezoid>();
    this->segments = std::vector<cg3::Segment2d>();
    this->points = std::vector<cg3::Point2d>();
};

std::vector<cg3::Segment2d> TrapezoidalMap::getSegments() const{
    return segments;
};
std::vector<cg3::Segment2d>& TrapezoidalMap::getSegmentsRfr(){
    return segments;
};

std::vector<cg3::Point2d> TrapezoidalMap::getPoints() const{
    return points;
};
std::vector<cg3::Point2d>& TrapezoidalMap::getPointsRfr(){
    return points;
};

std::vector<Trapezoid> TrapezoidalMap::getTrapezoids() const{
    return trapezoids;
};
std::vector<Trapezoid>& TrapezoidalMap::getTrapezoidsRfr(){
    return trapezoids;
};

size_t TrapezoidalMap::addSegment(const cg3::Segment2d& s){
    segments.push_back(s);
    return segments.size()-1;
};
size_t TrapezoidalMap::addPoint(const cg3::Point2d& p){
    points.push_back(p);
    return points.size()-1;
};
size_t TrapezoidalMap::addTrapezoid(const Trapezoid& t){
    trapezoids.push_back(t);
    return trapezoids.size()-1;
};

void TrapezoidalMap::permuteSegmentList(){
   std::random_shuffle(segments.begin(), segments.end());
};


void TrapezoidalMap::setTrapezoids(std::vector<Trapezoid> trapezoids){
    this->trapezoids = trapezoids;
};

Trapezoid& TrapezoidalMap::trapezoid(const size_t& id){
    return trapezoids[id];
}

const Trapezoid& TrapezoidalMap::trapezoid(const size_t& id) const {
    return trapezoids[id];
}

cg3::Point2d& TrapezoidalMap::point(const size_t& id){
    return points[id];
};
cg3::Segment2d& TrapezoidalMap::segment(const size_t& id){
    return segments[id];
};

//void setNodeToTrapezoid(const size_t& idTrap, const Node* node);

void TrapezoidalMap::removeTrapezoid(const size_t& id){
    trapezoids.erase(trapezoids.begin() + id);
    for(size_t i = id; i<trapezoids.size(); i++){
        trapezoids[i].node->reduceId();
    }
};


void TrapezoidalMap::clear(){
    trapezoids.~vector();
    segments.~vector();
    points.~vector();
}














/*
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
}
*/

