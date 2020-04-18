#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"

class TrapezoidalMap{
private:
    std::vector<cg3::Segment2d> segments;
    std::vector<cg3::Point2d> points;
    std::vector<Trapezoid> trapezoids;
public:


    TrapezoidalMap();

    std::vector<cg3::Segment2d> getSegments() const;
    std::vector<cg3::Segment2d>& getSegmentsRfr();

    std::vector<cg3::Point2d> getPoints() const;
    std::vector<cg3::Point2d>& getPointsRfr();

    std::vector<Trapezoid> getTrapezoids() const;
    std::vector<Trapezoid>& getTrapezoidsRfr();

    size_t addSegment(const cg3::Segment2d& s);
    size_t addPoint(const cg3::Point2d& p);
    size_t addTrapezoid(const Trapezoid& t);

    void permuteSegmentList();

    void setSegments(std::vector<cg3::Segment2d> sl);
    void setTrapezoids(std::vector<Trapezoid> trapezoids);

    Trapezoid& trapezoid(const size_t& id);
    const Trapezoid& trapezoid(const size_t& id) const;

    cg3::Point2d& point(const size_t& id);
    cg3::Segment2d& segment(const size_t& id);

    //void setNodeToTrapezoid(const size_t& idTrap, const Node* node);

    void removeTrapezoid(const size_t& id);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
