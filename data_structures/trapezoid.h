#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>

class Trapezoid
{
private:

    /*Pag 20 slides*/
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftp;
    cg3::Point2d rightp;

   /*To check: Neighborhoods?*/
public:
    Trapezoid();

    cg3::Segment2d getTop() const;
    cg3::Segment2d getBottom() const;
    cg3::Point2d getLeftp() const;
    cg3::Point2d getRightp() const;

    void setTop(cg3::Segment2d);
    void setBottom(cg3::Segment2d);
    void setLeftp(cg3::Point2d);
    void setRightp(cg3::Point2d);

    void clear();
    //getArea(top, bottom, leftp, rightp)
};

#endif // TRAPEZOID_H
