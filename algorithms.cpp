#include "algorithms.h"

#define BOUNDINGBOX 1e+6

Algorithms::Algorithms()
{

}

TrapezoidalMap Algorithms::BuildTrapezoidalMap(std::vector<cg3::Segment2d> segmentList){
    TrapezoidalMap tm = TrapezoidalMap(segmentList);
    /*Starting bounding box aka first trapezoid*/
    cg3::Point2d BBx1y1 = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx2y1 = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx1y2 = cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX);
    cg3::Point2d BBx2y2 = cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX);

    cg3::Segment2d boundingBoxTop = cg3::Segment2d(BBx1y1, BBx2y1);
    cg3::Segment2d boundingBoxBottom = cg3::Segment2d(BBx1y2, BBx2y2);

    Trapezoid firstTrapezoid = Trapezoid(boundingBoxTop, boundingBoxBottom);
    Node * firstNode = new LeafNode(&firstTrapezoid);

    tm.dag.setRoot(firstNode);

    /*Validate segments*/
    //tm.validateSegmentList();

    /*Permute segments order*/
    if (tm.getSegmentListSize() > 1){
         tm.permuteSegmentList();
    }
    for (size_t i = 0; i < tm.getSegmentListSize(); i++){
        std::set<Trapezoid*> trapezoidSet = FollowSegment(tm, tm.dag, tm.getSegmentList()[i]); //?
        if (trapezoidSet.size() == 1){
            Node* subgraph = tm.dag.simpleSubgraphFromSegment(tm.getSegmentList()[i]);
            tm.dag.setRoot(subgraph);
        }
    }

    return tm;
};


/*

Follow Segment

*/

std::set<Trapezoid*> Algorithms::FollowSegment(TrapezoidalMap tm, Dag dag, cg3::Segment2d s1){
    cg3::Point2d p1 = s1.p1();
    cg3::Point2d q1 = s1.p2();

    LeafNode * node = (LeafNode*)dag.QueryPoint(dag.getRoot(), p1);
    Trapezoid* delta0 = node->getTrapezoid();

    std::set<Trapezoid*> trapezoidSet = std::set<Trapezoid*>();
    trapezoidSet.insert(delta0);

    std::set<Trapezoid*>::iterator j = trapezoidSet.begin();

    while (q1.x() > (*j)->getRightp().x()){
        if (isAbove((*j)->getRightp(), s1)){
           trapezoidSet.insert((*j)->getLowerRightNeighbor());
        }else{
            trapezoidSet.insert((*j)->getUpperRightNeighbor());
        }
        j++;
    }
    tm.setTrapezoidSet(trapezoidSet);
    return trapezoidSet;
}






bool Algorithms::isAbove(cg3::Point2d p, cg3::Segment2d s){
     return ((s.p2().x() - s.p1().x())*(p.y() - s.p1().y()) - (s.p2().y() - s.p1().y())*(p.x() - s.p1().x())) > 0;
}

