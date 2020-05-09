#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"
#include "trapezoid.h"

class TrapezoidalMap{
private:
    std::vector<cg3::Segment2d> segments;
    std::vector<cg3::Point2d> points;
    std::vector<Trapezoid> trapezoids;
public:
    std::vector<bool> isDeleted;

    TrapezoidalMap();

    std::vector<cg3::Segment2d> getSegments() const;
    std::vector<cg3::Segment2d>& getSegmentsRfr();

    std::vector<bool> getIsDeleted() const{
        return isDeleted;
    }
    void setIsDeleted(std::vector<bool> isDelete){
        this->isDeleted = isDelete;
    }

    std::vector<cg3::Point2d> getPoints() const;
    std::vector<cg3::Point2d>& getPointsRfr();

    std::vector<Trapezoid> getActiveTrapezoids() const;
    std::vector<Trapezoid> getTrapezoids() const;
    std::vector<Trapezoid>& getTrapezoidsRfr();

    size_t addSegment(const cg3::Segment2d& s);
    size_t addPoint(const cg3::Point2d& p);
    size_t addTrapezoid(const Trapezoid& t);

    void setTrapezoids(std::vector<Trapezoid> trapezoids);
    Trapezoid& trapezoid(const size_t& id);



    cg3::Point2d& point(const size_t& id);
    cg3::Segment2d& segment(const size_t& id);

    cg3::Segment2d getLastSegment();
    cg3::Point2d getP1LastSegment();
    cg3::Point2d getQ1LastSegment();

    size_t getIdLastSegment();
    size_t getIdP1LastSegment();
    size_t getIdQ1LastSegment();

    void HandleOneTIntersection(std::vector<size_t> trapsIntersected, std::vector<std::vector<size_t>> &newTrapezoidIds);
    //void HandleTwoTIntersected(std::vector<size_t>& trapsIntersected, std::vector<std::vector<size_t>> &newTrapezoidIds);

    void HandleCaseP1Inside(Trapezoid& currentT, Trapezoid& nextT,
                            std::vector<size_t>& elegibleForMerge,
                            std::vector<std::vector<size_t>>& newTrapezoidIds);

    /*void HandleCasePointsOutside(Trapezoid& currentT, Trapezoid& nextT,
                                 std::vector<size_t>& elegibleForMerge,
                                 std::vector<std::vector<size_t>>& newTrapezoidIds);*/

    void HandleCaseQ1Inside(Trapezoid& currentT, std::vector<size_t>& elegibleForMerge,
                            std::vector<std::vector<size_t>>& newTrapezoidIds);

    void indirectUpdateNeighbors(const size_t& current, bool left, const size_t& idNewT);

    //void SplitInFour(Trapezoid& currentT, std::vector<std::vector<size_t>>& newTrapezoidIds);

    void PerformeMerge(std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds);
    //void PerformeMerge(Trapezoid& t1, Trapezoid& t2);

    std::vector<Trapezoid> SplitInFour(const Trapezoid& current, const cg3::Segment2d s);
    std::vector<Trapezoid> SplitInThree(const Trapezoid& current, const cg3::Segment2d segment, const cg3::Point2d splitPoint);
    std::vector<Trapezoid> SplitVerticaly(const Trapezoid& currentT, const cg3::Point2d splitPoint);
    std::vector<Trapezoid> SplitHorizontaly(const Trapezoid& intermediateT, const cg3::Segment2d innerSegment);

    void removeTrapezoid(const size_t& id);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
