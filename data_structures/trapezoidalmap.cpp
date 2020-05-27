#include "trapezoidalmap.h"
#include "QRandomGenerator"
#define BOUNDINGBOX 1e+6
/**
 * @brief Constructor for trapezoidalMap.
 * Inizialize all vectors empty except for the trapezoid vector where is inserted a Trapezoid which is the first trapezoid
 * (a trapezoid of the size of the boundingbox)
 */
TrapezoidalMap::TrapezoidalMap(){
    _trapezoids = std::vector<Trapezoid>();
    _trapezoids.push_back(Trapezoid());
    _trapezoids[0].setNode(0);
    _isDeleted = std::vector<bool>();
    _isDeleted.push_back(false);
    _elegibleForMerge.reserve(2);
    _segments = std::vector<cg3::Segment2d>();
};
/**
 * @brief Returns a const vector with all the trapezoids in the map.
 * @return vector with all the trapezoids.
 */
const std::vector<Trapezoid> TrapezoidalMap::trapezoids() const{
    return _trapezoids;
};
/**
 * @brief Returns the vector with the flags trapezoids in the map.
 * @return vector with all the trapezoids.
 */
const std::vector<bool> TrapezoidalMap::isDeleted() const{
    return _isDeleted;
};

std::vector<size_t>& TrapezoidalMap::elegibleForMerge(){
    return _elegibleForMerge;
}
const size_t& TrapezoidalMap::getIdElegibleForMerge() const{
    return _elegibleForMerge[0];
}
size_t TrapezoidalMap::getTrapezoidsSize() const{
    return _trapezoids.size();
};
/**
 * @brief Insert a trapezoid in the trapezoidalMap and return the index correspondent to his position.
 */
inline size_t TrapezoidalMap::addTrapezoid(const Trapezoid& t){
    _trapezoids.push_back(t);
    _isDeleted.push_back(false);
    return _trapezoids.size()-1;
};
/**
 * @brief Substitute a trapezoid in the trapezoidalMap with a new one.
 * @param oldId, id of the old trapezoid already in the map.
 * @param newT, new Trapezoid
 * @return the index of the newT inserted, which is oldId.
 */
size_t TrapezoidalMap::replace(const size_t &oldId, const Trapezoid &newT){
    _trapezoids[oldId] = newT;
    return oldId;
};
/**
 * @brief Insert a segment in the trapezoidalMap and return the index correspondent to his position.
 */
size_t TrapezoidalMap::addSegment(const cg3::Segment2d& s){
    _segments.push_back(s);
    return _segments.size()-1;
};
/**
 * @brief Insert a point in the trapezoidalMap and return the index correspondent to his position.
 */
size_t TrapezoidalMap::addPoint(const cg3::Point2d& p){
    _points.push_back(p);
    return _points.size()-1;
};
/**
 * @brief Insert the id of a trapezoid that will be merged in the elegibleForMerge vector.
 * @param mergeCandidateId
 */
void TrapezoidalMap::addElegibleForMerge(const size_t& mergeCandidateId){
    _elegibleForMerge.push_back(mergeCandidateId);
}
/**
 * @brief Given the index of a trapezoid in the Trapezoidal Map return his reference if hasn't been deleted.
 * @param id of the trapezoid.
 * @return reference to the trapezoid at position id.
 * @note The assert can be substitued with a throw exception.
 * @note THIS FUNCTION SHOULD BE INLINE but linker doesn't want to, don't understand why.
 */
Trapezoid& TrapezoidalMap::trapezoid(const size_t& id){
    if(!_isDeleted[id]){
        return _trapezoids[id];
    }
    assert(_isDeleted[id] == false);
    return _trapezoids[SIZE_MAX];
}
/**
 * @brief Given the index of a trapezoid in the Trapezoidal Map return his const reference if hasn't been deleted.
 * @param id of the trapezoid.
 * @return const reference to the trapezoid at position id.
 * @note The assert can be substitued with a throw exception.
 * @note THIS FUNCTION SHOULD BE INLINE but linker doesn't want to, don't understand why.
 */
