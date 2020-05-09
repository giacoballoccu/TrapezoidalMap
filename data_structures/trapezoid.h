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

    /*Pag 20 slides*/
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftp;
    cg3::Point2d rightp;

    size_t upperLeftNeighbor;
    size_t lowerLeftNeighbor;
    size_t upperRightNeighbor;
    size_t lowerRightNeighbor;


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

    size_t getUpperLeftNeighbor() const;
    size_t getUpperRightNeighbor() const;
    size_t getLowerLeftNeighbor() const;
    size_t getLowerRightNeighbor() const;

    void setUpperLeftNeighbor(const size_t& id);
    void setUpperRightNeighbor(const size_t& id);
    void setLowerLeftNeighbor(const size_t& id);
    void setLowerRightNeighbor(const size_t& id);

    void setNode(Node* node){
        this->node = node;
    }

    size_t getId(){
        return node->getId();
    }
    size_t upperLeftNeighborRfr(){
        return upperLeftNeighbor;
    };
    size_t lowerLeftNeighborRfr(){
        return lowerLeftNeighbor;
    };

    size_t upperRightNeighborRfr(){
        return upperRightNeighbor;
    };
    size_t lowerRightNeighborRfr(){
        return lowerRightNeighbor;
    };

    void setTop(cg3::Segment2d s);
    void setBottom(cg3::Segment2d s);
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


    Node*& nodeReference(){
        return node;
    }

    cg3::Segment2d getLeftEdge() const;
    cg3::Segment2d getRightEdge() const;
    const std::array<cg3::Point2d, 4> getPoints() const;

    void clear();

};


#endif // TRAPEZOID_H
