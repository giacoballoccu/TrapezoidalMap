#ifndef GEOUTILS_H
#define GEOUTILS_H
#include <cg3/geometry/segment2.h>

namespace geoutils {
    int isPointAbove(cg3::Point2d p, cg3::Segment2d s);

    double calculateYCoord(cg3::Segment2d s, double x);

    void validateSegment(cg3::Segment2d& s);

    double sixDecimal(double number);
    bool arePointsEqual(cg3::Point2d p1, cg3::Point2d p2);

}

#endif // GEOUTILS_H
