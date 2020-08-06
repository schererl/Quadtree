#include <iostream>
#include <cmath>

using namespace std;

#ifdef _MSC_VER

#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "SOIL/SOIL.h"

#include "ImageClass.h"

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(1,1);
    glEnd();
    glutSwapBuffers();
}

// Funcao chamada quando uma tecla e' pressionada
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)		// ESC ?
	{
		exit(0);
	}
	// Redesenha a tela
	glutPostRedisplay();
}

// Funcao responsavel por inicializar parametros e variaveis
int main()
{

    int argc = 1;
    char *argv[] = { (char *)"gl", 0 };
	
    glutInit(&argc,argv);
	cout << "Ola" << endl;
	
	// Define do modo de operacao da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(400,400);

	// Cria a janela passando como argumento o titulo da mesma
	glutCreateWindow("Teste GLUT");

	// Registra a funcao callback de redesenho da janela de visualizacao
	glutDisplayFunc(draw);

	// Registra a funcao callback para tratamento das teclas ASCII
	glutKeyboardFunc (keyboard);

	// Define a janela de visualizacao 2D
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,1.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}

