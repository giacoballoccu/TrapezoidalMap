#include "trapezoidalmap.h"
#include "QRandomGenerator"
TrapezoidalMap::TrapezoidalMap(){
    this->trapezoids= std::vector<Trapezoid>();
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

std::vector<Trapezoid> TrapezoidalMap::getTrapezoids() const{
    return trapezoids;
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


void TrapezoidalMap::setTrapezoids(std::vector<Trapezoid> trapezoids){
    this->trapezoids = trapezoids;
};

Trapezoid& TrapezoidalMap::trapezoid(const size_t& id){
    return trapezoids[id];
}

const Trapezoid& TrapezoidalMap::trapezoid(const size_t& id) const {
    return trapezoids[id];
}

cg3::Point2d& TrapezoidalMap::point(const size_t& id){
    return points[id];
};
cg3::Segment2d& TrapezoidalMap::segment(const size_t& id){
    return segments[id];
};

//void setNodeToTrapezoid(const size_t& idTrap, const Node* node);

void TrapezoidalMap::removeTrapezoid(const size_t& id){
    trapezoids.erase(trapezoids.begin() + id);
    for (size_t i = id; i < trapezoids.size(); i++){
        trapezoid(i).node->reduceId();
    }
};
/*
void TrapezoidalMap::SplitInFour(const size_t& id, std::vector<Trapezoid>& splitResult, cg3::Segment2d s){
    Trapezoid current = this->trapezoid(id);

    Trapezoid A = Trapezoid();
    Trapezoid B = Trapezoid();
    Trapezoid C = Trapezoid();
    Trapezoid D = Trapezoid();

    A.top.setP2(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getTop(), s.p1().x())));
    A.Top().setP1(current.getTop().p1());
    A.Bottom().setP2(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getBottom(), s.p1().x())));
    A.Bottom().setP1(current.getBottom().p1());
    A.Rightp() = s.p1();
    A.Leftp() = current.getTop().p1();


    A.lowerRightNeighbor = &C;
    A.upperRightNeighbor = &B;
    if (this->lowerLeftNeighbor != nullptr){
         A.lowerLeftNeighbor = this->lowerLeftNeighbor;
         if(this->lowerLeftNeighbor->lowerRightNeighbor == this){
             this->lowerLeftNeighbor->lowerRightNeighbor = &A;
         }
         if(this->lowerLeftNeighbor->upperRightNeighbor == this){
             this->lowerLeftNeighbor->upperRightNeighbor = &A;
         }
    }
    if (this->upperLeftNeighbor != nullptr){
         A.upperLeftNeighbor = this->upperLeftNeighbor;
         if(this->upperLeftNeighbor->upperRightNeighbor == this){
             this->upperLeftNeighbor->upperRightNeighbor = &A;
         }
         if(this->upperLeftNeighbor->lowerRightNeighbor == this){
             this->upperLeftNeighbor->lowerRightNeighbor = &A;
         }
    }



    B.top.setP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getTop(), s.p1().x())));
    B.top.setP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.getTop(), s.p2().x())));
    B.bottom = s;
    B.leftp = s.p1();
    B.rightp = s.p2();
    B.lowerLeftNeighbor = &A;
    B.lowerRightNeighbor = &D;
    B.upperLeftNeighbor = &A;
    B.upperRightNeighbor = &D;

    C.top = s;
    C.bottom.setP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.bottom, s.p1().x())));
    C.bottom.setP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.bottom, s.p2().x())));
    C.leftp = s.p1();
    C.rightp = s.p2();
    C.lowerLeftNeighbor = &A;
    C.lowerRightNeighbor = &D;
    C.upperLeftNeighbor = &A;
    C.upperRightNeighbor = &D;

    D.top.setP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.getTop(), s.p2().x())));
    D.top.setP2(current.top.p2());
    D.bottom.setP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(current.getBottom(), s.p2().x())));
    D.bottom.setP2(current.bottom.p2());
    D.leftp = s.p2();
    D.rightp = top.p2();
    D.lowerLeftNeighbor = &C;
    D.upperLeftNeighbor = &B;
    if (this->lowerRightNeighbor != nullptr){
         D.lowerRightNeighbor = this->lowerRightNeighbor;
         if(this->lowerRightNeighbor->lowerLeftNeighbor == this){
             this->lowerRightNeighbor->lowerLeftNeighbor = &D;
         }
         if(this->lowerRightNeighbor->upperLeftNeighbor == this){
             this->lowerRightNeighbor->upperLeftNeighbor = &D;
         }
    }
    if (this->upperRightNeighbor != nullptr){
         D.upperRightNeighbor = this->upperRightNeighbor;
         if(this->upperRightNeighbor->upperLeftNeighbor == this){
             this->upperRightNeighbor->upperLeftNeighbor = &D;
         }
         if(this->upperRightNeighbor->lowerLeftNeighbor == this){
             this->upperRightNeighbor->lowerLeftNeighbor = &D;
         }
    }

    splitResult.push_back(A);
    splitResult.push_back(B);
    splitResult.push_back(C);
    splitResult.push_back(D);

}

*/




const std::vector<size_t> TrapezoidalMap::SplitInFour(const size_t& id, cg3::Segment2d s){
    const Trapezoid& current = trapezoid(id);
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

    return idsTrapezoid;
}

/*

  SplitVerticaly handle the case when ONLY the first end point is inside the trapezoid, it split the trapezoid in a half using the
  point as bound and then split the right trapezoid obtained by the split in half HORIZONTALY.
  This method adds 3 trapezoid in splitResult ordered from left to right and from top to bottom.
  INPUT: id of the trapezoid that need to be split
         splitResult the vector where the result of the split will be pushed
         s segment

*/
const std::vector<size_t> TrapezoidalMap::SplitVerticaly(const size_t& idCurrent, const size_t& idNext, cg3::Segment2d s){
   Trapezoid& current = trapezoid(idCurrent);
   Trapezoid& neighbor = this->trapezoid(idNext);
   std::vector<size_t> idsTrapezoid = std::vector<size_t>();

   Trapezoid leftHalf = Trapezoid();
   cg3::Point2d t1TopP2 = cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getTop(), s.p1().x()));
   cg3::Point2d t1BottomP2 = cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(current.getBottom(), s.p1().x()));
   leftHalf.setTop(cg3::Segment2d(current.getTop().p1(), t1TopP2));
   leftHalf.setBottom(cg3::Segment2d(current.getBottom().p1(), t1BottomP2));
   leftHalf.setRightp(s.p1());
   leftHalf.setLeftp(current.getLeftp());

   Trapezoid rightHalf = Trapezoid();
   cg3::Point2d t2TopP2 = cg3::Point2d(current.getRightp().x(), geoutils::calculateYCoord(current.getTop(), current.getRightp().x()));
   cg3::Point2d t2BottomP2 = cg3::Point2d(current.getRightp().x(), geoutils::calculateYCoord(current.getBottom(), current.getRightp().x()));
   rightHalf.setTop(cg3::Segment2d(t1TopP2, t2TopP2));
   rightHalf.setBottom(cg3::Segment2d(t1BottomP2, t2BottomP2));
   rightHalf.setRightp(current.getRightp());
   rightHalf.setLeftp(s.p1());

   cg3::Segment2d innerSegment = cg3::Segment2d(s.p1(), geoutils::calculateYCoord(s, rightHalf.getRightp().x()));

   /*Case when the segment START in this trapezoid and end in the neighbors*/
   if (s.p1().x() < current.getTop().p2().x() and s.p2().x() > current.getTop().p2().x()){
        std::vector<Trapezoid> hSplitResult = this->SplitHorizontaly(rightHalf, innerSegment);

        size_t idLeft = this->addTrapezoid(leftHalf);
        idsTrapezoid.push_back(idLeft);

        size_t idRightUpper = this->addTrapezoid(hSplitResult[0]);
        idsTrapezoid.push_back(idRightUpper);

        size_t idRightLower = this->addTrapezoid(hSplitResult[1]);
        idsTrapezoid.push_back(idRightLower);

        Trapezoid& referenceLeft = trapezoid(idLeft);
        Trapezoid& referenceRightU= trapezoid(idRightUpper);
        Trapezoid& referenceRightL = trapezoid(idRightLower);
        /*Update of neighbors with the new trapezoids*/
        referenceLeft.updateLeftNeighbors(current);
        referenceLeft.updateRightNeighbors(referenceRightU, referenceRightL);

        referenceRightU.updateRightNeighbors(current);
        referenceRightL.updateRightNeighbors(current);
        referenceRightU.updateLeftNeighbors(referenceLeft);
        referenceRightL.updateLeftNeighbors(referenceLeft);

        /*Update of the neighborhod*/
        neighbor.updateLeftNeighbors(referenceRightU, referenceRightL);


   /*Case when the segment END in this trapezoid and start in the neighbors*/
   }else{
       std::vector<Trapezoid> hSplitResult = this->SplitHorizontaly(leftHalf, innerSegment);
       size_t idLeftUpper = this->addTrapezoid(hSplitResult[0]);
       idsTrapezoid.push_back(idLeftUpper);

       size_t idLeftLower = this->addTrapezoid(hSplitResult[1]);
       idsTrapezoid.push_back(idLeftLower);

       size_t idRight = this->addTrapezoid(rightHalf);
       idsTrapezoid.push_back(idRight);

       Trapezoid& referenceRight = trapezoid(idRight);
       Trapezoid& referenceLeftU= trapezoid(idLeftUpper);
       Trapezoid& referenceLeftL = trapezoid(idLeftLower);

       /*Update of neighbors with the new trapezoids*/
       referenceRight.updateRightNeighbors(current);
       referenceRight.updateLeftNeighbors(referenceLeftU, referenceLeftL);

       referenceLeftU.updateRightNeighbors(referenceRight);
       referenceLeftU.updateLeftNeighbors(current);
       referenceLeftL.updateRightNeighbors(referenceRight);
       referenceLeftL.updateLeftNeighbors(current);


   }

   return idsTrapezoid;
}


std::vector<Trapezoid> TrapezoidalMap::SplitHorizontaly(Trapezoid& intermediateT, cg3::Segment2d innerSegment){
    std::vector<Trapezoid> hsplitResult = std::vector<Trapezoid>();

    Trapezoid upper = Trapezoid();
    upper.setTop(intermediateT.getTop());
    upper.setBottom(innerSegment);
    upper.setLeftp(intermediateT.getLeftp());
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

void TrapezoidalMap::clear(){
    trapezoids.~vector();
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

