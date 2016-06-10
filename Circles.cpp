#define WINDOWS     1 /* Set to 1 for Windows, 0 else */
#define UNIX_LINUX  0 /* Set to 1 for Unix/Linux, 0 else */

#if WINDOWS
#include <windows.h>
#include <GL/glut.h>
#endif
#if UNIX_LINUX
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#define XOFF          50
#define YOFF          50
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600


void display(void);
void myinit(void);
void idle(void);
void draw_circle(int x, int y, int r);
void circlePoint(int x, int y, int xc, int yc, int r);

/* Function to handle file input; modification may be needed */
void file_in(int x[], int y[], int r[]);

/* Declaring global variables and arrays for computations and to store info. from the input file */
int Xc = 0, Yc = 0, R = 0, w = 0, l = 0, W = 0, H = 0;
int C = 0, K = 800; /* K is the no. of frames for the circle to reach its full radius */
float S = 0.0f;
int Xa[] = { 0, 0, 0, 0, 0, 0 }, Ya[] = { 0, 0, 0, 0, 0, 0 }, Ra[] = { 0, 0, 0, 0, 0, 0 };

/*-----------------
The main function
------------------*/
int main(int argc, char **argv)
{
	int p = 0;
	cout << "Enter the x coordinate, y coordiante of the center and the radius in succession\n" ;
	cin >> Xc >> Yc >> R;
	

	glutInit(&argc, argv);

	/* Use both double buffering and Z buffer */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(XOFF, YOFF);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CS6533 Assignment 1");
	glutDisplayFunc(display);

	/* Function call to handle file input here */
	file_in(Xa, Ya, Ra);
	w = abs(Xa[p]) + Ra[p];
	l = abs(Ya[p]) + Ra[p];
	
	for (int i = 1; i <= 5; i++)
	{
		if (abs(Xa[i]) + Ra[i] > w) { w = abs(Xa[i]) + Ra[i]; }
		if (abs(Ya[i]) + Ra[i] > l) { l = abs(Ya[i]) + Ra[i]; }
	}

	W = 2 * w;
	H = 2 * l;

	/* Calculating the scaling factor */

	if ((WINDOW_WIDTH/WINDOW_HEIGHT) > (W/H))
	{ 	
		S = WINDOW_HEIGHT/static_cast<float>(H);
	}
	else
	{
		S = WINDOW_WIDTH/static_cast<float>(W);
	}
	
	glutIdleFunc(idle);
	myinit();
	glutMainLoop();
}

/*----------
file_in(): file input function. Modify here.
------------*/
void file_in(int x[], int y[], int r[])
{
	ifstream infile("input_circles.txt");
	int n;
	infile >> n;
	for (int i = 0; i < 6; i++)
	{
		infile >> x[i] >> y[i] >> r[i];
	}
	
}


/*---------------------------------------------------------------------
display(): This function is called once for _every_ frame.
---------------------------------------------------------------------*/
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 0.84, 0);              /* draw in golden yellow */
	
	glPointSize(1.0);                     /* size of each point */

	glBegin(GL_POINTS);
	
	glVertex2i(Xc, Yc);					  /* draw a vertex here */
	
	draw_circle(Xc + WINDOW_WIDTH/2, Yc + WINDOW_HEIGHT/2, R); /* calling the draw circle func. */
	
	
	for (int i = 0, r = 0; i <= 5; i++)
	{
		draw_circle((Xa[i])*S + (WINDOW_WIDTH / 2), (Ya[i])*S + WINDOW_HEIGHT / 2, ((Ra[i] * S) * C) / K);
	}
	

	
	glEnd();

	glFlush();                            /* render graphics */

	glutSwapBuffers();                    /* swap buffers */
}

/*---------------------------------------------------------------------
myinit(): Set up attributes and viewing
---------------------------------------------------------------------*/
void myinit()
{
	glClearColor(0.0, 0.0, 0.92, 0.0);    /* blue background*/

										  /* set up viewing */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}

void draw_circle(int x, int y, int r) /* Bresenham's Algorithm applied */
{
	int xc = x, yc = y, D;
	
	glVertex2i(xc + r, yc);
	glVertex2i(xc - r, yc);
	glVertex2i(xc, yc + r);
	glVertex2i(xc, yc - r);
	
	int x1 = xc + r - 0.5, y1 = yc + 1;
	D = (4 * ((x1 - xc) ^ 2 + (y1 - yc) ^ 2 - r ^ 2));
	
	for (;y1 <= yc + (r / sqrt(2));)
	{
		if (D <= 0)
		{
			glVertex2i(x1 + 0.5, y1);
			circlePoint(x1 + 0.5, y1, xc, yc, r);
			x1 = x1;
			y1 = y1 + 1;
			D = D + 4 * (2 * (y1 - yc) + 1);

		}
		else
		{
			glVertex2i(x1 - 0.5, y1);
			circlePoint(x1 - 0.5, y1, xc, yc, r);
			x1 = x1 - 1;
			y1 = y1 + 1;
			D = D - 8 * ((x1 - xc) - (y1 - yc) - 1);
		}

	}
}

/* Drawing the corresponding points from the point obtained from bresenham's algo. */

void circlePoint(int x, int y, int xc, int yc, int r) 
{
	glVertex2i(xc - (x - xc), y);
	glVertex2i(xc - (x - xc), yc - (y - yc));
	glVertex2i(x, yc - (y - yc));
	glVertex2i(xc + (y - yc), yc + (x - xc));
	glVertex2i(xc + (y - yc), yc - (x - xc));
	glVertex2i(xc - (y - yc), yc - (x - xc));
	glVertex2i(xc - (y - yc), yc + (x - xc));
}

void idle(void) /* growing animation */
{
	C += 1;
	if (C > K) C = 1;
	display();

}