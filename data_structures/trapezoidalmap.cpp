#include "trapezoidalmap.h"
#include "QRandomGenerator"
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

std::vector<cg3::Point2d>& TrapezoidalMap::getPointsRfr(){
    return _points;
};
std::vector<cg3::Segment2d>& TrapezoidalMap::getSegmentsRfr(){
    return _segments;
};
std::vector<Trapezoid>& TrapezoidalMap::getTrapezoidsRfr(){
    return _trapezoids;
};


std::vector<Trapezoid> TrapezoidalMap::getActiveTrapezoids() const{
    std::vector<Trapezoid> result = std::vector<Trapezoid>();
    for (size_t i=0; i < _trapezoids.size(); i++){
        if (!_isDeleted[i]){
            result.push_back(_trapezoids[i]);
        }
    }
    return result;
};

size_t TrapezoidalMap::addTrapezoid(const Trapezoid& t){
    _trapezoids.push_back(t);
    _isDeleted.push_back(false);
    return _trapezoids.size()-1;
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

Trapezoid& TrapezoidalMap::trapezoid(const size_t& id){
    if(!_isDeleted[id]){
        return _trapezoids[id];
    }
    throw __EXCEPTIONS;
}

cg3::Point2d& TrapezoidalMap::point(const size_t& id){
    return _points[id];
};
cg3::Segment2d& TrapezoidalMap::segment(const size_t& id){
    return _segments[id];
};


const Trapezoid& TrapezoidalMap::trapezoid(const size_t& id) const{
    if(!_isDeleted[id]){
        return _trapezoids[id];
    }
    throw __EXCEPTIONS;
}

const cg3::Point2d& TrapezoidalMap::point(const size_t& id) const{
    return _points[id];
};
const cg3::Segment2d& TrapezoidalMap::segment(const size_t& id) const{
    return _segments[id];
};

void TrapezoidalMap::removeTrapezoid(const size_t& id){
    _isDeleted[id] = true;
};



void TrapezoidalMap::HandleCaseSegmentInside(std::vector<size_t> trapsIntersected, std::vector<std::vector<size_t> > &newTrapezoidIds){
    Trapezoid& current = trapezoid(trapsIntersected[0]);
    cg3::Segment2d s = getLastSegment();

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    std::vector<Trapezoid> splitResult = SplitInFour(current, s);

    size_t idA = this->addTrapezoid(splitResult[0]);
    size_t idB = this->addTrapezoid(splitResult[1]);
    size_t idC = this->addTrapezoid(splitResult[2]);
    size_t idD = this->addTrapezoid(splitResult[3]);

    idsTrapezoid.push_back(idA);
    idsTrapezoid.push_back(idB);
    idsTrapezoid.push_back(idC);
    idsTrapezoid.push_back(idD);

    Trapezoid& referenceA = trapezoid(idA);
    Trapezoid& referenceB = trapezoid(idB);
    Trapezoid& referenceC = trapezoid(idC);
    Trapezoid& referenceD = trapezoid(idD);

    referenceA.updateNeighborsRight(current, idB, idC);
    if(current.lowerLeftNeighbor() != SIZE_MAX){
        trapezoid(current.lowerLeftNeighbor()).updateRightNeighbors(idA);
        trapezoid(current.upperLeftNeighbor()).updateRightNeighbors(idA);
    }

    referenceB.updateNeighbors(idA, idD);
    referenceC.updateNeighbors(idA, idD);
    referenceD.updateNeighborsLeft(current, idB, idC);
    if(current.lowerRightNeighbor() != SIZE_MAX){
        trapezoid(current.lowerRightNeighbor()).updateLeftNeighbors(idD);
        trapezoid(current.upperRightNeighbor()).updateLeftNeighbors(idD);
    }

    newTrapezoidIds.push_back(idsTrapezoid);
}

/*

  SplitVerticaly handle the case when ONLY the first end point is inside the trapezoid, it split the trapezoid in a half using the
  point as bound and then split the right trapezoid obtained by the split in half HORIZONTALY.
  This method adds 3 trapezoid in splitResult ordered from left to right and from top to bottom.
  INPUT: id of the trapezoid that need to be split
         splitResult the vector where the result of the split will be pushed
         segmentsegment

*/

/*
void TrapezoidalMap::HandleTwoTIntersected(std::vector<size_t>& trapsIntersected, std::vector<std::vector<size_t>> &newTrapezoidIds){
    cg3::Segment2d segment = getLastSegment();
    Trapezoid& firstTrapezoid = trapezoid(trapsIntersected[0]);
    Trapezoid& secondTrapezoid = trapezoid(trapsIntersected[1]);

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    std::vector<Trapezoid> firstTSplitResult = SplitInThree(firstTrapezoid, segment, segment.p1());
    std::vector<Trapezoid> secondTSplitResult = SplitInThree(secondTrapezoid, segment, segment.p2());



    size_t idLeft = this->addTrapezoid(firstTSplitResult[0]);
    size_t idRightUpper = this->addTrapezoid(firstTSplitResult[1]);
    size_t idRightLower = this->addTrapezoid(firstTSplitResult[2]);

    size_t idRight = this->addTrapezoid(secondTSplitResult[0]);
    size_t idLeftUpper = this->addTrapezoid(secondTSplitResult[1]);
    size_t idLeftLower = this->addTrapezoid(secondTSplitResult[2]);

    Trapezoid& referenceLeft = trapezoid(idLeft);
    Trapezoid& referenceRightU = trapezoid(idRightUpper);
    Trapezoid& referenceRightL = trapezoid(idRightLower);

    Trapezoid& referenceRight = trapezoid(idRight);
    Trapezoid& referenceLeftU = trapezoid(idLeftUpper);
    Trapezoid& referenceLeftL = trapezoid(idLeftLower);


    referenceLeft.updateLeftNeighborsOld(firstTrapezoid);
    firstTrapezoid.indirectUpdateLeftNeighbors(referenceLeft);
    referenceLeft.updateRightNeighbors(referenceRightU, referenceRightL);
    referenceRightU.updateLeftNeighbors(referenceLeft);
    referenceRightU.updateRightNeighborsOld(firstTrapezoid);
    referenceRightL.updateLeftNeighbors(referenceLeft);
    referenceRightL.updateRightNeighborsOld(firstTrapezoid);


    referenceRight.updateRightNeighborsOld(secondTrapezoid);
    secondTrapezoid.indirectUpdateRightNeighbors(referenceRight);
    referenceRight.updateLeftNeighbors(referenceLeftU, referenceLeftL);
    referenceLeftU.updateRightNeighbors(referenceRight);
    referenceLeftU.updateLeftNeighborsOld(secondTrapezoid);
    referenceLeftL.updateRightNeighbors(referenceRight);
    referenceLeftL.updateLeftNeighborsOld(secondTrapezoid);


    if (geoutils::isPointAbove(firstTrapezoid.getRightp(), segment)){
        PerformeMerge(referenceRightL, referenceLeftL);
        removeTrapezoid(idLeftLower);

        if(referenceRightU.lowerRightNeighbor != referenceLeftU.upperRightNeighbor){

            referenceLeftU.updateLRNeighbor(referenceRightU);
            referenceRightU.updateLeftNeighbors(referenceLeftU);
            size_t id = firstTrapezoid.upperRightNeighborRfr()->getId();
            trapezoid(id).updateLeftNeighbors(referenceRightL);
        }else{
            referenceLeftU.updateLeftNeighbors(referenceLeftU);
            referenceLeftU.updateLLNeighbor(referenceLeftU);
            size_t id = secondTrapezoid.upperLeftNeighborRfr()->getId();
            trapezoid(id).updateRightNeighbors(referenceRightL);
        }

        idsTrapezoid.push_back(idLeft);
        idsTrapezoid.push_back(idRightUpper);
        idsTrapezoid.push_back(idRightLower);
        newTrapezoidIds.push_back(idsTrapezoid);
        idsTrapezoid.clear();
        idsTrapezoid.push_back(idRight);
        idsTrapezoid.push_back(idLeftUpper);
        idsTrapezoid.push_back(idRightLower);
        newTrapezoidIds.push_back(idsTrapezoid);


    }else{
        PerformeMerge(referenceRightU, referenceLeftU);
        removeTrapezoid(idLeftUpper);

        if(referenceRightL.lowerRightNeighborRfr() != referenceRightL.upperRightNeighborRfr()){
            referenceRightL.updateURNeighbor(referenceLeftL);
            referenceLeftL.updateLeftNeighbors(referenceRightL);
            size_t id = referenceRightL.lowerRightNeighbor->getId();
            trapezoid(id).updateLeftNeighbors(referenceLeftL);
        }else{
            referenceRightL.updateRightNeighbors(referenceLeftL);
            referenceLeftL.updateULNeighbor(referenceRightL);
            size_t id = secondTrapezoid.lowerLeftNeighborRfr()->getId();
            trapezoid(id).updateRightNeighbors(referenceLeftL);
        }

        idsTrapezoid.push_back(idLeft);
        idsTrapezoid.push_back(idRightUpper);
        idsTrapezoid.push_back(idRightLower);
        newTrapezoidIds.push_back(idsTrapezoid);
        idsTrapezoid.clear();
        idsTrapezoid.push_back(idRight);
        idsTrapezoid.push_back(idRightUpper);
        idsTrapezoid.push_back(idLeftLower);
        newTrapezoidIds.push_back(idsTrapezoid);
    }

}
*/
void TrapezoidalMap::HandleCaseP1Inside(size_t& currentId, size_t& neighborId, std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    Trapezoid& current = trapezoid(currentId);
    cg3::Segment2d segment = getLastSegment();

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();
    std::vector<Trapezoid> tSplitResult = SplitInThree(current, segment, segment.p1());

    /*Add new trapezoid on the map*/
    size_t idLeft = this->addTrapezoid(tSplitResult[0]);
    size_t idRightUpper = this->addTrapezoid(tSplitResult[1]);
    size_t idRightLower = this->addTrapezoid(tSplitResult[2]);

    idsTrapezoid.push_back(idLeft);
    idsTrapezoid.push_back(idRightUpper);
    idsTrapezoid.push_back(idRightLower);

    Trapezoid& referenceLeft = trapezoid(idLeft);
    Trapezoid& referenceRightU = trapezoid(idRightUpper);
    Trapezoid& referenceRightL = trapezoid(idRightLower);

    /*Set neighbors internally for firstTSplit*/
    referenceLeft.updateLeftNeighborsOld(current);
    referenceLeft.updateRightNeighbors(idRightUpper, idRightLower);
    referenceRightU.updateLeftNeighbors(idLeft);
    referenceRightU.updateRightNeighborsOld(current);
    referenceRightL.updateLeftNeighbors(idLeft);
    referenceRightL.updateRightNeighborsOld(current);
    indirectUpdateNeighbors(currentId, true, idLeft);

    newTrapezoidIds.push_back(idsTrapezoid);
    /*Merge lower*/
    if (geoutils::isPointAbove(current.rightp(), segment)){
        elegibleForMerge.push_back(idRightLower);
        //if(current.getUpperRightNeighbor() != current.getUpperRightNeighbor()){
            //neighbor.updateULNeighbor(referenceRightU);
            indirectUpdateNeighbors(currentId, false, idRightUpper);
        /*}else{
            //neighbor.updateLLNeighbor(referenceRightU);
            current.indirectUpdateLeftNeighbors(referenceLeft);
            current.indirectUpdateRightNeighbors(referenceRightU);
        }*/
    /*Merge upperr*/
    }else{
        elegibleForMerge.push_back(idRightUpper);
       // if(current.getUpperRightNeighbor() != current.getUpperRightNeighbor()){
            //neighbor.updateLLNeighbor(referenceRightL);
            indirectUpdateNeighbors(currentId, false, idRightLower);
        /*}else{
            current.indirectUpdateLeftNeighbors(referenceLeft);
            current.indirectUpdateRightNeighbors(referenceRightL);
        }*/

    }
}

void TrapezoidalMap::indirectUpdateNeighbors(const size_t& current, bool left, const size_t& idNewT){
    if(left == true){
        if(trapezoid(current).lowerLeftNeighbor() != SIZE_MAX){
            size_t lowerLeft = trapezoid(current).lowerLeftNeighbor();
            if(trapezoid(lowerLeft).lowerRightNeighbor() == current){
                trapezoid(lowerLeft).setLowerRightNeighbor(idNewT);
            }
            if(trapezoid(lowerLeft).upperRightNeighbor() == current){
                trapezoid(lowerLeft).setUpperRightNeighbor(idNewT);
            }
        }

        if(trapezoid(current).upperLeftNeighbor() != SIZE_MAX){
            size_t upperLeft = trapezoid(current).upperLeftNeighbor();
            if(trapezoid(upperLeft).lowerRightNeighbor() == current){
                trapezoid(upperLeft).setLowerRightNeighbor(idNewT);
            }
            if(trapezoid(upperLeft).upperRightNeighbor() == current){
               trapezoid(upperLeft).setUpperRightNeighbor(idNewT);
            }
         }
    }else{
        if(trapezoid(current).lowerRightNeighbor() != SIZE_MAX){
            size_t lowerRight = trapezoid(current).lowerRightNeighbor();
            if(trapezoid(lowerRight).lowerLeftNeighbor() == current){
                trapezoid(lowerRight).setLowerLeftNeighbor(idNewT);
            }
            if(trapezoid(lowerRight).upperLeftNeighbor() == current){
                trapezoid(lowerRight).setUpperLeftNeighbor(idNewT);
            }
        }

        if(trapezoid(current).upperRightNeighbor() != SIZE_MAX){
            size_t upperRight = trapezoid(current).upperRightNeighbor();
            if(trapezoid(upperRight).lowerLeftNeighbor() == current){
                trapezoid(upperRight).setLowerLeftNeighbor(idNewT);
            }
            if(trapezoid(upperRight).upperLeftNeighbor() == current){
               trapezoid(upperRight).setUpperLeftNeighbor(idNewT);
            }
         }
    }

}

void TrapezoidalMap::HandleCaseQ1Inside(size_t& currentId, std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    Trapezoid& current = trapezoid(currentId);
    cg3::Segment2d segment= getLastSegment();

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    std::vector<Trapezoid> tSplitResult = SplitInThree(current, segment, segment.p2());

    /*Add new trapezoid on the map*/
    size_t idRight = this->addTrapezoid(tSplitResult[0]);
    size_t idLeftUpper = this->addTrapezoid(tSplitResult[1]);
    size_t idLeftLower = this->addTrapezoid(tSplitResult[2]);

    idsTrapezoid.push_back(idRight);
    idsTrapezoid.push_back(idLeftUpper);
    idsTrapezoid.push_back(idLeftLower);

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
    indirectUpdateNeighbors(currentId, false, idRight);

    newTrapezoidIds.push_back(idsTrapezoid);

    Trapezoid& mergeCandidate = trapezoid(elegibleForMerge[0]);
    if(mergeCandidate.top().p2() == referenceLeftL.top().p1() and mergeCandidate.bottom().p2() == referenceLeftL.bottom().p1()){
        //if (geoutils::isPointAbove(current.rightp(), segment)){
            elegibleForMerge.push_back(idLeftLower);
            size_t mergeResult = elegibleForMerge[0];
            PerformeMerge(elegibleForMerge, newTrapezoidIds);
            referenceRight.updateLLNeighbor(mergeResult);
            indirectUpdateNeighbors(currentId, true, idLeftUpper);
        /*}else{ //no merge
            indirectUpdateNeighbors(currentId, true, idLeftUpper);
            referenceLeftL.updateLeftNeighbors(newTrapezoidIds[newTrapezoidIds.size() -2][2]);
            trapezoid(newTrapezoidIds[newTrapezoidIds.size() -2][2]).updateRightNeighbors(idLeftLower);
        }*/
    }else{
        //if (geoutils::isPointAbove(current.rightp(), segment)){
            elegibleForMerge.push_back(idLeftUpper);
            size_t mergeResult = elegibleForMerge[0];
            PerformeMerge(elegibleForMerge, newTrapezoidIds);
            referenceRight.updateULNeighbor(mergeResult);
            indirectUpdateNeighbors(currentId, true, idLeftLower);
        /*}else{ //no merge
            indirectUpdateNeighbors(currentId, true, idLeftLower);
            referenceLeftL.updateLeftNeighbors(newTrapezoidIds[newTrapezoidIds.size() -2][1]);
            trapezoid(newTrapezoidIds[newTrapezoidIds.size() -2][1]).updateRightNeighbors(idLeftUpper);

        }*/
    }

}

void TrapezoidalMap::HandleCasePointsOutside(size_t& currentId, size_t& previous, size_t& neighbor, std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds ){
    cg3::Segment2d segment= getLastSegment();
    Trapezoid& current = trapezoid(currentId);
    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    cg3::Point2d p1 = cg3::Point2d(current.leftp().x(), geoutils::calculateYCoord(segment, current.leftp().x()));
    cg3::Point2d q1 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(segment, current.rightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, q1);
    std::vector<Trapezoid> hSplit = SplitHorizontaly(current, innerSegment);
    Trapezoid upper = hSplit[0];
    Trapezoid lower = hSplit[1];

    size_t idUpper = this->addTrapezoid(upper);
    size_t idLower = this->addTrapezoid(lower);

    idsTrapezoid.push_back(idUpper);
    idsTrapezoid.push_back(idLower);

    Trapezoid& referenceUpper = trapezoid(idUpper);
    Trapezoid& referenceLower = trapezoid(idLower);

    /*
    referenceLower.updateRightNeighbors(current);
    referenceLower.updateLeftNeighbors(current);
    upper.updateRightNeighbors(current);
    upper.updateLeftNeighbors(current);
    neighbor.updateLeftNeighbors(upper, referenceLower);

    referenceUpper.updateRightNeighbors(current);
    referenceUpper.updateLeftNeighbors(current);
    lower.updateRightNeighbors(current);
    lower.updateLeftNeighbors(current);
    neighbor.updateLeftNeighbors(referenceUpper, lower);
*/
    newTrapezoidIds.push_back(idsTrapezoid);


    /*Merge handling*/
    Trapezoid& mergeCandidate = trapezoid(elegibleForMerge[0]);
    /*Merge with lower*/
    if(mergeCandidate.top().p2() == referenceLower.top().p1() and mergeCandidate.bottom().p2() == referenceLower.bottom().p1()){
        elegibleForMerge.push_back(idLower);
        PerformeMerge(elegibleForMerge, newTrapezoidIds);
    }else{
        elegibleForMerge.push_back(idUpper);
        PerformeMerge(elegibleForMerge, newTrapezoidIds);
    }
    /*Merge isn't finished yet*/
    if (geoutils::isPointAbove(current.rightp(), segment)){
        elegibleForMerge.push_back(newTrapezoidIds[newTrapezoidIds.size()-1][1]);
    /*Merge is concluded*/
    }else{
        elegibleForMerge.push_back(newTrapezoidIds[newTrapezoidIds.size()-1][0]);
    }


}



void TrapezoidalMap::PerformeMerge(std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    size_t last = elegibleForMerge.size() -1;
    Trapezoid& leftMost = trapezoid(elegibleForMerge[0]);
    Trapezoid& rightMost = trapezoid(elegibleForMerge[last]);
    leftMost.setTopP2(rightMost.top().p2());
    leftMost.setBottomP2(rightMost.bottom().p2());
    leftMost.setRightp(rightMost.rightp());

    leftMost.updateRightNeighborsOld(rightMost);

    size_t j = 1;
    for(size_t i = ((newTrapezoidIds.size()) - last); i < newTrapezoidIds.size(); i++){
        removeTrapezoid(elegibleForMerge[j]);
        (*std::find(newTrapezoidIds[i].begin(), newTrapezoidIds[i].end(), elegibleForMerge[j])) = elegibleForMerge[0];
        j++;
    }
    elegibleForMerge.clear();
}

/*
 * std::vector<size_t>::iterator it;
    for(it = elegibleForMerge.begin() + 1; it != elegibleForMerge.end(); ++it){
        removeTrapezoid((*it));

    }
    size_t offset = 0;
    std::vector<std::vector<Trapezoid>::iterator>::iterator it;
    for(it = elegibleForMerge.begin() + 1; it != elegibleForMerge.end(); ++it){
        size_t xPosition = std::get<0>(std::get<1>(*it));
        size_t yPosition = std::get<1>(std::get<1>(*it));

        std::vector<size_t>::iterator it2;
        if(yPosition == newTrapezoidIds[xPosition].size()-1){
            for(size_t i = xPosition+1; i < newTrapezoidIds.size(); i++){
                for(size_t j = 0; j < newTrapezoidIds[i].size(); j++){
                    newTrapezoidIds[i][j]--;
                }
            }
        }else{
            for(size_t i = yPosition; i < newTrapezoidIds[xPosition].size(); i++){
                newTrapezoidIds[xPosition][i]--;
            }
        }
        newTrapezoidIds[xPosition][yPosition] = std::get<0>(elegibleForMerge[0]);
    }

    for(it = elegibleForMerge.begin() + 1; it != elegibleForMerge.end(); ++it){
        size_t trapId = std::get<0>(*it);
        removeTemporaryTrapezoid(trapId - offset);
        offset++;
    }

    elegibleForMerge.clear();
    return leftMost;
}
*/
/**
 * Given a Trapezoid& already in the tMap and the segment that intersect it returns four trapezoid generated from the segment
 * @param[in] current RValue to the Trapezoid we want to split
 * @param[in] s Segment internal to the Trapezoid
 * @return Vector with the four trapezoid obtained by the split.
 */
std::vector<Trapezoid> TrapezoidalMap::SplitInFour(const Trapezoid& current, const cg3::Segment2d s){
    std::vector<Trapezoid> result = std::vector<Trapezoid>();

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
    D.setRightp(current.top().p2());

    result.push_back(A);
    result.push_back(B);
    result.push_back(C);
    result.push_back(D);

    return result;
}
std::vector<Trapezoid> TrapezoidalMap::SplitInThree(const Trapezoid& current, const cg3::Segment2d segment, const cg3::Point2d splitPoint){
    std::vector<Trapezoid> result = std::vector<Trapezoid>();

    std::vector<Trapezoid> vSplitResult = SplitVerticaly(current, splitPoint);
    Trapezoid leftMost = vSplitResult[0];
    Trapezoid rightMost = vSplitResult[1];

    if(splitPoint == segment.p1()){
        cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), cg3::Point2d(rightMost.rightp().x(), geoutils::calculateYCoord(segment, rightMost.rightp().x())));
        std::vector<Trapezoid> hSplitResult = SplitHorizontaly(rightMost, innerSegment);

        Trapezoid rightUpper = hSplitResult[0];
        Trapezoid rightLower = hSplitResult[1];

        result.push_back(leftMost);
        result.push_back(rightUpper);
        result.push_back(rightLower);
    }else{
        cg3::Segment2d innerSegment = cg3::Segment2d(cg3::Point2d(leftMost.leftp().x(), geoutils::calculateYCoord(segment, leftMost.leftp().x())),segment.p2());
        std::vector<Trapezoid> hSplitResult = SplitHorizontaly(leftMost, innerSegment);

        Trapezoid leftUpper = hSplitResult[0];
        Trapezoid leftLower = hSplitResult[1];
        result.push_back(rightMost);
        result.push_back(leftUpper);
        result.push_back(leftLower);
    }

    return result;
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














