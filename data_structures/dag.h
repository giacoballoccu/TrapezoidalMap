#ifndef DAG_H
#define DAG_H
#include "node.h"

class Dag
{

private:
    Node node;
public:

    Dag(Node root);

    std::list<Node> getAllTrapezoids();
    //std::list<Node> getAllPoints();
    //std::list<Node> getAllSegments();
};

#endif // DAG_H
