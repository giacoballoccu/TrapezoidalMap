#include "trapezoidalmap.h"
#include "QRandomGenerator"
TrapezoidalMap::TrapezoidalMap(){
    this->trapezoids= std::list<Trapezoid>();
    this->segments = std::vector<cg3::Segment2d>();
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

std::list<Trapezoid> TrapezoidalMap::getTrapezoids() const{
    return trapezoids;
};
std::list<Trapezoid>& TrapezoidalMap::getTrapezoidsRfr(){
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
    return trapezoids.size()-1;
};

size_t TrapezoidalMap::getIdLastSegment(){
    return segments.size()-1;
}

size_t TrapezoidalMap::getIdP1LastSegment(){
    return points.size()-2;
}

size_t TrapezoidalMap::getIdQ1LastSegment(){
    return segments.size()-1;
}


void TrapezoidalMap::permuteSegmentList(){
   std::random_shuffle(segments.begin(), segments.end());
};


void TrapezoidalMap::setTrapezoids(std::list<Trapezoid> trapezoids){
    this->trapezoids = trapezoids;
};

Trapezoid& TrapezoidalMap::trapezoid(const size_t& id){
    std::list<Trapezoid>::iterator it = trapezoids.begin();
    std::advance(it, id);
    return (*it);
}



cg3::Point2d& TrapezoidalMap::point(const size_t& id){
    return points[id];
};
cg3::Segment2d& TrapezoidalMap::segment(const size_t& id){
    return segments[id];
};

//void setNodeToTrapezoid(const size_t& idTrap, const Node* node);

void TrapezoidalMap::removeTrapezoid(const size_t& id){
    std::list<Trapezoid>::iterator it = trapezoids.begin();
    std::advance(it, id);

    trapezoids.erase(it);
    for (it; it != trapezoids.end(); it++){
        (*it).node->reduceId();
    }
};

void TrapezoidalMap::removeTemporaryTrapezoid(const size_t& id){
    std::list<Trapezoid>::iterator it = trapezoids.begin();
    std::advance(it, id);

    trapezoids.erase(it);

}

std::list<Trapezoid>::iterator TrapezoidalMap::getIterator(const size_t& id){
    std::list<Trapezoid>::iterator it = trapezoids.begin();
    std::advance(it, id);
    return it;
}

void TrapezoidalMap::SplitInFour(const size_t& id, cg3::Segment2d s, std::vector<std::vector<size_t>>& newTrapezoidIds){
    Trapezoid& current = trapezoid(id);
    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

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


    size_t idA = this->addTrapezoid(A);
    idsTrapezoid.push_back(idA);

    size_t idB = this->addTrapezoid(B);
    idsTrapezoid.push_back(idB);

    size_t idC = this->addTrapezoid(C);
    idsTrapezoid.push_back(idC);

    size_t idD = this->addTrapezoid(D);
    idsTrapezoid.push_back(idD);

    Trapezoid& referenceA = trapezoid(idA);
    Trapezoid& referenceB = trapezoid(idB);
    Trapezoid& referenceC = trapezoid(idC);
    Trapezoid& referenceD = trapezoid(idD);

    referenceA.updateRightNeighbors(current, referenceB, referenceC);
    referenceB.updateNeighbors(referenceA, referenceD);
    referenceC.updateNeighbors(referenceA, referenceD);
    referenceD.updateLeftNeighbors(current, referenceB, referenceC);

    newTrapezoidIds.push_back(idsTrapezoid);
}

/*

  SplitVerticaly handle the case when ONLY the first end point is inside the trapezoid, it split the trapezoid in a half using the
  point as bound and then split the right trapezoid obtained by the split in half HORIZONTALY.
  This method adds 3 trapezoid in splitResult ordered from left to right and from top to bottom.
  INPUT: id of the trapezoid that need to be split
         splitResult the vector where the result of the split will be pushed
         s segment

*/
std::vector<Trapezoid> TrapezoidalMap::SplitVerticaly(const size_t& idCurrent, cg3::Point2d splitPoint){
    Trapezoid& current = trapezoid(idCurrent);
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

void TrapezoidalMap::HandleCaseP1Inside(size_t& idCurrent, size_t& idNext, cg3::Segment2d segment, std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    Trapezoid& current = trapezoid(idCurrent);
    Trapezoid& neighbor = this->trapezoid(idNext);

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    std::vector<Trapezoid> vSplitResult = SplitVerticaly(idCurrent, segment.p1());
    cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), cg3::Point2d(vSplitResult[1].getRightp().x(), geoutils::calculateYCoord(segment, vSplitResult[1].getRightp().x())));
    std::vector<Trapezoid> hSplitResult = SplitHorizontaly(vSplitResult[1], innerSegment);

    Trapezoid rightUpper = hSplitResult[0];
    Trapezoid rightLower = hSplitResult[1];
    /*Add new trapezoid on the map*/
    size_t idLeft = this->addTrapezoid(vSplitResult[0]);
    idsTrapezoid.push_back(idLeft);

    Trapezoid& referenceLeft = trapezoid(idLeft);

    /*Update of neighbors with the new trapezoids*/
    referenceLeft.updateLeftNeighbors(current);

    size_t idRightUpper = this->addTrapezoid(rightUpper);
    idsTrapezoid.push_back(idRightUpper);

    Trapezoid& referenceRightU= trapezoid(idRightUpper);
    referenceRightU.updateRightNeighbors(neighbor);
    referenceRightU.updateLeftNeighbors(referenceLeft);
    referenceLeft.updateRightNeighbors(referenceRightU, rightLower);

    size_t idRightLower = this->addTrapezoid(rightLower);
    idsTrapezoid.push_back(idRightLower);

    Trapezoid& referenceRightL = trapezoid(idRightLower);
    referenceRightL.updateRightNeighbors(neighbor);
    referenceRightL.updateLeftNeighbors(referenceLeft);
    referenceLeft.updateRightNeighbors(rightUpper, referenceRightL);



    newTrapezoidIds.push_back(idsTrapezoid);


    if (geoutils::isAbove(current.getRightp(), segment)){ /*Merge down*/
        std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idRightLower, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 2));
        elegibleForMerge.push_back(idAndPosition);
        referenceRightU.updateLRNeighbor(neighbor);
        referenceRightL.updateRightNeighbors(neighbor);
        neighbor.updateLeftNeighbors(referenceRightU, referenceRightL);
        current.upperRightNeighborRfr()->updateLeftNeighbors(referenceRightL);
    }else{ /*Merge up*/
        std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idRightUpper, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 1));
        elegibleForMerge.push_back(idAndPosition);
        referenceRightU.updateRightNeighbors(neighbor);
        referenceRightL.updateURNeighbor(neighbor);
        neighbor.updateLeftNeighbors(referenceRightU, referenceRightL);
        current.lowerRightNeighborRfr()->updateLeftNeighbors(referenceRightU);
    }


}

