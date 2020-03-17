#include "node.h"

/*Node Constructors*/
Node::Node(){
    this->right = nullptr;
    this->left = nullptr;
};

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
    return undefined;
};
void Node::clear() {
    left = nullptr;
    right = nullptr;
};

/*XNode Constructors*/
XNode::XNode(){
    Node();
};
XNode::XNode(cg3::Point2d point){
    XNode();
    this->point = point;
};


/*XNode Methods*/
NodeType XNode::getType() const{
    return xNode;
};
void XNode::clear(){
    point.~Point2();
    this->Node::clear();
};

/*YNode Constructors*/
YNode::YNode(){
    Node();
};
YNode::YNode(cg3::Segment2d segment){
    YNode();
    this->segment = segment;
};

/*YNode Methods*/
NodeType YNode::getType() const{
    return yNode;
};
void YNode::clear(){
    segment.~Segment();
    this->Node::clear();
};

/*LeafNode Constructors*/
LeafNode::LeafNode(){
    Node();
};
LeafNode::LeafNode(Trapezoid trapezoid){
    LeafNode();
    this->trapezoid = trapezoid;
};


/*LeafNode Methods*/
NodeType LeafNode::getType() const{
    return leafNode;
};
void LeafNode::clear(){
    trapezoid.clear();
};
