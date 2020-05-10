#ifndef NODE_H
#define NODE_H

#include <cstddef>
#include <iostream>
#include <array>

/*
 * DAG:
    type x nodes linked to points
    type y nodes linked to segments
    trapezoids stored in the leaves
*/
enum NodeType{
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
    size_t _left, _right;
    size_t _id;
    NodeType _type;
    std::vector<size_t> _fathers();
public:

    Node(NodeType type, const size_t& id);

    size_t left() const;
    size_t right() const;
    size_t id();
    NodeType type() const;

    void setLeft(const size_t& idLeftChild);
    void setRight(const size_t& idRightChild);

};




#endif // NODE_H

