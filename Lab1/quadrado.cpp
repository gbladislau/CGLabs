#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 600

float gX = 0.0;
float gY = 0.0;
int keyStatus[256];

float initialgX = 0.0;
float initialgY = 0.0;
int initialMouseX = 0;
int initialMouseY = 0;

bool insideSquare = false;

void display(void)
{
    /* Limpar todos os pixels  */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f(1.0, 0.8, 0.0);
    /* Desenhar um polígono branco (retângulo) */
    glBegin(GL_POLYGON);
        glVertex3f(0.25 + gX, 0.25 + gY, 0.0);
        glVertex3f(0.75 + gX, 0.25 + gY, 0.0);
        glVertex3f(0.75 + gX, 0.75 + gY, 0.0);
        glVertex3f(0.25 + gX, 0.75 + gY, 0.0);
    glEnd();

    /* Desenhar no frame buffer! */
    glutSwapBuffers(); // Funcao apropriada para janela double buffer
}

void keyPress(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 1;
    glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void idle(void)
{
    if (keyStatus[(int)'a'])
        gX += -0.0001;

    if (keyStatus[(int)'s'])
        gY += -0.0001;

    if (keyStatus[(int)'w'])
        gY += 0.0001;

    if (keyStatus[(int)'d'])
        gX += 0.0001;
    glutPostRedisplay();
}

float rescale(int value){
    return ((float) value/ (float)TAMANHO_JANELA);
}
void toggleInsideSquare(){
    insideSquare = !insideSquare;
}

bool isInside(int x, int y){
    return (rescale(x) >= (0.25 + gX) ) && (rescale(x) <= (gX + 0.75) ) && (rescale(TAMANHO_JANELA - y) >=  (0.25 + gY)) && (rescale(TAMANHO_JANELA - y) <= (gY + 0.75));
}
void mouse(int button, int state, int x, int y)
{
    initialMouseX = x;
    initialMouseY = y;
    insideSquare = false;
    if (isInside(x,y))
    {
        toggleInsideSquare();
        initialgX = gX;
        initialgY = gY;
    }


    glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
    if (insideSquare)
    {
        gX = initialgX + rescale(x - initialMouseX);
        gY = initialgY - rescale(y - initialMouseY);

    }
    glutPostRedisplay();
}

void init(void)
{
    /* selecionar cor de fundo (preto) */
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* inicializar sistema de visualizacao */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(TAMANHO_JANELA, TAMANHO_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("hello world");
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    glutMainLoop();

    return 0;
}
