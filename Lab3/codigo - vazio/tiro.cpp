#include "tiro.h"
#include <math.h>
#define DISTANCIA_MAX 500

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
        glColor3f(R,G,B);
        glPointSize(5);
        
        int qntdPoints = 360/20;
        for (int p = 0; p < qntdPoints; p++){
            glPushMatrix();
                glTranslatef(0,radius,0);
                glBegin(GL_POLYGON);
                    glVertex3f(0,0,0);        
                glEnd();
            glPopMatrix();
            glRotatef(20,0,0,1);
        }

    glPopMatrix();   
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glTranslatef(x,y,0);
    DesenhaCirc(radiusTiro,1,1,1);
}

void Tiro::Move()
{
    glPushMatrix();
        glRotatef(this->gDirectionAng,0,0,1);
        DesenhaTiro(this->gX+this->gVel,this->gY);
        this->gX += this->gVel;
    glPopMatrix();
}

bool Tiro::Valido()
{
    if( this->gX > DISTANCIA_MAX || this->gY > DISTANCIA_MAX){
        return false;
    }
    return true;
}
