#ifndef NODE_H
#define NODE_H
#include <cg3/geometry/segment2.h>

enum node_type{p, s, t};

class Node{
private:
    node_type type;
    Node *left, *right;
    /*To check: List of nodes for fathers?*/

    /*Need refactor after the Trapezoid class creation*/
    int data;
    /*Value can be a Trapezoid a point or a segment*/
public:

    Node();
    Node(Node *right, Node *left, node_type type);

    void setRight(Node *right);
    void setLeft(Node *left);
    void setType(node_type type);

    Node getLeft() const;
    Node getRight() const;
    node_type getType() const;

    void clear();
};

#endif // NODE_H

