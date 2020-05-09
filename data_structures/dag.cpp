#include "dag.h"
#define BOUNDINGBOX 1e+6
Dag::Dag(){
    //root = new Node(leafNode, 0);
};

Node* Dag::getRoot() const{
    return root;
};

Node*& Dag::getRootReference(){
    return root;
}

void Dag::setRoot(Node *node){
    root = node;
}

void Dag::setRootRfr(Node *&node){
    root = node;
}

void Dag::substituteTargetNode(Node* root, Node *target, Node* newNode){
    if (root == nullptr){
        return;
    }else{
        if(this->root->getType() == leafNode){
            this->setRootRfr(newNode);
        }
        if(root->getRight() == nullptr and root->getLeft() != nullptr){
            if (root->getLeft() == target){
                root->setLeftRfr(newNode);
                return;
             }
             substituteTargetNode(root->getRightRfr(), target, newNode);
        }

        if(root->getRight() != nullptr and root->getLeft() == nullptr){
            if (root->getRight() == target){
                root->setRightRfr(newNode);
                return;
             }
             substituteTargetNode(root->getRightRfr(), target, newNode);
        }

        if(root->getRightRfr() != nullptr and root->getLeftRfr() != nullptr){
            if (root->getLeftRfr() == target){
                root->setLeftRfr(newNode);
                return;
             }
            if (root->getRightRfr() == target){
               root->setRightRfr(newNode);
               return;
             }
             substituteTargetNode(root->getLeftRfr(), target, newNode);
             substituteTargetNode(root->getRightRfr(), target, newNode);
        }
}
};



Node*& Dag::getLeafNodeRfr(size_t& id){
    return root;
}

void Dag::clear(){
   delete root;
    operator delete(this);
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
