#ifndef DRAWABLE_TRAPEDOIDALMAP_H
#define DRAWABLE_TRAPEDOIDALMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>

class Drawable_TrapedoidalMap : public TrapezoidalMap, public cg3::DrawableObject{
private:
        cg3::Color polygonColor;
        cg3::Color borderColor;

        unsigned int segmentSize;
public:
    Drawable_TrapedoidalMap();

    void draw() const;

    void setRandomPolygonColor();
    cg3::Color generateRandomColor() const;
};

#endif // DRAWABLE_TRAPEDOIDALMAP_H
