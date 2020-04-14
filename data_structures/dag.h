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

    Node* getRoot();
    std::vector<LeafNode*> getTrapezoids() const;
    void setRoot(Node *n);
    Trapezoid* QueryPoint(Node* root, cg3::Point2d point);

    void substituteTargetNode(Node* root, Node* target, Node *newNode);

    Trapezoid* getLeftMostTrapezoid(Node* node);

};

#endif // DAG_H
