#include "algorithms.h"

#define BOUNDINGBOX 1e+6

namespace Algorithms{
/**
 * @brief BuildTrapezoidalMap, performe the incremental step.
 * Given the tMap, the dag and a segment, validate the segment (if the p1 x is bigger than p2 swap p1 and p2),
 * insert the segment in the trapezoidal map obtaining the ids of the trapezoid intersected by that segment, and for each trapezoid
 * intersected check the configuration segment/current trapezoid to decide which type of case is and do the update accordingly to that.
 * The update is done first in the trapezoidal map, doing the split and the eventual merge of the trapezoids, and then using the vector
 * lastTrapezoidsInserted to update the dag.
 * @param tMap
 * @param dag
 * @param segment, the segment inserted in the trapezoidal map.
 * @note degenerative can become true only if is intersected only 1 trapezoid and the one point of the current trapezoid is shared with the segment
 * check ifs
 */
void buildTrapezoidalMap(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d segment){

    /*Validate segments*/
    geoutils::validateSegment(segment);

    std::vector<Node> nodes = std::vector<Node>();
    bool degenerative = false;
    std::vector<size_t> trapsIntersected = std::vector<size_t>();
    std::vector<size_t> lastTrapezoidsInserted = std::vector<size_t>();

    followSegment(tMap, dag, segment, trapsIntersected);

    nodes.push_back(Node(xNode, tMap.addPoint(segment.p1())));
    nodes.push_back(Node(yNode, tMap.addSegment(segment)));
    nodes.push_back(Node(xNode, tMap.addPoint(segment.p2())));

    /*Performe splitting and neighbor fixing according to case*/
    for(size_t i = 0; i < trapsIntersected.size(); i++){
        Trapezoid currentT = tMap.trapezoidcpy(trapsIntersected[i]);
        /*DEGENERATIVE: P1 is INSIDE currentT, Q1 is EQUALT to rightp, 1 Trapezoid Intersected*/
        if (segment.p1().x() > currentT.leftp().x() and (geoutils::arePointsEqual(segment.p2(),currentT.top().p2()) or geoutils::arePointsEqual(segment.p2(),currentT.bottom().p2()) or geoutils::arePointsEqual(segment.p2(), currentT.rightp()))){
            degenerative = true;
            tMap.HandleCaseSegmentInsideDegenerativeRight(trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            degenerative = false;
            /*DEGENERATIVE: P1 is EQUAL leftp, P2 is INSIDE currentT, 1 Trapezoid Intersected*/
        }else if((geoutils::arePointsEqual(segment.p1(),currentT.top().p1()) or geoutils::arePointsEqual(segment.p1(),currentT.bottom().p1()) or geoutils::arePointsEqual(segment.p1(), currentT.leftp())) and segment.p2().x() < currentT.rightp().x()){
            degenerative = true;
            tMap.HandleCaseSegmentInsideDegenerativeLeft(trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            degenerative = false;
        }else if(segment.p1().x() < currentT.leftp().x() and geoutils::arePointsEqual(segment.p2(), currentT.rightp())){
            tMap.HandleCaseQ1InsideDegenerative(trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
        }else if(geoutils::arePointsEqual(segment.p1().x(),currentT.leftp().x()) and segment.p2().x() > currentT.rightp().x()){
            tMap.HandleCaseP1InsideDegenerative(trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
        }
        /*P1 is INSIDE currentT, Q1 is INSIDE currentT*/
        else if(segment.p1().x() >= currentT.leftp().x() and segment.p2().x() <= currentT.rightp().x()){
            tMap.HandleCaseSegmentInside(trapsIntersected[0], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            /*P1 is INSIDE currentT, Q1 is OUTSIDE currentT*/
        }else if (segment.p1().x() > currentT.leftp().x() and segment.p2().x() > currentT.rightp().x()){
            tMap.HandleCaseP1Inside(trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            /*P1 is OUTSIDE currentT, Q1 is OUTSIDE currentT*/
        }else if(segment.p1().x() < currentT.leftp().x() and segment.p2().x() > currentT.rightp().x()){
            tMap.HandleCasePointsOutside(trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            /*P1 is OUTSIDE currentT, Q1 is INSIDE currentT*/
        }else if(segment.p1().x() < currentT.leftp().x() and segment.p2().x() < currentT.rightp().x()){
            tMap.HandleCaseQ1Inside(trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
        }
    }


};

/**
 * \brief Creates a subgraph depending by the case and insert it in place of the current trapezoid node.
 * Performe the update of the dag, generating the subgraph correspondent to the case and substituing the root of the subgraph
 * in place of the current trapezoid intersected. Using the same id the fathers node of the removed one stil point to the correct
 * node without need of performing extra operations.
 * @param tMap
 * @param dag
 * @param current, the trapezoid intersected by the segment that needs to be removed from the dag, the remove is done in one step.
 * @param s, the segment inserted in the trapezoidal map.
 * @param nodes, a vector that holds the nodes of the segment and his endpoints. (The order is: P1, S1, Q1)
 * @param idsTrapezoid, the ids of the new inserted trapezoids, will be used to create for each one of them a leaf node
 * and set their crosslink.
 * @param degenerative, boolean used to handle the degenerative cases. Handle the update of the dag when one of the segment's
 * endpoint is equal to rightp or leftp of the intersected trapezoid.
 * @return Dag updated with the new trapezoids and current removed from dag.
 */
void performeDagUpdate(TrapezoidalMap& tMap, Dag& dag, const Trapezoid& current, const cg3::Segment2d s, const std::vector<Node>& nodes,  std::vector<size_t>& idsTrapezoid, bool degenerative){
    /*Degenerate case*/
    if(degenerative){
        size_t idP1 = dag.replace(current.node(), nodes[0]);
        size_t idS1 = dag.addNode(nodes[1]);
        size_t idQ1 = dag.addNode(nodes[2]);

        /*LeafNode generation and update of crosslink to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
        }

        if ((geoutils::arePointsEqual(s.p1(),current.top().p1()) or geoutils::arePointsEqual(s.p1(),current.bottom().p1()) or geoutils::arePointsEqual(s.p1(), current.leftp())) and s.p2().x() < current.rightp().x()){
            dag.addChildrenToNode(idP1, SIZE_MAX, idQ1);
            dag.addChildrenToNode(idQ1, idS1, tMap.trapezoid(idsTrapezoid[0]).node());
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
        }else{
            dag.addChildrenToNode(idP1, tMap.trapezoid(idsTrapezoid[0]).node(), idQ1);
            dag.addChildrenToNode(idQ1, idS1, SIZE_MAX);
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
        }

    }
    /*Case p1 and q1 inside trapezoid*/
    else if(s.p1() >= current.leftp() and s.p2() <= current.rightp()){
        size_t idP1 = dag.replace(current.node(), nodes[0]);
        size_t idS1 = dag.addNode(nodes[1]);
        size_t idQ1 = dag.addNode(nodes[2]);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
        }
        dag.addChildrenToNode(idP1, tMap.trapezoid(idsTrapezoid[0]).node(), idQ1);
        dag.addChildrenToNode(idQ1, idS1, tMap.trapezoid(idsTrapezoid[3]).node());
        dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());

    }else if (s.p1() >= current.leftp() and s.p2() > current.rightp()){ //Case p1 inside trapezoid
        size_t idP1 = dag.replace(current.node(), nodes[0]);
        size_t idS1 = dag.addNode(nodes[1]);
        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
        }
        if(idsTrapezoid.size() == 2){
            dag.addChildrenToNode(idP1, SIZE_MAX, idS1);
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[0]).node(), tMap.trapezoid(idsTrapezoid[1]).node());
        }else{
            dag.addChildrenToNode(idP1, tMap.trapezoid(idsTrapezoid[0]).node(), idS1);
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
        }

        /*Case p1 and q1 outside trapezoid*/
    }else if(s.p1() < current.leftp() and s.p2() > current.rightp()){
        size_t idS1 = dag.replace(current.node(), nodes[1]);
        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node() == SIZE_MAX){
                tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
            }
        }
        dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[0]).node(), tMap.trapezoid(idsTrapezoid[1]).node());
        /*Case q1 only inside trapezoid*/
    }else if(s.p1() < current.leftp() and s.p2() <= current.rightp()){
        size_t idQ1 = dag.replace(current.node(), nodes[2]);
        size_t idS1 = dag.addNode(nodes[1]);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node() == SIZE_MAX){
                tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
            }
        }
        if(idsTrapezoid.size() == 2){
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[0]).node(), tMap.trapezoid(idsTrapezoid[1]).node());
            dag.addChildrenToNode(idQ1, idS1, SIZE_MAX);
        }else{
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
            dag.addChildrenToNode(idQ1, idS1, tMap.trapezoid(idsTrapezoid[0]).node());
        }
    }

}


