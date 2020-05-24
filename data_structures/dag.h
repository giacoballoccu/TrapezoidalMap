#ifndef DAG_H
#define DAG_H
#include "node.h"
#include "utils/geoutils.h"

/**
 * @brief The Dag class
 * A data structure to simulate a directed acyclic graph, the nodes are stored in a vector nodes and flagged as deleted in the vector
 * isDeleted. From this class is possible to add, substitute, remove nodes and add children to them.
 */
class Dag{
private:
    std::vector<Node> _nodes;
public:

    Dag();

    Node& node(const size_t& id);
    const Node& root() const;
    const Node& node(const size_t& id) const;

    size_t addNode(const Node& node);
    void addChildrenToNode(const size_t& target, const size_t& leftChild, const size_t& rightChild);
    size_t replace(const size_t &oldId, const Node &newNode);

    void clear();
};

#endif // DAG_H
