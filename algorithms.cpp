#include "algorithms.h"

#define BOUNDINGBOX 1e+6

namespace Algorithms{

void BuildTrapezoidalMap(TrapezoidalMap& tm, Dag& dag, cg3::Segment2d segment){

    /*Validate segments*/
    geoutils::validateSegment(segment);

    tm.addSegment(segment);
    tm.addPoint(segment.p1());
    tm.addPoint(segment.p2());

    bool degenerative = false;
    std::vector<size_t> trapsIntersected = FollowSegment(tm, dag, segment);
    std::vector<size_t> lastTrapezoidsInserted = std::vector<size_t>();
    std::vector<size_t> elegibleForMerge = std::vector<size_t>();

    /*Performe splitting and neighbor fixing according to case*/
    for(size_t i = 0; i < trapsIntersected.size(); i++){
        Trapezoid currentT = tm.trapezoidcpy(trapsIntersected[i]);
        /*DEGENERATIVE: P1 is INSIDE currentT, Q1 is EQUALT to rightp, 1 Trapezoid Intersected*/
        if (segment.p1().x() > currentT.leftp().x() and (geoutils::arePointsEqual(segment.p2(),currentT.top().p2()) or geoutils::arePointsEqual(segment.p2(),currentT.bottom().p2()) or geoutils::arePointsEqual(segment.p2(), currentT.rightp()))){
            degenerative = true;
            lastTrapezoidsInserted = tm.HandleCaseSegmentInsideDegenerativeRight(trapsIntersected[i]);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
            degenerative = false;
        /*DEGENERATIVE: P1 is EQUAL leftp, P2 is INSIDE currentT, 1 Trapezoid Intersected*/
        }else if((geoutils::arePointsEqual(segment.p1(),currentT.top().p1()) or geoutils::arePointsEqual(segment.p1(),currentT.bottom().p1()) or geoutils::arePointsEqual(segment.p1(), currentT.leftp())) and segment.p2().x() < currentT.rightp().x()){
            degenerative = true;
            lastTrapezoidsInserted = tm.HandleCaseSegmentInsideDegenerativeLeft(trapsIntersected[i]);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
            degenerative = false;
        }else if(segment.p1().x() < currentT.leftp().x() and segment.p2() == currentT.rightp()){
            degenerative = true;
            lastTrapezoidsInserted = tm.HandleCaseQ1InsideDegenerative(trapsIntersected[i], elegibleForMerge);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
            degenerative = false;
        }else if(segment.p1().x() == currentT.leftp().x() and segment.p2().x() > currentT.rightp().x()){
            degenerative = true;
            lastTrapezoidsInserted = tm.HandleCaseP1InsideDegenerative(trapsIntersected[i], elegibleForMerge);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
            degenerative = false;
        }
        /*P1 is INSIDE currentT, Q1 is INSIDE currentT*/
        else if(segment.p1().x() >= currentT.leftp().x() and segment.p2().x() <= currentT.rightp().x()){
            lastTrapezoidsInserted = tm.HandleCaseSegmentInside(trapsIntersected[0]);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
        /*P1 is INSIDE currentT, Q1 is OUTSIDE currentT*/
        }else if (segment.p1().x() > currentT.leftp().x() and segment.p2().x() > currentT.rightp().x()){
            lastTrapezoidsInserted = tm.HandleCaseP1Inside(trapsIntersected[i], elegibleForMerge);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
        /*P1 is OUTSIDE currentT, Q1 is OUTSIDE currentT*/
        }else if(segment.p1().x() < currentT.leftp().x() and segment.p2().x() > currentT.rightp().x()){
            lastTrapezoidsInserted = tm.HandleCasePointsOutside(trapsIntersected[i], elegibleForMerge, lastTrapezoidsInserted);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
        /*P1 is OUTSIDE currentT, Q1 is INSIDE currentT*/
        }else if(segment.p1().x() < currentT.leftp().x() and segment.p2().x() < currentT.rightp().x()){
            lastTrapezoidsInserted = tm.HandleCaseQ1Inside(trapsIntersected[i], elegibleForMerge);
            AddSubgraphToDag(tm, dag, currentT, lastTrapezoidsInserted, degenerative);
        }else{
            float debug = 1.000;
        }
    }
};


void AddSubgraphToDag(TrapezoidalMap& tMap, Dag& dag, Trapezoid& current, std::vector<size_t>& idsTrapezoid, bool degenerative){
    cg3::Segment2d s = tMap.getLastSegment();
    std::vector<size_t> idsNode = std::vector<size_t>();
    /*Degenerate case*/
    if(degenerative){
        Node p1 = Node(xNode, tMap.getIdP1LastSegment());
        Node s1 = Node(yNode, tMap.getIdLastSegment());
        Node q1 = Node(xNode, tMap.getIdQ1LastSegment());

        size_t idP1 = dag.replace(current.node(), p1);
        size_t idS1 = dag.addNode(s1);
        size_t idQ1 = dag.addNode(q1);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            Node leaf = Node(leafNode, idsTrapezoid[i]);
            idsNode.push_back(dag.addNode(leaf));
            tMap.trapezoid(idsTrapezoid[i]).setNode(idsNode[i]);
        }
        if(idsNode.size() == 2){
            dag.addChildrenToNode(idP1, SIZE_MAX, idQ1);
            dag.addChildrenToNode(idQ1, idS1, SIZE_MAX);
            dag.addChildrenToNode(idS1, idsNode[0], idsNode[1]);
        }else{
            if ((geoutils::arePointsEqual(s.p1(),current.top().p1()) or geoutils::arePointsEqual(s.p1(),current.bottom().p1()) or geoutils::arePointsEqual(s.p1(), current.leftp())) and s.p2().x() < current.rightp().x()){
                dag.addChildrenToNode(idP1, SIZE_MAX, idQ1);
                dag.addChildrenToNode(idQ1, idS1, idsNode[0]);
                dag.addChildrenToNode(idS1, idsNode[1], idsNode[2]);
            }else{
                dag.addChildrenToNode(idP1, idsNode[0], idQ1);
                dag.addChildrenToNode(idQ1, idS1, SIZE_MAX);
                dag.addChildrenToNode(idS1, idsNode[1], idsNode[2]);
            }
        }
    }
    /*Case p1 and q1 inside trapezoid*/
    else if(s.p1() > current.leftp() and s.p2() < current.rightp()){
        Node p1 = Node(xNode, tMap.getIdP1LastSegment());
        Node s1 = Node(yNode, tMap.getIdLastSegment());
        Node q1 = Node(xNode, tMap.getIdQ1LastSegment());

        size_t idP1 = dag.replace(current.node(), p1);
        size_t idS1 = dag.addNode(s1);
        size_t idQ1 = dag.addNode(q1);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            Node leaf = Node(leafNode, idsTrapezoid[i]);
            idsNode.push_back(dag.addNode(leaf));
            tMap.trapezoid(idsTrapezoid[i]).setNode(idsNode[i]);
        }
            dag.addChildrenToNode(idP1, idsNode[0], idQ1);
            dag.addChildrenToNode(idQ1, idS1, idsNode[3]);
            dag.addChildrenToNode(idS1, idsNode[1], idsNode[2]);

    }else if (s.p1() > current.leftp() and s.p2() > current.rightp()){ //Case p1 inside trapezoid
        Node p1 = Node(xNode, tMap.getIdP1LastSegment());
        Node s1 = Node(yNode, tMap.getIdLastSegment());


        size_t idP1 = dag.replace(current.node(), p1);
        size_t idS1 = dag.addNode(s1);
        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            Node leaf = Node(leafNode, idsTrapezoid[i]);
            idsNode.push_back(dag.addNode(leaf));
            tMap.trapezoid(idsTrapezoid[i]).setNode(idsNode[i]);
        }

        dag.addChildrenToNode(idP1, idsNode[0], idS1);
        dag.addChildrenToNode(idS1, idsNode[1], idsNode[2]);
    /*Case p1 and q1 outside trapezoid*/
    }else if(s.p1() < current.leftp() and s.p2() > current.rightp()){
        Node s1 = Node(yNode, tMap.getIdLastSegment());

        size_t idS1 = dag.replace(current.node(), s1);
         /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node() == SIZE_MAX){
                Node leaf = Node(leafNode, idsTrapezoid[i]);
                idsNode.push_back(dag.addNode(leaf));
                tMap.trapezoid(idsTrapezoid[i]).setNode(idsNode[i]);
            }else{
                idsNode.push_back(tMap.trapezoid(idsTrapezoid[i]).node());
            }

        }
        dag.addChildrenToNode(idS1, idsNode[0], idsNode[1]);
     /*Case q1 only inside trapezoid*/
    }else if(s.p1() < current.leftp() and s.p2() < current.rightp()){
        Node s1 = Node(yNode,tMap.getIdLastSegment());
        Node q1 = Node(xNode, tMap.getIdQ1LastSegment());

        size_t idQ1 = dag.replace(current.node(), q1);
        size_t idS1 = dag.addNode(s1);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node() == SIZE_MAX){
                Node leaf = Node(leafNode, idsTrapezoid[i]);
                idsNode.push_back(dag.addNode(leaf));
                tMap.trapezoid(idsTrapezoid[i]).setNode(idsNode[i]);
            }else{
                idsNode.push_back(tMap.trapezoid(idsTrapezoid[i]).node());
            }
        }

        dag.addChildrenToNode(idS1, idsNode[1], idsNode[2]);
        dag.addChildrenToNode(idQ1, idS1, idsNode[0]);
    }

}



