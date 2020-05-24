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
inline void TrapezoidalMap::addElegibleForMerge(const size_t& mergeCandidateId){
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
 * @brief Handle the case when the segment is totally inside a trapezoid.
 * Case when the segment is fully inside the trapezoid intersected, the case will split the trapezoid in four new trapezoid,
 * substituting the old one with one of the new, and updating their references.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is crossing the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The four trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCaseSegmentInside(const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);
    SplitInFour(currentId, segment, lastTrapezoidsInserted);

    size_t idA = lastTrapezoidsInserted[0];
    size_t idB = lastTrapezoidsInserted[1];
    size_t idC = lastTrapezoidsInserted[2];
    size_t idD = lastTrapezoidsInserted[3];

    Trapezoid& referenceA = trapezoid(idA);
    Trapezoid& referenceB = trapezoid(idB);
    Trapezoid& referenceC = trapezoid(idC);
    Trapezoid& referenceD = trapezoid(idD);

    referenceA.updateNeighborsRight(current, idB, idC);
    referenceB.updateNeighbors(idA, idD);
    referenceC.updateNeighbors(idA, idD);
    referenceD.updateNeighborsLeft(current, idB, idC);
    indirectUpdateNeighbors(current, currentId, true, idA);
    indirectUpdateNeighbors(current, currentId, false, idD);
}
/**
 * @brief Handle the case when the segment is totally inside a trapezoid and the right endpoint of the segment is the leftp or
 * the p1 of the top or the p1 of the bottom.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The three trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCaseSegmentInsideDegenerativeRight(const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);
    lastTrapezoidsInserted.clear();
    SplitInThree(currentId, segment, segment.p1(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idLeft = lastTrapezoidsInserted[0];
    size_t idRightUpper = lastTrapezoidsInserted[1];
    size_t idRightLower = lastTrapezoidsInserted[2];

    bool isQ1EqualTopP2 = geoutils::arePointsEqual(segment.p2(), current.top().p2());
    bool isQ1EqualBotP2 = geoutils::arePointsEqual(segment.p2(), current.bottom().p2());

    Trapezoid& referenceLeft = trapezoid(idLeft);
    Trapezoid& referenceRightU = trapezoid(idRightUpper);
    Trapezoid& referenceRightL = trapezoid(idRightLower);

    /*Set neighbors internally*/
    referenceLeft.updateLeftNeighborsOld(current);
    referenceLeft.updateRightNeighbors(idRightUpper, idRightLower);
    referenceRightU.updateLeftNeighbors(idLeft);
    referenceRightL.updateLeftNeighbors(idLeft);

    /*Set neighbors depending by the case*/
    indirectUpdateNeighbors(current, currentId, true, idLeft);
    if (isQ1EqualTopP2 and !isQ1EqualBotP2){
        indirectUpdateNeighbors(current, currentId, false, idRightLower);
        referenceRightL.updateRightNeighborsOld(current);
    }else if(!isQ1EqualTopP2 and isQ1EqualBotP2){
        indirectUpdateNeighbors(current, currentId, false, idRightUpper);
        referenceRightU.updateRightNeighborsOld(current);
    }else{
        referenceRightL.updateRightNeighbors(current.lowerRightNeighbor());
        referenceRightU.updateRightNeighbors(current.upperRightNeighbor());
        if(current.lowerRightNeighbor() != SIZE_MAX){
            trapezoid(current.lowerRightNeighbor()).updateLeftNeighbors(idRightLower);
        }
        if(current.upperRightNeighbor() != SIZE_MAX){
            trapezoid(current.upperRightNeighbor()).updateLeftNeighbors(idRightUpper);
        }
    }
}

