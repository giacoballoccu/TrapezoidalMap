#include "trapezoidalmap.h"
#include "QRandomGenerator"
TrapezoidalMap::TrapezoidalMap(){
    trapezoids = std::vector<Trapezoid>();
    //trapezoids.push_back(Trapezoid());
    isDeleted = std::vector<bool>();
    segments = std::vector<cg3::Segment2d>();
    this->points = std::vector<cg3::Point2d>();
};

std::vector<cg3::Segment2d> TrapezoidalMap::getSegments() const{
    return segments;
};
std::vector<cg3::Segment2d>& TrapezoidalMap::getSegmentsRfr(){
    return segments;
};

std::vector<cg3::Point2d> TrapezoidalMap::getPoints() const{
    return points;
};
std::vector<cg3::Point2d>& TrapezoidalMap::getPointsRfr(){
    return points;
};

std::vector<Trapezoid> TrapezoidalMap::getTrapezoids() const{
    return trapezoids;
};

std::vector<Trapezoid> TrapezoidalMap::getActiveTrapezoids() const{
    std::vector<Trapezoid> result = std::vector<Trapezoid>();
    for (size_t i=0; i < trapezoids.size(); i++){
        if (!isDeleted[i]){
            result.push_back(trapezoids[i]);
        }
    }
    return result;
};
std::vector<Trapezoid>& TrapezoidalMap::getTrapezoidsRfr(){
    return trapezoids;
};

size_t TrapezoidalMap::addSegment(const cg3::Segment2d& s){
    segments.push_back(s);
    return segments.size()-1;
};
size_t TrapezoidalMap::addPoint(const cg3::Point2d& p){
    points.push_back(p);
    return points.size()-1;
};
size_t TrapezoidalMap::addTrapezoid(const Trapezoid& t){
    trapezoids.push_back(t);
    isDeleted.push_back(false);
    return trapezoids.size()-1;
};


cg3::Segment2d TrapezoidalMap::getLastSegment(){
    return segment(segments.size()-1);
}
cg3::Point2d TrapezoidalMap::getP1LastSegment(){
    return point(points.size()-2);
};
cg3::Point2d TrapezoidalMap::getQ1LastSegment(){
    return point(points.size()-1);
};

size_t TrapezoidalMap::getIdLastSegment(){
    return segments.size()-1;
};
size_t TrapezoidalMap::getIdP1LastSegment(){
    return points.size() - 2;
};
size_t TrapezoidalMap::getIdQ1LastSegment(){
    return points.size() - 1;
};

void TrapezoidalMap::setTrapezoids(std::vector<Trapezoid> trapezoids){
    this->trapezoids = trapezoids;
};

Trapezoid& TrapezoidalMap::trapezoid(const size_t& id){
    if(!isDeleted[id]){
        return trapezoids[id];
    }
    throw __EXCEPTIONS;
}


cg3::Point2d& TrapezoidalMap::point(const size_t& id){
    return points[id];
};
cg3::Segment2d& TrapezoidalMap::segment(const size_t& id){
    return segments[id];
};

void TrapezoidalMap::removeTrapezoid(const size_t& id){
    isDeleted[id] = true;
};



