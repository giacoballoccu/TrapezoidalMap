#include "algorithms.h"

#define BOUNDINGBOX 1e+6

namespace Algorithms{

void inizializateDataStructures(TrapezoidalMap& tm, Dag& dag){
    /*Starting bounding box aka first trapezoid*/
    Trapezoid firstTrapezoid = Trapezoid();
    size_t firstTrapezoidId = tm.addTrapezoid(firstTrapezoid);
    Node *firstNode = new Node(leafNode, firstTrapezoidId);

    tm.trapezoid(firstTrapezoidId).nodeReference() = firstNode;

    dag.setRoot(firstNode);
};

void BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, std::vector<cg3::Segment2d> segmentList){
    /*Permute segments order*/
    for (size_t i = 0; i < tm.getSegments().size(); i++){
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
        generateSubgraph(tm, current, newTrapezoidIds[i]);

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
        tm.HandleOneTIntersection(trapsIntersected, newTrapezoidIds);
    }else{
        if (s.p1() > currentT.getLeftp() and s.p2() > currentT.getRightp()){
            Trapezoid& nextT = tm.trapezoid(trapsIntersected[i+1]);
            tm.HandleCaseP1Inside(currentT, nextT, elegibleForMerge, newTrapezoidIds);
        }else if(s.p1() < currentT.getLeftp() and s.p2() > currentT.getRightp()){
            //Trapezoid& nextT = tm.trapezoid(trapsIntersected[i+1]);
            //tm.HandleCasePointsOutside(currentT, nextT, elegibleForMerge, newTrapezoidIds);
        }else{
            tm.HandleCaseQ1Inside(currentT, elegibleForMerge, newTrapezoidIds);
        }
    }

}



void generateSubgraph(TrapezoidalMap& tMap, Trapezoid& current, std::vector<size_t>& idsTrapezoid){
    cg3::Segment2d s = tMap.getLastSegment();
    Node*& target = current.nodeReference();
    if(idsTrapezoid.size() == 4){

        Node *p1 = new Node(xNode, tMap.getIdP1LastSegment());
        Node *s1 = new Node(yNode, tMap.getIdLastSegment());
        Node *q1 = new Node(xNode, tMap.getIdQ1LastSegment());

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            Node *leaf = new Node(leafNode, idsTrapezoid[i]);
            tMap.trapezoid(idsTrapezoid[i]).setNode(leaf);
        }
        s1->setLeftRfr(tMap.trapezoid(idsTrapezoid[1]).nodeReference());
        s1->setRightRfr(tMap.trapezoid(idsTrapezoid[2]).nodeReference());
        q1->setLeftRfr(s1);
        q1->setRightRfr(tMap.trapezoid(idsTrapezoid[3]).nodeReference());
        p1->setLeftRfr(tMap.trapezoid(idsTrapezoid[0]).nodeReference());
        p1->setRightRfr(q1);

        target->swap(p1);

    }else{
        /*Case p1 only inside trapezoid*/
        if (s.p1() > current.getLeftp() and s.p2() > current.getRightp()){
            Node *p1 = new Node(xNode, tMap.getIdP1LastSegment());
            Node *s1 = new Node(yNode, tMap.getIdLastSegment());

            /*LeafNode generation and update of trapezoid pointer to the node*/
            for(size_t i=0; i < idsTrapezoid.size(); i++){
                Node *leaf = new Node(leafNode, idsTrapezoid[i]);
                tMap.trapezoid(idsTrapezoid[i]).setNode(leaf);
            }

                p1->setLeft(tMap.trapezoid(idsTrapezoid[0]).nodeReference());
                s1->setLeft(tMap.trapezoid(idsTrapezoid[1]).nodeReference());
                s1->setRight(tMap.trapezoid(idsTrapezoid[2]).nodeReference());
                p1->setRight(s1);


           target->swap(p1);
        /*Case p1 and q1 outside trapezoid*/
        }else if(s.p1() < current.getLeftp() and s.p2() > current.getRightp()){
            Node *s1 = new Node(yNode, tMap.getIdLastSegment());

             /*LeafNode generation and update of trapezoid pointer to the node*/
            for(size_t i=0; i < idsTrapezoid.size(); i++){
                if (tMap.trapezoid(idsTrapezoid[i]).node == nullptr){
                    Node *leaf = new Node(leafNode, idsTrapezoid[i]);
                    tMap.trapezoid(idsTrapezoid[i]).setNode(leaf);
                }
            }

            s1->setLeft(tMap.trapezoid(idsTrapezoid[0]).nodeReference());
            s1->setRight(tMap.trapezoid(idsTrapezoid[1]).nodeReference());

            target->swap(s1);
         /*Case q1 only inside trapezoid*/
        }else{
            Node *s1 = new Node(yNode,tMap.getIdLastSegment());
            Node *q1 = new Node(xNode, tMap.getIdQ1LastSegment());

            /*LeafNode generation and update of trapezoid pointer to the node*/
            for(size_t i=0; i < idsTrapezoid.size(); i++){
                if (tMap.trapezoid(idsTrapezoid[i]).node == nullptr){
                    Node *leaf = new Node(leafNode, idsTrapezoid[i]);
                    tMap.trapezoid(idsTrapezoid[i]).setNode(leaf);
                }
            }

            s1->setLeft(tMap.trapezoid(idsTrapezoid[1]).nodeReference());
            s1->setRight(tMap.trapezoid(idsTrapezoid[2]).nodeReference());
            q1->setLeft(s1);
            q1->setRight(tMap.trapezoid(idsTrapezoid[0]).nodeReference());

            target->swap(q1);
        }
    }

}



size_t QueryPoint(TrapezoidalMap& tm, Dag& dag, cg3::Point2d& point){
    Node *tmp = dag.getRoot();
    while(tmp != nullptr){
        if(tmp->getType() == xNode){
            if (point.x() < tm.point(tmp->getId()).x()){
                tmp = tmp->getLeft();
            }else{
                tmp = tmp->getRight();
            }
        }

        if(tmp->getType() == yNode){
            if(geoutils::isPointAbove(point, tm.segment(tmp->getId()))){ //check if a poin is above a segment function
               tmp = tmp->getLeft();
            }else{
                tmp = tmp->getRight();
            }
        }

        if(tmp->getType() == leafNode){
            return tmp->getId();
        }
    }
    throw __EXCEPTIONS;
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
    while (q1.x() > tMap.trapezoid(trapezoidsIntersected[j]).getRightp().x()){
        if (geoutils::isPointAbove(tMap.trapezoid(trapezoidsIntersected[j]).getRightp(), s1)){
            trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).getLowerRightNeighbor());
        }else{
           trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).getUpperRightNeighbor());
        }
        j++;
    }
    return trapezoidsIntersected;

}

}
