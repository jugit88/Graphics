/*
 * Lorenz Attractor
 *
 *  Display Visual of Lorenz Attractor.
 *
 *  Key bindings
  1      Decrease r parameter by 1
  2      Increase r parameter by 1
  3      Decrease s parameter by 0.5
  4      Increase s parameter by 0.5
  5		 Decrease b parameter by 0.1
  6		 Increase b parameter by 0.1
  +/-    Increase/decrease w
  arrows Change view angle
  0      Reset view angle
  ESC    Exit

 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h> 
double s  = 10;
double b  = 2.666;
double r  = 28;
double x = 0.01, y = 0.01, z = 0.01 , dt = 0.01;



//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


//  Globals
int th=30;       // Azimuth of view angle
int ph=20;       // Elevation of view angle
int mode=3;     // Dimension (1-4)
// double z=0;     // Z variable
double w=1;     // W variable
double dim=3;   // Dimension of orthogonal box
char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
	char    buf[LEN];
	char*   ch=buf;
	va_list args;
	//  Turn the parameters into a character string
	va_start(args,format);
	vsnprintf(buf,LEN,format,args);
	va_end(args);
	//  Display the characters one at a time at the current raster position
	while (*ch)
	  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Display the scene
 */
void display()
{
	//  Clear the image
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//  Reset previous transforms
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();

	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);

  
// Create Axis
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(1,0,0);
	glVertex3d(0,0,0);
	glVertex3d(0,1,0);
	glVertex3d(0,0,0);
	glVertex3d(0,0,1);
	glEnd();
	//  Label axes
	glRasterPos3d(1,0,0);
	Print("X");
	glRasterPos3d(0,1,0);
	Print("Y");
	glRasterPos3d(0,0,1);
	Print("Z");
	//  Display parameters
	glWindowPos2i(5,5);
	Print("View Angle=%d,%d,%s ",th,ph,text[mode]);
	Print("r=%f,b=%f,s=%f, w=%f",r,b,s,w);
	glScaled(.02,.02,.02);
	glBegin(GL_LINE_STRIP);
	glColor3f(.1,0.5,1);
	glPointSize(5);
	// Perform Lorenz Equations N times. Image ideal around N = 10,000  
	  for (int i = 0; i < 10000; i++) {

				double dx = s*(y-x);
				double dy = x*(r-z)-y;
				double dz = (x*y) - (b*z);
				x += dt*dx;
				y += dt*dy;
				z += dt*dz;
			glVertex4d(x, y, z, w);
		 }



	glEnd();
	//  Draw axes in white
	
	//  Flush and swap
	glFlush();
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
	if (ch == 27)
	  exit(0);
	//  Reset view angle
	else if (ch == '0')
	  th = ph = 0;
	// Modify parameters
	else if (ch == '1') {
	  if (r >= 1) 
	  	r -= 1; 
	  else
	  	Print("NA");
	}
	else if (ch == '2')
	  if (r <= 50) 
	  	r+= 1; 
	  else
	  	Print("NA");
	else if (ch == '3') {
	  if (s >= 0.5)
	  	s -= 0.5;
	  else 
	    Print("NA");
	}
	else if (ch == '4')
	  s += 0.5;
	else if (ch == '5') {
	  if (b >= 0.1)
	  	b -= 0.1;
	  else 
	  	Print("NA");
	}
	else if (ch == '6')
	  b += 0.1;
	
	// Increase w by 0.1
	else if (ch == '+')
	  w += 0.1;
	
	//  Decrease w by 0.1
	else if (ch == '-') {
	  if (w >= 0.1)
	  	w -= 0.1;
	  else 
	  	Print("NA");
	}
	
	
	// Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
  

}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	//  Right arrow key - increase azimuth by 5 degrees
	if (key == GLUT_KEY_RIGHT)
	  th += 5;
	//  Left arrow key - decrease azimuth by 5 degrees
	else if (key == GLUT_KEY_LEFT)
	  th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
	  ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
	  ph -= 5;
	
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	double w2h = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	//  Orthogonal projection box adjusted for the
	//  aspect ratio of the window
	glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
	glutInit(&argc,argv);
	//  Request double buffered, true color window 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	//  Request 650 x 600 pixel window
	glutInitWindowSize(650,600);
	//  Create the window
	glutCreateWindow("Lorenz Attractor");
	//  Tell GLUT to call "display" when the scene should be drawn
	glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
	glutReshapeFunc(reshape);
	//  Tell GLUT to call "special" when an arrow key is pressed
	glutSpecialFunc(special);
	//  Tell GLUT to call "key" when a key is pressed
	glutKeyboardFunc(key);
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	//  Return code
  
	return 0;
}
