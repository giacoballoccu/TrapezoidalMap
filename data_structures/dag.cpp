#include "dag.h"
#define BOUNDINGBOX 1e+6
Dag::Dag(){
    _nodes = std::vector<Node>();
    Node root = Node(leafNode, 0);
    _nodes.push_back(root);
};
/**
 * @brief Return the const rvalue to root node. The root is always at position 0.
 */
const Node& Dag::root() const{
    return _nodes[0];
};
/**
 * @brief Return the const rvalue of the node at index id.
 */
const Node& Dag::node(const size_t& id) const{
    return _nodes[id];
}
/**
 * @brief Return the rvalue of the node at index id.
 */
Node& Dag::node(const size_t& id){
    return _nodes[id];
};
/**
 * @brief Insert a node in the dag.
 * @return id of the index of the newly inserted node.
 */
size_t Dag::addNode(const Node& node){
    _nodes.push_back(node);
    return _nodes.size() -1;
};

/**
 * @brief Set to a target node leftChild and rightChild.
 * @param target, id of the target node in the dag.
 * @param leftChild, id of the new leftChild in the dag.
 * @param rightChild, id of the new rightChild in the dag.
 */
void Dag::addChildrenToNode(const size_t& target, const size_t& leftChild, const size_t& rightChild){
    _nodes[target].setLeft(leftChild);
    _nodes[target].setRight(rightChild);
};
/**
 * @brief Substitute a node in the dag with a new node
 * @param oldId
 * @param newNode
 * @return id of the index of the newly inserted node which is the same of the oldNode.
 * @note This methods keep the fathers updated.
 */
size_t Dag::replace(const size_t &oldId, const Node &newNode){
    _nodes[oldId] = newNode;
    return oldId;
};

void Dag::clear(){
    _nodes.clear();
}
