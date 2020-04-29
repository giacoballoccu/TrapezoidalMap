#include "algorithms.h"

#define BOUNDINGBOX 1e+6

Algorithms::Algorithms()
{

}

void Algorithms::inizializateDataStructures(TrapezoidalMap& tm, Dag& dag){
    /*Starting bounding box aka first trapezoid*/
    Trapezoid firstTrapezoid = Trapezoid();
    size_t idTrap = tm.addTrapezoid(firstTrapezoid);

    Node *firstNode = new Node(leafNode, idTrap);

    tm.trapezoid(idTrap).node = firstNode;

    dag.setRoot(firstNode);
};

void Algorithms::BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, std::vector<cg3::Segment2d>& segmentList){
    /*Permute segments order*/
    if (tm.getSegments().size() > 1){
         tm.permuteSegmentList();
    }
    for (size_t i = 0; i < tm.getSegments().size(); i++){
        BuildTrapezoidalMap(tm, dag, segmentList[i]);
    }

}

void Algorithms::BuildTrapezoidalMap(TrapezoidalMap& tm, Dag& dag, cg3::Segment2d segment){

    /*Validate segments*/
    geoutils::validateSegment(segment);

    std::vector<std::list<Trapezoid>::iterator> trapsIntersected = FollowSegment(tm, dag, segment);

    std::vector<std::vector<size_t>> newTrapezoidIds = std::vector<std::vector<size_t>>();
    std::vector<std::tuple<size_t, std::tuple<size_t,size_t>>> elegibleForMerge =  std::vector<std::tuple<size_t, std::tuple<size_t,size_t>>>();

    /*Performe splitting and neighbor fixing according to case*/
    for(size_t i = 0; i < trapsIntersected.size(); i++){
        Update(tm, i, trapsIntersected, segment, newTrapezoidIds, elegibleForMerge);
    }

    /*Update Dag*/
    for(size_t i = 0; i < trapsIntersected.size(); ++i){
        Trapezoid current = (*trapsIntersected[i]);
        dag.substituteTargetNode(dag.getRootReference(), current.node, generateSubgraph(tm, current, newTrapezoidIds[i], segment));
    }

    /*Delete old trapezoid from Trapezoidal Map*/
    for(size_t i = 0; i < trapsIntersected.size(); ++i){
        tm.removeTrapezoid((*trapsIntersected[i]).node->getId());
        (*trapsIntersected[i]).node->clear();
    }



};


void Algorithms::Update(TrapezoidalMap& tm, const size_t& i, std::vector<std::list<Trapezoid>::iterator>& trapsIntersected, cg3::Segment2d s, std::vector<std::vector<size_t>>& newTrapezoidIds, std::vector<std::tuple<size_t, std::tuple<size_t,size_t>>>& elegibleForMerge){
    Trapezoid current = (*trapsIntersected[i]);
    size_t idCurrent = current.node->getId();

     /*Case p1 and q1 inside trapezoid*/
    if(trapsIntersected.size() == 1){
        tm.SplitInFour(idCurrent, s, newTrapezoidIds);
    }else{
        /*Case p1 only inside trapezoid*/
        if (s.p1() > current.getLeftp() and s.p2() > current.getRightp()){
            size_t idNext = (*trapsIntersected[i+1]).node->getId();
            tm.HandleCaseP1Inside(idCurrent, idNext, s, elegibleForMerge, newTrapezoidIds);
        /*Case p1 and q1 outside trapezoid*/
        }else if(s.p1() < current.getLeftp() and s.p2() > current.getRightp()){
            size_t idNext = (*trapsIntersected[i+1]).node->getId();
            tm.HandleCasePointsOutside(idCurrent, idNext, s, elegibleForMerge, newTrapezoidIds);
         /*Case q1 only inside trapezoid*/
        }else{
            tm.HandleCaseQ1Inside(idCurrent, s, elegibleForMerge, newTrapezoidIds);
        }
    }

}

