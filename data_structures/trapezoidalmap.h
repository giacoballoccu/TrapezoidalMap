#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"

class TrapezoidalMap{
private:
    Dag dag;
    Trapezoid* leftMostTrapezoid;
    std::vector<cg3::Segment2d> segmentList;
    std::set<Trapezoid*> trapezoidSet;
public:
    TrapezoidalMap();
    TrapezoidalMap(std::vector<cg3::Segment2d> segmentList);

    Dag getDag() const;
    std::vector<cg3::Segment2d> getSegmentList() const;
    size_t getSegmentListSize() const;

    void addSegment(cg3::Segment2d);
    void permuteSegmentList();

    void setDag(Dag dag);
    void setSegmentList(std::vector<cg3::Segment2d> sl);
    void setLeftMostTrapezoid(Trapezoid *t);
    void setTrapezoidSet(std::set<Trapezoid*> trapezoidSet);

    Trapezoid* getLeftMostTrapezoid() const;
    void getAllRightNeighbors(Trapezoid *t, std::set<Trapezoid*>& result);


    void clear();
};
#endif // TRAPEZOIDALMAP_H
