#include "dag.h"

Dag::Dag(){

};
/*
Dag::Dag(std::vector<cg3::Segment2d> segments){
    buildDagFromSegments(segments);
};


void buildDagFromSegments(std::vector<cg3::Segment2d> segments){

};
*/

XNode Dag::getRoot() const{
    return root;
}