const Trapezoid& TrapezoidalMap::trapezoid(const size_t& id) const{
    if(!_isDeleted[id]){
        return _trapezoids[id];
    }
    assert(_isDeleted[id] == false);
    return _trapezoids[SIZE_MAX];
}

/**
 * @brief Given the index of a point in the Trapezoidal Map return his reference.
 * @param id of the point.
 * @return reference to the point at position id.
 */
inline cg3::Point2d& TrapezoidalMap::point(const size_t& id){
    return _points[id];
};
/**
 * @brief Given the index of a segment in the Trapezoidal Map return his reference.
 * @param id of the segment.
 * @return reference to the segment at position id.
 */
inline cg3::Segment2d& TrapezoidalMap::segment(const size_t& id){
    return _segments[id];
};
/**
 * @brief Given the index of a trapezoid in the Trapezoidal Map return his const copy if hasn't been deleted.
 * @param id of the trapezoid.
 * @return const copy to the trapezoid at position id.
 */
const Trapezoid TrapezoidalMap::trapezoidcpy(const size_t& id) const{
    if(!_isDeleted[id]){
        return _trapezoids[id];
    }
    return _trapezoids[id];
}
/**
 * @brief Given the index of a point in the Trapezoidal Map return his const reference.
 * @param id of the point.
 * @return const reference to the point at position id.
 */
const cg3::Point2d& TrapezoidalMap::point(const size_t& id) const{
    return _points[id];
};
/**
 * @brief Given the index of a segment in the Trapezoidal Map return his const reference.
 * @param id of the segment.
 * @return const reference to the segment at position id.
 */
const cg3::Segment2d& TrapezoidalMap::segment(const size_t& id) const{
    return _segments[id];
};
/**
 * @brief Flag trapezoid as removed.
 */
inline void TrapezoidalMap::removeTrapezoid(const size_t& id){
    _isDeleted[id] = true;
};




/**
 * @brief Return true if the two trapezoid can be merge false otherwise.
 * @param t1, the trapezoid of the leftmost trapezoid (between them).
 * @param t2, the trapezoid of the rightmost trapezoid (between them).
 * @return bool, true if they can merge, false otherwise.
 */
bool TrapezoidalMap::canTheyMerge(const Trapezoid& t1, const Trapezoid& t2){
    if(geoutils::arePointsEqual(t1.top().p2(), t2.top().p1()) and geoutils::arePointsEqual(t1.bottom().p2(), t2.bottom().p1())){
        return true;
    }else{
        return false;
    }
}

/**
 * @brief Performe the merge between two trapezoid, returning the id of the result. Keeps the adjacencies.
 * From the elegibleForMerge vector, stored as a attribute of the trapezoidal map, performe the merge between the two trapezoid
 * in the vector, remove the trapezoid used for the merge from the TrapezoidalMap and wipe the elegibleForMerge vector in order
 * to make it ready for the next merges.
 * @return The id of the merged trapezoid in the trapezoidal map.
 */
size_t TrapezoidalMap::PerformeMerge(){
    std::vector<size_t>& elegibleForMerge = this->elegibleForMerge();
    size_t merged = elegibleForMerge[0];
    size_t last = elegibleForMerge[1];
    Trapezoid& leftMost = trapezoid(elegibleForMerge[0]);
    Trapezoid& rightMost = trapezoid(last);
    leftMost.setTopP2(rightMost.top().p2());
    leftMost.setBottomP2(rightMost.bottom().p2());
    leftMost.setRightp(rightMost.rightp());

    leftMost.updateRightNeighborsOld(rightMost);

    std::vector<size_t>::iterator it;
    for(it = elegibleForMerge.begin() + 1; it != elegibleForMerge.end(); ++it){
        removeTrapezoid((*it));
    }
    elegibleForMerge.clear();

    return merged;
}


