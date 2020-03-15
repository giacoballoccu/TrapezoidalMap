#include "node.h"

Node::Node(){
    this->right = nullptr;
    this->left = nullptr;
};

Node::Node(Node *left, Node *right, node_type type){
    this->right = right;
    this->left = left;
    this->type = type;
}

Node Node::getLeft() const{
    return *left;
};
Node Node::getRight() const{
    return *right;
};
node_type Node::getType() const{
    return type;
};

void Node::setRight(Node *right){
    this->right = right;
};

void Node::setLeft(Node *left){
    this->left = left;
};

void Node::setType(node_type type){
    this->type = type;
};

void Node::clear(){
    delete right;
    delete left;
    // To check
    delete this;
}
