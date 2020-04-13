#ifndef DAG_H
#define DAG_H
#include "node.h"
#include "utils/geoutils.h"

class Dag{

    private:
   // Node *root;
    //std::vector<LeafNode*> trapezoids;
public:
    Node *root;
    Dag();
    Dag(Node* boundingBox);
    //Dag(std::vector<cg3::Segment2d> segments);

    Node* getRoot();
    std::vector<LeafNode*> getTrapezoids() const;
    void setRoot(Node *n);
    Trapezoid* QueryPoint(Node* root, cg3::Point2d point);
    Node* simpleSubgraphFromSegment(cg3::Segment2d segment);
    void substituteTargetNode(Node* root, Node* target, Node *newNode);

    //void addTrapezoids(Node* root);
    //void createAndInsertLeaves(Node * node, bool leftChild);
    bool isAbove(cg3::Point2d p, cg3::Segment2d s);
Trapezoid* getLeftMostTrapezoid(Node* node);
    //Trapezoid* getLeftMostTrapezoid();
    //void retriveAllTrapezoids(Node *root);

};

#endif // DAG_H
