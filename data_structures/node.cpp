#include "node.h"

/*Node Constructors*/

Node::Node(NodeType type, const size_t& id){
    this->type = type;
    this->id = id;
    right = nullptr;
    left = nullptr;
};

Node::Node(const Node& other){
    type = other.type;
    id = other.id;
    right = other.right;
    left = other.left;
};

void Node::swap(Node*& other){
    std::swap(left, other->left);
    std::swap(right, other->right);
    std::swap(type, other->type);
    std::swap(id, other->id);
}


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

size_t Node::getId(){
    return id;
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





Node::~Node(){
    delete left;
    delete right;
    operator delete(this);
}

