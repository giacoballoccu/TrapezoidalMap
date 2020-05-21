#include "trapezoidalmap.h"
#include "QRandomGenerator"
#define BOUNDINGBOX 1e+6

TrapezoidalMap::TrapezoidalMap(){
    _trapezoids = std::vector<Trapezoid>();
    _trapezoids.push_back(Trapezoid());
    _trapezoids[0].setNode(0);
    _isDeleted = std::vector<bool>();
    _isDeleted.push_back(false);
    _segments = std::vector<cg3::Segment2d>();
    _points = std::vector<cg3::Point2d>();
};
const std::vector<Trapezoid> TrapezoidalMap::trapezoids() const{
    return _trapezoids;
};
const std::vector<cg3::Segment2d> TrapezoidalMap::segments() const{
    return _segments;
};
const std::vector<cg3::Point2d> TrapezoidalMap::points() const{
    return _points;
};
const std::vector<bool> TrapezoidalMap::isDeleted() const{
    return _isDeleted;
};

std::vector<cg3::Point2d>& TrapezoidalMap::getPointsRfr(){
    return _points;
};
std::vector<cg3::Segment2d>& TrapezoidalMap::getSegmentsRfr(){
    return _segments;
};
std::vector<Trapezoid>& TrapezoidalMap::getTrapezoidsRfr(){
    return _trapezoids;
};

inline size_t TrapezoidalMap::addTrapezoid(const Trapezoid& t){
    _trapezoids.push_back(t);
    _isDeleted.push_back(false);
    return _trapezoids.size()-1;
};
size_t TrapezoidalMap::replace(const size_t &oldId, const Trapezoid &newT){
    _trapezoids[oldId] = newT;
    return oldId;
};
size_t TrapezoidalMap::addSegment(const cg3::Segment2d& s){
    _segments.push_back(s);
    return _segments.size()-1;
};
size_t TrapezoidalMap::addPoint(const cg3::Point2d& p){
    _points.push_back(p);
    return _points.size()-1;
};

const cg3::Segment2d& TrapezoidalMap::getLastSegment() const{
    return segment(_segments.size()-1);
}
const cg3::Point2d& TrapezoidalMap::getP1LastSegment() const{
    return point(_points.size()-2);
};
const cg3::Point2d& TrapezoidalMap::getQ1LastSegment() const{
    return point(_points.size()-1);
};



size_t TrapezoidalMap::getIdLastSegment() const{
    return _segments.size()-1;
};
size_t TrapezoidalMap::getIdP1LastSegment() const{
    return _points.size() - 2;
};
size_t TrapezoidalMap::getIdQ1LastSegment() const{
    return _points.size() - 1;
};

void TrapezoidalMap::setTrapezoids(std::vector<Trapezoid> trapezoids){
    _trapezoids = trapezoids;
};
void TrapezoidalMap::setIsDeleted(std::vector<bool> isDeleted){
    _isDeleted = isDeleted;
}

inline Trapezoid& TrapezoidalMap::trapezoid(const size_t& id){
    if(!_isDeleted[id]){
        return _trapezoids[id];
    }
    assert(_isDeleted[id] == false);

}

cg3::Point2d& TrapezoidalMap::point(const size_t& id){
    return _points[id];
};
cg3::Segment2d& TrapezoidalMap::segment(const size_t& id){
    return _segments[id];
};


Trapezoid TrapezoidalMap::trapezoidcpy(const size_t& id) const{
    if(!_isDeleted[id]){
        return _trapezoids[id];
    }
    return _trapezoids[id];
}

const cg3::Point2d& TrapezoidalMap::point(const size_t& id) const{
    return _points[id];
};
const cg3::Segment2d& TrapezoidalMap::segment(const size_t& id) const{
    return _segments[id];
};

inline void TrapezoidalMap::removeTrapezoid(const size_t& id){
    _isDeleted[id] = true;
};



