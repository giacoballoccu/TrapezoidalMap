#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"

class TrapezoidalMap{
private:
    Trapezoid *leftMostTrapezoid; //Advanced
    std::vector<cg3::Segment2d> segmentList;
    std::set<Trapezoid*> trapezoidSet; //Basic
public:

    TrapezoidalMap();
    TrapezoidalMap(std::vector<cg3::Segment2d> segmentList);

    std::vector<cg3::Segment2d> getSegmentList() const;
    size_t getSegmentListSize() const;

    void addSegment(cg3::Segment2d s);
    void permuteSegmentList();

    void setSegmentList(std::vector<cg3::Segment2d> sl);
    void setLeftMostTrapezoid(Trapezoid *t);
    void setTrapezoidSet(std::set<Trapezoid*> trapezoidSet);


    void addTrapezoid(Trapezoid *t);
    void removeTrapezoid(Trapezoid *t);
    Trapezoid* getLeftMostTrapezoid() const;
    void getAllRightNeighbors(Trapezoid *t, std::set<Trapezoid*>& result);
    std::set<Trapezoid*> getTrapezoidSet() const;

    void clear();
};
#endif // TRAPEZOIDALMAP_H