void TrapezoidalMap::HandleCaseQ1Inside(size_t& idCurrent, cg3::Segment2d segment, std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    Trapezoid& current = trapezoid(idCurrent);

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    std::vector<Trapezoid> vSplitResult = SplitVerticaly(idCurrent, segment.p2());
    cg3::Segment2d innerSegment = cg3::Segment2d(cg3::Point2d(vSplitResult[0].getLeftp().x(), geoutils::calculateYCoord(segment, vSplitResult[0].getLeftp().x())),segment.p2());
    std::vector<Trapezoid> hSplitResult = SplitHorizontaly(vSplitResult[0], innerSegment);

    Trapezoid rightTrapezoid = vSplitResult[1];
    Trapezoid leftUpper = hSplitResult[0];
    Trapezoid leftLower = hSplitResult[1];

    size_t idRight = this->addTrapezoid(vSplitResult[1]);
    idsTrapezoid.push_back(idRight);

    size_t idLeftUpper = this->addTrapezoid(leftUpper);
    idsTrapezoid.push_back(idLeftUpper);

    size_t idLeftLower = this->addTrapezoid(leftLower);
    idsTrapezoid.push_back(idLeftLower);

    Trapezoid& referenceRight = trapezoid(idRight);
    Trapezoid& referenceLeftU= trapezoid(idLeftUpper);
    Trapezoid& referenceLeftL = trapezoid(idLeftLower);

    /*Update of neighbors with the new trapezoids*/
    referenceRight.updateRightNeighborsOld(current);





    newTrapezoidIds.push_back(idsTrapezoid);

    if(elegibleForMerge.size() > 0){
        if(geoutils::isAbove(current.getLeftp(), segment)){ /*Merge down*/
            referenceLeftL.updateRightNeighbors(referenceRight);
            referenceLeftU.updateLeftNeighborsOld(current);
            current.lowerLeftNeighborRfr()->updateLRNeighbor(referenceLeftL);
            size_t mergedId = std::get<0>(elegibleForMerge[0]);

            referenceRight.updateLeftNeighbors(referenceLeftU, trapezoid(mergedId));

            std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idLeftLower, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 2));
            elegibleForMerge.push_back(idAndPosition);
            PerformeMerge(elegibleForMerge, newTrapezoidIds);
        }else{
            referenceLeftU.updateRightNeighbors(referenceRight);
            referenceLeftL.updateLeftNeighborsOld(current);
            current.lowerLeftNeighborRfr()->updateURNeighbor(referenceLeftL);
            current.lowerLeftNeighborRfr()->updateLRNeighbor(referenceLeftL);
            size_t mergedId = std::get<0>(elegibleForMerge[0]);

            referenceRight.updateLeftNeighbors(trapezoid(mergedId), referenceLeftL);

            std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idLeftUpper, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 1));
            elegibleForMerge.push_back(idAndPosition);
            PerformeMerge(elegibleForMerge, newTrapezoidIds);
        }
    }

}