std::vector<size_t> TrapezoidalMap::HandleCaseSegmentInside(const size_t& currentId){
    Trapezoid current = trapezoidcpy(currentId);
    cg3::Segment2d s = getLastSegment();

    std::vector<size_t> idsTrapezoid = SplitInFour(currentId, s);

    size_t idA = idsTrapezoid[0];
    size_t idB = idsTrapezoid[1];
    size_t idC = idsTrapezoid[2];
    size_t idD = idsTrapezoid[3];

    Trapezoid& referenceA = trapezoid(idA);
    Trapezoid& referenceB = trapezoid(idB);
    Trapezoid& referenceC = trapezoid(idC);
    Trapezoid& referenceD = trapezoid(idD);

    /*if(geoutils::isPointAbove(s.p1(), current.top()) == -1){
        referenceA.updateRightNeighbors(idC);
        referenceB.updateRightNeighbors(idD);
        referenceC.updateLeftNeighbors(idA);
        referenceC.updateRightNeighbors(idD);
        referenceD.updateLeftNeighbors(idB, idC);
    }else if (geoutils::isPointAbove(s.p2(), current.top()) == -1){
        referenceA.updateRightNeighbors(idB, idC);
        referenceB.updateLeftNeighbors(idA);
        referenceC.updateLeftNeighbors(idA);
        referenceC.updateRightNeighbors(idD);
        referenceD.updateLeftNeighbors(idC);
    }else if(geoutils::isPointAbove(s.p1(), current.bottom()) == -1){
        referenceA.updateRightNeighbors(idB);
        referenceB.updateRightNeighbors(idD);
        referenceB.updateLeftNeighbors(idA);
        referenceC.updateRightNeighbors(idD);
        referenceD.updateLeftNeighbors(idB, idC);
    }else if(geoutils::isPointAbove(s.p2(), current.bottom()) == -1){
        referenceA.updateRightNeighbors(idB, idC);
        referenceB.updateLeftNeighbors(idA);
        referenceB.updateRightNeighbors(idD);
        referenceC.updateLeftNeighbors(idA);
        referenceD.updateLeftNeighbors(idB);
    }else{*/
    referenceA.updateNeighborsRight(current, idB, idC);
    referenceB.updateNeighbors(idA, idD);
    referenceC.updateNeighbors(idA, idD);
    referenceD.updateNeighborsLeft(current, idB, idC);
    indirectUpdateNeighbors(current, currentId, true, idA);
    indirectUpdateNeighbors(current, currentId, false, idD);


    return idsTrapezoid;
}
std::vector<size_t> TrapezoidalMap::HandleCaseSegmentInsideDegenerativeRight(const size_t &currentId){
    Trapezoid current = trapezoidcpy(currentId);
    cg3::Segment2d segment = getLastSegment();

    std::vector<size_t> idsTrapezoid = SplitInThree(currentId, segment, segment.p1());

    /*Add new trapezoid on the map*/
    size_t idLeft = idsTrapezoid[0];
    size_t idRightUpper = idsTrapezoid[1];
    size_t idRightLower = idsTrapezoid[2];

    Trapezoid& referenceLeft = trapezoid(idLeft);
    Trapezoid& referenceRightU = trapezoid(idRightUpper);
    Trapezoid& referenceRightL = trapezoid(idRightLower);

    /*Set neighbors internally for firstTSplit*/
    referenceLeft.updateLeftNeighborsOld(current);
    referenceLeft.updateRightNeighbors(idRightUpper, idRightLower);
    referenceRightU.updateLeftNeighbors(idLeft);
    referenceRightL.updateLeftNeighbors(idLeft);
    indirectUpdateNeighbors(current, currentId, true, idLeft);
    if (geoutils::arePointsEqual(segment.p2(), current.top().p2()) and (geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) == false)){
        indirectUpdateNeighbors(current, currentId, false, idRightLower);
        referenceRightL.updateRightNeighborsOld(current);
    }else if(geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) and (geoutils::arePointsEqual(segment.p2(), current.top().p2()) == false)){
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
    return idsTrapezoid;

}
std::vector<size_t> TrapezoidalMap::HandleCaseSegmentInsideDegenerativeLeft(const size_t &currentId){
    Trapezoid current = trapezoidcpy(currentId);
    cg3::Segment2d segment= getLastSegment();

    std::vector<size_t> idsTrapezoid = SplitInThree(currentId, segment, segment.p2());

    /*Add new trapezoid on the map*/
    size_t idRight = idsTrapezoid[0];
    size_t idLeftUpper = idsTrapezoid[1];
    size_t idLeftLower = idsTrapezoid[2];

    Trapezoid& referenceRight = trapezoid(idRight);
    Trapezoid& referenceLeftU = trapezoid(idLeftUpper);
    Trapezoid& referenceLeftL = trapezoid(idLeftLower);

    /*Set neighbors internally for tSplitResult*/
    referenceRight.updateRightNeighborsOld(current);
    referenceRight.updateLeftNeighbors(idLeftUpper, idLeftLower);
    referenceLeftU.updateRightNeighbors(idRight);
    referenceLeftL.updateRightNeighbors(idRight);
    indirectUpdateNeighbors(current, currentId, false, idRight);

    if(geoutils::arePointsEqual(segment.p1(),current.top().p1()) and (geoutils::arePointsEqual(segment.p1(), current.bottom().p1()) == false)){
        indirectUpdateNeighbors(current, currentId, true, idLeftLower);
        referenceLeftL.updateLeftNeighborsOld(current);
    }else if(geoutils::arePointsEqual(segment.p1(), current.bottom().p1()) and (geoutils::arePointsEqual(segment.p1(),current.top().p1()) == false)){
        indirectUpdateNeighbors(current, currentId, true, idLeftUpper);
        referenceLeftU.updateLeftNeighborsOld(current);
    }else{ //check
        referenceLeftL.updateLeftNeighbors(current.lowerLeftNeighbor());
        referenceLeftU.updateLeftNeighbors(current.upperLeftNeighbor());
        if(current.lowerLeftNeighbor() != SIZE_MAX){
            trapezoid(current.lowerLeftNeighbor()).updateRightNeighbors(idLeftLower);
        }
        if(current.upperLeftNeighbor() != SIZE_MAX){
            trapezoid(current.upperLeftNeighbor()).updateRightNeighbors(idLeftUpper);
        }
    }
    return idsTrapezoid;
}
/*

  SplitVerticaly handle the case when ONLY the first end point is inside the trapezoid, it split the trapezoid in a half using the
  point as bound and then split the right trapezoid obtained by the split in half HORIZONTALY.
  This method adds 3 trapezoid in splitResult ordered from left to right and from top to bottom.
  INPUT: id of the trapezoid that need to be split
         splitResult the vector where the result of the split will be pushed
         segmentsegment

*/
std::vector<size_t> TrapezoidalMap::HandleCaseP1InsideDegenerative(const size_t &currentId, std::vector<size_t> &elegibleForMerge){
    cg3::Segment2d segment = getLastSegment();
    Trapezoid current = trapezoidcpy(currentId);
    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    cg3::Point2d q1 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(segment, current.rightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), q1);

    std::vector<Trapezoid> hSplit = SplitHorizontaly(current, innerSegment);

    idsTrapezoid.push_back(this->replace(currentId, hSplit[0]));
    idsTrapezoid.push_back(this->addTrapezoid(hSplit[1]));

    size_t idUpper = idsTrapezoid[0];
    size_t idLower = idsTrapezoid[1];
    Trapezoid& referenceUpper = trapezoid(idUpper);
    Trapezoid& referenceLower = trapezoid(idLower);

    if (geoutils::isPointAbove(current.rightp(), segment)){
        elegibleForMerge.push_back(idLower);
        indirectUpdateNeighbors(current, currentId, false, idUpper);
        referenceUpper.updateRightNeighborsOld(current);

    }else{
        elegibleForMerge.push_back(idUpper);
        indirectUpdateNeighbors(current, currentId, false, idLower);
        referenceLower.updateRightNeighborsOld(current);
    }

    if(geoutils::arePointsEqual(segment.p1(), current.top().p1()) and (geoutils::arePointsEqual(segment.p1(), current.bottom().p1()) ==false)){
        referenceLower.updateLeftNeighborsOld(current);
        indirectUpdateNeighbors(current, currentId, true, idLower);
    }else if(geoutils::arePointsEqual(segment.p1(), current.bottom().p1()) and (geoutils::arePointsEqual(segment.p1(), current.top().p1()) == false)){
        referenceUpper.updateLeftNeighborsOld(current);
        indirectUpdateNeighbors(current, currentId, true, idUpper);
    }else if(geoutils::arePointsEqual(segment.p1(), current.top().p1()) and geoutils::arePointsEqual(segment.p1(), current.bottom().p1())){
       float debug = 1;
   }else{
        referenceUpper.updateLeftNeighbors(current.upperLeftNeighbor());
        referenceLower.updateLeftNeighbors(current.lowerLeftNeighbor());
        if (geoutils::isPointAbove(current.rightp(), segment)){
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

    return idsTrapezoid;
}

std::vector<size_t> TrapezoidalMap::HandleCaseQ1InsideDegenerative(const size_t &currentId, std::vector<size_t> &elegibleForMerge){
    cg3::Segment2d segment = getLastSegment();
    Trapezoid current = trapezoidcpy(currentId);
    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    cg3::Point2d p1 = cg3::Point2d(current.leftp().x(), geoutils::calculateYCoord(segment, current.leftp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, segment.p2());

    std::vector<Trapezoid> hSplit = SplitHorizontaly(current, innerSegment);

    idsTrapezoid.push_back(this->replace(currentId,hSplit[0]));
    idsTrapezoid.push_back(this->addTrapezoid(hSplit[1]));
    //removeTrapezoid(currentId);
    size_t idUpper = idsTrapezoid[0];
    size_t idLower = idsTrapezoid[1];
    Trapezoid& referenceUpper = trapezoid(idUpper);
    Trapezoid& referenceLower = trapezoid(idLower);

    Trapezoid mergeCandidate = trapezoid(elegibleForMerge[0]);
    size_t notMerged;
    size_t merged;
    if(canTheyMerge(mergeCandidate, referenceLower)){
        referenceUpper.updateLeftNeighborsOld(current);
        elegibleForMerge.push_back(idLower);
        idsTrapezoid[1] = PerformeMerge(elegibleForMerge);
        indirectUpdateNeighbors(current, currentId, true, idUpper); //???  not always

        if (geoutils::arePointsEqual(segment.p2(), current.top().p2()) and (geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) == false)){
            trapezoid(idsTrapezoid[0]).updateRightNeighbors(SIZE_MAX);
            trapezoid(idsTrapezoid[1]).updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, idsTrapezoid[1]);
        }else if(geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) and (geoutils::arePointsEqual(segment.p2(), current.top().p2()) == false)){
            trapezoid(idsTrapezoid[1]).updateRightNeighbors(SIZE_MAX);
            trapezoid(idsTrapezoid[0]).updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, idsTrapezoid[0]);
        }else if(geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) and (geoutils::arePointsEqual(segment.p2(), current.top().p2()))){
             trapezoid(idsTrapezoid[1]).updateRightNeighbors(SIZE_MAX);
             trapezoid(idsTrapezoid[0]).updateRightNeighbors(SIZE_MAX);
             if(current.upperLeftNeighbor() == current.lowerLeftNeighbor()){ //assert
                 trapezoid(idsTrapezoid[0]).updateLeftNeighbors(current.upperLeftNeighbor());
             }else{
                 trapezoid(idsTrapezoid[0]).updateLeftNeighborsOld(current);
             }
        }else{
            referenceUpper.updateLeftNeighborsOld(current);
            referenceUpper.updateRightNeighbors(current.upperRightNeighbor());
            trapezoid(idsTrapezoid[1]).updateRightNeighbors(current.lowerRightNeighbor());
            indirectUpdateNeighbors(current, currentId, true, idUpper);
            trapezoid(current.upperRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idUpper);
            trapezoid(current.lowerRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idsTrapezoid[1]);
        }
    }else if (canTheyMerge(mergeCandidate, referenceUpper)){
        referenceLower.updateLeftNeighborsOld(current);
        elegibleForMerge.push_back(idUpper);
        idsTrapezoid[0] = PerformeMerge(elegibleForMerge);
        notMerged = idLower;
        merged = idsTrapezoid[0];
        indirectUpdateNeighbors(current, currentId, true, idLower);
        if (geoutils::arePointsEqual(segment.p2(), current.top().p2()) and (geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) == false)){
            trapezoid(idsTrapezoid[0]).updateRightNeighbors(SIZE_MAX);
            trapezoid(idsTrapezoid[1]).updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, idsTrapezoid[1]);
        }else if(geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) and (geoutils::arePointsEqual(segment.p2(), current.top().p2()) == false)){
            trapezoid(idsTrapezoid[1]).updateRightNeighbors(SIZE_MAX);
            trapezoid(idsTrapezoid[0]).updateRightNeighborsOld(current);
            indirectUpdateNeighbors(current, currentId, false, idsTrapezoid[0]);
        }else if(geoutils::arePointsEqual(segment.p2(), current.bottom().p2()) and (geoutils::arePointsEqual(segment.p2(), current.top().p2()))){
             trapezoid(idsTrapezoid[1]).updateRightNeighbors(SIZE_MAX);
             trapezoid(idsTrapezoid[0]).updateRightNeighbors(SIZE_MAX);
             if(current.upperLeftNeighbor() == current.lowerLeftNeighbor()){
                 trapezoid(idsTrapezoid[1]).updateLeftNeighbors(current.lowerLeftNeighbor());
             }else{
                 trapezoid(idsTrapezoid[1]).updateLeftNeighborsOld(current);
             }
        }else{
            referenceLower.updateLeftNeighborsOld(current);
            referenceLower.updateRightNeighbors(current.lowerRightNeighbor());
            trapezoid(idsTrapezoid[0]).updateRightNeighbors(current.upperRightNeighbor());
            indirectUpdateNeighbors(current, currentId, true, idLower);
            trapezoid(current.upperRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idsTrapezoid[0]);
            trapezoid(current.lowerRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idLower);
        }
    }else{
        float debug =1;
    }


    //if(canTheyMerge(mergeCandidate, referenceLower)){
    //    trapezoid(idLower).updateRightNeighborsOld(current);
    //    trapezoid(idUpper).updateRightNeighborsOld(current);//foglio23
    //    elegibleForMerge.push_back(idLower);
    //    idsTrapezoid[1] = PerformeMerge(elegibleForMerge);
    //    indirectUpdateNeighbors(current, currentId, true, idUpper);
    //    indirectUpdateNeighbors(current, currentId, false, idsTrapezoid[1]);
    //    referenceUpper.updateLeftNeighbors(current.upperLeftNeighbor());
    //}else{
    //    trapezoid(idUpper).updateRightNeighborsOld(current);
    //    trapezoid(idLower).updateRightNeighborsOld(current);
    //    elegibleForMerge.push_back(idUpper);
    //    idsTrapezoid[0] = PerformeMerge(elegibleForMerge);
    //    indirectUpdateNeighbors(current, currentId, true, idLower);
    //    referenceLower.updateLeftNeighbors(current.lowerLeftNeighbor());
    //    indirectUpdateNeighbors(current, currentId, false, idsTrapezoid[0]);
    //}


    return idsTrapezoid;
}

