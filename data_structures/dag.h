#ifndef DAG_H
#define DAG_H
#include "node.h"
#include "utils/geoutils.h"


class Dag{
private:
    Node * root;
public:

    Dag();

    Node* getRoot() const;
    Node*& getRootReference();
    Node*& getLeafNodeRfr(size_t& id);


    void setRootRfr(Node*& node);
    void setRoot(Node * node);

    void substituteTargetNode(Node* root, Node* target, Node *newNode);
    void clear();
};

#endif // DAG_H
