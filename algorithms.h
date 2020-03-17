#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#import "data_structures/node.h"
#import "data_structures/dag.h"
#import "data_structures/trapezoidalmap.h"
class Algorithms
{
public:
    Algorithms();

    Node QueryTrapezoidMap(TrapezoidalMap trapezoidalMap, Node *node, cg3::Point2d point);
    TrapezoidalMap BuildTrapezoidalMap(std::list<cg3::Segment2d> segmentList);

    bool isAbove(cg3::Point2d p, cg3::Segment2d s);
};

#endif // ALGORITHMS_H
