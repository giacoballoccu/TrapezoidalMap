#include "dag.h"

Dag::Dag(){

};
/*
Dag::Dag(std::vector<cg3::Segment2d> segments){
    buildDagFromSegments(segments);
};


void buildDagFromSegments(std::vector<cg3::Segment2d> segments){

};
*/

Node Dag::getRoot() const{
    return root;
}

void Dag::setRoot(Node n){
    root = n;
};

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
