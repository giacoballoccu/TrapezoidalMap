#ifndef GEOUTILS_H
#define GEOUTILS_H
#include <cg3/geometry/segment2.h>

namespace geoutils {
    bool isAbove(cg3::Point2d p, cg3::Segment2d s);

    float calculateYCoord(cg3::Segment2d s, float x);
}

#endif // GEOUTILS_H
