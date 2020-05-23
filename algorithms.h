#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#import "data_structures/node.h"
#import "data_structures/dag.h"
#import "data_structures/trapezoidalmap.h"
namespace Algorithms {
    void buildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, cg3::Segment2d segment);
    size_t queryPoint(const TrapezoidalMap& tm,const Dag& dag, const cg3::Point2d& p1, const cg3::Point2d& p2);
    void followSegment(const TrapezoidalMap& tMap, const Dag& dag, const cg3::Segment2d& s1, std::vector<size_t>& trapezoidsIntersected);
    void performeDagUpdate(TrapezoidalMap& tMap, Dag& dag, const Trapezoid& current, const cg3::Segment2d s, const std::vector<Node>& nodes,  std::vector<size_t>& idsTrapezoid, bool degenerative);
}

#endif // ALGORITHMS_H
