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
    Node *left, *right;
    size_t id;
    NodeType type;

public:

    Node(NodeType type, const size_t& id);
    Node*& getLeftRfr();
    Node*& getRightRfr();
    Node* getLeft() const;
    Node* getRight() const;

    size_t& getId(){
        return id;
    };

    void reduceId(){
        id--;
    }

    void setLeftRfr(Node *&left);
    void setRightRfr(Node *&right);
    void setLeft(Node *left);
    void setRight(Node *right);
    //virtual void setTrapezoidId(const size_t& id);
    NodeType getType() const;
    void clear();
};




#endif // NODE_H

