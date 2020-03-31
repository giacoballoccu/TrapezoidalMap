#include "dag.h"
#define BOUNDINGBOX 1e+6
Dag::Dag(){
    root = nullptr;
};

Dag::Dag(Node* boundingBox){
    root = boundingBox;
};
/*
Dag::Dag(std::vector<cg3::Segment2d> segments){
    buildDagFromSegments(segments);
};


void buildDagFromSegments(std::vector<cg3::Segment2d> segments){

};
*/

Node* Dag::getRoot(){
    return root;
}

void Dag::setRoot(Node *n){
    root = n;
};

Node* Dag::QueryPoint(Node* root, cg3::Point2d point){
    if(root->getType() == leafNode){
        return root;
    }
    else if(root->getType() == xNode){
        XNode* xnode = (XNode*)(root);
        if (point.x() < xnode->getPoint().x()){
                return QueryPoint(root->getRight(), point);
            }else{
                return QueryPoint(root->getLeft(), point);
            }
    }
    else if(root->getType() == yNode){
        YNode * ynode = (YNode*)(root);
        if(isAbove(point, ynode->getSegment())){ //check if a poin is above a segment function
            return QueryPoint(root->getLeft(), point);
        }else{
            return QueryPoint(root->getRight(), point);
    }
    }

    throw -1; //Control can't reach this point if input is correct, just to supprese the warning
};

/*

Generate subgraph

*/
Node* Dag::simpleSubgraphFromSegment(cg3::Segment2d segment){
    Node* p1 = new XNode(segment.p1());
    Node*  q1 = new XNode(segment.p2());
    Node* s1 = new YNode(segment);

    q1->setLeft(s1);
    p1->setRight(q1);
    addTrapezoids(p1);
    return p1;
};


void Dag::addTrapezoids(Node* root){
    if (root == nullptr or root->getType() == leafNode){
        return;
    }else{
        if(root->left == nullptr and root->right != nullptr){
            createAndInsertLeaves(root, true);
            addTrapezoids(root->right);
        }
        if(root->right == nullptr and root->left != nullptr){
            createAndInsertLeaves(root, false);
            addTrapezoids(root->left);
        }
        if(root->right == nullptr and root->left == nullptr){
            createAndInsertLeaves(root, true);
            createAndInsertLeaves(root, false);
        }
        if(root->right != nullptr and root->left != nullptr){
            addTrapezoids(root->left);
            addTrapezoids(root->right);
        }
    }
};

void Dag::createAndInsertLeaves(Node * node, bool leftChild){
    if (node->getType() == xNode){
        XNode * x = (XNode*)node;
        Trapezoid delta = Trapezoid(x->getPoint(), leftChild);
        Node * leaf = new LeafNode(&delta);
        if (leftChild){
             node->setLeft(leaf);
        }else{
            node->setRight(leaf);
        }

    }
    if (node->getType() == yNode){
        YNode * y = (YNode*)node;
        Trapezoid delta = Trapezoid(y->getSegment(), leftChild); // se è a sx di un segmento è above
        Node * leaf = new LeafNode(&delta);
        if (leftChild){
             node->setLeft(leaf);
        }else{
            node->setRight(leaf);
        }
    }
    if(node->getType() == leafNode){
        return;
    }
}




/*Trapezoid* Dag::getLeftMostTrapezoid(Node* node){
    Node * root = this->getRoot();
    while(node->getType() != leafNode){
        node = node->getLeft();
    }
    this->root = root;
    return node;
}*/

/*
!!! Move it in utility
Cross product to understand if the point is above or below line, formula returns 0 if segment and point are collinear.
1 if the point is above the segment

*/
bool Dag::isAbove(cg3::Point2d p, cg3::Segment2d s){
     return ((s.p2().x() - s.p1().x())*(p.y() - s.p1().y()) - (s.p2().y() - s.p1().y())*(p.x() - s.p1().x())) > 0;
}

/*
void Dag::retriveAllTrapezoids(Node *root){
    if (root == nullptr){
        return;
    }else{
        if (root->getType() == leafNode){
            trapezoids.resize(trapezoids.size()+1);
            trapezoids.push_back((LeafNode*)root);
        }else{
            if(root->getRight() != nullptr){
                retriveAllTrapezoids(root->getRight());
            }
            if(root->getLeft() != nullptr){
                retriveAllTrapezoids(root->getLeft());
            }
        }
    }
};
*/
