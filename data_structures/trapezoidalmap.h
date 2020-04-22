#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"

class TrapezoidalMap{
private:
    std::vector<cg3::Segment2d> segments;
    std::vector<cg3::Point2d> points;
    std::list<Trapezoid> trapezoids;
public:


    TrapezoidalMap();

    std::vector<cg3::Segment2d> getSegments() const;
    std::vector<cg3::Segment2d>& getSegmentsRfr();

    std::vector<cg3::Point2d> getPoints() const;
    std::vector<cg3::Point2d>& getPointsRfr();

    std::list<Trapezoid> getTrapezoids() const;
    std::list<Trapezoid>& getTrapezoidsRfr();

    size_t addSegment(const cg3::Segment2d& s);
    size_t addPoint(const cg3::Point2d& p);
    size_t addTrapezoid(const Trapezoid& t);

    void permuteSegmentList();

    void setTrapezoids(std::list<Trapezoid> trapezoids);
    Trapezoid& trapezoid(const size_t& id);

    cg3::Point2d& point(const size_t& id);
    cg3::Segment2d& segment(const size_t& id);

    size_t getIdLastSegment();

    size_t getIdP1LastSegment();

    size_t getIdQ1LastSegment();


    const std::vector<size_t> SplitInFour(const size_t& id, cg3::Segment2d s);
    const std::vector<size_t> SplitVerticaly(const size_t& idCurrent, const size_t& idNext, cg3::Segment2d s);
    std::vector<Trapezoid> SplitHorizontaly(Trapezoid& intermediateT, cg3::Segment2d innerSegment);


    void removeTrapezoid(const size_t& id);
    void removeTrapezoid(Trapezoid& t);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
