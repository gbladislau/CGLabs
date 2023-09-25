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
    double x, y;
    x = this->getX();
    y = this->getY();
    this->setX((x * cos(ang_rad)) - (y * sin(ang_rad)));
    this->setY((x * sin(ang_rad)) + (y * cos(ang_rad)));
}

float Point2D::AngleBeetwen(Point2D p1, Point2D p2){
    double x = p1.getX() - p2.getX();
    double y = p1.getY() - p2.getY();
    //std::cout << x <<" " << y << std::endl;
    return (atan2(y,x)*180.0/M_PI);
}