size_t QueryPoint(TrapezoidalMap& tm, Dag& dag, cg3::Point2d& p1, cg3::Point2d& q1){
    Node tmp = dag.root();
    size_t id = 0;
    while(tmp.type() != leafNode){
        if(tmp.type() == xNode){
            if (p1.x() < tm.point(tmp.id()).x()){
                tmp = dag.node(tmp.left());
                id = tmp.id();
            }else{
                tmp = dag.node(tmp.right());
                id = tmp.id();
            }
        }
        if(tmp.type() == yNode){
            if(geoutils::isPointAbove(p1, tm.segment(tmp.id())) > 0){ //check if a poin is above a segment function
               tmp = dag.node(tmp.left());
               id = tmp.id();
            }else if(geoutils::isPointAbove(p1, tm.segment(tmp.id())) == -1){
                if(geoutils::isPointAbove(q1, tm.segment(tmp.id()))){
                    tmp = dag.node(tmp.left());
                    id = tmp.id();
                }else{
                    tmp = dag.node(tmp.right());
                    id = tmp.id();
                }
            }else{
                tmp = dag.node(tmp.right());
                id = tmp.id();
            }
        }

    }
    return id;
};


/*

Follow Segment

*/

std::vector<size_t> FollowSegment(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d& s1){
    cg3::Point2d p1 = s1.p1();
    cg3::Point2d q1 = s1.p2();

    std::vector<size_t> trapezoidsIntersected = std::vector<size_t>();
    size_t idDelta = QueryPoint(tMap, dag, p1, q1);

    trapezoidsIntersected.push_back(idDelta);
    int j = 0;
    while (q1.x() > tMap.trapezoid(trapezoidsIntersected[j]).rightp().x()){
        if (geoutils::isPointAbove(tMap.trapezoid(trapezoidsIntersected[j]).rightp(), s1)){
            trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).lowerRightNeighbor());
        }else{
           trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).upperRightNeighbor());
        }
        j++;
    }
    return trapezoidsIntersected;

}

}