std::vector<size_t> TrapezoidalMap::HandleCaseP1Inside(const size_t& currentId, std::vector<size_t>& elegibleForMerge){
    Trapezoid current = trapezoidcpy(currentId);
    cg3::Segment2d segment = getLastSegment();

    std::vector<size_t> idsTrapezoid = SplitInThree(currentId, segment, segment.p1());

    /*Add new trapezoid on the map*/
    size_t idLeft = idsTrapezoid[0];
    size_t idRightUpper = idsTrapezoid[1];
    size_t idRightLower = idsTrapezoid[2];

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

    if (geoutils::isPointAbove(current.rightp(), segment)){
        elegibleForMerge.push_back(idRightLower);
        indirectUpdateNeighbors(current, currentId, false, idRightUpper);
    /*Merge upperr*/
    }else{
        elegibleForMerge.push_back(idRightUpper);
        indirectUpdateNeighbors(current, currentId, false, idRightLower);
    }

    return idsTrapezoid;
}

std::vector<size_t> TrapezoidalMap::HandleCasePointsOutside(const size_t& currentId,  std::vector<size_t>& elegibleForMerge, std::vector<size_t>& lastTrapezoidsInserted){
    cg3::Segment2d segment = getLastSegment();
    Trapezoid current = trapezoidcpy(currentId);
    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    cg3::Point2d p1 = cg3::Point2d(current.leftp().x(), geoutils::calculateYCoord(segment, current.leftp().x()));
    cg3::Point2d q1 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(segment, current.rightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, q1);
    std::vector<Trapezoid> hSplit = SplitHorizontaly(current, innerSegment);
    Trapezoid upper = hSplit[0];
    Trapezoid lower = hSplit[1];

    size_t idUpper = this->replace(currentId, upper);
    size_t idLower = this->addTrapezoid(lower);
    //removeTrapezoid(currentId);

    idsTrapezoid.push_back(idUpper);
    idsTrapezoid.push_back(idLower);

    Trapezoid& referenceUpper = trapezoid(idUpper);
    Trapezoid& referenceLower = trapezoid(idLower);

    /*Merge handling*/
    referenceUpper.updateRightNeighborsOld(current);
    referenceLower.updateRightNeighborsOld(current);
    Trapezoid& mergeCandidate = trapezoid(elegibleForMerge[0]);
    size_t merged = elegibleForMerge[0];
    /*Merge with lower*/
    if(canTheyMerge(mergeCandidate, referenceLower)){
        elegibleForMerge.push_back(idLower);
        idsTrapezoid[1] = PerformeMerge(elegibleForMerge);
        size_t leftNeighbor;
        if(lastTrapezoidsInserted.size() == 3){
             leftNeighbor = lastTrapezoidsInserted[1];
        }else{
             leftNeighbor = lastTrapezoidsInserted[0];
        }
        referenceUpper.updateLeftNeighborsOld(current);
        trapezoid(leftNeighbor).updateLRNeighbor(idUpper);
        /*Merge isn't finished yet*/
        if (geoutils::isPointAbove(current.rightp(), segment)){
            elegibleForMerge.push_back(idsTrapezoid[1]);
            indirectUpdateNeighbors(current, currentId, false, idUpper);
        /*Merge is concluded*/
        }else{
            elegibleForMerge.push_back(idsTrapezoid[0]);
            indirectUpdateNeighbors(current, currentId, false, merged);
        }
        indirectUpdateNeighbors(current, currentId, true, idUpper);
    }else if(canTheyMerge(mergeCandidate, referenceUpper)){
        elegibleForMerge.push_back(idUpper);
        idsTrapezoid[0] = PerformeMerge(elegibleForMerge);
        size_t leftNeighbor;
        if(lastTrapezoidsInserted.size() == 3){
             leftNeighbor = lastTrapezoidsInserted[2];
        }else{
             leftNeighbor = lastTrapezoidsInserted[1];
        }
        //referenceLower.updateLeftNeighbors(leftNeighbor); 17 maggio
        referenceLower.updateLeftNeighborsOld(current);
        trapezoid(leftNeighbor).updateURNeighbor(idLower);
        if (geoutils::isPointAbove(current.rightp(), segment)){
            elegibleForMerge.push_back(idsTrapezoid[1]);
            indirectUpdateNeighbors(current,currentId, false, merged);
        /*Merge is concluded*/
        }else{
            elegibleForMerge.push_back(idsTrapezoid[0]);
            indirectUpdateNeighbors(current, currentId, false, idLower);
        }
        indirectUpdateNeighbors(current, currentId, true, idLower);
    }else{
        float debug =1;
    }
    //}else{ //handle duplicate segment
    //    if ((segment.p1() == current.leftp() and segment.p2() >= current.rightp()) ){
    //        indirectUpdateNeighbors(currentId, true, idUpper);
    //        referenceUpper.updateRightNeighborsOld(current);
    //        indirectUpdateNeighbors(currentId, false, idLower);
    //    }else if(segment.p1() == current.leftp() and segment.p2() < current.rightp()) {
    //        indirectUpdateNeighbors(currentId, true, idLower);
    //        referenceLower.updateLeftNeighborsOld(current);
    //        indirectUpdateNeighbors(currentId, false, idUpper);
    //    }else if(segment.p1() > current.leftp() and segment.p2() == current.rightp()){
    //        indirectUpdateNeighbors(currentId, true, idLower);
    //        referenceUpper.updateRightNeighborsOld(current);
    //        indirectUpdateNeighbors(currentId, false, idLower);
    //    }else if(segment.p1() <= current.leftp() and segment.p2() == current.rightp()){
    //        indirectUpdateNeighbors(currentId, true, idUpper);
    //        referenceLower.updateRightNeighborsOld(current);
    //        indirectUpdateNeighbors(currentId, false, idLower);
    //    }
    //
    //
    //    //trapezoid(idLower).updateLeftNeighbors(idUpper, 3)
    //}

    return idsTrapezoid;



}

