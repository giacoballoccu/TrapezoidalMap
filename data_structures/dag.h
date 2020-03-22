#ifndef DAG_H
#define DAG_H
#include "node.h"

class Dag{

private:
    Node root;
    std::vector<LeafNode*> trapezoids;
public:
    Dag();
    //Dag(std::vector<cg3::Segment2d> segments);

    Node getRoot() const;
    std::vector<LeafNode*> getTrapezoids() const;
    void setRoot(Node n);
    //void buildDagFromSegments(std::vector<cg3::Segment2d> segments);
    void retriveAllTrapezoids(Node *root);

};

#endif // DAG_H
