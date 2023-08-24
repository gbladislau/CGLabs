#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

struct MouseCoordanates{
   int previousX = 0.0;
   int previousY = 0.0;
};
MouseCoordanates mouseCoordanates;

struct MovingPoint
{
   GLfloat previousX = 0.0;
   GLfloat previousY = 0.0;
   int pointIndex = 0;
   bool isMoving = false;
};
MovingPoint movingPoint;

float size = 1.0;

//Pontos de controle da Spline
GLfloat ctrlpoints[4][3] = {
        { 0.1, 0.1, 0.0}, { 0.3, 0.9, 0.0}, 
        {0.7, 0.1, 0.0}, {0.9, 0.9, 0.0}};


void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_MAP1_VERTEX_3);

   //Definicao do polinomio com os pontos de controle
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   
   //Muda para a matriz de projecao (aulas futuras)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //Define a area/volume de visualizacao. Os objetos desenhados devem estar dentro desta area
   glOrtho(0.0, size, 0.0, size, 0.0 , size);
}

void display(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);
   
   /* Desenha a curva aproximada por n+1 pontos. */
   int n = 30;
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= n; i++){
        //Avaliacao do polinomio, retorna um vertice (equivalente a um glVertex3fv) 
        glEvalCoord1f((GLfloat) i/(GLfloat)n);
      }
   glEnd();
   
   /* Desenha os pontos de controle. */
   glPointSize(5.0);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
   glutSwapBuffers();
}

void reshape(int w, int h)
{
   //Define a porcao visivel da janela
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   
   //Muda para a matriz de projecao (aulas futuras)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   //Controla o redimensionamento da janela mantendo o aspect ration do objeto
   if (w <= h)
      glOrtho(-size, size, -size*(GLfloat)h/(GLfloat)w, 
               size*(GLfloat)h/(GLfloat)w, -size, size);
   else
      glOrtho(-size*(GLfloat)w/(GLfloat)h, 
               size*(GLfloat)w/(GLfloat)h, -size, size, -size, size);
   
   //Muda para a matriz de trasformacoes (aulas futuras)
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

float recalc (int value){
   return (float) value / TAMANHO_JANELA;
}

bool isInRange(GLfloat coordenadas[3]){
   if (sqrt(pow(recalc(mouseCoordanates.previousX)-coordenadas[0],2)+ pow(recalc(TAMANHO_JANELA-mouseCoordanates.previousY)-(coordenadas[1]),2) <= recalc(30)))
      return true;
   return false;
}

void moveNearestPoint(int x, int y){
   for (int i = 0; i < 4; i++)
   {
      if(isInRange(ctrlpoints[i])){
         printf("ACHEI um  %d", i);
         movingPoint.isMoving = true;
         movingPoint.pointIndex = i;
         movingPoint.previousX = ctrlpoints[i][0];
         movingPoint.previousY = ctrlpoints[i][1];
         break;
      }
   }
}

void mouse(int button, int state, int x, int y){
   mouseCoordanates.previousX = x;
   mouseCoordanates.previousY = y;
   if (!state){
      moveNearestPoint(x,y);
   } else movingPoint.isMoving = false;
   glutPostRedisplay();
}

void motion(int x, int y){
   if(movingPoint.isMoving){
      ctrlpoints[movingPoint.pointIndex][0] = (movingPoint.previousX + recalc(x - mouseCoordanates.previousX)) ;
      ctrlpoints[movingPoint.pointIndex][1] = (movingPoint.previousY - recalc(y - mouseCoordanates.previousY)) ;
   }
   glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   //glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
