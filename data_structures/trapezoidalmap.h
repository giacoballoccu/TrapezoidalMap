#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"
#include "trapezoid.h"

class TrapezoidalMap{
private:
    std::vector<cg3::Segment2d> _segments;
    std::vector<cg3::Point2d> _points;
    std::vector<Trapezoid> _trapezoids;
    std::vector<bool> _isDeleted;
public:
    TrapezoidalMap();

    const std::vector<Trapezoid> trapezoids() const;
    const std::vector<cg3::Segment2d> segments() const;
    const std::vector<cg3::Point2d> points() const;
    const std::vector<bool> isDeleted() const;

    std::vector<Trapezoid>& getTrapezoidsRfr();
    std::vector<cg3::Segment2d>& getSegmentsRfr();
    std::vector<cg3::Point2d>& getPointsRfr();

    size_t addSegment(const cg3::Segment2d& s);
    size_t addPoint(const cg3::Point2d& p);
    size_t addTrapezoid(const Trapezoid& t);

    void setTrapezoids(std::vector<Trapezoid> trapezoids);
    void setIsDeleted(std::vector<bool> isDeleted);

    Trapezoid& trapezoid(const size_t& id);
    cg3::Point2d& point(const size_t& id);
    cg3::Segment2d& segment(const size_t& id);

    const Trapezoid& trapezoid(const size_t& id) const;
    const cg3::Point2d& point(const size_t& id) const;
    const cg3::Segment2d& segment(const size_t& id) const;


    const cg3::Segment2d& getLastSegment() const;
    const cg3::Point2d& getP1LastSegment() const;
    const cg3::Point2d& getQ1LastSegment() const;


    size_t getIdLastSegment() const;
    size_t getIdP1LastSegment() const;
    size_t getIdQ1LastSegment() const;

    void HandleCaseSegmentInside(std::vector<size_t> trapsIntersected,
                                 std::vector<std::vector<size_t>>& newTrapezoidIds);
    void HandleCaseP1Inside(size_t& currentT, size_t& nextT,
                            std::vector<size_t>& elegibleForMerge,
                            std::vector<std::vector<size_t>>& newTrapezoidIds);

    void HandleCasePointsOutside(size_t& currentT, size_t& nextT, size_t& prevT,
                                 std::vector<size_t>& elegibleForMerge,
                                 std::vector<std::vector<size_t>>& newTrapezoidIds);

    void HandleCaseQ1Inside(size_t& currentT, std::vector<size_t>& elegibleForMerge,
                            std::vector<std::vector<size_t>>& newTrapezoidIds);

    void indirectUpdateNeighbors(const size_t& current, bool left, const size_t& idNewT);


    void PerformeMerge(std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds);

    std::vector<Trapezoid> SplitInFour(const Trapezoid& current, const cg3::Segment2d s);
    std::vector<Trapezoid> SplitInThree(const Trapezoid& current, const cg3::Segment2d segment, const cg3::Point2d splitPoint);
    std::vector<Trapezoid> SplitVerticaly(const Trapezoid& currentT, const cg3::Point2d splitPoint);
    std::vector<Trapezoid> SplitHorizontaly(const Trapezoid& intermediateT, const cg3::Segment2d innerSegment);

    void removeTrapezoid(const size_t& id);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
