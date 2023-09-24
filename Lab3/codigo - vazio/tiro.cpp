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
        //std::cout << x << y << std::endl;
        DesenhaCirc(radiusTiro,1,1,1);
    glPopMatrix();
}

void Tiro::Move()
{
    glPushMatrix();
        glRotatef(this->gDirectionAng,0,0,1);

        this->gX+=this->gVel;
        this->gY+=this->gVel;

        glTranslatef(this->gX,this->gY,0);
        DesenhaTiro(this->gX,this->gY);
    glPopMatrix();
}

bool Tiro::Valido()
{
    if( this->gX >= DISTANCIA_MAX || this->gY >= DISTANCIA_MAX){
        std::cout << "DELETOU TIRO" << std::endl;
        return false;
    }
    return true;
}