void TrapezoidalMap::HandleOneTIntersection(std::vector<size_t> trapsIntersected, std::vector<std::vector<size_t>>& newTrapezoidIds){
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
    if(current.lowerLeftNeighborRfr() != SIZE_MAX){
        trapezoid(current.getLowerLeftNeighbor()).updateRightNeighbors(idA);
        trapezoid(current.getUpperLeftNeighbor()).updateRightNeighbors(idA);
    }

    referenceB.updateNeighbors(idA, idD);
    referenceC.updateNeighbors(idA, idD);
    referenceD.updateNeighborsLeft(current, idB, idC);
    if(current.lowerRightNeighborRfr() != SIZE_MAX){
        trapezoid(current.getLowerRightNeighbor()).updateLeftNeighbors(idD);
        trapezoid(current.getUpperRightNeighbor()).updateLeftNeighbors(idD);
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
void TrapezoidalMap::HandleCaseP1Inside(Trapezoid& current, Trapezoid& neighbor, std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
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
    indirectUpdateNeighbors(current.getId(), true, idLeft);

    newTrapezoidIds.push_back(idsTrapezoid);
    /*Merge lower*/
    if (geoutils::isPointAbove(current.getRightp(), segment)){
        elegibleForMerge.push_back(idRightLower);
        //if(current.getUpperRightNeighbor() != current.getUpperRightNeighbor()){
            //neighbor.updateULNeighbor(referenceRightU);
            indirectUpdateNeighbors(current.getId(), false, idRightUpper);
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
            indirectUpdateNeighbors(current.getId(), false, idRightLower);
        /*}else{
            current.indirectUpdateLeftNeighbors(referenceLeft);
            current.indirectUpdateRightNeighbors(referenceRightL);
        }*/

    }
}

void TrapezoidalMap::indirectUpdateNeighbors(const size_t& current, bool left, const size_t& idNewT){
    if(left == true){
        if(trapezoid(current).getLowerLeftNeighbor() != SIZE_MAX){
            size_t lowerLeft = trapezoid(current).getLowerLeftNeighbor();
            if(trapezoid(lowerLeft).getLowerRightNeighbor() == current){
                trapezoid(lowerLeft).setLowerRightNeighbor(idNewT);
            }
            if(trapezoid(lowerLeft).getUpperRightNeighbor() == current){
                trapezoid(lowerLeft).setUpperRightNeighbor(idNewT);
            }
        }

        if(trapezoid(current).getUpperLeftNeighbor() != SIZE_MAX){
            size_t upperLeft = trapezoid(current).getUpperLeftNeighbor();
            if(trapezoid(upperLeft).getLowerRightNeighbor() == current){
                trapezoid(upperLeft).setLowerRightNeighbor(idNewT);
            }
            if(trapezoid(upperLeft).getUpperRightNeighbor() == current){
               trapezoid(upperLeft).setUpperRightNeighbor(idNewT);
            }
         }
    }else{
        if(trapezoid(current).getLowerRightNeighbor() != SIZE_MAX){
            size_t lowerRight = trapezoid(current).getLowerRightNeighbor();
            if(trapezoid(lowerRight).getLowerLeftNeighbor() == current){
                trapezoid(lowerRight).setLowerLeftNeighbor(idNewT);
            }
            if(trapezoid(lowerRight).getUpperLeftNeighbor() == current){
                trapezoid(lowerRight).setUpperLeftNeighbor(idNewT);
            }
        }

        if(trapezoid(current).getUpperRightNeighbor() != SIZE_MAX){
            size_t upperRight = trapezoid(current).getUpperRightNeighbor();
            if(trapezoid(upperRight).getLowerLeftNeighbor() == current){
                trapezoid(upperRight).setLowerLeftNeighbor(idNewT);
            }
            if(trapezoid(upperRight).getUpperLeftNeighbor() == current){
               trapezoid(upperRight).setUpperLeftNeighbor(idNewT);
            }
         }
    }

}

void TrapezoidalMap::HandleCaseQ1Inside(Trapezoid& current, std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
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
    indirectUpdateNeighbors(current.getId(), false, idRight);

    newTrapezoidIds.push_back(idsTrapezoid);

    Trapezoid& mergeCandidate = trapezoid(elegibleForMerge[0]);
    if(mergeCandidate.getRightp() == referenceLeftL.getTop().p1()){
        elegibleForMerge.push_back(idLeftLower);
        size_t mergeResult = elegibleForMerge[0];
        PerformeMerge(elegibleForMerge, newTrapezoidIds);
        referenceRight.updateLLNeighbor(mergeResult);
        indirectUpdateNeighbors(current.getId(), true, idLeftUpper);
    }else{
        elegibleForMerge.push_back(idLeftUpper);
        size_t mergeResult = elegibleForMerge[0];
        PerformeMerge(elegibleForMerge, newTrapezoidIds);
        referenceRight.updateULNeighbor(mergeResult);
        indirectUpdateNeighbors(current.getId(), true, idLeftLower);
    }

}
/*
void TrapezoidalMap::HandleCasePointsOutside(Trapezoid& current, Trapezoid& neighbor, std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds ){
    cg3::Segment2d segment= getLastSegment();

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    cg3::Point2d p1 = cg3::Point2d(current.getLeftp().x(), geoutils::calculateYCoord(segment, current.getLeftp().x()));
    cg3::Point2d q1 = cg3::Point2d(current.getRightp().x(), geoutils::calculateYCoord(segment, current.getRightp().x()));
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

    newTrapezoidIds.push_back(idsTrapezoid);


    Merge handling
    if (geoutils::isAbove(current.getRightp(), segment)){
       if(elegibleForMerge.size() > 0){
            if(current.getRightp() == lower.getTop().p2()){ //controlla
                elegibleForMerge.push_back(idsTrapezoid[0]);
                PerformeMerge(elegibleForMerge, newTrapezoidIds);
            }
       }
       elegibleForMerge.push_back(idsTrapezoid[1]);
    }else{
        if(elegibleForMerge.size() > 0){
            if(current.getRightp() == upper.getBottom().p2()){
                elegibleForMerge.push_back(idsTrapezoid[1]);
                PerformeMerge(elegibleForMerge, newTrapezoidIds);
            }
        }
        elegibleForMerge.push_back(idsTrapezoid[0]);
    }


}
*/


void TrapezoidalMap::PerformeMerge(std::vector<size_t>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    size_t last = elegibleForMerge.size() -1;
    Trapezoid& leftMost = trapezoid(elegibleForMerge[0]);
    Trapezoid& rightMost = trapezoid(elegibleForMerge[last]);
    leftMost.getTopRfr().setP2(rightMost.getTop().p2());
    leftMost.getBottomRfr().setP2(rightMost.getBottom().p2());
    leftMost.setRightp(rightMost.getRightp());

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

    cg3::Point2d AtopP2 = cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getTop(), s.p1().x()));
    A.getTopRfr().setP2(AtopP2);
    A.getTopRfr().setP1(current.getTop().p1());
    cg3::Point2d AbotP2 = cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getBottom(), s.p1().x()));
    A.getBottomRfr().setP2(AbotP2);
    A.getBottomRfr().setP1(current.getBottom().p1());
    A.setRightp(s.p1());
    A.setLeftp(current.getLeftp());

    B.getTopRfr().setP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getTop(), s.p1().x())));
    B.getTopRfr().setP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.getTop(), s.p2().x())));
    B.setBottom(s);
    B.setLeftp(s.p1());
    B.setRightp(s.p2());

    C.setTop(s);
    C.getBottomRfr().setP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getBottom(), s.p1().x())));
    C.getBottomRfr().setP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.getBottom(), s.p2().x())));
    C.setLeftp(s.p1());
    C.setRightp(s.p2());

    D.getTopRfr().setP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.getTop(), s.p2().x())));
    D.getTopRfr().setP2(current.getTop().p2());
    D.getBottomRfr().setP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.getBottom(), s.p2().x())));
    D.getBottomRfr().setP2(current.getBottom().p2());
    D.setLeftp(s.p2());
    D.setRightp(current.getTop().p2());

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
        cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), cg3::Point2d(rightMost.getRightp().x(), geoutils::calculateYCoord(segment, rightMost.getRightp().x())));
        std::vector<Trapezoid> hSplitResult = SplitHorizontaly(rightMost, innerSegment);

        Trapezoid rightUpper = hSplitResult[0];
        Trapezoid rightLower = hSplitResult[1];

        result.push_back(leftMost);
        result.push_back(rightUpper);
        result.push_back(rightLower);
    }else{
        cg3::Segment2d innerSegment = cg3::Segment2d(cg3::Point2d(leftMost.getLeftp().x(), geoutils::calculateYCoord(segment, leftMost.getLeftp().x())),segment.p2());
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
    upper.setTop(intermediateT.getTop());
    upper.setBottom(innerSegment);
    upper.setLeftp(intermediateT.getLeftp()); //mm
    upper.setRightp(intermediateT.getRightp());

    Trapezoid lower = Trapezoid();
    lower.setTop(innerSegment);
    lower.setBottom(intermediateT.getBottom());
    lower.setLeftp(intermediateT.getLeftp());
    lower.setRightp(intermediateT.getRightp());

    hsplitResult.push_back(upper);
    hsplitResult.push_back(lower);
    return hsplitResult;

}
std::vector<Trapezoid> TrapezoidalMap::SplitVerticaly(const Trapezoid& current, const cg3::Point2d splitPoint){
    std::vector<Trapezoid> vSplitResult = std::vector<Trapezoid>();

    /*Vertical split of trapezoid*/
    Trapezoid leftHalf = Trapezoid();
    cg3::Point2d t1TopP2 = cg3::Point2d(splitPoint.x(), geoutils::calculateYCoord(current.getTop(), splitPoint.x()));
    cg3::Point2d t1BottomP2 = cg3::Point2d(splitPoint.x(), geoutils::calculateYCoord(current.getBottom(), splitPoint.x()));
    leftHalf.setTop(cg3::Segment2d(current.getTop().p1(), t1TopP2));
    leftHalf.setBottom(cg3::Segment2d(current.getBottom().p1(), t1BottomP2));
    leftHalf.setRightp(splitPoint);
    leftHalf.setLeftp(current.getLeftp());

    Trapezoid rightHalf = Trapezoid();
    cg3::Point2d t2TopP2 = cg3::Point2d(current.getRightp().x(), geoutils::calculateYCoord(current.getTop(), current.getRightp().x()));
    cg3::Point2d t2BottomP2 = cg3::Point2d(current.getRightp().x(), geoutils::calculateYCoord(current.getBottom(), current.getRightp().x()));
    rightHalf.setTop(cg3::Segment2d(t1TopP2, t2TopP2));
    rightHalf.setBottom(cg3::Segment2d(t1BottomP2, t2BottomP2));
    rightHalf.setRightp(current.getRightp());
    rightHalf.setLeftp(splitPoint);

    vSplitResult.push_back(leftHalf);
    vSplitResult.push_back(rightHalf);

    return vSplitResult;
}
/*void TrapezoidalMap::PerformeMerge(Trapezoid& t1, Trapezoid& t2){
    t1.getTopRfr().setP2(t2.getTop().p2());
    t1.getBottomRfr().setP2(t2.getBottom().p2());
    t1.setRightp(t2.getRightp());

    t1.updateRightNeighborsOld(t2);
}*/
void TrapezoidalMap::clear(){
    trapezoids.~vector();
    segments.~vector();
    points.~vector();
    isDeleted.~vector();
}














