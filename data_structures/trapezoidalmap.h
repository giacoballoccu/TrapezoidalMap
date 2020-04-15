#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"

class TrapezoidalMap{
private:
    Trapezoid *leftMostTrapezoid; //Advanced
    std::vector<cg3::Segment2d> segmentList;

public:
    std::set<Trapezoid*> trapezoidSet; //Basic

    TrapezoidalMap();
    TrapezoidalMap(std::vector<cg3::Segment2d> segmentList);


    std::vector<cg3::Segment2d> getSegmentList() const;
    size_t getSegmentListSize() const;

    void addSegment(cg3::Segment2d s);
    void permuteSegmentList();
    void setSegmentList(std::vector<cg3::Segment2d> sl);

    std::set<Trapezoid*> getTrapezoidSet() const;
    void setTrapezoidSet(std::set<Trapezoid*> trapezoidSet);
    void addTrapezoid(Trapezoid *t);
    void removeTrapezoid(Trapezoid *t);

    void setLeftMostTrapezoid(Trapezoid *t);
    Trapezoid* getLeftMostTrapezoid() const;


    void clear();
};
#endif // TRAPEZOIDALMAP_H
