#include "node.h"

/*Node Constructors*/
Node::Node(NodeType type, const size_t& id){
    this->type = type;
    this->id = id;
    this->left = nullptr;
    this->right = nullptr;
};


/*Node Methods*/
Node*& Node::getLeftRfr(){
    return left;
};
Node*& Node::getRightRfr(){
    return right;
};
Node* Node::getLeft() const{
    return left;
};
Node* Node::getRight() const{
    return right;
};

void Node::setLeft(Node *left){
    this->left = left;
};
void Node::setRight(Node *right){
    this->right = right;
};

void Node::setLeftRfr(Node *&left){
    this->left = left;
};
void Node::setRightRfr(Node *&right){
    this->right = right;
};

//virtual void setTrapezoidId(const size_t& id);
NodeType Node::getType() const{
    return type;
};

void Node::clear() {
    left = nullptr;
    right = nullptr;
    delete this;
};

