#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#import "data_structures/node.h"
#import "data_structures/dag.h"
#import "data_structures/trapezoidalmap.h"
namespace Algorithms {

    Node QueryTrapezoidMap(TrapezoidalMap trapezoidalMap, Node *node, cg3::Point2d point);
    void BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, std::vector<cg3::Segment2d>& segmentList);
    void BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, cg3::Segment2d segment);
    std::vector<size_t> FollowSegment(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d& s1);
    void inizializateDataStructures(TrapezoidalMap& tm, Dag& dag);


    size_t QueryPoint(TrapezoidalMap& tm, Dag& dag, cg3::Point2d& point);

    void generateSubgraph(TrapezoidalMap& tMap, Trapezoid& current,
                        std::vector<size_t>& newTrapezoidIds);

    void Update(TrapezoidalMap& tm, const size_t& i, std::vector<size_t>& trapsIntersected,
              std::vector<std::vector<size_t>>& newTrapezoidIds,
              std::vector<size_t>& elegibleForMerge);
    };

#endif // ALGORITHMS_H