/**
 * @brief queryPoint performe the point location of a given point p1 returning the id of the trapezoid where the point is located.
 * Using the searching structure dag, search for a point p1 until a leafNode is found. If the current node is a xNode checks
 * his x comparing it with the point to decide the turn, if it is a yNode checks if the point is above or below the segment, in case
 * the point and the segment are collinear (Degenerate case) uses the second point q1 to decide the turn. When a leafNode is found
 * return his id.
 * @note In case of single point location p1 == q1
 * @param tm
 * @param dag
 * @param p1, point to query
 * @param q1, second point to query in case the first point is equal to another point already in the map.
 * @return The id of the trapezoid where the point is located.
 */
size_t queryPoint(const TrapezoidalMap& tm, const Dag& dag,const cg3::Point2d& p1,const cg3::Point2d& q1){
    Node tmp = dag.root();
    while(tmp.type() != leafNode){
        if(tmp.type() == xNode){
            if (p1.x() < tm.point(tmp.id()).x()){
                tmp = dag.node(tmp.left());
            }else{
                tmp = dag.node(tmp.right());
            }
        }
        if(tmp.type() == yNode){
            if(geoutils::isPointAbove(p1, tm.segment(tmp.id())) > 0){ //check if a poin is above a segment function
                tmp = dag.node(tmp.left());
            }else if(geoutils::isPointAbove(p1, tm.segment(tmp.id())) == -1){
                if(geoutils::isPointAbove(q1, tm.segment(tmp.id()))){
                    tmp = dag.node(tmp.left());
                }else{
                    tmp = dag.node(tmp.right());
                }
            }else{
                tmp = dag.node(tmp.right());
            }
        }

    }
    return tmp.id();
};


/**
 * @brief followSegment, return a list of trapezoid intersected by the segment s.
 * The first trapezoid is obtained using the point location function query point, from it the others are obtained looking to
 * his right neighbors.
 * @param tMap
 * @param dag
 * @param s, segment that is getting inserted in the trapezoidalMap.
 * @return fill the list trapezoidsIntersected with the trapezoids intersected by the segment s.
 */
void followSegment(const TrapezoidalMap& tMap, const Dag& dag, const cg3::Segment2d& s, std::vector<size_t>& trapezoidsIntersected){
    trapezoidsIntersected.push_back(queryPoint(tMap, dag, s.p1(), s.p2()));
    int j = 0;
    while (s.p2().x() > tMap.trapezoid(trapezoidsIntersected[j]).rightp().x()){
        if (geoutils::isPointAbove(tMap.trapezoid(trapezoidsIntersected[j]).rightp(), s)){
            trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).lowerRightNeighbor());
        }else{
            trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).upperRightNeighbor());
        }
        j++;
    }
}

}
