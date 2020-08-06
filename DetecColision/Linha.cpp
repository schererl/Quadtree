

#include <cstdlib>


void Linha::geraLinhaCurta()
{
    float deltaX,deltaY;
    
    x1 = (rand() % 90)/10.0;
    y1 = x1;//(rand() % float(x1))/10.0;
    deltaX = (rand() % 100)/50.0 - 1;
    deltaY = (rand() % 100)/50.0 - 1;
    
    x2 = x1 + deltaX;
    y2 = y1 + deltaY;

}
void Linha::geraLinha()
{
	x1 = (rand() % 100)/10.0;
	y1 = (rand() % 100)/10.0;
	x2 = (rand() % 100)/10.0;
	y2 = (rand() % 100)/10.0;
}

void Linha::desenhaLinha()
{
	glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	glEnd();
}