/**
 * @brief Split a trapezoid already in the map, with four new trapezoid. Doesn't handle adjacency.
 * Given the id of a trapezoid already in the tMap and the segment inside him returns four trapezoid generated using the endpoints
 * as a split points. A is the leftmost, D is the rightmost, B is the one at the upper center and C is the one in the lower center.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is crossing the current trapezoid.
 * @param idsTrapezoid, empty vector that will contain the result of the split.
 * @return The four trapezoid obtained by the split, pushed into the vector idsTrapezoid.
 */
void TrapezoidalMap::SplitInFour(const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& idsTrapezoid){
    Trapezoid current = trapezoidcpy(currentId);

    Trapezoid A = Trapezoid();
    Trapezoid B = Trapezoid();
    Trapezoid C = Trapezoid();
    Trapezoid D = Trapezoid();

    cg3::Point2d AtopP2 = cg3::Point2d(segment.p1().x(), geoutils::calculateYCoord(current.top(), segment.p1().x()));
    A.setTopP2(AtopP2);
    A.setTopP1(current.top().p1());
    cg3::Point2d AbotP2 = cg3::Point2d(segment.p1().x(), geoutils::calculateYCoord(current.bottom(), segment.p1().x()));
    A.setBottomP2(AbotP2);
    A.setBottomP1(current.bottom().p1());
    A.setRightp(segment.p1());
    A.setLeftp(current.leftp());

    B.setTopP1(cg3::Point2d(segment.p1().x(), geoutils::calculateYCoord(current.top(), segment.p1().x())));
    B.setTopP2(cg3::Point2d(segment.p2().x(), geoutils::calculateYCoord(current.top(), segment.p2().x())));
    B.setBottom(segment);
    B.setLeftp(segment.p1());
    B.setRightp(segment.p2());

    C.setTop(segment);
    C.setBottomP1(cg3::Point2d(segment.p1().x(), geoutils::calculateYCoord(current.bottom(), segment.p1().x())));
    C.setBottomP2(cg3::Point2d(segment.p2().x(), geoutils::calculateYCoord(current.bottom(), segment.p2().x())));
    C.setLeftp(segment.p1());
    C.setRightp(segment.p2());

    D.setTopP1(cg3::Point2d(segment.p2().x(), geoutils::calculateYCoord(current.top(), segment.p2().x())));
    D.setTopP2(current.top().p2());
    D.setBottomP1(cg3::Point2d(segment.p2().x(), geoutils::calculateYCoord(current.bottom(), segment.p2().x())));
    D.setBottomP2(current.bottom().p2());
    D.setLeftp(segment.p2());
    D.setRightp(current.rightp());

    idsTrapezoid.push_back(this->replace(currentId, A));
    idsTrapezoid.push_back(this->addTrapezoid(B));
    idsTrapezoid.push_back(this->addTrapezoid(C));
    idsTrapezoid.push_back(this->addTrapezoid(D));
}
/**
 * @brief Split a trapezoid already in the map, with three new trapezoid. Doesn't handle adjacency.
 * Given the id of a trapezoid already in the tMap, the segment that intersect it and the split point (the point of the segment which
 * is inside the current trapezoid) returns three trapezoid. The first is the result of a VERTICAL SPLIT using as pivot
 * the SPLITPOINT, the other two are obtained from a HORIZONTAL SPLIT. The intermediateT for the Horizontal split is based on the fact
 * that the segment is exiting or entering the current trapezoid.
 *
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is crossing the current trapezoid.
 * @param splitPoint, point used as a pivot for the vertical split of the trapezoid.
 * @return The three trapezoids obtained by the split, pushed into the vector idsTrapezoid.
 */
