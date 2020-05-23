#include "geoutils.h"

namespace geoutils {

/**
 * @brief Matrix 3x3 determinant, return 1 if the point is above the segment, 0 if is below and -1 if they are collinear
 * @param p, point
 * @param s, segment
 * @return Return 1 if the point is above the segment, 0 if is below and -1 if they are collinear
 */
int isPointAbove(cg3::Point2d p, cg3::Segment2d s){
    double matrixDet = sixDecimal(s.p2().x() - s.p1().x()) * sixDecimal(p.y() - s.p1().y()) - sixDecimal(s.p2().y() - s.p1().y()) * sixDecimal(p.x() - s.p1().x());

    if (matrixDet > 0) return 1;
    if (matrixDet < 0) return 0;
    return -1;
}

/**
 * @brief Return the input number approximate to his 6th decimal digit.
 * @param number, double to round
 * @return the same number reduced to 6 decimal places
 */
double sixDecimal(double number){
    return round( number * 100000.0 ) / 100000.0;
}

/**
 * @brief Return true if points are more or less equal false otherwise.
 * @param p1, first point
 * @param p2, second point
 * @return true if the absolute value of the difference of their x and y is smaller than a threshold.
 * @note the threshold can't be lowered, to lower it is necessary to implement a sanity check for the points elsewhere.
 */
bool arePointsEqual(cg3::Point2d p1, cg3::Point2d p2){
    double threshold = 0.1;
    if(abs(p1.x() - p2.x()) <= threshold){
        if(abs(p1.y() - p2.y()) <= threshold){
            return true;
        }

    }
    return false;
}

/**
 * @brief Calculate the intersection of a segment with an x and return the y value obtained.
 * @param s, segment
 * @param x, x coordinate position
 * @return the y that x with have if is intersecting the segment
 */
double calculateYCoord(cg3::Segment2d s, double x){
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();
    float m = (p2.y() - p1.y())/(p2.x()-p1.x());
    return sixDecimal(m*(x - p1.x()) + p1.y());
}

/**
 * @brief Sanity check to the segment in order to have the p1 of the segment the endpoint with the smaller x.
 * @param s, segment
 */
void validateSegment(cg3::Segment2d& s){
    if (s.p1().x() > s.p2().x()){
        cg3::Point2d tmp = s.p2();
        s.setP2(s.p1());
        s.setP1(tmp);
    }
    return;
}
}
