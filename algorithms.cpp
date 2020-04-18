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

    std::vector<size_t> trapezoidsIntersected = FollowSegment(tm, dag, segment);

    /*check: understand if is needed to keep and order in tm*/
    if (trapezoidsIntersected.size() == 1){
        subgraphFromOneTrapezoid(tm, dag, trapezoidsIntersected[0], segment);

    }else{

    }

};

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

std::vector<size_t> Algorithms::FollowSegment(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d& s1){
    cg3::Point2d p1 = s1.p1();
    cg3::Point2d q1 = s1.p2();

    std::vector<size_t> trapezoidsIntersected = std::vector<size_t>();
    size_t idDelta = QueryPoint(tMap, dag, p1);

    trapezoidsIntersected.push_back(idDelta);
    int j = 0;
    while (q1.x() > tMap.trapezoid(trapezoidsIntersected[j]).getRightp().x()){
        if (geoutils::isAbove(tMap.trapezoid(trapezoidsIntersected[j]).getRightp(), s1)){
           trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).getLowerRightNeighbor()->node->getId());
        }else{
            trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).getUpperRightNeighbor()->node->getId());
        }
        j++;
    }
    return trapezoidsIntersected;

}


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

   /* if (dag.getRoot() == tMap.trapezoid(id).node){
        dag.setRoot(p1);
    }
    else{

    }*/
    dag.substituteTargetNode(dag.getRootReference(), target.node, p1); //logn
    //Node*& nodeptr = dag.getLeafNode(id);
    //nodeptr = nuovoIndirizzo;
}

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
