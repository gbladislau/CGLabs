#include "robo.h"
#include <math.h>

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R,G,B);

    glBegin(GL_POLYGON);
        glVertex3f(-width/2, 0, 0.0);
        glVertex3f(width/2, 0 , 0.0);
        glVertex3f(width/2,height, 0.0);
        glVertex3f(-width/2,height, 0.0);
    glEnd();

    // glColor3f(R,R,R);
    // glPointSize(6);
    // glBegin(GL_POINTS);
    //     glVertex3f(0,0,0);
    // glEnd();

}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{

}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{

}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();
        glTranslatef(x,y,0);
        glRotatef(theta1,0,0,1);
        DesenhaRect(paddleHeight,paddleWidth,0,0,1);
        
        glTranslatef(0,paddleHeight,0);
        glRotatef(theta2,0,0,1);
        DesenhaRect(paddleHeight,paddleWidth,1,1,0);


        glTranslatef(0,paddleHeight,0);
        glRotatef(theta3,0,0,1);
        DesenhaRect(paddleHeight,paddleWidth,0,1,0);

    glPopMatrix();
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glLoadIdentity();
    glPushMatrix();
        glTranslatef(x,y,0);
        DesenhaBraco(0,baseHeight,theta1,theta2,theta3);
        DesenhaRect(baseHeight,baseWidth,1,0,0);
    glPopMatrix();
}

void Robo::RodaBraco1(GLfloat inc)
{
    this->gTheta1+=inc;
}

void Robo::RodaBraco2(GLfloat inc)
{
    this->gTheta2+=inc;
}

void Robo::RodaBraco3(GLfloat inc)
{
    this->gTheta3+=inc;
}

void Robo::MoveEmX(GLfloat dx)
{
    this->gX += dx*50;
}

//Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut){

}

Tiro* Robo::Atira()
{

}
