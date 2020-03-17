#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"

class TrapezoidalMap{
private:
    Dag dag;
    Trapezoid leftMostTrapezoid;
    std::list<cg3::Segment2d> segmentList;
public:
    TrapezoidalMap(std::list<cg3::Segment2d> segmentList);

    Dag getDag() const;
    std::list<cg3::Segment2d> getSegmentList() const;
    size_t getSegmentListSize() const;

    void addSegment(cg3::Segment2d);
    void permuteSegmentList();

    void setDag(Dag dag);
    void setSegmentList(std::list<cg3::Segment2d>);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