std::vector<size_t> TrapezoidalMap::HandleCaseQ1Inside(const size_t& currentId, std::vector<size_t>& elegibleForMerge){
    Trapezoid current = trapezoidcpy(currentId);
    cg3::Segment2d segment= getLastSegment();

    std::vector<size_t> idsTrapezoid = SplitInThree(currentId, segment, segment.p2());

    /*Add new trapezoid on the map*/
    size_t idRight = idsTrapezoid[0];
    size_t idLeftUpper = idsTrapezoid[1];
    size_t idLeftLower = idsTrapezoid[2];

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

    Trapezoid& mergeCandidate = trapezoid(elegibleForMerge[0]);
    if(canTheyMerge(mergeCandidate, referenceLeftL)){
        //if (geoutils::isPointAbove(current.rightp(), segment)){
            elegibleForMerge.push_back(idLeftLower);
            size_t mergeResult = PerformeMerge(elegibleForMerge);
            idsTrapezoid[2] = mergeResult;
            referenceRight.updateLLNeighbor(mergeResult);
            indirectUpdateNeighbors(current, currentId, true, idLeftUpper);


    }else if(canTheyMerge(mergeCandidate, referenceLeftU)){
        //if (geoutils::isPointAbove(current.rightp(), segment)){
            elegibleForMerge.push_back(idLeftUpper);
            size_t mergeResult = PerformeMerge(elegibleForMerge);
            idsTrapezoid[1] = mergeResult;
            referenceRight.updateULNeighbor(mergeResult);
            indirectUpdateNeighbors(current, currentId, true, idLeftLower);

    }else{
        float debug =1;
    }
   /* }else{
        referenceLeftU.updateLeftNeighbors(current.upperLeftNeighbor());
        trapezoid(current.upperLeftNeighbor()).updateRightNeighbors(idLeftUpper);
        referenceLeftL.updateLeftNeighbors(current.lowerLeftNeighbor());
        trapezoid(current.lowerLeftNeighbor()).updateRightNeighbors(idLeftLower);

    }*/


    return idsTrapezoid;
}



