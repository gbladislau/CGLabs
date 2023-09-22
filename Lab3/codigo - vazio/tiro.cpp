#include "tiro.h"
#include <math.h>
#include <cstdio>
#define DISTANCIA_MAX 500

extern void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut);
//Funcao auxiliar de translação
extern void TranslatePoint(GLfloat x, GLfloat y, GLfloat dx, GLfloat dy, GLfloat &xOut, GLfloat &yOut);

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
        glColor3f(R,G,B);
        glPointSize(1);
        
        int qntdPoints = 360/20;
        GLfloat x = 0;
        GLfloat y = 0;
        glBegin(GL_POLYGON);

        for (int p = 0; p < qntdPoints; p++){
            
            TranslatePoint(x,y,0,radius,x,y);
                glVertex3f(x,y,0);   
            RotatePoint( x,  y, 20,x, y);
        }
        glEnd();     
      

    glPopMatrix();   
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glPushMatrix(); 
    //printf("gx: %f gy: %f\n",x,y);
        glTranslatef(x,y,0);
        DesenhaCirc(radiusTiro,1,1,1);
    glPopMatrix();
}

void Tiro::Move()
{
    glPushMatrix();
        glRotatef(this->gDirectionAng,0,0,1);
        DesenhaTiro(this->gX,this->gY);
    glPopMatrix();
}

bool Tiro::Valido()
{
    if( this->gX > DISTANCIA_MAX || this->gY > DISTANCIA_MAX){
        return false;
    }
    return true;
}
