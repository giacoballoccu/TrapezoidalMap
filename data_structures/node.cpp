#include "node.h"

/*Node Constructors*/

Node::Node(NodeType type, const size_t& id){
    _type = type;
    _id = id;
    _left = SIZE_MAX;
    _right = SIZE_MAX;
};


size_t Node::left() const{
    return _left;
};
size_t Node::right() const{
    return _right;
};
size_t Node::id(){
    return _id;
};
NodeType Node::type() const{
    return _type;
};

void Node::setLeft(const size_t& idLeftChild){
    _left = idLeftChild;
};
void Node::setRight(const size_t& idRightChild){
    _right = idRightChild;
};

