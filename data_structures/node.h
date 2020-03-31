#ifndef NODE_H
#define NODE_H

#include "trapezoid.h"

/*Reference: Pag 27
 * DAG:
    type x nodes linked to points
    type y nodes linked to segments
    trapezoids stored in the leaves
*/
enum NodeType{
 undefinedNode,
 xNode,
 yNode,
 leafNode,
};

/*

General Definition for the Node class used in the DAG, used as main class for the type of nodes
that inherit from this node class.

*/

class Node{
private:
// std::list<Node*> parents;
public:
    Node *left, *right; //In xNode geometrical left and right, in yNode geometrical above and below, in leafNode nullptr

    Node();
    Node* getLeft() const;
    Node* getRight() const;

    void setLeft(Node *left);
    void setRight(Node *right);

    virtual NodeType getType() const;

    virtual void clear();
};


/*Inheritance with type of Node*/

/*

This XNode class rappresent endpoints in the DAG

*/
class XNode : public Node{
private:
    cg3::Point2d point;
public:
    XNode(cg3::Point2d point);

    cg3::Point2d getPoint() const;
    NodeType getType() const override;

    void clear() override;

};

/*

This YNode class rappresents segments in the DAG

*/
class YNode : public Node{
private:
    cg3::Segment2d segment;
public:
    YNode(cg3::Segment2d segment);

    cg3::Segment2d getSegment() const;
    NodeType getType() const override;

    void clear() override;
};


/*

This LeafNode class rappresents Trapezoids in the DAG

*/
class LeafNode : public Node{
private:
    Trapezoid *trapezoid;
public:
    LeafNode(Trapezoid* trapezoid);

    Trapezoid* getTrapezoid() const;
    NodeType getType() const override;
    void clear() override;
};


#endif // NODE_H