Node* Algorithms::generateSubgraph(TrapezoidalMap& tMap, Trapezoid current, std::vector<size_t> idsTrapezoid, cg3::Segment2d& s){
    /*Case p1 only inside trapezoid*/
    if (s.p1() > current.getLeftp() and s.p2() > current.getRightp()){
        size_t idS1 = tMap.addSegment(s);
        size_t idP1 = tMap.addPoint(s.p1());
        Node *p1 = new Node(xNode, idP1);
        Node *s1 = new Node(yNode, idS1);


        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            Node *leaf = new Node(leafNode, idsTrapezoid[i]);
            tMap.trapezoid(idsTrapezoid[i]).node = leaf;
        }

            p1->setLeft(tMap.trapezoid(idsTrapezoid[0]).node);
            s1->setLeft(tMap.trapezoid(idsTrapezoid[1]).node);
            s1->setRight(tMap.trapezoid(idsTrapezoid[2]).node);
            p1->setRight(s1);


        return p1;

    /*Case p1 and q1 outside trapezoid*/
    }else if(s.p1() < current.getLeftp() and s.p2() > current.getRightp()){
        Node *s1 = new Node(yNode, tMap.getIdLastSegment());

         /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node == nullptr){
                Node *leaf = new Node(leafNode, idsTrapezoid[i]);
                tMap.trapezoid(idsTrapezoid[i]).node = leaf;
            }
        }

        s1->setLeft(tMap.trapezoid(idsTrapezoid[0]).node);
        s1->setRight(tMap.trapezoid(idsTrapezoid[1]).node);

        return s1;
    /*Case q1 only inside trapezoid*/
    }else if(s.p1() < current.getLeftp() and s.p2() < current.getRightp()){
        size_t idQ1 = tMap.addPoint(s.p2());
        Node *q1 = new Node(xNode, idQ1);
        Node *s1 = new Node(yNode, tMap.getIdLastSegment());

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node == nullptr){
                Node *leaf = new Node(leafNode, idsTrapezoid[i]);
                tMap.trapezoid(idsTrapezoid[i]).node = leaf;
            }
        }

        s1->setLeft(tMap.trapezoid(idsTrapezoid[1]).node);
        s1->setRight(tMap.trapezoid(idsTrapezoid[2]).node);
        q1->setLeft(s1);
        q1->setRight(tMap.trapezoid(idsTrapezoid[0]).node);

        return q1;
    /*Case p1 and q1 inside trapezoid. */
    }else{
        size_t idS1 = tMap.addSegment(s);
        size_t idP1 = tMap.addPoint(s.p1());
        size_t idQ1 = tMap.addPoint(s.p2());
        Node *p1 = new Node(xNode, idP1);
        Node *s1 = new Node(yNode, idS1);
        Node *q1 = new Node(xNode, idQ1);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            Node *leaf = new Node(leafNode, idsTrapezoid[i]);
            tMap.trapezoid(idsTrapezoid[i]).node = leaf;
        }
        s1->setLeft(tMap.trapezoid(idsTrapezoid[1]).node);
        s1->setRight(tMap.trapezoid(idsTrapezoid[2]).node);
        q1->setLeft(s1);
        q1->setRight(tMap.trapezoid(idsTrapezoid[3]).node);
        p1->setLeft(tMap.trapezoid(idsTrapezoid[0]).node);
        p1->setRight(q1);

        return p1;
    }
}

size_t Algorithms::QueryPoint(TrapezoidalMap& tMap, Dag& dag, cg3::Point2d& point){
    Node *tmp = dag.getRoot();
    while(tmp != nullptr){
        if(tmp->getType() == xNode){
            if (point.x() < tMap.point(tmp->getId()).x()){
                tmp = tmp->getLeft();
            }else{
                tmp = tmp->getRight();
            }
        }

        if(tmp->getType() == yNode){
            if(geoutils::isAbove(point, tMap.segment(tmp->getId()))){ //check if a poin is above a segment function
               tmp = tmp->getLeft();
            }else{
                tmp = tmp->getRight();
            }
        }

        if(tmp->getType() == leafNode){
            return tmp->getId();
        }
    }

    return -1;
};


/*

Follow Segment

*/

std::vector<std::list<Trapezoid>::iterator> Algorithms::FollowSegment(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d& s1){
    cg3::Point2d p1 = s1.p1();
    cg3::Point2d q1 = s1.p2();

    std::vector<std::list<Trapezoid>::iterator> trapezoidsIntersected = std::vector<std::list<Trapezoid>::iterator>();
    size_t idDelta = QueryPoint(tMap, dag, p1);

    trapezoidsIntersected.push_back(tMap.getIterator(idDelta));
    int j = 0;
    while (q1.x() > (*trapezoidsIntersected[j]).getRightp().x()){
        if (geoutils::isAbove((*trapezoidsIntersected[j]).getRightp(), s1)){
           trapezoidsIntersected.push_back(tMap.getIterator((*trapezoidsIntersected[j]).getLowerRightNeighbor()->node->getId()));
        }else{
           trapezoidsIntersected.push_back(tMap.getIterator((*trapezoidsIntersected[j]).getUpperRightNeighbor()->node->getId()));
        }
        j++;
    }
    return trapezoidsIntersected;

}

