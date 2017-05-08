/*
 *  Textures and Lighting-Homework 3
 *
 *  Demonstrates using lighting and textures. The 3D object in this scene is a primitive replica of
 *  the parthenon in Athens, Greece. Today the building has eroded significantly so this drawing tries to
 *  emulate the structure's appearance upon completion.  
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=0;       //  Display axes
int th=-45;         //  Azimuth of view angle
int ph=15;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[7]; // Texture names
#define PI 3.1415927


/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,int tex_num)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[tex_num]);
   //  Front
  
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-.5, 1.6);
   glTexCoord2f(1,0); glVertex3f(+1,-.5, 1.6);
   glTexCoord2f(1,1); glVertex3f(+1,+.5, 1.6);
   glTexCoord2f(0,1); glVertex3f(-1,+.5, 1.6);
   glEnd();
   //  Back
  
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-.5,-1.6);
   glTexCoord2f(1,0); glVertex3f(-1,-.5,-1.6);
   glTexCoord2f(1,1); glVertex3f(-1,+.5,-1.6);
   glTexCoord2f(0,1); glVertex3f(+1,+.5,-1.6);
   glEnd();
   //  Right
  
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-.5,+1.6);
   glTexCoord2f(1,0); glVertex3f(+1,-.5,-1.6);
   glTexCoord2f(1,1); glVertex3f(+1,+.5,-1.6);
   glTexCoord2f(0,1); glVertex3f(+1,+.5,+1.6);
   glEnd();
   //  Left
  
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-.5,-1.6);
   glTexCoord2f(1,0); glVertex3f(-1,-.5,+1.6);
   glTexCoord2f(1,1); glVertex3f(-1,+.5,+1.6);
   glTexCoord2f(0,1); glVertex3f(-1,+.5,-1.6);
   glEnd();
   //  Top
  
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+.5,+1.6);
   glTexCoord2f(1,0); glVertex3f(+1,+.5,+1.6);
   glTexCoord2f(1,1); glVertex3f(+1,+.5,-1.6);
   glTexCoord2f(0,1); glVertex3f(-1,+.5,-1.6);
   glEnd();
   //  Bottom
  
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-.5,-1.6);
   glTexCoord2f(1,0); glVertex3f(+1,-.5,-1.6);
   glTexCoord2f(1,1); glVertex3f(+1,-.5,+1.6);
   glTexCoord2f(0,1); glVertex3f(-1,-.5,+1.6);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
void pyramid(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
  
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
    //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      
      glNormal3f(0,1.25,1.5);
      glTexCoord2f(0,0); glVertex3f( 0.0f, .25f, 0.0f);
      glTexCoord2f(1,0); glVertex3f(-1.0f, -.25f, 1.0f);
      glTexCoord2f(0.5,1); glVertex3f(1.0f, -.25f, 1.0f);
 
      // Right
      glNormal3f(1,2,0);
      glTexCoord2f(0,0); glVertex3f(0.0f, .25f, 0.0f);
      glTexCoord2f(1,0); glVertex3f(1.0f, -0.25f, 1.0f);
      glTexCoord2f(0.5,1); glVertex3f(1.0f, -.25f, -1.0f);
 
      // Back
      glNormal3f(0,1.75,-1.5);
      glTexCoord2f(0,0); glVertex3f(0.0f, .25f, 0.0f);
      glTexCoord2f(1,0); glVertex3f(1.0f, -.25f, -1.0f);
      glTexCoord2f(0.5,1); glVertex3f(-1.0f, -.25f, -1.0f);
 
      // Left
      glNormal3f(-1,2.25,0);
      glTexCoord2f(0,0); glVertex3f( 0.0f, .25f, 0.0f);
      glTexCoord2f(1,0); glVertex3f(-1.0f,-.25f,-1.0f);
      glTexCoord2f(0.5,1); glVertex3f(-1.0f,-.25f, 1.0f);
   glEnd();   // Done drawing the pyramid
   glDisable(GL_TEXTURE_2D);
 
}
/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}
// draw cylinders
void cylinder(double radius,double height, double s,double xtrans,double ztrans)
{
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    double x              = 0.0;
    double y              = 0.0;
    double angle          = 0.0;
    double angle_stepsize = 0.1;

   // tube
    glColor3f(1,1,0.8);
    glTranslated(xtrans,0,ztrans);
    glScaled(s,s,s);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,texture[5]);
    glBegin(GL_QUAD_STRIP);
       angle = 0.0;
       while( angle < PI*2 ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(x,0,y);
            glTexCoord2f(x,height); glVertex3f(x, height , y);
            glTexCoord2f(x,0); glVertex3f(x, 0.0 , y);
            angle = angle + angle_stepsize;
        }
        glTexCoord2f(0,1); glVertex3f(radius, height, 0.0);
        glTexCoord2f(1,0); glVertex3f(radius, 0.0, 0.0);
    glEnd(); 
    // Top Circle 
    glColor3f(1,1,0.7);
    glBegin(GL_POLYGON);
        angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(x,0,y);
            glTexCoord2f(x,height); glVertex3f(x, height, y);
            angle = angle + angle_stepsize;
        }
        glTexCoord2f(radius,height); glVertex3f(radius, height, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    
}
// draw parthenon
void parthenon() {
   // glRotated(90,0,0,0);
   // front pillars
   for (int i = 0;i < 4;i++) {
      cylinder(0.05,0.495,1,0.2,0);
   }
   // side pillars
   for (int i = 0; i < 8;i++) {
      cylinder(0.05,0.495,1,0,0.2);
   }
   glTranslated(-0.6,0,-1.6);
   
   for (int i = 0; i < 8;i++) {
      cylinder(0.05,0.495,1,0,0.2);
   }
   glTranslated(-0.2,0,0.2);
   // back pillars
   for (int i = 0; i < 4;i++) {
      cylinder(0.05,0.495,1,0.2,0);
   }
   glTranslated(0,1.0,0);
   pyramid(-0.3,-0.33,-0.9,0.4,0.5,.95,0);
   cube(0.0,-0.305,0.0,0.97,0.098,0.625,0,5);
   cube(0.0,-1.3,0.0,1.2,0.1,0.7,0,4);
   cube(0.0,-1.4,0.0,1.3,0.1,0.75,0,4);
   cube(0.0,-1.5,0.0,1.4,0.1,0.8,0,4);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{

   const double len=2.0;
   if (axes) {
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
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
   //  Length of axes
   //  Eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {4*Cos(zh),ylight,4*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
   
   //  Draw scene
   parthenon();

   
  




   
   
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
  
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
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
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle texture mode
   // else if (ch == 'm' || ch == 'M')
   //    mode = 1-mode;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
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
   Project(45,asp,dim);
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
   glutInitWindowSize(600,600);
   glutCreateWindow("Parthenon");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   // texture[2] = LoadTexBMP("limestone2.bmp");
   // texture[3] = LoadTexBMP("carrara-marble-tiles.bmp");
   texture[4] = LoadTexBMP("limestone1.bmp");
   texture[5] = LoadTexBMP("00.bmp");
   // texture[6] = LoadTexBMP("04.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