void TrapezoidalMap::HandleCasePointsOutside(size_t& idCurrent, size_t& idNext,  cg3::Segment2d segment, std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    Trapezoid& current = trapezoid(idCurrent);
    Trapezoid& neighbor = this->trapezoid(idNext);

    std::vector<size_t> idsTrapezoid = std::vector<size_t>();

    cg3::Point2d p1 = cg3::Point2d(current.getLeftp().x(), geoutils::calculateYCoord(segment, current.getLeftp().x()));
    cg3::Point2d q1 = cg3::Point2d(current.getRightp().x(), geoutils::calculateYCoord(segment, current.getRightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, q1);
    std::vector<Trapezoid> hSplit = SplitHorizontaly(current, innerSegment);
    Trapezoid upper = hSplit[0];
    Trapezoid lower = hSplit[1];

    size_t idUpper = this->addTrapezoid(upper);
    idsTrapezoid.push_back(idUpper);
    size_t idLower = this->addTrapezoid(lower);
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


    /*Merge handling*/
    if (geoutils::isAbove(current.getRightp(), segment)){
       if(elegibleForMerge.size() > 0){
            if(current.getRightp() == lower.getTop().p2()){ //controlla
                std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idUpper, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 1));
                elegibleForMerge.push_back(idAndPosition);
                PerformeMerge(elegibleForMerge, newTrapezoidIds);
            }
       }
       std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idLower, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 2));
       elegibleForMerge.push_back(idAndPosition);
    }else{
        if(elegibleForMerge.size() > 0){
            if(current.getRightp() == upper.getBottom().p2()){
                std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idLower, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 2));
                elegibleForMerge.push_back(idAndPosition);
                PerformeMerge(elegibleForMerge, newTrapezoidIds);
            }
        }
        std::tuple<size_t,std::tuple<size_t, size_t>> idAndPosition = std::tuple<size_t,std::tuple<size_t, size_t>>(idUpper, std::tuple<size_t, size_t>(newTrapezoidIds.size()-1, 1));
        elegibleForMerge.push_back(idAndPosition);
    }


}

