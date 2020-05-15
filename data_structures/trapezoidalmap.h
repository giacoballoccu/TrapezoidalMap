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
    inline size_t addTrapezoid(const Trapezoid& t);

    void setTrapezoids(std::vector<Trapezoid> trapezoids);
    void setIsDeleted(std::vector<bool> isDeleted);

    inline Trapezoid& trapezoid(const size_t& id);
    cg3::Point2d& point(const size_t& id);
    cg3::Segment2d& segment(const size_t& id);

    Trapezoid trapezoidcpy(const size_t& id) const;
    const cg3::Point2d& point(const size_t& id) const;
    const cg3::Segment2d& segment(const size_t& id) const;


    const cg3::Segment2d& getLastSegment() const;
    const cg3::Point2d& getP1LastSegment() const;
    const cg3::Point2d& getQ1LastSegment() const;


    size_t getIdLastSegment() const;
    size_t getIdP1LastSegment() const;
    size_t getIdQ1LastSegment() const;

    std::vector<size_t> HandleCaseSegmentInside(const size_t& currentId);
    std::vector<size_t> HandleCaseSegmentInsideDegenerativeLeft(const size_t& currentId);
    std::vector<size_t> HandleCaseSegmentInsideDegenerativeRight(const size_t& currentId);

    std::vector<size_t> HandleCaseP1Inside(const size_t& currentId, std::vector<size_t>& elegibleForMerge);
    std::vector<size_t> HandleCaseP1InsideDegenerative(const size_t& currentId, std::vector<size_t>& elegibleForMerge);

    std::vector<size_t> HandleCasePointsOutside(const size_t& currentId, std::vector<size_t>& elegibleForMerge, std::vector<size_t>& lastTrapezoidsInserted);

    std::vector<size_t> HandleCaseQ1Inside(const size_t& currentId, std::vector<size_t>& elegibleForMerge);
    std::vector<size_t> HandleCaseQ1InsideDegenerative(const size_t& currentId, std::vector<size_t>& elegibleForMerge);

    void indirectUpdateNeighbors(const Trapezoid& current, const size_t& currentId, bool left, const size_t& idNewT);

    bool canTheyMerge(const Trapezoid& t1, const Trapezoid& t2);
    size_t PerformeMerge(std::vector<size_t>& elegibleForMerge);

    std::vector<size_t> SplitInFour(const size_t& currentId, const cg3::Segment2d s);
    std::vector<size_t> SplitInThree(const size_t& currentId, const cg3::Segment2d segment, const cg3::Point2d splitPoint);
    std::vector<Trapezoid> SplitVerticaly(const Trapezoid& currentT, const cg3::Point2d splitPoint);
    std::vector<Trapezoid> SplitHorizontaly(const Trapezoid& intermediateT, const cg3::Segment2d innerSegment);

    size_t replace(const size_t &oldId, const Trapezoid &newT);
    inline void removeTrapezoid(const size_t& id);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
