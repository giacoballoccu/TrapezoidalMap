#ifndef NODE_H
#define NODE_H

#include <cstddef>
#include <iostream>
#include <array>

/**
 * @brief The NodeType enum
 * xNode are the nodes which carry the id of a point.
 * yNode are the nodes which carry the id of a segment.
 * leafNode are the nodes which carry the id of a trapezoid.
 */
enum NodeType{
    xNode,
    yNode,
    leafNode,
};

/**
 * @brief The Node class
 * Node is the class used by the dag, contains an id to an object (Point, Segment, Trapezoid) that can be retrived in
 * the trapezoidal map, the type enum tell which is the type of object the id point.
 */
class Node{
private:
    size_t _left, _right;
    size_t _id;
    NodeType _type;
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

