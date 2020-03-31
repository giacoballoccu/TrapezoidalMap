#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/bounding_box2.h>
#include <cg3/geometry/polygon2.h>
#include "data_structures/segment_intersection_checker.h"

class Trapezoid
{
private:

    /*Pag 20 slides*/
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftp;
    cg3::Point2d rightp;

    Trapezoid* upperLeftNeighbor;
    Trapezoid* lowerLeftNeighbor;
    Trapezoid* upperRightNeighbor;
    Trapezoid* lowerRightNeighbor;

public:
    Trapezoid();
    Trapezoid(cg3::Point2d p, bool left);
    Trapezoid(cg3::Segment2d s, bool above);
    Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom);

    cg3::Segment2d getTop() const;
    cg3::Segment2d getBottom() const;
    cg3::Point2d getLeftp() const;
    cg3::Point2d getRightp() const;

    Trapezoid* getUpperLeftNeighbor() const;
    Trapezoid* getUpperRightNeighbor() const;
    Trapezoid* getLowerLeftNeighbor() const;
    Trapezoid* getLowerRightNeighbor() const;

    void setTop(cg3::Segment2d s);
    void setBottom(cg3::Segment2d s);
    void setLeftp(cg3::Point2d p);
    void setRightp(cg3::Point2d p);

    void setUpperLeftNeighbor(Trapezoid *t);
    void setUpperRightNeighbor(Trapezoid *t);
    void setLowerLeftNeighbor(Trapezoid *t);
    void setLowerRightNeighbor(Trapezoid *t);

    cg3::BoundingBox2 computeTrapezoid() const;
    //bool isSegmentIntersecting(cg3::Segment2d s) const;
    //bool isSegmentInside(cg3::Point2d p1, cg3::Point2d q1) const;
    cg3::Segment2d getLeftEdge() const;
    cg3::Segment2d getRightEdge() const;

    void clear();
};

#endif // TRAPEZOID_H
