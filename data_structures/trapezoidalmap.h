#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "node.h"
#include "dag.h"
#include "trapezoid.h"
/**
* @class TrapezoidalMap
* @brief Holds the trapezoids, the segments and the points currently in the trapezoidal map, and the methods to handle the insertion cases
* The classes stores all the segments, points and trapezoids in vectors in order to retrive them in O(1) having the index of the
* element. Trapezoids are NEVER deleted but flagged with a boolean in the isDeleted vector, a vector which has the same size of trapezoids.
* Elegible for merge is a vector used to store the trapezoid which will be merged, the max size that elegibleForMerge can reach is two.
*/
class TrapezoidalMap{
private:
    std::vector<cg3::Segment2d> _segments;
    std::vector<cg3::Point2d> _points;
    std::vector<Trapezoid> _trapezoids;
    std::vector<bool> _isDeleted; //Related to trapezoids
    std::vector<size_t> _elegibleForMerge; //Holds the trapezoid that will be merged
public:
    TrapezoidalMap();

    const std::vector<Trapezoid> trapezoids() const;
    const std::vector<bool> isDeleted() const;
    std::vector<size_t>& elegibleForMerge();

    size_t getTrapezoidsSize() const;

    size_t addSegment(const cg3::Segment2d& s);
    size_t addPoint(const cg3::Point2d& p);
    size_t addTrapezoid(const Trapezoid& t);
    void addElegibleForMerge(const size_t& id);

    Trapezoid& trapezoid(const size_t& id);
    const Trapezoid& trapezoid(const size_t& id) const;
    cg3::Point2d& point(const size_t& id);
    cg3::Segment2d& segment(const size_t& id);

    const Trapezoid trapezoidcpy(const size_t& id) const;
    const cg3::Point2d& point(const size_t& id) const;
    const cg3::Segment2d& segment(const size_t& id) const;

    const size_t& getIdElegibleForMerge() const;
    bool canTheyMerge(const Trapezoid& t1, const Trapezoid& t2);
    size_t PerformeMerge();

    void SplitInFour(const size_t& currentId, const cg3::Segment2d& s, std::vector<size_t>& lastTrapezoidsInserted);
    void SplitInThree(const size_t& currentId, const cg3::Segment2d& segment, const cg3::Point2d& splitPoint, std::vector<size_t>& lastTrapezoidsInserted);
    void SplitVerticaly(const Trapezoid& currentT, const cg3::Point2d& splitPoint, std::vector<Trapezoid>& vSplitResult);
    void SplitHorizontaly(Trapezoid& intermediateT, const cg3::Segment2d& innerSegment, std::vector<Trapezoid>& hSplitResult);

    void indirectUpdateNeighbors(const Trapezoid& current, const size_t& currentId, bool left, const size_t& idNewT);

    size_t replace(const size_t &oldId, const Trapezoid &newT);
    void removeTrapezoid(const size_t& id);

    void clear();
};
#endif // TRAPEZOIDALMAP_H
