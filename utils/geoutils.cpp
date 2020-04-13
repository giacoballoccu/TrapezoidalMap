#include "geoutils.h"
namespace geoutils {

/*
!!! Move it in utility
Cross product to understand if the point is above or below line, formula returns 0 if segment and point are collinear.
1 if the point is above the segment

*/
bool isAbove(cg3::Point2d p, cg3::Segment2d s){
     return ((s.p2().x() - s.p1().x())*(p.y() - s.p1().y()) - (s.p2().y() - s.p1().y())*(p.x() - s.p1().x())) > 0;
}

/*

This function calculate the y value in a given x of a segment from the line equation passing from two point and intersection

*/
float calculateYCoord(cg3::Segment2d s, float x){
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();
    float m = (p2.y() - p1.y())/(p2.x()-p1.x());
    return (m*(x - p1.x()) + p1.y());
}

}
