#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/bounding_box2.h>
#include <cg3/geometry/polygon2.h>>

class Trapezoid
{
private:

    /*Pag 20 slides*/
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftp;
    cg3::Point2d rightp;

    Trapezoid *topNeighbor;
    Trapezoid *botNeighbor;
    Trapezoid *leftNeighbor;
    Trapezoid *rightNeighbor;
   /*To check: Neighborhoods?*/
public:
    Trapezoid();
    Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom);

    cg3::Segment2d getTop() const;
    cg3::Segment2d getBottom() const;
    cg3::Point2d getLeftp() const;
    cg3::Point2d getRightp() const;

    Trapezoid* getTopNeighbor() const;
    Trapezoid* getBotNeighbor() const;
    Trapezoid* getLeftNeighbor() const;
    Trapezoid* getRightNeighbor() const;

    void setTop(cg3::Segment2d s);
    void setBottom(cg3::Segment2d s);
    void setLeftp(cg3::Point2d p);
    void setRightp(cg3::Point2d p);

    void setTopNeighbor(Trapezoid *t);
    void setBotNeighbor(Trapezoid *t);
    void setLeftNeighbor(Trapezoid *t);
    void setRightNeighbor(Trapezoid *t);

    cg3::Polygon2 computeTrapezoid() const;

    void clear();
};

#endif // TRAPEZOID_H
