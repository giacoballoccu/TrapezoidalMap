#include "node.h"

/**
 * @brief Constructor for node.
 * @param type, enum with type of node, xNode, yNode, leafNode
 * @param id, id of the object in the trapezoidal map
 * @note SIZE_MAX is the static equivalent to nullptr.
 */
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
/**
 * @brief Return the type of the node.
 * @return type of the node as NodeType enum.
 */
NodeType Node::type() const{
    return _type;
};

/**
 * @brief Set the left child.
 * @param id of the new child, node has to be already in the dag.
 */
void Node::setLeft(const size_t& idLeftChild){
    _left = idLeftChild;
};
/**
 * @brief Set the right child.
 * @param id of the new child, node has to be already in the dag.
 */
void Node::setRight(const size_t& idRightChild){
    _right = idRightChild;
};

