/*
 *  Homework 2
 *
 *  Draw a few 3d objects. Includes a rocket, spere, and cylinder.
 *
 *  Key bindings:
 *  0,1,2      Toggle between orthogonal(0),perspective(1),first person perspective(2)
 *  +/-        Changes field of view for perspective
 *  b/B        Toggle axes
 *  arrows     Change view angle(for perspective and orthogonal ONLY)
 *  PgDn/PgUp  Zoom in and out
 *  a/w/s/d    move left/move in/move out/move right in first person perspective     
 *  4          Reset view angle
 *  ESC        Exit
 *  
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=-5;         //  Azimuth of view angle
int ph=35;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
double zh=0;      //  Rotation of teapot
float xpos = 0.0;
float ypos = 1.0;
float zpos = 10.0;
float yrot = 0.0;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
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
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode == 1 || mode == 2)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */

static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
static void rocket(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz,double sx, double sy, double sz)
{
   // Dimensions used to size airplane
   const double wid=0.03*2;
   const double nose=+0.50*2;
   const double cone= 0.20*2;
   const double strk=-0.20*2;
   const double tail=-0.50*2;
   //  Unit vector in direction of flght
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double mat[16];
   mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
   mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
   mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
   mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

   //  Save current transforms
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glMultMatrixd(mat);
   //  Nose (4 sided)
   glScaled(sx,sy,sz);
   glColor3f(0,0,1);
   glBegin(GL_TRIANGLES);
   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid, wid);
   glVertex3d(cone,-wid, wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid,-wid);
   glVertex3d(cone,-wid,-wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid, wid);
   glVertex3d(cone, wid,-wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone,-wid, wid);
   glVertex3d(cone,-wid,-wid);
   glEnd();
   //  Fuselage (square tube)
   glColor3f(1,1,1);
   glBegin(GL_POLYGON);

   glVertex3d(cone, wid, wid);
   glVertex3d(cone,-wid, wid);
   glVertex3d(tail/3,-wid, wid);
   glVertex3d(tail/3, wid, wid);

   glVertex3d(cone, wid,-wid);
   glVertex3d(cone,-wid,-wid);
   glVertex3d(tail/3,-wid,-wid);
   glVertex3d(tail/3, wid,-wid);

   glVertex3d(cone, wid, wid);
   glVertex3d(cone, wid,-wid);
   glVertex3d(tail/3, wid,-wid);
   glVertex3d(tail/3, wid, wid);

   glVertex3d(cone,-wid, wid);
   glVertex3d(cone,-wid,-wid);
   glVertex3d(tail/3,-wid,-wid);
   glVertex3d(tail/3,-wid, wid);

   glVertex3d(tail/3,-wid, wid);
   glVertex3d(tail/3, wid, wid);
   glVertex3d(tail/3, wid,-wid);
   glVertex3d(tail/3,-wid,-wid);
   glEnd();
   // second fuselage

   // third fuselage
   glColor3f(0.5,0.5,0.5);
   glBegin(GL_POLYGON);
   double wid1 = wid + 0.02;
   glVertex3d(tail/3, wid1, wid1);
   glVertex3d(tail/3,-wid1, wid1);
   glVertex3d(tail+.1,-wid1, wid1);
   glVertex3d(tail+.1, wid1, wid1);

   glVertex3d(tail/3, wid1,-wid1);
   glVertex3d(tail/3,-wid1,-wid1);
   glVertex3d(tail+.1,-wid1,-wid1);
   glVertex3d(tail+.1, wid1,-wid1);

   glVertex3d(tail/3, wid1, wid1);
   glVertex3d(tail/3, wid1,-wid1);
   glVertex3d(tail+.1, wid1,-wid1);
   glVertex3d(tail+.1, wid1, wid1);

   glVertex3d(tail/3,-wid1, wid1);
   glVertex3d(tail/3,-wid1,-wid1);
   glVertex3d(tail+.1,-wid1,-wid1);
   glVertex3d(tail+.1,-wid1, wid1);

   glVertex3d(tail+.1,-wid1, wid1);
   glVertex3d(tail+.1, wid1, wid1);
   glVertex3d(tail+.1, wid1,-wid1);
   glVertex3d(tail+.1,-wid1,-wid1);
   glEnd();
   
   // Rocket tails
   glColor3f(1,0,0);
   glBegin(GL_POLYGON);
   // glColor3f(1,0,0);
   // glColor3f(1,0,0);
   glVertex3d(strk-.3, 0.0, 0.0);
   glVertex3d(tail-.1, 0.3, 0.0);
   glVertex3d(tail-.1, 0.0, 0.0);
   // glColor3f(0,0,1);
   glVertex3d(strk-.3, 0.0, 0.0);
   glVertex3d(tail-.1, 0.0, 0.0);
   glVertex3d(tail-.1, 0.0, 0.3);
   // glColor3f(1,0,0);
   glVertex3d(strk-.3, 0.0, 0.0);
   glVertex3d(tail-.1, -0.3, 0.0);
   glVertex3d(tail-.1, 0.0, 0.0);
   // glColor3f(1,0,0);
   glVertex3d(strk-.3, 0.0, 0.0);
   glVertex3d(tail-.1, 0.0, 0.0);
   glVertex3d(tail-.1, 0.0, -0.3);
   glEnd();
   // fire
   glColor3f(1,0.6,0);
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(tail-.1,-0.15,0.0);
   glVertex3d(tail-.1,0.15,0.0);
   glVertex3d(tail-.3,0.0,-0.2);

   glVertex3d(tail-.1,-0.15,0.0);
   glVertex3d(tail-.1,0.15,0.0);
   glVertex3d(tail-.3,0.0,0.2);
   glEnd();
   
   //  Undo transformations
   glPopMatrix();
}

