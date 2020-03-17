#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"

class TrapezoidalMap{
private:
    Dag dag;

    std::vector<cg3::Segment2d> segmentList;
public:
    TrapezoidalMap();

    Dag getDag() const;
    std::vector<cg3::Segment2d> getSegmentList() const;
    size_t getSegmentListSize() const;



    void addSegment(cg3::Segment2d);
    void permuteSegmentList();

    void setDag(Dag dag);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
