#include "dag.h"
#define BOUNDINGBOX 1e+6
Dag::Dag(){
    root = nullptr;
};

Dag::Dag(Node* boundingBox){
    root = boundingBox;
};

Node* Dag::getRoot(){
    return root;
}

void Dag::setRoot(Node *n){
    root = n;
};

Trapezoid* Dag::getLeftMostTrapezoid(Node* node){
    while(node != nullptr){
        if (node->getType() == leafNode){
            LeafNode *tmp = (LeafNode*) node;
            return tmp->getTrapezoid();
        }else{
            node = node->left;
            }
    }

};

void Dag::substituteTargetNode(Node* root, Node* target, Node* newNode){
    if (root == nullptr){
        return;
    }else{
        if(this->root->getType() == leafNode){
            this->root = newNode;
        }
        if(root->getRight() == nullptr and root->getLeft() != nullptr){
            if (root->getLeft() == target){
                root->left = newNode;
                return;
             }
             substituteTargetNode(root->left, target, newNode);
        }

        if(root->getRight() != nullptr and root->getLeft() == nullptr){
            if (root->getRight() == target){
                root->right = newNode;
                return;
             }
             substituteTargetNode(root->right, target, newNode);
        }

        if(root->getRight() != nullptr and root->getLeft() != nullptr){
            if (root->getLeft() == target){
                root->left = newNode;
                return;
             }
            if (root->getRight() == target){
               root->right = newNode;
               return;
             }
             substituteTargetNode(root->left, target, newNode);
             substituteTargetNode(root->right, target, newNode);
        }
}
};

Trapezoid* Dag::QueryPoint(Node* root, cg3::Point2d point){
    Node *tmp = root;
    while(tmp != nullptr){
        if(tmp->getType() == xNode){
            XNode* xnode = (XNode*)(tmp);
            if (point.x() < xnode->getPoint().x()){
                tmp = tmp->getLeft();
            }else{
                tmp = tmp->getRight();
            }
        }

        if(tmp->getType() == yNode){
            YNode * ynode = (YNode*)(tmp);
            if(geoutils::isAbove(point, ynode->getSegment())){ //check if a poin is above a segment function
               tmp = tmp->getLeft();
            }else{
                tmp = tmp->getRight();
            }
        }

        if(tmp->getType() == leafNode){
            LeafNode * l = (LeafNode*)tmp;
            return l->getTrapezoid();
        }
    }
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
    return p1;
};







/*Trapezoid* Dag::getLeftMostTrapezoid(Node* node){
    Node * root = this->getRoot();
    while(node->getType() != leafNode){
        node = node->getLeft();
    }
    this->root = root;
    return node;
}*/



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