static void sphere1(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}
#define PI 3.1415927

/************************** draw_cylinder() **************************
 * This function will draw the cylinder
 *
 *   @parameter1: radius = The radius of cylinder
 *   @parameter2: height = Height of the cylinder
 *   @parameter3: R = Red value of the cylinder's color
 *   @parameter4: G = Green value of the cylinder's color
 *   @parameter5: B = Blue value of the cylinder's color
 *
 *   @return: Nothing
 */
 // modified from here: https://gist.github.com/nikAizuddin/5ea402e9073f1ef76ba6
void cylinder(double radius,double height, double sx,double sy, double sz)
{
    double x              = 0.0;
    double y              = 0.0;
    double angle          = 0.0;
    double angle_stepsize = 0.1;

   // tube
    glColor3f(0.2,0.5,0.5);
    glScaled(sx,sy,sz);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    // Top Circle 
    glColor3f(0.2,0.5,0.0);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
  
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else if (mode == 0)
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   else if (mode == 2) {
      float xtrans = -xpos;
      float ztrans = -zpos;
      float ytrans = -ypos;
      float sceneroty = 360.0f - yrot;

      glRotatef(ph,1,0,0);
      glRotatef(sceneroty,0,1,0);
      glTranslatef(xtrans,ytrans,ztrans); 
      // Draw_Grid();  
   }
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
  
   
   rocket(-1, 1, 0 ,-1,0,0 , 0,1,0,0.45,0.45,0.45);
   rocket(Cos(zh),Sin(zh), 0 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh),.75,.75,.75);
   sphere1(-0.5,0.2,-0.9 , .85);
   // glBegin()
   glTranslatef(0.2,-0.4,1.0);
   glRotatef(-90, 1.0, 0.0, 0.0);
   cylinder(0.3, 1.0,1.3,1.3,1.3);
   
   glTranslatef(-1.3,0.1,3.0);
   glRotatef(-50, 1.0, 0.0, 0.0);
   cylinder(0.4,0.35,0.5,0.4,0.5);   

   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT) {
      th += 5;
      yrot = th;
   }

   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT) {
      th -= 5;
      yrot = th;
   }
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP) {
      ph += 5;
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN) {
      ph -= 5;
   }
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP) {
      dim += 0.1;
   }
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1) {
      dim -= 0.1;
   }
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   else if (ch == 'w') {
      xpos -= Sin(th) * 0.5;
      zpos -= Cos(th) * 0.5;
   }
   else if (ch == 'a') {
      xpos -= Cos(th) * 0.5;
      zpos += Sin(th) * 0.5;
   }
   else if (ch == 's') {
      xpos += Sin(th) * 0.5;
      zpos += Cos(th) * 0.5;
   }
   else if (ch == 'd') {
      xpos += Cos(th) * 0.5;
      zpos -= Sin(th) * 0.5;
   }
   else if (ch == '0') {
      th = ph = 0;
      mode = 0;
   }
   else if (ch == '1') {
      th = ph = 0;
      mode = 1;
   }
   else if (ch == '2') {
      xpos = 0;
      zpos = 10;
      ypos = 1.5;
      th = ph = yrot = 0;
      mode = 2;
   }
//  Reset view angle
   else if (ch == '4')
   th = ph = 0;
    // Toggle axes
   else if (ch == 'b' || ch == 'B')
      axes = 1-axes;
      
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
    // Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,700);
   glutCreateWindow("Projections");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
