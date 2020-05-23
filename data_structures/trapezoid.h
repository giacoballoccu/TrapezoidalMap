#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/bounding_box2.h>
#include <cg3/geometry/polygon2.h>
#include "data_structures/segment_intersection_checker.h"
#include "utils/geoutils.h"
#include "node.h"
/**
 * @brief The Trapezoid class
 * A trapeozid is rapresented by two segment top and bottom and two points leftp and rightp. The vertices of the trapezoid are
 * top p1 top 2 bottom p2 bottom p1. A trapezoid can have at max four different neighbors, the link with the neighbors is implemented
 * with ids, that can be used for retrivial in the trapezoidal map class. The class holds the methods to update the neighbors of the trapezoid.
 * The trapezoid possess also the id of his associate leafNode if it exists, this rappresents the CROSSLINK between trapezoids and nodes.
 * @note If the trapezoid touches the bounding box he can have a not defined leftp or rightp, the default leftp is top.p1 and the default
 * rightp is top.p2.
 */
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

    const cg3::Segment2d& top() const;
    const cg3::Segment2d& bottom() const;
    const cg3::Point2d& leftp() const;
    const cg3::Point2d& rightp() const;
    size_t node() const;

    cg3::Segment2d& topRfr();
    cg3::Segment2d& bottomRfr();
    cg3::Point2d& leftpRfr();
    cg3::Point2d& rightpRfr();

    size_t upperLeftNeighbor() const;
    size_t upperRightNeighbor() const;
    size_t lowerLeftNeighbor() const;
    size_t lowerRightNeighbor() const;

    void setNode(const size_t& nodeId);
    void setTop(const cg3::Segment2d& s);
    void setTopP1(cg3::Point2d p);
    void setTopP2(cg3::Point2d p);
    void setBottom(const cg3::Segment2d& s);
    void setBottomP1(cg3::Point2d p);
    void setBottomP2(cg3::Point2d p);
    void setLeftp(const cg3::Point2d& p);
    void setRightp(const cg3::Point2d& p);

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
