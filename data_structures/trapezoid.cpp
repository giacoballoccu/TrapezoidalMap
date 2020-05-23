#include "trapezoid.h"
#define BOUNDINGBOX 1e+6

/**
 * @brief Trapezoid Constructor
 * Return a trapezoid of the size of the boundingbox with no neighbors and node.
 */
Trapezoid::Trapezoid(){
    _top = cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX), cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX));
    _bottom = cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX), cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX));
    _leftp = _top.p1();
    _rightp = _top.p2();
    _upperLeftNeighbor = SIZE_MAX;
    _lowerLeftNeighbor = SIZE_MAX;
    _upperRightNeighbor = SIZE_MAX;
    _lowerRightNeighbor = SIZE_MAX;
    _node = SIZE_MAX;
}
/**
 * @brief Trapezoid::~Trapezoid
 */
Trapezoid::~Trapezoid(){
    _top.~Segment();
    _bottom.~Segment();
    _rightp.~Point2();
    _leftp.~Point2();
};

/*Getter*/
const cg3::Segment2d& Trapezoid::top() const {
    return _top;
}
const cg3::Segment2d& Trapezoid::bottom() const{
    return _bottom;
}
const cg3::Point2d& Trapezoid::leftp() const{
    return _leftp;
}
const cg3::Point2d& Trapezoid::rightp() const{
    return _rightp;
}

size_t Trapezoid::lowerRightNeighbor() const{
    return _lowerRightNeighbor;
};
size_t Trapezoid::lowerLeftNeighbor() const{
    return _lowerLeftNeighbor;
};
size_t Trapezoid::upperLeftNeighbor() const{
    return _upperLeftNeighbor;
};
size_t Trapezoid::upperRightNeighbor() const{
    return _upperRightNeighbor;
};
/**
 * @brief Return the id of the node associate to this trapezoid.
 */
size_t Trapezoid::node() const{
    return _node;
}
/**
 * @brief Return the verices of the trapezoid in CLOCKWISE ORDER.
 */
const std::array<cg3::Point2d, 4> Trapezoid::getVertices() const{
    std::array<cg3::Point2d, 4> vertices = {_top.p1(), _top.p2(),  _bottom.p2(), _bottom.p1()};
    return vertices;
};

/*Setter*/
void Trapezoid::setTop(const cg3::Segment2d& s){
    _top = s;
};
void Trapezoid::setTopP1(cg3::Point2d p){
    _top.setP1(p);
};
void Trapezoid::setTopP2(cg3::Point2d p){
    _top.setP2(p);
};
void Trapezoid::setBottom(const cg3::Segment2d& s){
    _bottom = s;
};
void Trapezoid::setBottomP1(cg3::Point2d p){
    _bottom.setP1(p);
};
void Trapezoid::setBottomP2(cg3::Point2d p){
    _bottom.setP2(p);
};
void Trapezoid::setLeftp(const cg3::Point2d& p){
    _leftp = p;
};
void Trapezoid::setRightp(const cg3::Point2d& p){
    _rightp = p;
};

void Trapezoid::setNode(const size_t& nodeId){
    _node = nodeId;
}

/**
 * \brief Given one id, update the current instance lowerLeft neighbors.
 * Given one ids, update the current instance's lowerLeft with the id.
 */
void Trapezoid::updateLLNeighbor(const size_t& idLowerLeft){
    _lowerLeftNeighbor = idLowerLeft;
}
/**
 * \brief Given one id, update the current instance upperLeft neighbors.
 * Given one ids, update the current instance's upperLeft with the id.
 */
void Trapezoid::updateULNeighbor(const size_t& idUpperLeft){
    _upperLeftNeighbor = idUpperLeft;
}
/**
 * \brief Given one id, update the current instance lowerRight neighbors.
 * Given one ids, update the current instance's lowerRight with the id.
 */
void Trapezoid::updateLRNeighbor(const size_t& idLowerRight){
    _lowerRightNeighbor = idLowerRight;
}
/**
 * \brief Given one id, update the current instance upperRight neighbors.
 * Given one ids, update the current instance's upperRight with the id.
 */
void Trapezoid::updateURNeighbor(const size_t& idUpperRight){
    _upperRightNeighbor = idUpperRight;
}
/**
 * \brief Given one id, update the current instance left neighbors.
 * Given one ids, update the current instance's upperLeft and lowerLeft with the same id.
 */
void Trapezoid::updateLeftNeighbors(const size_t& idShared){
    _upperLeftNeighbor = idShared;
    _lowerLeftNeighbor = idShared;
};
/**
 * \brief Given one id, update the current instance right neighbors.
 * Given one ids, update the current instance's upperRight and lowerRight with the same id.
 */
void Trapezoid::updateRightNeighbors(const size_t& idShared){
    _upperRightNeighbor = idShared;
    _lowerRightNeighbor = idShared;
};
/**
 * \brief Given two ids, update the current instance left neighbors.
 * Given two ids, update the current instance's upperLeft and lowerLeft respectively with the first id and the second id.
 */
void Trapezoid::updateLeftNeighbors(const size_t& idUpperLeft, const size_t& idLowerLeft){
    _upperLeftNeighbor = idUpperLeft;
    _lowerLeftNeighbor = idLowerLeft;
};
/**
 * \brief Given two ids, update the current instance right neighbors.
 * Given two ids, update the current instance's upperRight and lowerRight respectively with the first id and the second id.
 */
