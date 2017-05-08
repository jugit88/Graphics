/*
 *  Final Project: Golden Gate Bridge
 *  Todo: Add detail to the bridge and possibly add water to the scene as well. 
 *  Add Textures and appropriate shaders
 *  Optimize performace of program(slight latency)
 *
 *  Key bindings:
 *  m/M        Cycle through shaders
 *  x/X        Increase/decrease mandelbrot X-value
 *  y/Y        Increase/decrease mandelbrot Y-value
 *  z/Z        Zoom in/out of mandelbrot
 *  o/O        Cycle through objects
 *  p/P        Toggle between orthogonal & perspective projections
 *  s/S        Start/stop light movement
 *  -/+        Decrease/increase light elevation
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include <math.h> 
int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int move=1;       //  Move light
int roll=1;       //  Rolling brick texture
int proj=1;       //  Projection type
int obj=1;        //  Object
int th=-65;         //  Azimuth of view angle
int ph=15;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
int zh=90;        //  Light azimuth
int suza=0;       //  Object
float Ylight=2;   //  Light elevation
float x = 0;float y = 0;float xp = 0; float yp = 0;float xpos = 0;float ypos = 0;
int alpha = 70;
int fog = 0;
int emission  =   0;  // Emission intensity (%)
float shinyvec[1];


#define MODE 8
int shader[MODE] = {0,0,0,0,0,0,0,0}; //  Shader programs
char* text[] = {"No Shader","Constant Color","Lighting","Brick","Mandelbrot Set","Toon Shader","Pixel Lighting","Textures"};
float X=0,Y=0,Z=1; //  Mandelbrot X,Y,Z
GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
   {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
   {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, 
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};


int gp;                      // G Pressed? ( New )
GLuint filter;                      // Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
GLuint fogfilter= 0;                    // Which Fog To Use
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f}; 

const int NUM = 100;

float t_array[2950][3];
float n_array[2950][3];

#define PI 3.1415926

void cables(float arr[][3]) {
   int i;
   for (i = 0; i < 2950;i++) {
      arr[i][0] = x;
      arr[i][1] = y;
      arr[i][2] = 0;
      x += 0.0005;
      y = pow(4.1*x/5,2);
   }
} 
void cables1(float arr[][3]) {
   int i;
   for (i = 0; i < 2950;i++) {
      arr[i][0] = xp;
      arr[i][1] = yp;
      arr[i][2] = 0;
      xp -= 0.0005;
      yp = pow(4.1*xp/5,2);
   }
} 

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */

static void Cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   // float white[] = {1,1,1,1};
   // float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   // glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   // glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   // glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   // //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   // // glColor3f(1,1,1);
   // glBindTexture(GL_TEXTURE_2D,texture[4]);
   
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   //  Cube
   //  Front
   glColor3f(1,.2,0);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,+1);
   glTexCoord2f(0,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,+1);
   glEnd();
   //  Back
   // glColor3f(0,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   // glColor3f(1,1,0);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   // glColor3f(0,1,0);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   // glColor3f(0,1,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   // glColor3f(1,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
   // glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
}
void Init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &t_array[0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
}
void cylinder(double radius,double height, double s,double xtrans,double ztrans) {
// {
//     float white[] = {1,1,1,1};
//     float Emission[]  = {0.0,0.0,0.01*emission,1.0};
//     glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
//     glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
//     glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    double x              = 0.0;
    double y              = 0.0;
    double angle          = 0.0;
    double angle_stepsize = 0.1;

   // tube
    glColor3f(1,1,0.8);
    glTranslated(xtrans,0,ztrans);
    glScaled(s,s,s);
    // glEnable(GL_TEXTURE_2D);
    // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    glColor3f(1,1,1);
    // glBindTexture(GL_TEXTURE_2D,texture[5]);
    glBegin(GL_QUAD_STRIP);
       angle = 0.0;
      while( angle < PI*2 ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(x,y,0);
            glTexCoord2f(x,y); glVertex3f(x, y , height);
            glTexCoord2f(x,y); glVertex3f(x, y , 0);
            angle = angle + angle_stepsize;
      }
           glTexCoord2f(0,1); glVertex3f(radius, 0, height);
           glTexCoord2f(1,0); glVertex3f(radius, 0.0, 0.0);
           
       
    glEnd(); 
    // Top Circle 
    glColor3f(1,1,0.7);
    glBegin(GL_POLYGON);
        angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(x,y,0);
            glTexCoord2f(x,y); glVertex3f(x, y, height);
            angle = angle + angle_stepsize;
        }
        glTexCoord2f(radius,0); glVertex3f(radius, 0, height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
 }
