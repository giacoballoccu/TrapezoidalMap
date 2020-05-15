#include "dag.h"
#define BOUNDINGBOX 1e+6
Dag::Dag(){
    _nodes = std::vector<Node>();
    _isDeleted = std::vector<bool>();
    Node root = Node(leafNode, 0);
    _nodes.push_back(root);
    _isDeleted.push_back(false);
};

const Node& Dag::root() const{
    return _nodes[0];
};

const Node& Dag::node(const size_t& id) const{
    if(_isDeleted[id] != true){
        return _nodes[id];
    }
    throw __EXCEPTIONS;
}
Node& Dag::root(){
    return _nodes[0];
};
Node& Dag::node(const size_t& id){
    if(_isDeleted[id] != true){
        return _nodes[id];
    }
    throw __EXCEPTIONS;
};

size_t Dag::addNode(const Node& node){
    _nodes.push_back(node);
    _isDeleted.push_back(false);
    return _nodes.size() -1;
};
void Dag::removeNode(const size_t &id){
    _isDeleted[id] = true;
};


void Dag::addChildrenToNode(const size_t& target, const size_t& leftChild, const size_t& rightChild){
    _nodes[target].setLeft(leftChild);
    _nodes[target].setRight(rightChild);
};
size_t Dag::replace(const size_t &oldId, const Node &newNode){
    _nodes[oldId] = newNode;
    return oldId;
};


void Dag::clear(){
   _nodes.clear();
   _isDeleted.clear();
}
