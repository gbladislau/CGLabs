#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 600

float gX = 0.25;
float gY = 0.25;
int keyStatus[256];

void display(void)
{
    /* Limpar todos os pixels  */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f(1.0, 0.8, 0.0);
    /* Desenhar um polígono branco (retângulo) */
    glBegin(GL_POLYGON);
    glVertex3f(gX, gY, 0.0);
    glVertex3f(0.5 + gX, gY, 0.0);
    glVertex3f(0.5 + gX, 0.5 + gY, 0.0);
    glVertex3f(gX, 0.5 + gY, 0.0);
    glEnd();

    /* Desenhar no frame buffer! */
    glutSwapBuffers(); // Funcao apropriada para janela double buffer
}

void keyPress(unsigned char key, int x, int y)
{
    // switch (key)
    // {
    // case 'a':
    //    gX += -0.01;
    //    break;
    // case 's':
    //    gY += -0.01;
    //    break;
    // case 'w':
    //    gY += 0.01;
    //    break;
    // case 'd':
    //    gX += 0.01;
    //    break;
    // default:
    //    break;
    // }
    keyStatus[(int)(key)] = 1;
    glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

int mouseState = 1;

void idle(void)
{
    if (!mouseState)
    {
    }
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

int initialMouseX = 0;
int initialMouseY = 0;
bool insideSquare = false;
void mouse(int button, int state, int x, int y)
{
    initialMouseX = x;
    initialMouseY = TAMANHO_JANELA - y;
    printf("x, y, button, state : (%f, %f, %d, %d)\n", (float)(x) / TAMANHO_JANELA, (float)(TAMANHO_JANELA - y) / TAMANHO_JANELA, button, state);
    printf("gX, gY, button, state : (%f, %f, %d, %d)\n", gX, gY, button, state);

    if ((float)initialMouseX / TAMANHO_JANELA >= gX && (float)initialMouseX / TAMANHO_JANELA <= gX + 0.5 &&
        (float)initialMouseY / TAMANHO_JANELA >= gY && (float)initialMouseY / TAMANHO_JANELA <= gY + 0.5)
        insideSquare = true;
    else insideSquare = false;

    glutPostRedisplay();
}

void mouseMotion(int x, int y)
{

    if (insideSquare)
    {
        // Valor = atual - pos inicial
        gX = ((float)(x) / TAMANHO_JANELA) - ((float)initialMouseY / TAMANHO_JANELA) ;
        gY = ((float)(TAMANHO_JANELA - y) / TAMANHO_JANELA) - ((float)initialMouseY / TAMANHO_JANELA);
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
    glutInitWindowPosition(400, 100);
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
