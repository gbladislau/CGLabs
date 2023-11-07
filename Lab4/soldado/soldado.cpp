#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <math.h>
#include <cstring>
#include <stdlib.h>
#include "objloader.h"
using namespace std;

//Malhas a serem desenhadas
mesh soldado;
mesh soldado_transf;
mesh arma;

//Controles gerais
int zoom = 150;
int lookatToggle = 1;
int transformacaoArmaToggle = 1;
int armaToggle = 1;
int coordsysToggle = 1;
double camXYAngle=0;
double camXZAngle=0;
int lastX = 0;
int lastY = 0;
int buttonDown=0;
int soldado_orig = 1;

void init ()
{
    glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    //Carrega as meshes dos arquivos
    soldado.loadMesh("Blender/untitledc1.obj");
    soldado_transf.loadMesh("Blender/untitledc2-transf.obj");
    arma.loadMesh("Blender/armanova1.obj");
}

void reshape (int w, int h)
{
    //Ajusta o tamanho da tela com a janela de visualizacao
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluPerspective (45, (GLfloat)h/(GLfloat)w, 1, 1000);
    else
        gluPerspective (45, (GLfloat)w/(GLfloat)h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

//Funcao auxiliar para normalizar um vetor a/|a|
void normalize(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

//Funcao auxiliar para fazer o produto vetorial entre dois vetores a x b = out
void cross(float a[3], float b[3], float out[3])
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

//Aplica a transformacao que coloca o sistema de coordendas local em uma posicao 
//do mundo definida por dois pontos a e b e um vetor up. O ponto b (p1) sera a origem,
//o vetor BA = A - B sera determinara o novo y, o vetor up definira a orientacao
//do novo z em torno de BA.
//A matriz de transformacao no opengl eh armazanada transposta: 
//m[4][4] = {Xx, Xy, Xz, 0.0, Yx, Yy, Yz, 0.0, Zx, Zy, Zz, 0.0, Tx, Ty, Tz, 1.0}
// a = p2 b = p1
void ChangeCoordSys(
        GLfloat ax, GLfloat ay, GLfloat az, 
        GLfloat bx, GLfloat by, GLfloat bz, 
        GLfloat upx, GLfloat upy, GLfloat upz)
{
    float x[3], y[3], z[3];

    // y linha
    GLfloat p2_p1[3] = { ax - bx, ay - by, az - bz};
    normalize(p2_p1);   
    y[0] = p2_p1[0]; y[1] = p2_p1[1]; y[2] = p2_p1[2];

    // x linha
    GLfloat x_linha[3];
    GLfloat up_v[3] = {upx,upy,upz}; 
    cross(y,up_v,x_linha);
    normalize(x_linha);
    x[0] = x_linha[0]; x[1] = x_linha[1]; x[2] = x_linha[2];

    // z linha
    cross(x,y,z);

    GLfloat m[4][4] = 
        {x[0],x[1],x[2],0.0,
         y[0],y[1],y[2],0.0,
         z[0],z[1],z[2],0.0,
         bx,by,bz,1.0};

    glMultMatrixf(&m[0][0]);
}

void DrawAxes(double size)
{
    GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, 
            no_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);

    //x axis
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_r);
        glColor3fv(mat_ambient_r);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //y axis
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_g);
        glColor3fv(mat_ambient_g);
        glRotatef(90,0,0,1);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //z axis
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_b);
        glColor3fv(mat_ambient_b);
        glRotatef(-90,0,1,0);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();
    
}

//ALTERE AQUI - SEU CODIGO AQUI
//Usar meshlab para obter os pontos abaixo
int pontoArmaAponta = 8501; //p2
int pontoPosicaoArma = 2906; //p1
int p_up1 = 2916;
int p_up2 = 3143; ////         Up vector
int p_up3 = 2697;
// int up[3] = {0, 0, 1};
void desenhaJogador(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        //Translada para o centro do soldado para facilitar a rotacao da camera
        glTranslatef(0,-50,0);

        float v1_x = soldado.vertsPos[p_up1].x - soldado.vertsPos[p_up2].x;
        float v1_y = soldado.vertsPos[p_up1].y - soldado.vertsPos[p_up2].y;
        float v1_z = soldado.vertsPos[p_up1].z - soldado.vertsPos[p_up2].z;

        float v2_x = soldado.vertsPos[p_up1].x - soldado.vertsPos[p_up3].x;
        float v2_y = soldado.vertsPos[p_up1].y - soldado.vertsPos[p_up3].y; 
        float v2_z = soldado.vertsPos[p_up1].z - soldado.vertsPos[p_up3].z; 

        float v1[3] = {v1_x,v1_y,v1_z};
        float v2[3] = {v2_x,v2_y,v2_z};
        float up[3];
        cross(v2,v1,up);
        // normalize(up);

        if (soldado_orig){
            soldado.draw();

            if (transformacaoArmaToggle == 2){
                ChangeCoordSys( soldado.vertsPos[pontoArmaAponta].x, soldado.vertsPos[pontoArmaAponta].y, soldado.vertsPos[pontoArmaAponta].z,
                                soldado.vertsPos[pontoPosicaoArma].x, soldado.vertsPos[pontoPosicaoArma].y, soldado.vertsPos[pontoPosicaoArma].z,
                                up[0], up[1], up[2]);
            } 
        } else {
            soldado_transf.draw();

            if (transformacaoArmaToggle == 2){
                ChangeCoordSys( soldado_transf.vertsPos[pontoArmaAponta].x, soldado_transf.vertsPos[pontoArmaAponta].y, soldado_transf.vertsPos[pontoArmaAponta].z,
                                soldado_transf.vertsPos[pontoPosicaoArma].x, soldado_transf.vertsPos[pontoPosicaoArma].y, soldado_transf.vertsPos[pontoPosicaoArma].z,
                                up[0], up[1], up[2]);
            } 
        }
        
        if (coordsysToggle == 1)
            DrawAxes(10);
        if (armaToggle == 1)
            arma.draw();
    glPopMatrix();
}

