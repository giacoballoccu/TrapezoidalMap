#include "algorithms.h"

#define BOUNDINGBOX 1e+6

Algorithms::Algorithms()
{

}

TrapezoidalMap BuildTrapezoidalMap(std::list<cg3::Segment2d> segmentList){
    TrapezoidalMap tm = TrapezoidalMap(segmentList);
    /*Starting bounding box aka first trapezoid*/
    //cg3::Segment2d boundingBoxTop = cg3::Segment2d(Point(BOUNDINGBOX))
    Trapezoid firstTrapezoid = Trapezoid();

    /*Permute segments order*/
    if (tm.getSegmentListSize() > 1){
         tm.permuteSegmentList();
    }



};
Node Algorithms::QueryTrapezoidMap(TrapezoidalMap trapezoidalMap, Node *node, cg3::Point2d point){
    if(node->getType() == leafNode){
        return *node;
    }
    else if(node->getType() == xNode){
        XNode* xnode = (XNode*)(node);
        if (point.x() < xnode->getPoint().x()){
                return QueryTrapezoidMap(trapezoidalMap, node->getRight(), point);
            }else{
                return QueryTrapezoidMap(trapezoidalMap, node->getLeft(), point);
            }
    }
    else if(node->getType() == yNode){
        YNode * ynode = (YNode*)(node);
        if(isAbove(point, ynode->getSegment())){ //check if a poin is above a segment function
            return QueryTrapezoidMap(trapezoidalMap, node->getLeft(), point);
        }else{
            return QueryTrapezoidMap(trapezoidalMap, node->getRight(), point);
    }
    }

    throw -1; //Control can't reach this point if input is correct, just to supprese the warning
};

/*

Cross product to understand if the point is above or below line, formula returns 0 if segment and point are collinear.
1 if the point is above the segment

*/
bool isAbove(cg3::Point2d p, cg3::Segment2d s){
     return ((s.p2().x() - s.p1().x())*(p.y() - s.p1().y()) - (s.p2().y() - s.p1().y())*(p.x() - s.p1().x())) > 0;
}
