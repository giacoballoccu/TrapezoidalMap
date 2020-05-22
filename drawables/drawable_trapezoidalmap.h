#ifndef DRAWABLE_TRAPEDOIDALMAP_H
#define DRAWABLE_TRAPEDOIDALMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>
#include "QRandomGenerator"

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject{
private:
        std::vector<cg3::Color> polygonColor;
        cg3::Color borderColor;
        unsigned int segmentSize;
        size_t trapezoidMarked;
public:
    DrawableTrapezoidalMap(TrapezoidalMap& tm);

    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;

    cg3::Color segmentColor;

    void setPolygonColors(size_t size);
    void setRandomPolygonColor();
    void markTrapezoid(size_t id);
    cg3::Color generateRandomColor() const;

};

#endif // DRAWABLE_TRAPEDOIDALMAP_H
