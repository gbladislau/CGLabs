#include "tiro.h"
#include <math.h>
#include <cstdio>
#include <iostream>
#include "point.h"
#define DISTANCIA_MAX 500

extern void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut);
//Funcao auxiliar de translação
extern void TranslatePoint(GLfloat x, GLfloat y, GLfloat dx, GLfloat dy, GLfloat &xOut, GLfloat &yOut);

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
        glColor3f(R,G,B);
        glPointSize(1);
        
        int qntdPoints = 360/1;
        Point2D *point = new Point2D(0,0);

        glBegin(GL_POLYGON);

            for (int p = 0; p < qntdPoints; p++){
                point->Translate(0,radius);
                glVertex3f(point->getX(),point->getY(),0);   
                point->RotateZ(1);
                point->Translate(0,-radius);
            }
            
        glEnd();     
      

    glPopMatrix();   
    delete point;
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glLoadIdentity();
    glPushMatrix(); 
        
        glTranslatef(x,y,0);
        DesenhaCirc(radiusTiro,1,1,1);
    glPopMatrix();
}

void Tiro::Move(GLdouble timedif)
{
    this->gX += (this->gVel * cos(this->gDirectionAng))*timedif;
    this->gY += (this->gVel * sin(this->gDirectionAng))*timedif;
}

bool Tiro::Valido()
{
    if( abs(this->gX) >= DISTANCIA_MAX/2 || abs(this->gY) >= DISTANCIA_MAX/2){
       // std::cout << "DELETOU TIRO" << std::endl;
        return false;
    }
    return true;
}
