#include "node.h"

/*Node Constructors*/
Node::Node(){
    this->right = nullptr;
    this->left = nullptr;
};
Node::Node(Node *left, Node *right){
    this->right = right;
    this->left = left;
}

/*Node Methods*/
Node Node::getLeft() const{
    return *left;
};
Node Node::getRight() const{
    return *right;
};
void Node::setRight(Node *right){
    this->right = right;
};
void Node::setLeft(Node *left){
    this->left = left;
};
NodeType Node::getType() const{
    return None;
};
void Node::clear() {
    left = nullptr;
    right = nullptr;
};

/*XNode Constructors*/
XNode::XNode(){
    Node();
};
XNode::XNode(Node *left, Node *right){
    Node(left, right);
};

/*XNode Methods*/
NodeType XNode::getType() const{
    return xNode;
};
void XNode::clear(){
    point.~Point2();
    left = nullptr;
    right = nullptr;
};

/*YNode Constructors*/
YNode::YNode(){
    Node();
};
YNode::YNode(Node *left, Node *right){
    Node(left, right);
};

/*YNode Methods*/
NodeType YNode::getType() const{
    return yNode;
};
void YNode::clear(){
    segment.~Segment();
    right = nullptr;
    left = nullptr;
};

/*LeafNode Constructors*/
LeafNode::LeafNode(){
    Node();
};
LeafNode::LeafNode(Node *left, Node *right){
    Node(left, right);
};

/*LeafNode Methods*/
NodeType LeafNode::getType() const{
    return leafNode;
};
void LeafNode::clear(){
    trapezoid.clear();
    right = nullptr;
    left = nullptr;
};