/*
void Algorithms::subgraphFromOneTrapezoid(TrapezoidalMap& tMap, Dag& dag, size_t& id, cg3::Segment2d& s){
    size_t idS1 = tMap.addSegment(s);
    size_t idP1 = tMap.addPoint(s.p1());
    size_t idQ1 = tMap.addPoint(s.p2());

    Node *p1 = new Node(xNode, idP1);
    Node *q1 = new Node(xNode, idQ1);
    Node *s1 = new Node(yNode, idS1);

    std::vector<Trapezoid> splitResult = tMap.trapezoid(id).SplitTrapezoid(s);

    Trapezoid target = tMap.trapezoid(id);
    tMap.removeTrapezoid(id);

    size_t idTrapA = tMap.addTrapezoid(splitResult[0]);
    Node *nodeA = new Node(leafNode, idTrapA);
    p1->setLeft(nodeA);


    size_t idTrapB = tMap.addTrapezoid(splitResult[1]);
    Node *nodeB = new Node(leafNode, idTrapB);
    s1->setLeft(nodeB);


    size_t idTrapC = tMap.addTrapezoid(splitResult[2]);
    Node *nodeC = new Node(leafNode, idTrapC);
    s1->setRight(nodeC);


    size_t idTrapD = tMap.addTrapezoid(splitResult[3]);
    Node *nodeD = new Node(leafNode, idTrapD);
    q1->setRight(nodeD);


    q1->setLeft(s1);
    p1->setRight(q1);


    tMap.trapezoid(idTrapA).node = nodeA;
    tMap.trapezoid(idTrapB).node = nodeB;
    tMap.trapezoid(idTrapC).node = nodeC;
    tMap.trapezoid(idTrapD).node = nodeD;

   /if (dag.getRoot() == tMap.trapezoid(id).node){
        dag.setRoot(p1);
    }
    else{

    }
    dag.substituteTargetNode(dag.getRootReference(), target.node, p1); //logn
    //Node*& nodeptr = dag.getLeafNode(id);
    //nodeptr = nuovoIndirizzo;
}*/

/*void Algorithms::addTrapezoids(Node* root, TrapezoidalMap& tm){
    if (root == nullptr or root->getType() == leafNode){
        return;
    }else{
        if(root->left == nullptr and root->right != nullptr){
            createAndInsertLeaves(root, tm, true);
            addTrapezoids(root->right, tm);
        }
        if(root->right == nullptr and root->left != nullptr){
            createAndInsertLeaves(root, tm, false);
            addTrapezoids(root->left, tm);
        }
        if(root->right == nullptr and root->left == nullptr){
            createAndInsertLeaves(root,tm, true);
            createAndInsertLeaves(root,tm, false);
        }
        if(root->right != nullptr and root->left != nullptr){
            addTrapezoids(root->left, tm);
            addTrapezoids(root->right, tm);
        }
    }
};

void Algorithms::createAndInsertLeaves(Node * node, TrapezoidalMap& tm, bool leftChild){
    if (node->getType() == xNode){
        XNode * x = (XNode*)node;
        Trapezoid* delta = new Trapezoid(x->getPoint(), leftChild);
        Node * leaf = new LeafNode(delta);
        delta->node = leaf;
        if (leftChild){
             node->setLeft(leaf);
             tm.addTrapezoid(delta);
        }else{
            node->setRight(leaf);
            tm.addTrapezoid(delta);
        }

    }
    if (node->getType() == yNode){
        YNode * y = (YNode*)node;
        Trapezoid *delta = new Trapezoid(y->getSegment(), leftChild); // se è a sx di un segmento è above
        Node * leaf = new LeafNode(delta);
                delta->node = leaf;
        if (leftChild){
             node->setLeft(leaf);
             tm.addTrapezoid(delta);
        }else{
            node->setRight(leaf);
            tm.addTrapezoid(delta);
        }
    }
    if(node->getType() == leafNode){
        return;
    }
}



*/
