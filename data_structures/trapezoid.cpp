#include "trapezoid.h"
#define BOUNDINGBOX 1e+6

Trapezoid::Trapezoid(){
    cg3::Point2d BBx1y1 = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx2y1 = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx1y2 = cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX);
    cg3::Point2d BBx2y2 = cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX);
    setTop(cg3::Segment2d(BBx1y1, BBx2y1));
    setBottom(cg3::Segment2d(BBx1y2, BBx2y2));
    setLeftp(this->top.p1());
    setRightp(this->top.p2());

    upperLeftNeighbor = nullptr;
    lowerLeftNeighbor = nullptr;
    upperRightNeighbor = nullptr;
    lowerRightNeighbor = nullptr;

}
Trapezoid::Trapezoid(cg3::Point2d p , bool left){
    cg3::Point2d BBx1y1 = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx2y1 = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx1y2 = cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX);
    cg3::Point2d BBx2y2 = cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX);
    upperLeftNeighbor = nullptr;
    lowerLeftNeighbor = nullptr;
    upperRightNeighbor = nullptr;
    lowerRightNeighbor = nullptr;

    if (left){
        cg3::Point2d BBx2y1 = cg3::Point2d(p.x(), BOUNDINGBOX);
        cg3::Point2d BBx2y2 = cg3::Point2d(p.x(), -BOUNDINGBOX);
        this->setTop(cg3::Segment2d(BBx1y1,BBx2y1));
        this->setBottom(cg3::Segment2d(BBx1y2,BBx2y2));
    }else{
        cg3::Point2d BBx1y1 = cg3::Point2d(p.x(), BOUNDINGBOX);
        cg3::Point2d BBx1y2 = cg3::Point2d(p.x(), -BOUNDINGBOX);
        this->setTop(cg3::Segment2d(BBx1y1, BBx2y1));
        this->setBottom(cg3::Segment2d(BBx1y2, BBx2y2));

    }
}

Trapezoid::Trapezoid(cg3::Segment2d s, bool above){
    cg3::Point2d BBx1y1 = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx2y1 = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx1y2 = cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX);
    cg3::Point2d BBx2y2 = cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX);

    upperLeftNeighbor = nullptr;
    lowerLeftNeighbor = nullptr;
    upperRightNeighbor = nullptr;
    lowerRightNeighbor = nullptr;

    this->setRightp(s.p1());
    this->setLeftp(s.p2());
    if (above){
        cg3::Point2d BBx1y1 = cg3::Point2d(leftp.x(), BOUNDINGBOX);
        cg3::Point2d BBx2y1 = cg3::Point2d(rightp.x(), BOUNDINGBOX);
        this->setTop(cg3::Segment2d(BBx2y1, BBx1y1));
        this->setBottom(s);

    }else{
        cg3::Point2d BBx1y2 = cg3::Point2d(leftp.x(), -BOUNDINGBOX);
        cg3::Point2d BBx2y2 = cg3::Point2d(rightp.x(), -BOUNDINGBOX);
        this->setTop(s);
        this->setBottom(cg3::Segment2d( BBx2y2, BBx1y2));
    }
};

Trapezoid::Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom){
    this->top = top;
    this->bottom = bottom;
    this->setRightp(top.p2());
    this->setLeftp(top.p1());

    upperLeftNeighbor = nullptr;
    lowerLeftNeighbor = nullptr;
    upperRightNeighbor = nullptr;
    lowerRightNeighbor = nullptr;
}

std::vector<Trapezoid> Trapezoid::SplitTrapezoid(cg3::Segment2d s){
    std::vector<Trapezoid> result = std::vector<Trapezoid>();

    Trapezoid A = Trapezoid();
    Trapezoid B = Trapezoid();
    Trapezoid C = Trapezoid();
    Trapezoid D = Trapezoid();

    A.top.setP2(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(this->getTop(), s.p1().x())));
    A.top.setP1(this->getTop().p1());
    A.bottom.setP2(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(this->getBottom(), s.p1().x())));
    A.bottom.setP1(this->getBottom().p1());
    A.rightp = s.p1();
    A.leftp = top.p1();
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



    B.top.setP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(this->top, s.p1().x())));
    B.top.setP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(this->top, s.p2().x())));
    B.bottom = s;
    B.leftp = s.p1();
    B.rightp = s.p2();
    B.lowerLeftNeighbor = &A;
    B.lowerRightNeighbor = &D;
    B.upperLeftNeighbor = &A;
    B.upperRightNeighbor = &D;

    C.top = s;
    C.bottom.setP1(cg3::Point2d(s.p1().x(), geoutils::calculateYCoord(this->bottom, s.p1().x())));
    C.bottom.setP2(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(this->bottom, s.p2().x())));
    C.leftp = s.p1();
    C.rightp = s.p2();
    C.lowerLeftNeighbor = &A;
    C.lowerRightNeighbor = &D;
    C.upperLeftNeighbor = &A;
    C.upperRightNeighbor = &D;

    D.top.setP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(this->getTop(), s.p2().x())));
    D.top.setP2(this->top.p2());
    D.bottom.setP1(cg3::Point2d(s.p2().x(), geoutils::calculateYCoord(this->getBottom(), s.p2().x())));
    D.bottom.setP2(this->bottom.p2());
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

    /*Fix they neighbor*/





    result.push_back(A);
    result.push_back(B);
    result.push_back(C);
    result.push_back(D);

    return result;
}


