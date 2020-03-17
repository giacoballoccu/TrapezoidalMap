#ifndef DAG_H
#define DAG_H
#include "node.h"

class Dag{

private:
    XNode root;
public:
    Dag();
    //Dag(std::vector<cg3::Segment2d> segments);

    XNode getRoot() const;
    //void buildDagFromSegments(std::vector<cg3::Segment2d> segments);
    //std::list<LeafNode> getAllTrapezoids();
    //std::list<XNode> getAllPoints();
    //std::list<YNode> getAllSegments();
};

#endif // DAG_H
