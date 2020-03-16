#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H
#include "trapezoid.h"
#include "dag.h"

class TrapezoidalMap{
protected:
    Trapezoid leftMostTrapezoid;
    Dag associateDag;
public:
    TrapezoidalMap();
};

#endif // TRAPEZOIDALMAP_H
