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
    void BuildTrapezoidalMap(TrapezoidalMap& tm, Dag& dag, cg3::Segment2d segment);
    std::vector<Trapezoid*> FollowSegment(TrapezoidalMap& tm, Dag& dag, cg3::Segment2d s1);

    void inizializateDataStructures(TrapezoidalMap& tm, Dag& dag);

    void subgraphFromOneTrapezoid(Trapezoid* t, cg3::Segment2d s);

    void addTrapezoids(Node* root, TrapezoidalMap &tm);
    void createAndInsertLeaves(Node * node, TrapezoidalMap &tm, bool leftChild);
    bool isAbove(cg3::Point2d p, cg3::Segment2d s);
};

#endif // ALGORITHMS_H
