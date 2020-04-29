#include "dag.h"
#define BOUNDINGBOX 1e+6
Dag::Dag(){

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
/*
Dag::Dag(Node* boundingBox){
    root = boundingBox;
};
*/
/*
bool Dag::isRoot(size_t id){
    if (root == id){
        return true;
    }else{
        return false;
    }
};

Node* Dag::getRoot() const{
    return nodes[root];
};
Node*& Dag::getRootReference(){
    return nodes[root];
};

void Dag::setRoot(const size_t& idRoot){
    root = idRoot;
};

void Dag::removeNode(const size_t& id){
    nodes.erase(nodes.begin() + id);
    if (root == id){
        root = -1;
    }
}

size_t Dag::addNoderfr(Node*& n){
    nodes.push_back(n);
    return nodes.size()-1;
}

size_t Dag::addNode(LeafNode* n){
    nodes.push_back(n);
    return nodes.size()-1;
}

size_t Dag::addNode2(Node& n){
    Node* node = &n;
    nodes.push_back(node);
    return nodes.size()-1;
}


Node*& Dag::node(const size_t& id) {
    return nodes[id];
}
Node* Dag::node(const size_t& id) const {
    return nodes[id];
}
*/
/*

  Used for testing


Trapezoid* Dag::getLeftMostTrapezoid(Node* node){
    while(node != nullptr){
        if (node->getType() == leafNode){
            LeafNode *tmp = (LeafNode*) node;
            return tmp->getTrapezoid();
        }else{
            node = node->left;
            }
    }

    return nullptr;
};
*/
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

void Dag::clear(std::list<Trapezoid> trapezoids){
    std::list<Trapezoid>::iterator it;
    for (it = trapezoids.begin(); it != trapezoids.end(); ++it){
        (*it).node->clear();
    }
    this->root = nullptr;
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
