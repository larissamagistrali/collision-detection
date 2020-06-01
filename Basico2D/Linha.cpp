

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Linha.h"void Linha::geraLinhaCurta(){    float deltaX,deltaY;    x1 = (rand() % 90)/10.0;    y1 = (rand() % 90)/10.0;    deltaX = (rand() % 100)/50.0 - 1;    deltaY = (rand() % 100)/50.0 - 1;    x2 = x1 + deltaX;    y2 = y1 + deltaY;}void Linha::geraLinha(){	x1 = (rand() % 100)/10.0;	y1 = (rand() % 100)/10.0;	x2 = (rand() % 100)/10.0;	y2 = (rand() % 100)/10.0;}void Linha::desenhaLinha(){	glBegin(GL_LINES);		glVertex2f(x1,y1);		glVertex2f(x2,y2);	glEnd();}