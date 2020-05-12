#include "drawable_trapezoidalmap.h"
#include "QRandomGenerator"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

DrawableTrapezoidalMap::DrawableTrapezoidalMap(TrapezoidalMap& tm):
    TrapezoidalMap(tm),
    polygonColor(),
    borderColor(80, 80, 180),
    segmentSize(3),
    trapezoidMarked(-1)
{

}

void DrawableTrapezoidalMap::draw() const{
    int i = 0;
    if (trapezoids().size() > 1){
        for (const Trapezoid t : trapezoids()) {
            if(isDeleted()[i] == true){

            }else{
                if (std::round(t.top().p1().x()) == std::round(t.bottom().p1().x())){
                    if (std::round(t.top().p1().y()) == std::round(t.bottom().p1().y())){
                        cg3::opengl::drawTriangle2(t.top().p1(), t.top().p2(), t.bottom().p2(), 3, true);
                    }
                }
                if (std::round(t.top().p2().x()) == std::round(t.bottom().p2().x())){
                    if (std::round(t.top().p2().y()) == std::round(t.bottom().p2().y())){
                        cg3::opengl::drawTriangle2(t.top().p1(), t.top().p2(), t.bottom().p1(), 3, true);
                    }
                }
                 cg3::opengl::drawQuad2(t.getVertices(), polygonColor[i], 3, true);
            }
             i++;
        }
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
    int R = QRandomGenerator::global()->bounded(25, 225);
    int G = QRandomGenerator::global()->bounded(55, 175);
    int B = QRandomGenerator::global()->bounded(75, 150);
    return cg3::Color(R, G, B);
};



void DrawableTrapezoidalMap::setPolygonColors(size_t size){
    size_t i = polygonColor.size();
    while (i < size+1){
        polygonColor.push_back(generateRandomColor());
        ++i;
    }
}

void DrawableTrapezoidalMap::markTrapezoid(size_t id){
    if(trapezoidMarked != -1){
        polygonColor[trapezoidMarked] = generateRandomColor();
    }
    polygonColor[id] = cg3::Color(255, 0, 0);
    trapezoidMarked = id;
}
