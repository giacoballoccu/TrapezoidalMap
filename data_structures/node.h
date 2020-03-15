#ifndef NODE_H
#define NODE_H

#include <cg3/geometry/segment2.h>
#include "trapezoid.h"

/*Reference: Pag 27
 * DAG:
    type x nodes linked to points
    type y nodes linked to segments
    trapezoids stored in the leaves
*/
enum NodeType{
 None,
 xNode,
 yNode,
 leafNode,
};

/*

General Definition for the Node class used in the DAG, used as main class for the type of nodes
that inherit from this node class.

*/

class Node{
protected:
    Node *left, *right;

public:

    Node();
    Node(Node *right, Node *left);

    void setRight(Node *right);
    void setLeft(Node *left);

    virtual NodeType getType() const;
    Node getLeft() const;
    Node getRight() const;

    virtual void clear();
};


/*Inheritance with type of Node*/

/*

This XNode class rappresent endpoints in the DAG

*/
class XNode : Node{
private:
    cg3::Point2d point;
public:
    XNode();
    XNode(Node *left, Node *right);

    NodeType getType() const override;
    void clear() override;

};

/*

This YNode class rappresents segments in the DAG

*/
class YNode : Node{
private:
    cg3::Segment2d segment;
public:
    YNode();
    YNode(Node *left, Node *right);

    NodeType getType() const override;
    void clear() override;
};


/*

This LeafNode class rappresents Trapezoids in the DAG

*/
class LeafNode : Node{
private:
    Trapezoid trapezoid;
public:
    LeafNode();
    LeafNode(Node *left, Node *right);

    NodeType getType() const override;
    void clear() override;
};


#endif // NODE_H

