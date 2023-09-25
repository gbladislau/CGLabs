#include "robo.h"
#include "point.h"
#include <math.h>
#include <iostream>

Point2D pontoDeApoio = Point2D();
Point2D ponto2 = Point2D();

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R,G,B);

    glBegin(GL_POLYGON);
        glVertex3f(-width/2, 0, 0.0);
        glVertex3f(width/2, 0 , 0.0);
        glVertex3f(width/2,height, 0.0);
        glVertex3f(-width/2,height, 0.0);
    glEnd();



}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
        glColor3f(R,G,B);
        glPointSize(2);
        
        int qntdPoints = 360/20;
        for (int p = 0; p < qntdPoints; p++){
            glPushMatrix();
                glTranslatef(0,radius,0);
                glBegin(GL_POINTS);
                    glVertex3f(0,0,0);        
                glEnd();
            glPopMatrix();
            glRotatef(20,0,0,1);
        }

    glPopMatrix();
}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
        glTranslatef(x,y,0);
        glRotatef(thetaWheel,0,0,1);
        DesenhaCirc(radiusWheel,R,G,B);
    glPopMatrix();

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
        glColor3f(1,1,1);
        glPointSize(6);
        glBegin(GL_LINES);
            glVertex3f(pontoDeApoio.getX(),pontoDeApoio.getY(),0);
            glVertex3f(ponto2.getX(),ponto2.getY(),0);
        glEnd();
        glTranslatef(x,y,0);
        DesenhaRect(baseHeight,baseWidth,1,0,0);
        DesenhaBraco(0,baseHeight,theta1,theta2,theta3);
        DesenhaRoda(baseWidth/2,0,thetaWheel,1,1,1);
        DesenhaRoda(-baseWidth/2,0,thetaWheel,1,1,1);
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
    this->gX += dx*100;
    this->gThetaWheel -= (dx*100/radiusWheel) *(180/M_PI);
}

//Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut){
    GLfloat ang_rad = angle *(M_PI/180.0);
    //std::cout << ang_rad << std::endl;
    xOut = (x * cos(ang_rad)) - (y * sin(ang_rad));
    yOut = (x * sin(ang_rad)) + (y * cos(ang_rad));
   // printf("r %f  %f\n",xOut,yOut);
}

//Funcao auxiliar de translação
void TranslatePoint(GLfloat x, GLfloat y, GLfloat dx, GLfloat dy, GLfloat &xOut, GLfloat &yOut){
    xOut = x+dx;
    yOut = y+dy;
   // printf("t %f %f\n",xOut,yOut);
}

Tiro* Robo::Atira()
{
    glLoadIdentity();
    glPushMatrix();

        Point2D * point = new Point2D();

        // std::cout << point->getX() <<" " << point->getY() << std::endl;

        point->Translate(0,paddleHeight);
        point->RotateZ(this->gTheta3);
        point->Translate(0,paddleHeight);
        
        point->RotateZ(this->gTheta2);
        point->Translate(0,paddleHeight);

        point->RotateZ(this->gTheta1);
        point->Translate(this->ObtemX(),this->ObtemY());
    
        point->Translate(0,baseHeight);

        Point2D reference = Point2D();
        // reference.Translate(0,paddleHeight);
        // reference.RotateZ(this->gTheta3);
        reference.Translate(0,paddleHeight);
        
        reference.RotateZ(this->gTheta2);
        reference.Translate(0,paddleHeight);

        reference.RotateZ(this->gTheta1);
        reference.Translate(this->ObtemX(),this->ObtemY());
    
        reference.Translate(0,baseHeight);
       
        pontoDeApoio.setX(reference.getX());
        pontoDeApoio.setY(reference.getY());
        ponto2.setX(point->getX());
        ponto2.setY(point->getY());

        GLfloat angulo = point->AngleBeetwen(reference,*point);
        std::cout <<  point->AngleBeetwen(reference,*point) << "\n" << angulo<< std::endl;
        return new Tiro(point->getX(),point->getY(), angulo);

    glPopMatrix();
    delete point;

}
