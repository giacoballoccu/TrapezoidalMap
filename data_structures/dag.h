#ifndef DAG_H
#define DAG_H
#include "node.h"

class Dag{

private:
    Node *root;
    //std::vector<LeafNode*> trapezoids;
public:
    Dag();
    Dag(Node* boundingBox);
    //Dag(std::vector<cg3::Segment2d> segments);

    Node* getRoot();
    std::vector<LeafNode*> getTrapezoids() const;
    void setRoot(Node *n);
    Node* QueryPoint(Node* root, cg3::Point2d point);
    Node* simpleSubgraphFromSegment(cg3::Segment2d segment);
    void addTrapezoids(Node* root);
    void createAndInsertLeaves(Node * node, bool leftChild);
    bool isAbove(cg3::Point2d p, cg3::Segment2d s);
    //Trapezoid* getLeftMostTrapezoid();
    //void retriveAllTrapezoids(Node *root);

};

#endif // DAG_H