void TrapezoidalMap::SplitInThree(const size_t& currentId, const cg3::Segment2d& segment, const cg3::Point2d& splitPoint, std::vector<size_t>& idsTrapezoid){
    Trapezoid current = trapezoidcpy(currentId);

    std::vector<Trapezoid> splitResult = std::vector<Trapezoid>();
    SplitVerticaly(current, splitPoint, splitResult);


    if(splitPoint == segment.p1()){
        Trapezoid rightMost = splitResult[1];
        splitResult.pop_back();
        cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), cg3::Point2d(rightMost.rightp().x(), geoutils::calculateYCoord(segment, rightMost.rightp().x())));
        SplitHorizontaly(rightMost, innerSegment, splitResult);

        idsTrapezoid.push_back(this->replace(currentId, splitResult[0]));
        idsTrapezoid.push_back(this->addTrapezoid(splitResult[1]));
        idsTrapezoid.push_back(this->addTrapezoid(splitResult[2]));
    }else{
        Trapezoid leftMost = splitResult[0];
        splitResult.erase(splitResult.begin());
        cg3::Segment2d innerSegment = cg3::Segment2d(cg3::Point2d(leftMost.leftp().x(), geoutils::calculateYCoord(segment, leftMost.leftp().x())),segment.p2());
        SplitHorizontaly(leftMost, innerSegment, splitResult);

        idsTrapezoid.push_back(this->replace(currentId, splitResult[0]));
        idsTrapezoid.push_back(this->addTrapezoid(splitResult[1]));
        idsTrapezoid.push_back(this->addTrapezoid(splitResult[2]));
    }
}

/**
 * @brief Split a trapezoid horizontaly in two trapezoids.
 * Given the reference of a trapezoid already in the tMap, the innersegment and the split point (the point of the segment which is inside
 * the current trapezoid) pushes the two vector which are the result of the split in the hSplitResult vector passed as parameter.
 *
 * @param intermediateT, the trapezoid to split in two new trapezoid.
 * @param innerSegment, the segment used to split the trapezoid in half, is the part of the segment that is getting inserted to the trapezoidal
 * map which is totally contained in the trapezoid intersected. It will be the bottom segment of one and the top of the other.
 * @param hSplitResult, vector where the obtained trapezoid are pushed.
 * @return The two trapezoids obtained by the horizontal split of the intermediateT, pushed into the vector hSplitResult.
 */
void TrapezoidalMap::SplitHorizontaly(Trapezoid& intermediateT, const cg3::Segment2d& innerSegment, std::vector<Trapezoid>& hSplitResult){
    Trapezoid upper = Trapezoid();
    upper.setTop(intermediateT.top());
    upper.setBottom(innerSegment);
    upper.setLeftp(intermediateT.leftp());
    upper.setRightp(intermediateT.rightp());

    Trapezoid lower = Trapezoid();
    lower.setTop(innerSegment);
    lower.setBottom(intermediateT.bottom());
    lower.setLeftp(intermediateT.leftp());
    lower.setRightp(intermediateT.rightp());

    hSplitResult.push_back(upper);
    hSplitResult.push_back(lower);
}
/**
 * @brief Split a trapezoid verticaly in two trapezoids.
 * Given the reference of a trapezoid already in the tMap, the split point (the point of the segment which is inside
 * the current trapezoid) pushes the two vector which are the result of the split in the vSplitResult vector passed as parameter.
 *
 * @param current, the trapezoid to split in two new trapezoid.
 * @param splitPoint, point used as a pivot for the vertical split.
 * @param vSplitResult, vector where the obtained trapezoid are pushed.
 * @return The two trapezoids obtained by the horizontal split of the intermediateT, pushed into the vector hSplitResult.
 */
