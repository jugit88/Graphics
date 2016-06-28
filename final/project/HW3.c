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
 *  m          Toggles texture mode modulate/replace
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
int gp;                      // G Pressed? ( New )
GLuint filter;                      // Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
GLuint fogfilter= 0;                    // Which Fog To Use
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f}; 
int fog = 1;
float x = 0;float y = 0;float xp = 0; float yp = 0;float xpos = 0;float ypos = 0;

float t_array[2950][3];
float n_array[2950][3];


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



static void Cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   // glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   //  Cube
   //  Front
   // transparent ? glColor3f(1,1,0): glColor4f(0,0,1)
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
   glDisable(GL_BLEND);
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
            glNormal3f(x,y,0);
            glTexCoord2f(x,y); glVertex3f(x, y , height);
            glTexCoord2f(x,y); glVertex3f(x, y, 0);
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
// draw parthenon
/*void parthenon() {
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
   pyramid(-0.3,-0.38,-0.9,0.4,0.5,.95,0);
   cube(0.0,-1.3,0.0,1.2,0.1,0.7,0);
   cube(0.0,-1.4,0.0,1.3,0.1,0.75,0);
   cube(0.0,-1.5,0.0,1.4,0.1,0.8,0);
}*/
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


   // bottom cross bars
   glPushMatrix();
   // glRotated(60,0,0,1);
   // glScaled(0.05,0.5,0.1);
   Cube(0.5,0,0.6,0.05,0.3,0.05,-60);
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
   bridge();
   
  




   
   
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
   double t = glutGet(GLUT_ELAPSED_TIME) * .001;
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
   cables(t_array);
   cables1(n_array);
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
