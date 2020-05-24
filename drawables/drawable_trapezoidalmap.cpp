#include "drawable_trapezoidalmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>
/**
 * @brief DrawableTrapezoidalMap constructor
 */
DrawableTrapezoidalMap::DrawableTrapezoidalMap():
    polygonColor(),
    borderColor(80, 80, 180),
    segmentSize(3),
    trapezoidMarked(SIZE_MAX) //Used to reset the color of the previous queried trapezoid.
{
    TrapezoidalMap();
    polygonColor.push_back(generateRandomColor()); //Set first color
}
/**
 * @brief DrawableTrapezoidalMap::draw
 * Print the trapezoid which aren't flagged as deleted and check if a trapezoid is degenerated into a triangle in order
 * to call the right drawing method and avoid pivot errors.
 */
void DrawableTrapezoidalMap::draw() const{
    int i = 0;
    std::vector<bool> _isDeleted = isDeleted();
    for (const Trapezoid& t : trapezoids()) {
        if(_isDeleted[i] == false){
            if (geoutils::arePointsEqual(t.top().p1(),t.bottom().p1())){
                cg3::opengl::drawTriangle2(t.top().p1(), t.top().p2(), t.bottom().p2(), polygonColor[i], 1, true);
            }else if (geoutils::arePointsEqual(t.top().p2(),t.bottom().p2())){
                cg3::opengl::drawTriangle2(t.bottom().p1(), t.bottom().p2(), t.top().p1(), polygonColor[i], 1, true);
            }else{
                cg3::opengl::drawQuad2(t.getVertices(), polygonColor[i], 1, true);
            }
        }
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
/**
 * @brief Pseudo generation of a color.
 * @return pseudo random cg3::Color
 */
inline cg3::Color DrawableTrapezoidalMap::generateRandomColor() const{
    return cg3::Color(QRandomGenerator::global()->bounded(25, 225),
                      QRandomGenerator::global()->bounded(55, 175),
                      QRandomGenerator::global()->bounded(75, 150));
};


/**
 * @brief Add random colors to the array of color until the array of color has the same size of the size passed as parameter.
 * @param size, size that the vector of color need to reach, is the size of the trapezoids vector.
 */
void DrawableTrapezoidalMap::setPolygonColors(size_t size){
    size_t i = polygonColor.size();
    while (i < size+1){
        polygonColor.push_back(generateRandomColor());
        ++i;
    }
}

void DrawableTrapezoidalMap::markTrapezoid(size_t id){
    if(trapezoidMarked != SIZE_MAX){
        polygonColor[trapezoidMarked] = generateRandomColor();
    }
    polygonColor[id] = cg3::Color(0, 0, 255);
    trapezoidMarked = id;
}
