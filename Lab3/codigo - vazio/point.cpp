#include "point.h"
#include <math.h>
#include <iostream>
#include <cstdio>

void Point2D::Translate(double dx, double dy)
{
    this->setX(this->getX() + dx);
    this->setY(this->getY() + dy);
}
void Point2D::RotateZ(double angle)
{
    double ang_rad = angle *(M_PI/180.0);
    //std::cout <<  angle << std::endl;
    this->setX((this->getX() * cos(ang_rad)) - (this->getY() * sin(ang_rad)));
    this->setY((this->getX() * sin(ang_rad)) + (this->getY() * cos(ang_rad)));
}
