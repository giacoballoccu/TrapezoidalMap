#include "drawable_trapezoidalmap.h"
#include "QRandomGenerator"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

DrawableTrapezoidalMap::DrawableTrapezoidalMap():
    polygonColor(),
    borderColor(80, 80, 180),
    segmentSize(3)
{

}

void DrawableTrapezoidalMap::draw() const{
    int i = 0;
    for (const Trapezoid *t : getTrapezoidSet()) {
         cg3::opengl::drawQuad2(t->getTop().p1(), t->getTop().p2(), t->getBottom().p2(), t->getBottom().p1(), polygonColor[i], 3, true);
         i++;
    }

};

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return boundingBox.diag();
}

cg3::Color DrawableTrapezoidalMap::generateRandomColor() const{
    int R = QRandomGenerator::global()->bounded(0, 255);
    int G = QRandomGenerator::global()->bounded(0, 255);
    int B = QRandomGenerator::global()->bounded(0, 255);
    return cg3::Color(R, G, B);
};



void DrawableTrapezoidalMap::setPolygonColors(size_t size){
    size_t i = polygonColor.size();
    while (i < size){
        polygonColor.push_back(generateRandomColor());
        ++i;
    }
}