/**
 * @brief Handle the case when the segment is totally inside a trapezoid and the left endpoint of the segment is the rightp or
 * the p2 of the top or the p2 of the bottom.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The three trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCaseSegmentInsideDegenerativeLeft(const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);
    lastTrapezoidsInserted.clear();
    bool isP1EqualTopP1 = geoutils::arePointsEqual(segment.p1(),current.top().p1());
    bool isP1EqualBotP1 = geoutils::arePointsEqual(segment.p1(), current.bottom().p1());
    SplitInThree(currentId, segment, segment.p2(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idRight = lastTrapezoidsInserted[0];
    size_t idLeftUpper = lastTrapezoidsInserted[1];
    size_t idLeftLower = lastTrapezoidsInserted[2];

    Trapezoid& referenceRight = trapezoid(idRight);
    Trapezoid& referenceLeftU = trapezoid(idLeftUpper);
    Trapezoid& referenceLeftL = trapezoid(idLeftLower);

    /*Set neighbors internally for tSplitResult*/
    referenceRight.updateRightNeighborsOld(current);
    referenceRight.updateLeftNeighbors(idLeftUpper, idLeftLower);
    referenceLeftU.updateRightNeighbors(idRight);
    referenceLeftL.updateRightNeighbors(idRight);
    indirectUpdateNeighbors(current, currentId, false, idRight);

    if(isP1EqualTopP1 and !isP1EqualBotP1){
        indirectUpdateNeighbors(current, currentId, true, idLeftLower);
        referenceLeftL.updateLeftNeighborsOld(current);
    }else if(!isP1EqualTopP1 and isP1EqualBotP1){
        indirectUpdateNeighbors(current, currentId, true, idLeftUpper);
        referenceLeftU.updateLeftNeighborsOld(current);
    }else{
        referenceLeftL.updateLeftNeighbors(current.lowerLeftNeighbor());
        referenceLeftU.updateLeftNeighbors(current.upperLeftNeighbor());
        if(current.lowerLeftNeighbor() != SIZE_MAX){
            trapezoid(current.lowerLeftNeighbor()).updateRightNeighbors(idLeftLower);
        }
        if(current.upperLeftNeighbor() != SIZE_MAX){
            trapezoid(current.upperLeftNeighbor()).updateRightNeighbors(idLeftUpper);
        }
    }

}
/**
 * @brief Handle the case when the p1 is equal to the leftp of the trapezoid and the q1 is out of the trapezoid.
 * Split the trapezoid in two other trapezoids, push their id in the lastTrapezoidInserted vector, based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Updates the adjacency for the next
 * step.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCaseP1InsideDegenerative(const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);

    bool isPointAbove = geoutils::isPointAbove(current.rightp(), segment);
    bool isP1EqualTopP1 = geoutils::arePointsEqual(segment.p1(),current.top().p1());
    bool isP1EqualBotP1 = geoutils::arePointsEqual(segment.p1(), current.bottom().p1());

    cg3::Point2d q1 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(segment, current.rightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), q1);

    std::vector<Trapezoid> hSplit = std::vector<Trapezoid>();
    SplitHorizontaly(current, innerSegment, hSplit);

    lastTrapezoidsInserted.clear();
    lastTrapezoidsInserted.push_back(this->replace(currentId, hSplit[0]));
    lastTrapezoidsInserted.push_back(this->addTrapezoid(hSplit[1]));

    size_t idUpper = lastTrapezoidsInserted[0];
    size_t idLower = lastTrapezoidsInserted[1];
    Trapezoid& referenceUpper = trapezoid(idUpper);
    Trapezoid& referenceLower = trapezoid(idLower);

    if (isPointAbove){
        addElegibleForMerge(idLower);
        indirectUpdateNeighbors(current, currentId, false, idUpper);
        referenceUpper.updateRightNeighborsOld(current);
    }else{
        addElegibleForMerge(idUpper);
        indirectUpdateNeighbors(current, currentId, false, idLower);
        referenceLower.updateRightNeighborsOld(current);
    }

    if(isP1EqualTopP1 and !isP1EqualBotP1){
        referenceLower.updateLeftNeighborsOld(current);
        indirectUpdateNeighbors(current, currentId, true, idLower);
    }else if(!isP1EqualTopP1 and isP1EqualBotP1){
        referenceUpper.updateLeftNeighborsOld(current);
        indirectUpdateNeighbors(current, currentId, true, idUpper);
    }else if(isP1EqualTopP1 and isP1EqualBotP1){

    }else{
        referenceUpper.updateLeftNeighbors(current.upperLeftNeighbor());
        referenceLower.updateLeftNeighbors(current.lowerLeftNeighbor());
        if (isPointAbove){
            referenceUpper.updateRightNeighborsOld(current);
        }else{
            referenceLower.updateRightNeighborsOld(current);
        }

        if(current.upperLeftNeighbor() != SIZE_MAX){
            trapezoid(current.upperLeftNeighbor()).updateRightNeighbors(idUpper);
        }
        if(current.lowerLeftNeighbor() != SIZE_MAX){
            trapezoid(current.lowerLeftNeighbor()).updateRightNeighbors(idLower);
        }
    }
}
/**
 * @brief Handle the case when the q1 is equal to the rightp of the trapezoid and the p1 is out of the trapezoid.
 * Split the trapezoid in two other trapezoids, push their id in the lastTrapezoidInserted vector, based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Completes the update of the adjacency,
 * based on the boolean isQ1EqualTopP2 and boolean isQ1EqualBotP2.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCaseQ1InsideDegenerative(const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);
    bool isQ1EqualTopP2 = geoutils::arePointsEqual(segment.p2(), current.top().p2());
    bool isQ1EqualBotP2 = geoutils::arePointsEqual(segment.p2(), current.bottom().p2());

    cg3::Point2d p1 = cg3::Point2d(current.leftp().x(), geoutils::calculateYCoord(segment, current.leftp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, segment.p2());

    std::vector<Trapezoid> hSplit = std::vector<Trapezoid>();
    SplitHorizontaly(current, innerSegment, hSplit);

    lastTrapezoidsInserted.clear();
    lastTrapezoidsInserted.push_back(this->replace(currentId,hSplit[0]));
    lastTrapezoidsInserted.push_back(this->addTrapezoid(hSplit[1]));

    size_t idUpper = lastTrapezoidsInserted[0];
    size_t idLower = lastTrapezoidsInserted[1];
    Trapezoid& referenceLower = trapezoid(idLower);
    Trapezoid& referenceUpper = trapezoid(idUpper);

    Trapezoid mergeCandidate = trapezoid(getIdElegibleForMerge());
    if(canTheyMerge(mergeCandidate, referenceLower)){
        referenceUpper.updateLeftNeighborsOld(current);
        addElegibleForMerge(idLower);
        lastTrapezoidsInserted[1] = PerformeMerge();
        Trapezoid& referenceLower = trapezoid(lastTrapezoidsInserted[1]);
        indirectUpdateNeighbors(current, currentId, true, idUpper);

        if (isQ1EqualTopP2 and !isQ1EqualBotP2){
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            referenceLower.updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[1]);
        }else if(!isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[0]);
        }else if(isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            if(current.upperLeftNeighbor() == current.lowerLeftNeighbor()){
                referenceUpper.updateLeftNeighbors(current.upperLeftNeighbor());
            }else{
                referenceUpper.updateLeftNeighborsOld(current);
            }
        }else{
            referenceUpper.updateLeftNeighborsOld(current);
            referenceUpper.updateRightNeighbors(current.upperRightNeighbor());
            referenceLower.updateRightNeighbors(current.lowerRightNeighbor());
            indirectUpdateNeighbors(current, currentId, true, idUpper);
            trapezoid(current.upperRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idUpper);
            trapezoid(current.lowerRightNeighbor()).indirectUpdateNeighborsLeft(currentId, lastTrapezoidsInserted[1]);
        }
    }else{
        referenceLower.updateLeftNeighborsOld(current);
        addElegibleForMerge(idUpper);
        lastTrapezoidsInserted[0] = PerformeMerge();
        Trapezoid& referenceUpper = trapezoid(lastTrapezoidsInserted[0]);
        indirectUpdateNeighbors(current, currentId, true, idLower);
        if (isQ1EqualTopP2 and !isQ1EqualBotP2){
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            referenceLower.updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[1]);
        }else if(!isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[0]);
        }else if(isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            if(current.upperLeftNeighbor() == current.lowerLeftNeighbor()){
                referenceLower.updateLeftNeighbors(current.lowerLeftNeighbor());
            }else{
                referenceLower.updateLeftNeighborsOld(current);
            }
        }else{
            referenceLower.updateLeftNeighborsOld(current);
            referenceLower.updateRightNeighbors(current.lowerRightNeighbor());
            referenceUpper.updateRightNeighbors(current.upperRightNeighbor());
            indirectUpdateNeighbors(current, currentId, true, idLower);
            trapezoid(current.upperRightNeighbor()).indirectUpdateNeighborsLeft(currentId, lastTrapezoidsInserted[0]);
            trapezoid(current.lowerRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idLower);
        }
    }

}
/**
 * @brief Handle the case when the p1 is inside the trapezoid and the p1 is out of the trapezoid.
 * Split the trapezoid in three other trapezoids, push their id in the lastTrapezoidInserted vector. Based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Updates the adjacency for the next
 * step.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCaseP1Inside(const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);
    bool isPointAbove = geoutils::isPointAbove(current.rightp(), segment);
    SplitInThree(currentId, segment, segment.p1(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idLeft = lastTrapezoidsInserted[0];
    size_t idRightUpper = lastTrapezoidsInserted[1];
    size_t idRightLower = lastTrapezoidsInserted[2];

    Trapezoid& referenceLeft = trapezoid(idLeft);
    Trapezoid& referenceRightU = trapezoid(idRightUpper);
    Trapezoid& referenceRightL = trapezoid(idRightLower);

    /*Set neighbors internally for firstTSplit*/
    referenceLeft.updateLeftNeighborsOld(current);
    referenceLeft.updateRightNeighbors(idRightUpper, idRightLower);
    referenceRightU.updateLeftNeighbors(idLeft);
    referenceRightL.updateLeftNeighbors(idLeft);
    referenceRightU.updateRightNeighborsOld(current);
    referenceRightL.updateRightNeighborsOld(current);
    indirectUpdateNeighbors(current, currentId, true, idLeft);

    if (isPointAbove){
        addElegibleForMerge(idRightLower);
        indirectUpdateNeighbors(current, currentId, false, idRightUpper);
        /*Merge upperr*/
    }else{
        addElegibleForMerge(idRightUpper);
        indirectUpdateNeighbors(current, currentId, false, idRightLower);
    }

}
/**
 * @brief Handle the case when the p1 is outside and the q1 is outside.
 * Split the trapezoid in two other trapezoids, push their id in the lastTrapezoidInserted vector. Based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Uses the lastTrapezoidsIntersected
 * of the previous step for update the adjacency, for the next step. Then push the new created trapezoids in the lastTrapezoidsInserted.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCasePointsOutside(const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);

    bool isPointAbove = geoutils::isPointAbove(current.rightp(), segment);
    cg3::Point2d p1 = cg3::Point2d(current.leftp().x(), geoutils::calculateYCoord(segment, current.leftp().x()));
    cg3::Point2d q1 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(segment, current.rightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, q1);
    std::vector<Trapezoid> hSplit = std::vector<Trapezoid>();
    SplitHorizontaly(current, innerSegment, hSplit);

    size_t idUpper = this->replace(currentId, hSplit[0]);
    size_t idLower = this->addTrapezoid(hSplit[1]);

    Trapezoid& referenceUpper = trapezoid(idUpper);
    Trapezoid& referenceLower = trapezoid(idLower);

    /*Merge handling*/
    referenceUpper.updateRightNeighborsOld(current);
    referenceLower.updateRightNeighborsOld(current);
    Trapezoid& mergeCandidate = trapezoid(getIdElegibleForMerge());
    size_t merged = getIdElegibleForMerge();
    /*Merge with lower*/
    if(canTheyMerge(mergeCandidate, referenceLower)){
        addElegibleForMerge(idLower);
        idLower = PerformeMerge();
        size_t leftNeighbor = lastTrapezoidsInserted.size() == 3 ? lastTrapezoidsInserted[1]:lastTrapezoidsInserted[0];
        referenceUpper.updateLeftNeighborsOld(current);
        trapezoid(leftNeighbor).updateLRNeighbor(idUpper);
        /*Merge isn't finished yet*/
        if (isPointAbove){
            addElegibleForMerge(idLower);
            indirectUpdateNeighbors(current, currentId, false, idUpper);
            /*Merge is concluded*/
        }else{
            addElegibleForMerge(idUpper);
            indirectUpdateNeighbors(current, currentId, false, merged);
        }
        indirectUpdateNeighbors(current, currentId, true, idUpper);
    }else{
        addElegibleForMerge(idUpper);
        idUpper = PerformeMerge();
        size_t leftNeighbor = lastTrapezoidsInserted.size() == 3 ? lastTrapezoidsInserted[2]:leftNeighbor = lastTrapezoidsInserted[1];

        referenceLower.updateLeftNeighborsOld(current);
        trapezoid(leftNeighbor).updateURNeighbor(idLower);
        if (isPointAbove){
            addElegibleForMerge(idLower);
            indirectUpdateNeighbors(current,currentId, false, merged);
            /*Merge is concluded*/
        }else{
            addElegibleForMerge(idUpper);
            indirectUpdateNeighbors(current, currentId, false, idLower);
        }
        indirectUpdateNeighbors(current, currentId, true, idLower);
    }

    lastTrapezoidsInserted.clear();
    lastTrapezoidsInserted.push_back(idUpper);
    lastTrapezoidsInserted.push_back(idLower);
}
/**
 * @brief Handle the case when the p1 is outside and the q1 is inside the current trapezoid.
 * Split the trapezoid in three other trapezoids, push their id in the lastTrapezoidInserted vector. Based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Uses the lastTrapezoidsIntersected
 * of the previous step for update the adjacency, for the next step. Then push the new created trapezoids in the lastTrapezoidsInserted.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void TrapezoidalMap::HandleCaseQ1Inside(const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = trapezoidcpy(currentId);

    lastTrapezoidsInserted.clear();
    SplitInThree(currentId, segment, segment.p2(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idRight = lastTrapezoidsInserted[0];
    size_t idLeftUpper = lastTrapezoidsInserted[1];
    size_t idLeftLower = lastTrapezoidsInserted[2];

    Trapezoid& referenceRight = trapezoid(idRight);
    Trapezoid& referenceLeftU = trapezoid(idLeftUpper);
    Trapezoid& referenceLeftL = trapezoid(idLeftLower);

    /*Set neighbors internally for tSplitResult*/
    referenceRight.updateRightNeighborsOld(current);
    referenceRight.updateLeftNeighbors(idLeftUpper, idLeftLower);
    referenceLeftU.updateRightNeighbors(idRight);
    referenceLeftL.updateRightNeighbors(idRight);
    referenceLeftL.updateLeftNeighborsOld(current);
    referenceLeftU.updateLeftNeighborsOld(current);
    indirectUpdateNeighbors(current, currentId, false, idRight);

    Trapezoid& mergeCandidate = trapezoid(getIdElegibleForMerge());
    if(canTheyMerge(mergeCandidate, referenceLeftL)){
        addElegibleForMerge(idLeftLower);
        size_t mergeResult = PerformeMerge();
        lastTrapezoidsInserted[2] = mergeResult;
        referenceRight.updateLLNeighbor(mergeResult);
        indirectUpdateNeighbors(current, currentId, true, idLeftUpper);
    }else{
        addElegibleForMerge(idLeftUpper);
        size_t mergeResult = PerformeMerge();
        lastTrapezoidsInserted[1] = mergeResult;
        referenceRight.updateULNeighbor(mergeResult);
        indirectUpdateNeighbors(current, currentId, true, idLeftLower);

    }
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

void TrapezoidalMap::clear(){
    _trapezoids.clear();
    _segments.clear();
    _points.clear();
    _isDeleted.clear();
    _elegibleForMerge.clear();
}