void TrapezoidalMap::indirectUpdateNeighbors(const Trapezoid& current, const size_t& currentId, bool left, const size_t& idNewT){
    if(left == true){
        if(current.lowerLeftNeighbor() != SIZE_MAX){
            size_t lowerLeft = current.lowerLeftNeighbor();
            if(trapezoid(lowerLeft).lowerRightNeighbor() == currentId){
                trapezoid(lowerLeft).setLowerRightNeighbor(idNewT);
            }
            if(trapezoid(lowerLeft).upperRightNeighbor() == currentId){
                trapezoid(lowerLeft).setUpperRightNeighbor(idNewT);
            }
        }

        if(current.upperLeftNeighbor() != SIZE_MAX){
            size_t upperLeft = current.upperLeftNeighbor();
            if(trapezoid(upperLeft).lowerRightNeighbor() == currentId){
                trapezoid(upperLeft).setLowerRightNeighbor(idNewT);
            }
            if(trapezoid(upperLeft).upperRightNeighbor() == currentId){
               trapezoid(upperLeft).setUpperRightNeighbor(idNewT);
            }
         }
    }else{
        if(current.lowerRightNeighbor() != SIZE_MAX){
            size_t lowerRight = current.lowerRightNeighbor();
            if(trapezoid(lowerRight).lowerLeftNeighbor() == currentId){
                trapezoid(lowerRight).setLowerLeftNeighbor(idNewT);
            }
            if(trapezoid(lowerRight).upperLeftNeighbor() == currentId){
                trapezoid(lowerRight).setUpperLeftNeighbor(idNewT);
            }
        }

        if(current.upperRightNeighbor() != SIZE_MAX){
            size_t upperRight = current.upperRightNeighbor();
            if(trapezoid(upperRight).lowerLeftNeighbor() == currentId){
                trapezoid(upperRight).setLowerLeftNeighbor(idNewT);
            }
            if(trapezoid(upperRight).upperLeftNeighbor() == currentId){
               trapezoid(upperRight).setUpperLeftNeighbor(idNewT);
            }
         }
    }

}
bool TrapezoidalMap::canTheyMerge(const Trapezoid& t1, const Trapezoid& t2){
    if(geoutils::arePointsEqual(t1.top().p2(), t2.top().p1()) and geoutils::arePointsEqual(t1.bottom().p2(), t2.bottom().p1())){
        return true;
    }else{
        return false;
    }
}

