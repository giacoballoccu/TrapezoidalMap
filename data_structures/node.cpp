#include "node.h"

/*Node Constructors*/
Node::Node(){
    this->right = nullptr;
    this->left = nullptr;
};

/*Node Methods*/
Node* Node::getLeft() const{
    return left;
};
Node* Node::getRight() const{
    return right;
};
void Node::setRight(Node *right){
    this->right = right;
};
void Node::setLeft(Node *left){
    this->left = left;
};
NodeType Node::getType() const{
    return undefinedNode;
};
void Node::clear() {
    left = nullptr;
    right = nullptr;
};

/*XNode Constructors*/

XNode::XNode(cg3::Point2d point){
    Node();
    this->point = point;
};


/*XNode Methods*/
cg3::Point2d XNode::getPoint() const{
    return point;
};
NodeType XNode::getType() const{
    return xNode;
};
void XNode::clear(){
    point.~Point2();
    this->Node::clear();
};

/*YNode Constructors*/
cg3::Segment2d YNode::getSegment() const{
    return segment;
};

YNode::YNode(cg3::Segment2d segment){
    Node();
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

LeafNode::LeafNode(Trapezoid *trapezoid){
    Node();
    this->trapezoid = trapezoid;
    this->trapezoid->node = this;
};


/*LeafNode Methods*/
Trapezoid* LeafNode::getTrapezoid() const{
    return trapezoid;
};

NodeType LeafNode::getType() const{
    return leafNode;
};
void LeafNode::clear(){
    trapezoid = nullptr;
    this->Node::clear();
};