void bridge() {
   // poles 
   glPushMatrix();
   Cube(-1.5,0,0.02,0.05,1.5,0.05,0);
   Cube(-1.5,0,0.48,0.05,1.5,0.05,0);
   Cube(1.5,0,0.02,0.05,1.5,0.05,0);
   Cube(1.5,0,0.48,0.05,1.5,0.05,0);
   glPopMatrix();
   // support on poles
   // left
   glPushMatrix();
   Cube(-1.5,0.2,0.25,0.02,0.06,0.2,0);
   Cube(-1.5,0.6,0.25,0.02,0.06,0.2,0);
   Cube(-1.5,1.0,0.25,0.02,0.06,0.2,0);
   Cube(-1.5,1.41,0.25,0.02,0.06,0.2,0);
   glPopMatrix();
   // right
   glPushMatrix();
   Cube(1.5,0.2,0.25,0.02,0.06,0.2,0);
   Cube(1.5,0.6,0.25,0.02,0.06,0.2,0);
   Cube(1.5,1.0,0.25,0.02,0.06,0.2,0);
   Cube(1.5,1.41,0.25,0.02,0.06,0.2,0);
   glPopMatrix();

   // road 
   Cube(0,-0.2,0.25,3.5,0.03,0.2,0);
   // rail spokes
   double x1 = -3.5;
   for (int i = 0; i <350;i++) {
      glPushMatrix();
      x1 += .02;  
      glTranslated(x1,0,0);
      glColor3f(0,0,1);
      Cube(0,-0.15,0.44,.001,0.04,0.003,0);
      Cube(0,-0.15,0.06,.001,0.04,0.003,0);
      glPopMatrix();
   }
   // top rails
   Cube(0,-0.11,0.44,3.5,0.002,0.004,0);
   Cube(0,-0.11,0.06,3.5,0.002,0.004,0);
   // TODO:water
   glPushMatrix();
   // glColor3f(0,0.4,1);
   glTranslated(0,-1.5,0);
   glScaled(10,10,10);
   glBegin(GL_QUADS);
      float time = 0.001 * glutGet(GLUT_ELAPSED_TIME);
      // glPushMatrix();
      glColor3f(0,0.4,1);
      glNormal3f(0,+1,0);
      glTexCoord2f(time,0); glVertex3f(0,0,0);
      glTexCoord2f(time,1); glVertex3f(0,0,1);
      glTexCoord2f(time,1); glVertex3f(1,0,1);
      glTexCoord2f(time,0); glVertex3f(1,0,0);
      

   glEnd();
   glPopMatrix();
   





   // bottom cross bars
   glPushMatrix();
   // glRotated(60,0,0,1);
   // glScaled(0.05,0.5,0.1);
   // Cube(0.5,0,0.6,0.05,0.3,0.05,-60);
   glPopMatrix();

   int i;
   // suspension cables
   // right middle back
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      float x1 = t_array[i][0]; float y1 = t_array[i][1];
      float theta = (t_array[i][1]- t_array[i-1][1]) / (t_array[i][0]- t_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(t_array[i][0],t_array[i][1],0.02);
      if (i*70 < 2950) {
          // glBegin(GL_LINE_STRIP);            
            glTranslated(t_array[i*70][0],t_array[i*70][1],0);
            glRotated(90,1,0,0);
            cylinder(0.005,t_array[i*70][1]+0.2,1,0,0);

            // glVertex3f(t_array[i*70][0],-0.2,0);
          // glEnd();
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(t_array[i-1][0] - x1,2) + pow(t_array[i-1][1] - y1,2));
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0);
      glPopMatrix();
     
   }
   // left middle back
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      
      float x2 = n_array[i][0]; float y2 = n_array[i][1];
      float theta = (n_array[i][1]- n_array[i-1][1]) / (n_array[i][0]- n_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(n_array[i][0],n_array[i][1],0.02);
      if (i*70 < 2950) {
           glTranslated(n_array[i*70][0],n_array[i*70][1],0);
            glRotated(90,1,0,0);
            cylinder(0.005,n_array[i*70][1]+0.2,1,0,0);
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(n_array[i-1][0] - x2,2) + pow(n_array[i-1][1] - y2,2));
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0);
      glPopMatrix();
      

   }
   // right middle front 
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      float x1 = t_array[i][0]; float y1 = t_array[i][1];
      float theta = (t_array[i][1]- t_array[i-1][1]) / (t_array[i][0]- t_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(t_array[i][0],t_array[i][1],0.48);
      if (i*70 < 2950) {
           glTranslated(t_array[i*70][0],t_array[i*70][1],0);
           glRotated(90,1,0,0);
           cylinder(0.005,t_array[i*70][1]+0.2,1,0,0);
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(t_array[i-1][0] - x1,2) + pow(t_array[i-1][1] - y1,2));
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0);

      glPopMatrix();
     
   }
   // left middle front
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      
      float x2 = n_array[i][0]; float y2 = n_array[i][1];
      float theta = (n_array[i][1]- n_array[i-1][1]) / (n_array[i][0]- n_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(n_array[i][0],n_array[i][1],0.48);
      if (i*70 < 2950) {
           glTranslated(n_array[i*70][0],n_array[i*70][1],0);
           glRotated(90,1,0,0);
           cylinder(0.005,n_array[i*70][1]+0.2,1,0,0);
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(n_array[i-1][0] - x2,2) + pow(n_array[i-1][1] - y2,2));
      
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0);
     
      
      glPopMatrix();
   }
   // right front 
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      glTranslated(3,0,0);
      float x2 = n_array[i][0]; float y2 = n_array[i][1];
      float theta = (n_array[i][1]- n_array[i-1][1]) / (n_array[i][0]- n_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(n_array[i][0],n_array[i][1],0.48);
      if (i*70 < 2950) {
          glTranslated(n_array[i*70][0],n_array[i*70][1],0);
          glRotated(90,1,0,0);
          cylinder(0.005,n_array[i*70][1]+0.2,1,0,0);
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(n_array[i-1][0] - x2,2) + pow(n_array[i-1][1] - y2,2));
      
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0); 
      glPopMatrix();
   }
    // right back 
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      glTranslated(3,0,0);
      float x2 = n_array[i][0]; float y2 = n_array[i][1];
      float theta = (n_array[i][1]- n_array[i-1][1]) / (n_array[i][0]- n_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(n_array[i][0],n_array[i][1],0.02);
      if (i*70 < 2950) {
            glTranslated(n_array[i*70][0],n_array[i*70][1],0);
            glRotated(90,1,0,0);
            cylinder(0.005,n_array[i*70][1]+0.2,1,0,0);
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(n_array[i-1][0] - x2,2) + pow(n_array[i-1][1] - y2,2));
      
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0); 
      glPopMatrix();
   }
   // left front 
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      glTranslated(-3,0,0);
      float x1 = t_array[i][0]; float y1 = t_array[i][1];
      float theta = (t_array[i][1]- t_array[i-1][1]) / (t_array[i][0]- t_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(t_array[i][0],t_array[i][1],0.48);
      if (i*70 < 2950) {
            glTranslated(t_array[i*70][0],t_array[i*70][1],0);
            glRotated(90,1,0,0);
            cylinder(0.005,t_array[i*70][1]+0.2,1,0,0);
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(t_array[i-1][0] - x1,2) + pow(t_array[i-1][1] - y1,2));
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0);

      glPopMatrix();
     
   }
   // left back 
   for (i = 1; i < 2950;i++) {
      glPushMatrix();
      glTranslated(-3,0,0);
      float x1 = t_array[i][0]; float y1 = t_array[i][1];
      float theta = (t_array[i][1]- t_array[i-1][1]) / (t_array[i][0]- t_array[i-1][0]);
      float a = atan(theta);
      float result = a * 180.0/PI;
      glTranslated(t_array[i][0],t_array[i][1],0.02);
      if (i*70 < 2950) {
            glTranslated(t_array[i*70][0],t_array[i*70][1],0);
            glRotated(90,1,0,0);
            cylinder(0.005,t_array[i*70][1]+0.2,1,0,0);
         
      }
      glRotated(result,0,0,1);
      glRotated(-90,0,1,0);
      float hyp = sqrt(pow(t_array[i-1][0] - x1,2) + pow(t_array[i-1][1] - y1,2));
      glScaled(0.1,0.1,hyp);
      cylinder(0.3,1,1,0,0);

      glPopMatrix();
     
   }



}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Light position and colors
   float Emission[]  = {0.0,0.0,0.0,1.0};
   float Ambient[]   = {0.3,0.3,0.3,1.0};
   float Diffuse[]   = {1.0,1.0,1.0,1.0};
   float Specular[]  = {1.0,1.0,1.0,1.0};
   float Position[]  = {2*Cos(zh),Ylight,2*Sin(zh),1.0};
   float Shinyness[] = {16};

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
    // Perspective - set eye position
   // fog
   if (fog) {
      glClearColor(0.5f,0.5f,0.5f,1.0f);          // We'll Clear To The Color Of The Fog ( Modified )
      glFogi(GL_FOG_MODE, fogMode[fogfilter]);        // Fog Mode
      glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
      glFogf(GL_FOG_DENSITY, 0.4f);              // How Dense Will The Fog Be
      glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
      glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
      glFogf(GL_FOG_END, 5.0f);               // Fog End Depth
      glEnable(GL_FOG); 
   }   
   if (proj)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Draw light position as sphere (still no lighting here)
   glColor3f(1,1,1);
   glPushMatrix();
   glTranslated(Position[0],Position[1],Position[2]);
   glutSolidSphere(0.03,10,10);
   glPopMatrix();
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
   //  Set materials
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   
   //
   //  Draw scene
   //
   //  Select shader (0 => no shader)
   glUseProgram(shader[mode]);
   //  For brick shader set "uniform" variables
   if (mode>0)
   {
      int id;
      float time = roll ? 0.001*glutGet(GLUT_ELAPSED_TIME) : 0;
      id = glGetUniformLocation(shader[mode],"Xcenter");
      if (id>=0) glUniform1f(id,X);
      id = glGetUniformLocation(shader[mode],"Ycenter");
      if (id>=0) glUniform1f(id,Y);
      id = glGetUniformLocation(shader[mode],"Zoom");
      if (id>=0) glUniform1f(id,Z);
      id = glGetUniformLocation(shader[mode],"time");
      if (id>=0) glUniform1f(id,time);
   }
   //  Draw the teapot or cube
   glColor3f(0,1,1);
   glEnable(GL_TEXTURE_2D);
 
   bridge();
   glDisable(GL_TEXTURE_2D);
   //  No shader for what follows
   glUseProgram(0);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
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
   Print("Angle=%d,%d  Dim=%.1f Projection=%s Mode=%s",
     th,ph,dim,proj?"Perpective":"Orthogonal",text[mode]);
   if (mode==4)
   {
      glWindowPos2i(5,25);
      Print("X=%f Y=%f Z=%f Mag=%f",X,Y,Z,1/Z);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
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
   Project(proj?fov:0,asp,dim);
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
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
      proj = 1-proj;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      move = 1-move;
   //  Toggle brick movement
   else if (ch == 'b' || ch == 'B')
      roll = 1-roll;
   //  Toggle objects
   else if (ch == 'o' || ch == 'O')
      obj = (obj+1)%3;
   //  Cycle modes
   else if (ch == 'm')
      mode = (mode+1)%MODE;
   else if (ch == 'M')
      mode = (mode+MODE-1)%MODE;
   //  Light elevation
   else if (ch == '+')
      Ylight += 0.1;
   else if (ch == '-')
      Ylight -= 0.1;
   //  Mandelbrot
   else if (ch == 'x')
      X += 0.01*Z;
   else if (ch == 'X')
      X -= 0.01*Z;
   else if (ch == 'y')
      Y += 0.01*Z;
   else if (ch == 'Y')
      Y -= 0.01*Z;
   else if (ch == 'z')
      Z *= 0.9;
   else if (ch == 'Z')
      Z *= 1.1;
   else if (ch == 'r' || ch=='R')
   {
      X = Y = 0;
      Z = 1;
   }
   //  Reproject
   Project(proj?fov:0,asp,dim);
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
   Project(proj?fov:0,asp,dim);
}

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
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
   glutCreateWindow("Shaders");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   cables(t_array);
   cables1(n_array);
   // cables2(n_array);
   Init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load object
   // suza = LoadOBJ("suzanne.obj");
   //  Load texture
   // LoadTexBMP("pi.bmp");
   //  Create Shader Programs
   // shader[1] = CreateShaderProg("simple.vert","simple.frag");
   // shader[2] = CreateShaderProg("phong.vert","phong.frag");
   // shader[3] = CreateShaderProg("model.vert","brick.frag");
   // shader[4] = CreateShaderProg("model.vert","mandel.frag");
   // shader[5] = CreateShaderProg("toon.vert","toon.frag");
   // shader[6] = CreateShaderProg("pixlight.vert","pixlight.frag");
   // shader[7] = CreateShaderProg("texture.vert","texture.frag");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
