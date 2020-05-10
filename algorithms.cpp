#include "algorithms.h"

#define BOUNDINGBOX 1e+6

namespace Algorithms{

/*void inizializateDataStructures(TrapezoidalMap& tm, Dag& dag){
    Starting bounding box aka first trapezoid
    Trapezoid firstTrapezoid = Trapezoid();
    size_t firstTrapezoidId = tm.addTrapezoid(firstTrapezoid);
    Node firstNode = Node(leafNode, firstTrapezoidId);

};*/

void BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, std::vector<cg3::Segment2d> segmentList){
    /*Permute segments order*/
    for (size_t i = 0; i < tm.segments().size(); i++){
        BuildTrapezoidalMap(tm, dag, segmentList[i]);
    }

}

void BuildTrapezoidalMap(TrapezoidalMap& tm, Dag& dag, cg3::Segment2d segment){

    /*Validate segments*/
    geoutils::validateSegment(segment);

    tm.addSegment(segment);
    tm.addPoint(segment.p1());
    tm.addPoint(segment.p2());

    std::vector<size_t> trapsIntersected = FollowSegment(tm, dag, segment);
    std::vector<std::vector<size_t>> newTrapezoidIds = std::vector<std::vector<size_t>>();
    std::vector<size_t> elegibleForMerge = std::vector<size_t>();

    /*Performe splitting and neighbor fixing according to case*/
    for(size_t i = 0; i < trapsIntersected.size(); i++){
        Update(tm, i, trapsIntersected, newTrapezoidIds, elegibleForMerge);
    }


    /*Update Dag*/
    for(size_t i = 0; i < trapsIntersected.size(); ++i){
        Trapezoid& current = tm.trapezoid(trapsIntersected[i]);
        //dag.substituteTargetNode(dag.getRootReference(), current.node, generateSubgraph(tm, current, newTrapezoidIds[i]));
        AddSubgraphToDag(tm, dag, current, newTrapezoidIds[i]);

    }

    /*Delete old trapezoid from Trapezoidal Map*/
    for(size_t i = 0; i < trapsIntersected.size(); ++i){
        //tm.trapezoid(trapsIntersected[i]).node->~Node();
        tm.removeTrapezoid(trapsIntersected[i]);
    }



};


void Update(TrapezoidalMap& tm, const size_t& i, std::vector<size_t>& trapsIntersected, std::vector<std::vector<size_t>>& newTrapezoidIds, std::vector<size_t>& elegibleForMerge){
    Trapezoid& currentT = tm.trapezoid(trapsIntersected[i]);
    cg3::Segment2d s = tm.getLastSegment();

    if(trapsIntersected.size() == 1){
        tm.HandleCaseSegmentInside(trapsIntersected, newTrapezoidIds);
    }else{
        if (s.p1() > currentT.leftp() and s.p2() > currentT.rightp()){
            tm.HandleCaseP1Inside(trapsIntersected[i], trapsIntersected[i+1], elegibleForMerge, newTrapezoidIds);
        }else if(s.p1() < currentT.leftp() and s.p2() > currentT.rightp()){
            //Trapezoid& nextT = tm.trapezoid(trapsIntersected[i+1]);
            //tm.HandleCasePointsOutside(currentT, nextT, elegibleForMerge, newTrapezoidIds);
        }else{
            tm.HandleCaseQ1Inside(trapsIntersected[i], elegibleForMerge, newTrapezoidIds);
        }
    }

}



void AddSubgraphToDag(TrapezoidalMap& tMap, Dag& dag, Trapezoid& current, std::vector<size_t>& idsTrapezoid){
    cg3::Segment2d s = tMap.getLastSegment();
    std::vector<size_t> idsNode = std::vector<size_t>();
    if(idsTrapezoid.size() == 4){

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

        //s1->setLeftRfr(tMap.trapezoid(idsTrapezoid[1]).nodeReference());
        //s1->setRightRfr(tMap.trapezoid(idsTrapezoid[2]).nodeReference());
        //q1->setLeftRfr(s1);
        //q1->setRightRfr(tMap.trapezoid(idsTrapezoid[3]).nodeReference());
        //p1->setLeftRfr(tMap.trapezoid(idsTrapezoid[0]).nodeReference());
        //p1->setRightRfr(q1);
        //
        //target->swap(p1);

    }else{
        /*Case p1 only inside trapezoid*/
        if (s.p1() > current.leftp() and s.p2() > current.rightp()){
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

            //p1->setLeft(tMap.trapezoid(idsTrapezoid[0]).nodeReference());
            //s1->setLeft(tMap.trapezoid(idsTrapezoid[1]).nodeReference());
            //s1->setRight(tMap.trapezoid(idsTrapezoid[2]).nodeReference());
            //p1->setRight(s1);
            //target->swap(p1);
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
                }
                idsNode.push_back(tMap.trapezoid(idsTrapezoid[i]).node());
            }

            dag.addChildrenToNode(idS1, idsNode[0], idsNode[1]);
            //s1->setLeft(tMap.trapezoid(idsTrapezoid[0]).nodeReference());
            //s1->setRight(tMap.trapezoid(idsTrapezoid[1]).nodeReference());
            //
            //target->swap(s1);
         /*Case q1 only inside trapezoid*/
        }else{
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
                }
                idsNode.push_back(tMap.trapezoid(idsTrapezoid[i]).node());
            }

            dag.addChildrenToNode(idS1, idsNode[1], idsNode[2]);
            dag.addChildrenToNode(idQ1, idS1, idsNode[0]);

            //s1->setLeft(tMap.trapezoid(idsTrapezoid[1]).nodeReference());
            //s1->setRight(tMap.trapezoid(idsTrapezoid[2]).nodeReference());
            //q1->setLeft(s1);
            //q1->setRight(tMap.trapezoid(idsTrapezoid[0]).nodeReference());
            //target->swap(q1);
        }
    }

}



size_t QueryPoint(TrapezoidalMap& tm, Dag& dag, cg3::Point2d& point){
    Node tmp = dag.root();
    size_t id = 0;
    while(tmp.type() != leafNode){
        if(tmp.type() == xNode){
            if (point.x() < tm.point(tmp.id()).x()){
                tmp = dag.node(tmp.left());
                id = tmp.id();
            }else{
                tmp = dag.node(tmp.right());
                id = tmp.id();
            }
        }
        if(tmp.type() == yNode){
            if(geoutils::isPointAbove(point, tm.segment(tmp.id()))){ //check if a poin is above a segment function
               tmp = dag.node(tmp.left());
               id = tmp.id();
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
    size_t idDelta = QueryPoint(tMap, dag, p1);

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
