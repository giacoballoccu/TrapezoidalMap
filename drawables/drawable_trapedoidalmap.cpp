#include "drawable_trapedoidalmap.h"
#include "QRandomGenerator"

Drawable_TrapedoidalMap::Drawable_TrapedoidalMap():
    polygonColor(80, 180, 80),
    borderColor(80, 80, 180),
    segmentSize(3)
{

}

void Drawable_TrapedoidalMap::draw() const{

};

cg3::Color Drawable_TrapedoidalMap::generateRandomColor() const{
    int R = QRandomGenerator::global()->bounded(0, 255);
    int G = QRandomGenerator::global()->bounded(0, 255);
    int B = QRandomGenerator::global()->bounded(0, 255);
    return cg3::Color(R, G, B, 255);
};

void Drawable_TrapedoidalMap::setRandomPolygonColor(){
    this->polygonColor = this->generateRandomColor();
};