size_t TrapezoidalMap::PerformeMerge(std::vector<size_t>& elegibleForMerge){
    size_t last = elegibleForMerge.size() -1;
    size_t mergedId = elegibleForMerge[0];
    Trapezoid& leftMost = trapezoid(elegibleForMerge[0]);
    Trapezoid& rightMost = trapezoid(elegibleForMerge[last]);
    leftMost.setTopP2(rightMost.top().p2());
    leftMost.setBottomP2(rightMost.bottom().p2());
    leftMost.setRightp(rightMost.rightp());

    leftMost.updateRightNeighborsOld(rightMost);

    std::vector<size_t>::iterator it;
    for(it = elegibleForMerge.begin() + 1; it != elegibleForMerge.end(); ++it){
        removeTrapezoid((*it));
    }
    elegibleForMerge.clear();

    return mergedId;
}


/**
 * Given a Trapezoid& already in the tMap and the segment that intersect it returns four trapezoid generated from the segment
 * @param[in] current RValue to the Trapezoid we want to split
 * @param[in] s Segment internal to the Trapezoid
 * @return Vector with the four trapezoid obtained by the split.
 */
std::vector<size_t> TrapezoidalMap::SplitInFour(const size_t& currentId, const cg3::Segment2d s){
    std::vector<size_t> idsTrapezoid = std::vector<size_t>();
    Trapezoid current = trapezoidcpy(currentId);

    Trapezoid A = Trapezoid();
    Trapezoid B = Trapezoid();
    Trapezoid C = Trapezoid();
    Trapezoid D = Trapezoid();

    cg3::Point2d AtopP2 = cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.top(), s.p1().x()));
    A.setTopP2(AtopP2);
    A.setTopP1(current.top().p1());
    cg3::Point2d AbotP2 = cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.bottom(), s.p1().x()));
    A.setBottomP2(AbotP2);
    A.setBottomP1(current.bottom().p1());
    A.setRightp(s.p1());
    A.setLeftp(current.leftp());

    B.setTopP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.top(), s.p1().x())));
    B.setTopP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.top(), s.p2().x())));
    B.setBottom(s);
    B.setLeftp(s.p1());
    B.setRightp(s.p2());

    C.setTop(s);
    C.setBottomP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.bottom(), s.p1().x())));
    C.setBottomP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.bottom(), s.p2().x())));
    C.setLeftp(s.p1());
    C.setRightp(s.p2());

    D.setTopP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.top(), s.p2().x())));
    D.setTopP2(current.top().p2());
    D.setBottomP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.bottom(), s.p2().x())));
    D.setBottomP2(current.bottom().p2());
    D.setLeftp(s.p2());
    D.setRightp(current.rightp());

    idsTrapezoid.push_back(this->replace(currentId, A));
    idsTrapezoid.push_back(this->addTrapezoid(B));
    idsTrapezoid.push_back(this->addTrapezoid(C));
    idsTrapezoid.push_back(this->addTrapezoid(D));

    return idsTrapezoid;
}
std::vector<size_t> TrapezoidalMap::SplitInThree(const size_t& currentId, const cg3::Segment2d segment, const cg3::Point2d splitPoint){
    std::vector<size_t> idsTrapezoid = std::vector<size_t>();
    Trapezoid current = trapezoidcpy(currentId);

    std::vector<Trapezoid> vSplitResult = SplitVerticaly(current, splitPoint);
    Trapezoid leftMost = vSplitResult[0];
    Trapezoid rightMost = vSplitResult[1];

    if(splitPoint == segment.p1()){
        cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), cg3::Point2d(rightMost.rightp().x(), geoutils::calculateYCoord(segment, rightMost.rightp().x())));
        std::vector<Trapezoid> hSplitResult = SplitHorizontaly(rightMost, innerSegment);

        Trapezoid rightUpper = hSplitResult[0];
        Trapezoid rightLower = hSplitResult[1];

        idsTrapezoid.push_back(this->replace(currentId, leftMost));
        idsTrapezoid.push_back(this->addTrapezoid(rightUpper));
        idsTrapezoid.push_back(this->addTrapezoid(rightLower));
    }else{
        cg3::Segment2d innerSegment = cg3::Segment2d(cg3::Point2d(leftMost.leftp().x(), geoutils::calculateYCoord(segment, leftMost.leftp().x())),segment.p2());
        std::vector<Trapezoid> hSplitResult = SplitHorizontaly(leftMost, innerSegment);

        Trapezoid leftUpper = hSplitResult[0];
        Trapezoid leftLower = hSplitResult[1];
        idsTrapezoid.push_back(this->replace(currentId, rightMost));
        idsTrapezoid.push_back(this->addTrapezoid(leftUpper));
        idsTrapezoid.push_back(this->addTrapezoid(leftLower));
    }

    return idsTrapezoid;
}
std::vector<Trapezoid> TrapezoidalMap::SplitHorizontaly(const Trapezoid& intermediateT, const cg3::Segment2d innerSegment){
    std::vector<Trapezoid> hsplitResult = std::vector<Trapezoid>();

    Trapezoid upper = Trapezoid();
    upper.setTop(intermediateT.top());
    upper.setBottom(innerSegment);
    upper.setLeftp(intermediateT.leftp()); //mm
    upper.setRightp(intermediateT.rightp());

    Trapezoid lower = Trapezoid();
    lower.setTop(innerSegment);
    lower.setBottom(intermediateT.bottom());
    lower.setLeftp(intermediateT.leftp());
    lower.setRightp(intermediateT.rightp());

    hsplitResult.push_back(upper);
    hsplitResult.push_back(lower);
    return hsplitResult;

}
std::vector<Trapezoid> TrapezoidalMap::SplitVerticaly(const Trapezoid& current, const cg3::Point2d splitPoint){
    std::vector<Trapezoid> vSplitResult = std::vector<Trapezoid>();

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

    return vSplitResult;
}

void TrapezoidalMap::clear(){
    _trapezoids.clear();
    _segments.clear();
    _points.clear();
    _isDeleted.clear();
}