void TrapezoidalMap::SplitVerticaly(const Trapezoid& current, const cg3::Point2d& splitPoint, std::vector<Trapezoid>& vSplitResult){
    /*Vertical split of trapezoid*/
    Trapezoid leftHalf = Trapezoid();
    cg3::Point2d t1TopP2 = cg3::Point2d(splitPoint.x(), geoutils::calculateYCoord(current.top(), splitPoint.x()));
    cg3::Point2d t1BottomP2 = cg3::Point2d(splitPoint.x(), geoutils::calculateYCoord(current.bottom(), splitPoint.x()));
    leftHalf.setTop(cg3::Segment2d(current.top().p1(), t1TopP2));
    leftHalf.setBottom(cg3::Segment2d(current.bottom().p1(), t1BottomP2));
    leftHalf.setRightp(splitPoint);
    leftHalf.setLeftp(current.leftp());

    Trapezoid rightHalf = Trapezoid();
    cg3::Point2d t2TopP2 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(current.top(), current.rightp().x()));
    cg3::Point2d t2BottomP2 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(current.bottom(), current.rightp().x()));
    rightHalf.setTop(cg3::Segment2d(t1TopP2, t2TopP2));
    rightHalf.setBottom(cg3::Segment2d(t1BottomP2, t2BottomP2));
    rightHalf.setRightp(current.rightp());
    rightHalf.setLeftp(splitPoint);

    vSplitResult.push_back(leftHalf);
    vSplitResult.push_back(rightHalf);
}

/**
 * @brief Update indirectly the adjacency of a trapezoid.
 * Given a trapezoid, his id, the boolean left and the id of the newT. Take the [left upper and left lower] if left is true or [right upper and lower upper]
 * otherwise and check their [right upper and lower upper] if left is true and [left upper and left lower] otherwise, if one is equal to
 * currentId the value is substituted with idNewT.
 * @param current, Trapezoid from where start the exploration of the neighbors.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param left, if is true check the right neighbors of the left neighbors of the current otherwise check the left neighbors of the
 * right neighbors of the current.
 * @param idNewT, the trapezoid that will be substitued if the neighbor is equal to currentId
 * @return the side effect of neighbors left or right fixed with the idNewT.
 */
void TrapezoidalMap::indirectUpdateNeighbors(const Trapezoid& current, const size_t& currentId, bool left, const size_t& idNewT){
    if(left == true){
        if(current.lowerLeftNeighbor() != SIZE_MAX){
            size_t lowerLeft = current.lowerLeftNeighbor();
            if(trapezoid(lowerLeft).lowerRightNeighbor() == currentId){
                trapezoid(lowerLeft).updateLRNeighbor(idNewT);
            }
            if(trapezoid(lowerLeft).upperRightNeighbor() == currentId){
                trapezoid(lowerLeft).updateURNeighbor(idNewT);
            }
        }

        if(current.upperLeftNeighbor() != SIZE_MAX){
            size_t upperLeft = current.upperLeftNeighbor();
            if(trapezoid(upperLeft).lowerRightNeighbor() == currentId){
                trapezoid(upperLeft).updateLRNeighbor(idNewT);
            }
            if(trapezoid(upperLeft).upperRightNeighbor() == currentId){
                trapezoid(upperLeft).updateURNeighbor(idNewT);
            }
        }
    }else{
        if(current.lowerRightNeighbor() != SIZE_MAX){
            size_t lowerRight = current.lowerRightNeighbor();
            if(trapezoid(lowerRight).lowerLeftNeighbor() == currentId){
                trapezoid(lowerRight).updateLLNeighbor(idNewT);
            }
            if(trapezoid(lowerRight).upperLeftNeighbor() == currentId){
                trapezoid(lowerRight).updateULNeighbor(idNewT);
            }
        }

        if(current.upperRightNeighbor() != SIZE_MAX){
            size_t upperRight = current.upperRightNeighbor();
            if(trapezoid(upperRight).lowerLeftNeighbor() == currentId){
                trapezoid(upperRight).updateLLNeighbor(idNewT);
            }
            if(trapezoid(upperRight).upperLeftNeighbor() == currentId){
                trapezoid(upperRight).updateULNeighbor(idNewT);
            }
        }
    }

}


void TrapezoidalMap::clear(){
    _trapezoids.clear();
    _segments.clear();
    _points.clear();
    _isDeleted.clear();
    _elegibleForMerge.clear();
}














