#include "alvo.h"
#include "point.h"
#include <math.h>

void Alvo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
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

void Alvo::DesenhaAlvo(GLfloat x, GLfloat y)
{
    glPushMatrix();
        glTranslatef(x,y,0);
        GLfloat cVec[3] = { 0 ,0 ,0};
        cVec[this->gColor] = 1.0;
        DesenhaCirc(radiusAlvo,cVec[0],cVec[1],cVec[2]);
    glPopMatrix();
 
}

void Alvo::Recria(GLfloat x, GLfloat y)
{
    this->gColor = (this->gColor+1)%3;
    this->gX = x;
    this->gY = y;
}

bool Alvo::Atingido(Tiro *tiro)
{   
    GLfloat x;
    GLfloat y;
    tiro->GetPos(x,y);
    return (sqrt(pow(this->gX - x,2)+ pow(this->gY - y,2)) <= radiusAlvo);
}