//Aplica a transformacao que modifica o mundo virtual para deixa-lo como se
//tivesse sendo visto da posicao determinada por eye (eyex, eyey, eyez)
//olhando para center (centerx, centery, centerz)
//e orientada em torno do vetor direcional por up (upx, upy, upz)
//A matriz de transformacao no opengl eh armazanada transposta: 
//m[4][4] = {Xx, Xy, Xz, 0.0, Yx, Yy, Yz, 0.0, Zx, Zy, Zz, 0.0, Tx, Ty, Tz, 1.0}
void MygluLookAt(
        GLfloat eyex, GLfloat eyey, GLfloat eyez, 
        GLfloat centerx, GLfloat centery, GLfloat centerz, 
        GLfloat upx, GLfloat upy, GLfloat upz)
{
    float forward[3], side[3], up[3];
    //column-major order
    
    GLfloat p2_p1[3] = { centerx - eyex, centery - eyey, centerz - eyez};
    normalize(p2_p1);   
    forward[0] = p2_p1[0]; forward[1] = p2_p1[1]; forward[2] = p2_p1[2];

    GLfloat x_linha[3];
    GLfloat up_v[3] = {upx,upy,upz}; 
    normalize(up_v);
    cross(forward,up_v,x_linha);
    normalize(x_linha);
    side[0] = x_linha[0]; side[1] = x_linha[1]; side[2] = x_linha[2];

    cross(side,forward,up);
    normalize(up);

    GLfloat m[4][4] = { side[0],up[0],-forward[0], 0,
                        side[1],up[1],-forward[1],0,
                        side[2],up[2],-forward[2],0,
                        0,0,0,1};


    glMultMatrixf(&m[0][0]);
    glTranslatef(-eyex, -eyey, -eyez);

}

void display(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Controla camera
    if (lookatToggle){
        //Limpa a cor com azulado
        glClearColor (0.30, 0.30, 1.0, 0.0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Utiliza uma esfera de raio zoom para guiar a posicao da camera
        //baseada em dois angulos (do plano XZ e do plano XY)
        gluLookAt(  zoom*sin(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
                    zoom*                         sin((camXYAngle*M_PI/180)),
                    zoom*cos(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
                    0, 0, 0,
                    0, 1, 0);
    } else{
        //Limpa a cor com azulado
        glClearColor (1.0, 0.30, 0.30, 0.0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        MygluLookAt(  zoom*sin(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
                    zoom*                         sin((camXYAngle*M_PI/180)),
                    zoom*cos(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
                    0, 0, 0,
                    0, 1, 0);
    }
    
    desenhaJogador();

    //Define e desenha a fonte de luz
    GLfloat light_position[] = {10, 10, 50, 1};
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glDisable(GL_LIGHTING);
        glPointSize(15);
        glColor3f(1.0,1.0,0.0);
        glBegin(GL_POINTS);
            glVertex3f(light_position[0],light_position[1],light_position[2]);
        glEnd();    
    glEnable(GL_LIGHTING);
    
    glutSwapBuffers ();
}

void keyPress(unsigned char key, int x, int y)
{
    switch(key){
    case '1':
        transformacaoArmaToggle = 1;
        break;
    case '2':
        transformacaoArmaToggle = 2;
        break;
    case 'a':
        armaToggle = !armaToggle;
        break;
    case 'c':
        coordsysToggle = !coordsysToggle;
        break;
    case 'm':
        lookatToggle = !lookatToggle;
        break;
    case 's':
        soldado_orig = !soldado_orig;
        break;
    case '+':
        zoom++;
        break;
    case '-':
        zoom--;
         break;
    case 27 :
         exit(0);
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        lastX = x;
        lastY = y;
        buttonDown = 1;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        buttonDown = 0;
    }
    glutPostRedisplay();
}

void mouse_motion(int x, int y)
{
    if (!buttonDown)
        return;
    
    camXZAngle -= x - lastX;
    camXYAngle += y - lastY;

    lastX = x;
    lastY = y;
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (700,700);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("Soldado com Arma");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutMotionFunc(mouse_motion);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
