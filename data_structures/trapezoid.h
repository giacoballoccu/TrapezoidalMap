#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/bounding_box2.h>
#include <cg3/geometry/polygon2.h>
#include "data_structures/segment_intersection_checker.h"
#include "utils/geoutils.h"
#include "node.h"
class Trapezoid
{
private:
    cg3::Segment2d _top;
    cg3::Segment2d _bottom;
    cg3::Point2d _leftp;
    cg3::Point2d _rightp;

    size_t _upperLeftNeighbor;
    size_t _lowerLeftNeighbor;
    size_t _upperRightNeighbor;
    size_t _lowerRightNeighbor;

    size_t _node;
public:
    Trapezoid();

    cg3::Segment2d top() const;
    cg3::Segment2d bottom() const;
    cg3::Point2d leftp() const;
    cg3::Point2d rightp() const;
    size_t node() const;

    cg3::Segment2d& topRfr();
    cg3::Segment2d& bottomRfr();
    cg3::Point2d& leftpRfr();
    cg3::Point2d& rightpRfr();

    size_t upperLeftNeighbor() const;
    size_t upperRightNeighbor() const;
    size_t lowerLeftNeighbor() const;
    size_t lowerRightNeighbor() const;

    void setUpperLeftNeighbor(const size_t& id);
    void setUpperRightNeighbor(const size_t& id);
    void setLowerLeftNeighbor(const size_t& id);
    void setLowerRightNeighbor(const size_t& id);

    void setNode(const size_t& nodeId);
    void setTop(cg3::Segment2d s);
    void setTopP1(cg3::Point2d p);
    void setTopP2(cg3::Point2d p);
    void setBottom(cg3::Segment2d s);
    void setBottomP1(cg3::Point2d p);
    void setBottomP2(cg3::Point2d p);
    void setLeftp(cg3::Point2d p);
    void setRightp(cg3::Point2d p);

    void updateLLNeighbor(const size_t& id);
    void updateULNeighbor(const size_t& id);
    void updateLRNeighbor(const size_t& id);
    void updateURNeighbor(const size_t& id);

    void updateLeftNeighbors(const size_t& sharedId);
    void updateRightNeighbors(const size_t& sharedId);
    void updateLeftNeighbors(const size_t& idUpperLeft, const size_t& idLowerLeft);
    void updateRightNeighbors(const size_t& idUpperRight, const size_t& idLowerRight);
    void updateLeftNeighborsOld(Trapezoid& old);
    void updateRightNeighborsOld(Trapezoid& old);
    void updateNeighborsLeft(const Trapezoid& old, const size_t& idUpperLeft, const size_t& idLowerLeft);
    void updateNeighborsRight(const Trapezoid& old, const size_t& idUpperRight, const size_t& idLowerRight);
    void updateNeighbors(const size_t idSharedLeft, const size_t idSharedRight);

    void indirectUpdateNeighborsLeft(const size_t &oldId,const size_t &idNewT);
    void indirectUpdateNeighborsRight(const size_t &oldId,const size_t &idNewT);
    const std::array<cg3::Point2d, 4> getVertices() const;

    ~Trapezoid();
};


#endif // TRAPEZOID_H
