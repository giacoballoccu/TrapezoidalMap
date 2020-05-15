#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#import "data_structures/node.h"
#import "data_structures/dag.h"
#import "data_structures/trapezoidalmap.h"
namespace Algorithms {

    Node QueryTrapezoidMap(TrapezoidalMap trapezoidalMap, Node *node, cg3::Point2d point);
    void BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, cg3::Segment2d segment);
    std::vector<size_t> FollowSegment(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d& s1);
    //void inizializateDataStructures(TrapezoidalMap& tm, Dag& dag);


    size_t QueryPoint(TrapezoidalMap& tm, Dag& dag, cg3::Point2d& p1, cg3::Point2d& p2);

    void AddSubgraphToDag(TrapezoidalMap& tMap, Dag& dag, Trapezoid& currentT, std::vector<size_t>& idsTrapezoid, bool degenerative);


}

#endif // ALGORITHMS_H
