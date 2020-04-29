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
    std::list<Trapezoid>::iterator getIterator(const size_t& id);


    cg3::Point2d& point(const size_t& id);
    cg3::Segment2d& segment(const size_t& id);

    size_t getIdLastSegment();

    size_t getIdP1LastSegment();

    size_t getIdQ1LastSegment();

    void HandleCaseP1Inside(size_t& idCurrent, size_t& idNext, cg3::Segment2d segment,
                            std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge,
                            std::vector<std::vector<size_t>>& newTrapezoidIds);

    void HandleCasePointsOutside(size_t& idCurrent, size_t& idNext,  cg3::Segment2d segment,
                                 std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge,
                                 std::vector<std::vector<size_t>>& newTrapezoidIds);

    void HandleCaseQ1Inside(size_t& idCurrent, cg3::Segment2d segment,
                            std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge,
                            std::vector<std::vector<size_t>>& newTrapezoidIds);



    void SplitInFour(const size_t& id, cg3::Segment2d s, std::vector<std::vector<size_t>>& newTrapezoidIds);
    std::vector<Trapezoid> SplitVerticaly(const size_t& idCurrent, cg3::Point2d splitPoint);
    std::vector<Trapezoid> SplitHorizontaly(Trapezoid& intermediateT, cg3::Segment2d innerSegment);
    Trapezoid PerformeMerge(std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds);


    void removeTrapezoid(const size_t& id);
    void removeTemporaryTrapezoid(const size_t& id);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
