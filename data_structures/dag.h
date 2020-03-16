#ifndef DAG_H
#define DAG_H
#include "node.h"

class Dag
{

private:
    Node node;
public:

    Dag(Node root);

    std::list<LeafNode> getAllTrapezoids();
    //std::list<XNode> getAllPoints();
    //std::list<YNode> getAllSegments();
};

#endif // DAG_H
