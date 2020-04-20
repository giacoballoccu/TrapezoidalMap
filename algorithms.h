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
    void BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, std::vector<cg3::Segment2d>& segmentList);
    void BuildTrapezoidalMap(TrapezoidalMap& tm, Dag& dag, cg3::Segment2d segment);
    std::vector<size_t> FollowSegment(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d& s1);

    void inizializateDataStructures(TrapezoidalMap& tm, Dag& dag);


    size_t QueryPoint(TrapezoidalMap& tMap, Dag& dag, cg3::Point2d& point);

    Node* generateSubgraph(TrapezoidalMap& tMap, Trapezoid current, std::vector<size_t> idsTrapezoid, cg3::Segment2d& s);
    //void subgraphFromOneTrapezoid(TrapezoidalMap& tm, Dag& dag, size_t& id, cg3::Segment2d& s);


    void Update(TrapezoidalMap& tm, Dag& dag,const size_t& i,const std::vector<size_t>& trapezoidsIntersected, cg3::Segment2d s);
    //void addTrapezoids(Node* root, TrapezoidalMap &tm);
    //void createAndInsertLeaves(Node * node, TrapezoidalMap &tm, bool leftChild);
    bool isAbove(cg3::Point2d p, cg3::Segment2d s);
};

#endif // ALGORITHMS_H
