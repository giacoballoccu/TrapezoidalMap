#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/bounding_box2.h>
#include <cg3/geometry/polygon2.h>
#include "data_structures/segment_intersection_checker.h"
#include "utils/geoutils.h"
class Node;
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
 Node *node;

    Trapezoid();
    Trapezoid(cg3::Point2d p, bool left);
    Trapezoid(cg3::Segment2d s, bool above);
    Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom);

    cg3::Segment2d getTop() const;
    cg3::Segment2d getBottom() const;
    cg3::Point2d getLeftp() const;
    cg3::Point2d getRightp() const;

    cg3::Segment2d& getTopRfr();
    cg3::Segment2d& getBottomRfr();
    cg3::Point2d& getLeftpRfr();
    cg3::Point2d& getRightpRfr();

    Trapezoid* getUpperLeftNeighbor() const;
    Trapezoid* getUpperRightNeighbor() const;
    Trapezoid* getLowerLeftNeighbor() const;
    Trapezoid* getLowerRightNeighbor() const;

    Trapezoid*& upperLeftNeighborRfr(){
        return upperLeftNeighbor;
    };
    Trapezoid*& lowerLeftNeighborRfr(){
        return lowerLeftNeighbor;
    };

    void setTop(cg3::Segment2d s);
    void setBottom(cg3::Segment2d s);
    void setLeftp(cg3::Point2d p);
    void setRightp(cg3::Point2d p);

    void setUpperLeftNeighbor(Trapezoid *t);
    void setUpperRightNeighbor(Trapezoid *t);
    void setLowerLeftNeighbor(Trapezoid *t);
    void setLowerRightNeighbor(Trapezoid *t);

    void updateLeftNeighbors(Trapezoid& shared);
    void updateRightNeighbors(Trapezoid& shared);
    void updateLeftNeighbors(Trapezoid& upperLeft, Trapezoid& lowerLeft);
    void updateRightNeighbors(Trapezoid& upperRight, Trapezoid& lowerRight);
    void updateLeftNeighbors(Trapezoid& old, Trapezoid& upperLeft, Trapezoid& lowerLeft);
    void updateRightNeighbors(Trapezoid& old, Trapezoid& upperRight, Trapezoid& lowerRight);
    void updateLeftNeighbors(const Trapezoid& old);
    void updateRightNeighbors(const Trapezoid& old);
    void updateNeighbors(Trapezoid& upperLeft, Trapezoid& lowerLeft, Trapezoid& upperRight, Trapezoid& lowerRight);
    void updateNeighbors(Trapezoid& sharedLeft, Trapezoid& sharedRight);

    std::vector<Trapezoid> SplitInFour(cg3::Segment2d s);



    cg3::Segment2d getLeftEdge() const;
    cg3::Segment2d getRightEdge() const;
    std::vector<cg3::Point2d> getPoints() const;

    void clear();

};

#endif // TRAPEZOID_H
