#ifndef DAG_H
#define DAG_H
#include "node.h"
#include "utils/geoutils.h"


class Dag{
private:
    std::vector<Node> _nodes;
    std::vector<bool> _isDeleted;
public:

    Dag();

    Node& node(const size_t& id);
    Node& root();
    const Node& root() const;
    const Node& node(const size_t& id) const;

    size_t addNode(const Node& node);
    void addChildrenToNode(const size_t& target, const size_t& leftChild, const size_t& rightChild);
    size_t replace(const size_t &oldId, const Node &newNode);
    void removeNode(const size_t& id);

    void clear();
};

#endif // DAG_H