std::vector<Trapezoid> TrapezoidalMap::SplitHorizontaly(Trapezoid& intermediateT, cg3::Segment2d innerSegment){
    std::vector<Trapezoid> hsplitResult = std::vector<Trapezoid>();

    Trapezoid upper = Trapezoid();
    upper.setTop(intermediateT.getTop());
    upper.setBottom(innerSegment);
    upper.setLeftp(intermediateT.getLeftp()); //mm
    upper.setRightp(intermediateT.getRightp());
    upper.updateLeftNeighbors(intermediateT);
    upper.updateRightNeighborsOld(intermediateT);

    Trapezoid lower = Trapezoid();
    lower.setTop(innerSegment);
    lower.setBottom(intermediateT.getBottom());
    lower.setLeftp(intermediateT.getLeftp());
    lower.setRightp(intermediateT.getRightp());
    lower.updateLeftNeighbors(intermediateT);
    lower.updateRightNeighborsOld(intermediateT);

    hsplitResult.push_back(upper);
    hsplitResult.push_back(lower);
    return hsplitResult;

}

Trapezoid TrapezoidalMap::PerformeMerge(std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>& elegibleForMerge, std::vector<std::vector<size_t>>& newTrapezoidIds){
    size_t last = elegibleForMerge.size() -1;
    Trapezoid& leftMost = trapezoid(std::get<0>(elegibleForMerge[0]));
    Trapezoid& rightMost = trapezoid(std::get<0>(elegibleForMerge[last]));
    leftMost.getTopRfr().setP2(rightMost.getTop().p2());
    leftMost.getBottomRfr().setP2(rightMost.getBottom().p2());
    leftMost.setRightp(rightMost.getRightp());

    leftMost.updateRightNeighborsOld(rightMost);


    size_t offset = 0;
    std::vector<std::tuple<size_t,std::tuple<size_t, size_t>>>::iterator it;
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


   /*for (it = elegibleForMerge.begin()+1; it != elegibleForMerge.end(); ++it){
        for (it2 = std::get<1>((*it)); it2 != it2.; it2++){
            (*it2) = (*it2) - 1;
        }
        (*std::get<1>((*it))) = std::get<0>(elegibleForMerge[0]);
        removeTemporaryTrapezoid(std::get<0>((*it)) - i);
        i++;
    }*/
    elegibleForMerge.clear();
    return leftMost;
}


void TrapezoidalMap::clear(){
    trapezoids.~list();
    segments.~vector();
    points.~vector();
}














/*
std::set<Trapezoid*> TrapezoidalMap::getAllRightNeighbors() const{
    std::set<Trapezoid*> result = std::set<Trapezoid*>();
    result.insert(leftMostTrapezoid);
    getAllRightNeighborsHelper(leftMostTrapezoid, result);
    return result;
}
void TrapezoidalMap::getAllRightNeighborsHelper(Trapezoid *t, std::set<Trapezoid*>& result) const{
   if(t == nullptr){
       return;
    }else{
       if (!t->neighborExist("upperRightNeighbor") and !t->neighborExist("lowerRightNeighbor")){
           result.insert(t);
           return;
       }
       if(t->neighborExist("upperRightNeighbor") and !t->neighborExist("lowerRightNeighbor")){
           result.insert(t->getUpperRightNeighbor());
           getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
           return;
       }
       if(!t->neighborExist("upperRightNeighbor") and t->neighborExist("lowerRightNeighbor")){
           result.insert(t->getLowerRightNeighbor());
           getAllRightNeighborsHelper(t->getLowerRightNeighbor(), result);
           return;
       }
       if (t->neighborExist("upperRightNeighbor") and t->neighborExist("lowerRightNeighbor")){
           if(t->getUpperRightNeighbor() == t->getLowerRightNeighbor()){
               result.insert(t->getUpperRightNeighbor());
               getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
               return;
           }else{
               result.insert(t->getUpperRightNeighbor());
               result.insert(t->getLowerRightNeighbor());
               getAllRightNeighborsHelper(t->getLowerRightNeighbor(), result);
               getAllRightNeighborsHelper(t->getUpperRightNeighbor(), result);
               return;
           }
       }
   }
}
*/