void Trapezoid::updateRightNeighbors(const size_t& idUpperRight, const size_t& idLowerRight){
    _upperRightNeighbor = idUpperRight;
    _lowerRightNeighbor = idLowerRight;
};
/**
 * \brief Given a trapezoid, update the current instance left neighbors.
 * Given a trapezoid, set the rightNeighbors of the trapezoid as the LeftNeigbors of the current instance.
 * @param old, the trapezoid from where extract the new upperLeft and lowerLeft for the current instance.
 */
void Trapezoid::updateLeftNeighborsOld(Trapezoid& old){
    _upperLeftNeighbor = old._upperLeftNeighbor;
    _lowerLeftNeighbor = old._lowerLeftNeighbor;
};
/**
 * \brief Given a trapezoid, update the current instance right neighbors.
 * Given a trapezoid, set the rightNeighbors of the trapezoid as the rightNeigbors of the current instance.
 * @param old, the trapezoid from where extract the new upperRight and lowerRight for the current instance.
 */
void Trapezoid::updateRightNeighborsOld(Trapezoid& old){
    _upperRightNeighbor = old._upperRightNeighbor;
    _lowerRightNeighbor = old._lowerRightNeighbor;
};

/**
 * \brief Given a trapezoid, and two ids, update the current instace neighbors.
 * Given a trapezoid, and two ids, set the rightNeighbors of the trapezoid as the rightNeigbors of the current instance and uses the ids
 * to update respectively his upperLeft neighbor and his lowerLeft neighbor
 * @param old, the trapezoid from where extract the new upperLeft and lowerLeft for the current instance.
 * @param idSharedLeft, id of the trapezoid that will be upperLeft and lowerLeft of the trapezoid.
 * @param idSharedRight, id of the trapezoid that will be upperRight and lowerRight of the trapezoid.
 */
void Trapezoid::updateNeighborsLeft(const Trapezoid& old, const size_t& idUpperLeft, const size_t& idLowerLeft){
    _upperRightNeighbor = old._upperRightNeighbor;
    _lowerRightNeighbor = old._lowerRightNeighbor;
    _upperLeftNeighbor = idUpperLeft;
    _lowerLeftNeighbor = idLowerLeft;
};
/**
 * \brief Given a trapezoid, and two ids, update the current instance neighbors.
 * Given a trapezoid, and two ids, set the leftNeighbors of the trapezoid as the leftNeigbors of the current instance and uses the ids
 * to update respectively his upperRight neighbor and his lowerRight neighbor.
 * @param old, the trapezoid from where extract the new upperLeft and lowerLeft for the current instance.
 * @param idSharedLeft, id of the trapezoid that will be upperLeft and lowerLeft of the trapezoid.
 * @param idSharedRight, id of the trapezoid that will be upperRight and lowerRight of the trapezoid.
 */
void Trapezoid::updateNeighborsRight(const Trapezoid& old, const size_t& idUpperRight, const size_t& idLowerRight){
    _upperLeftNeighbor = old._upperLeftNeighbor;
    _lowerLeftNeighbor = old._lowerLeftNeighbor;
    _upperRightNeighbor = idUpperRight;
    _lowerRightNeighbor = idLowerRight;
};
/**
 * \brief Given two trapezoid, update the current instance neighbors.
 * Given two trapezoid, set the first one as the left neighbors and the second one as the right neighbors of the current instance.
 * @param idSharedLeft, id of the trapezoid that will be upperLeft and lowerLeft of the trapezoid.
 * @param idSharedRight, id of the trapezoid that will be upperRight and lowerRight of the trapezoid.
 */
void Trapezoid::updateNeighbors(const size_t idSharedLeft, const size_t idSharedRight){
    _upperLeftNeighbor = idSharedLeft;
    _lowerLeftNeighbor = idSharedLeft;
    _upperRightNeighbor = idSharedRight;
    _lowerRightNeighbor = idSharedRight;
}

/**
 * \brief Check if the current instance has still references in the left neighbors to an oldId and replace them with idNewT.
 * @param oldId, id of the trapezoid we want to replace from the left neighbors of the current instance.
 * @param newId, id of the trapezoid that will replace oldId in the left neighbors.
 */
void Trapezoid::indirectUpdateNeighborsLeft(const size_t &oldId,const size_t &newId){
    if (this->upperLeftNeighbor() == oldId){
        this->updateULNeighbor(newId);
    }
    if(this->lowerLeftNeighbor() == oldId){
        this->updateLLNeighbor(newId);
    }
}
/**
 * \brief Check if the current instance has still references in the right neighbors to an oldId and replace them with idNewT.
 * @param oldId, id of the trapezoid we want to replace from the right neighbors of the current instance.
 * @param newId, id of the trapezoid that will replace oldId in the right neighbors.
 */
void Trapezoid::indirectUpdateNeighborsRight(const size_t &oldId,const size_t &idNewT){
    if (this->upperRightNeighbor() == oldId){
        this->updateURNeighbor(idNewT);
    }
    if(this->lowerRightNeighbor() == oldId){
        this->updateLRNeighbor(idNewT);
    }
}