void Trapezoid::clear(){
    top.~Segment();
    bottom.~Segment();
    rightp.~Point2();
    leftp.~Point2();
    upperLeftNeighbor = nullptr;
    lowerLeftNeighbor = nullptr;
    upperRightNeighbor = nullptr;
    lowerRightNeighbor = nullptr;
    delete this;
};



cg3::Segment2d Trapezoid::getLeftEdge() const{
    return cg3::Segment2d(top.p1(), bottom.p1());
}

cg3::Segment2d Trapezoid::getRightEdge() const{
    return cg3::Segment2d(top.p2(), bottom.p2());
}


/*
bool isPointInside(cg3::Point2d point, Trapezoid trapezoid) {
  vector<Point> points = polygon.getPoints();
  int i, j, nvert = points.size();
  bool c = false;

  for(i = 0, j = nvert - 1; i < nvert; j = i++) {
    if( ( (points[i].y >= point.y ) != (points[j].y >= point.y) ) &&
        (point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
      )
      c = !c;
  }

  return c;
}*/



/*

Getter

*/
cg3::Segment2d Trapezoid::getTop() const{
    return top;
}
cg3::Segment2d Trapezoid::getBottom() const{
    return bottom;
}
cg3::Point2d Trapezoid::getLeftp() const{
    return leftp;
}
cg3::Point2d Trapezoid::getRightp() const{
    return rightp;
}
Trapezoid* Trapezoid::getLowerRightNeighbor() const{
    return lowerRightNeighbor;
};
Trapezoid* Trapezoid::getLowerLeftNeighbor() const{
    return lowerLeftNeighbor;
};
Trapezoid* Trapezoid::getUpperLeftNeighbor() const{
    return upperLeftNeighbor;
};
Trapezoid* Trapezoid::getUpperRightNeighbor() const{
    return upperRightNeighbor;
};

std::vector<cg3::Point2d> Trapezoid::getPoints() const{
    std::vector<cg3::Point2d> vertices = {top.p1(), top.p2(),  bottom.p2(), bottom.p1()};
    return vertices;
};

bool Trapezoid::neighborExist(std::string neighborName) const{
    if(neighborName == "upperLeftNeighbor"){
        if(upperLeftNeighbor != nullptr){
            return true;
        }else{
            return false;
        }
    }
    if(neighborName == "upperRightNeighbor"){
        if(upperRightNeighbor != nullptr){
            return true;
        }else{
            return false;
        }
    }
    if(neighborName == "lowerLeftNeighbor"){
        if(lowerLeftNeighbor != nullptr){
            return true;
        }else{
            return false;
        }
    }
    if(neighborName == "lowerRightNeighbor"){
        if(lowerRightNeighbor!= nullptr){
            return true;
        }else{
            return false;
        }
    }

    return false;

}
/*

Setter

*/
void Trapezoid::setTop(cg3::Segment2d s){
    top = s;
};
void Trapezoid::setBottom(cg3::Segment2d s){
    bottom = s;
};
void Trapezoid::setLeftp(cg3::Point2d p){
    leftp = p;
};
void Trapezoid::setRightp(cg3::Point2d p){
    rightp = p;
};
void Trapezoid::setUpperRightNeighbor(Trapezoid *t){
     upperRightNeighbor = t;
};
void Trapezoid::setUpperLeftNeighbor(Trapezoid *t){
     upperLeftNeighbor = t;
};
void Trapezoid::setLowerRightNeighbor(Trapezoid *t){
     lowerLeftNeighbor = t;
};
void Trapezoid::setLowerLeftNeighbor(Trapezoid *t){
     lowerRightNeighbor = t;
};


