
#include "display.h"

#if GRAPHICS
#include "ScreenRecording.h"

#include "objects.h"

// J Switch for os type

#ifdef __APPLE__

#include <GLUT/glut.h>         // The GL Utility Toolkit (GLUT) Header

#include <OpenGL/glu.h>

#include <OpenGL/gl.h>

#include <GL/glui.h>

#elif __linux__

#include <GL/glut.h>

#include <GL/glu.h>

#include <GL/gl.h>

#include <glui.h>

#endif

#include <math.h>

GLUI_Button* startRecordButton;
GLUI_Button* endRecordButton;
GLUI_StaticText* statText;
GLUI_EditText *counter_edittext;

bool    g_key[255];             // Lookup Table For Key's State (NEW)

GLUI *glui, *glui2;

// Global Variables
int WinWidth = 1300;
int WinHeight = 900; ///was 800
int TIME;
int PAUSE=0;
int   main_window;

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

//live vars

int   show_sphere=1;
int   show_torus=1;
int   show_axes = 1;
int   show_text = 1;
float view_rotate[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
float section_rotate[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
float object_rotate[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
float lights_rotate[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

/********** Miscellaneous global variables **********/

GLfloat light0_ambient[] =  {0.8f, 0.8f, 0.8f, 0.0f};
GLfloat light0_diffuse[] =  {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light0_position[] = {30.5f, 30.5f, 67.0f, 0.0f};

//GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};
//GLfloat light0_ambient[] =  {0.25f, 0.25f, 0.25f, 1.0f};
//GLfloat light0_diffuse[] =  {.75f, .75f, .75f, 1.0f};
GLfloat lights_rotation[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

float scale = 1.0;
int sectX =0;
int sectX2  =xMAX;
int sectY =0;
int sectY2 =yMAX;
int sectZ = 0;
int sectZ2 =zMAX;

/*float camposx= 0.0f;
float camposy= -1000;
float camposz= - 250*/;
float camposx = -20.0f;
float camposy = 450.0f;////sectY2/2.0f-10;
float camposz = -100.0f*3;
float camPitch=1;
float camHeading=1;

float obj_pos[3] = {camposx, camposy, camposz};//{0.0,0.0,0.0};

float macro1_pos[3];
float macro2_pos[3];
float macro3_pos[3] = {xMAX-5, 65.0f, 0.0f};
float macro4_pos[3] = {0, 60.0f, 0.0f};
//----------------------------
//viewing variables
int pickAgent=0;
int pickedAgent;
int movie=0;
int startMovie=0;
int makeMovie=0;
int endMovie=0;
int labelledView = 0;
int gridView=0;
int meshView=1;
int bloodView=1;
int cytoView=0;
int junctionView=1;
int membrane=1;
int springAgentsView =1;
int rightView=0;
int nodeAgentView=1;
int linesView=0;
int astro =0;
int VEGFview=1;
int viewType=2;
int FAview=0;
int horizontalView = 0;
int cell0view=0;
int baseView = 0;
int stalkView = 0;
int tipView = 0;
int noneView = 0;
int surfaceTriangles=1;
int surfaceAgentsView = 1;
int pickedCell=ECELLS;
int meshSurfaceBlack=0;
int viewLight=0;
int vesselView=0;
int mutantView =0;
int viewInsideEnv=0;
int junctionOnlyView=0;
int viewBM = 0;

bool   gp;					// G Pressed? ( New )
//GLuint filter;						// Which Filter To Use
//GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
//GLuint fogfilter= 0;					// Which Fog To Use
GLfloat fogColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};		// Fog Color

using namespace std;


//-----------------------------------------------------------
void myGlutIdle( void ) {
    /* According to the GLUT specification, the current window is
     * undefined during an idle callback.  So we need to explicitly change
     * it if necessary */
    if ( glutGetWindow() != main_window )
        glutSetWindow(main_window);
    
    glutPostRedisplay();
}
//-----------------------------------------------------------
//-----------------------------------------------------------
void init(void) {
    //glClearColor(0.0, 0.0, 0.0, 0.0) ;
    //glClearColor(0.2, 0.2, 0.2, 0.0) ;
    glClearColor(0.2, 0.2, 0.2, 0.0) ;
          //glClearColor(1
        //glClearColor(.0f,0.5f,0.5f,1.0f);			// We'll Clear To The Color Of The Fog ( Modified )

glFogi(GL_FOG_MODE, GL_EXP);		// Fog Mode
glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
glFogf(GL_FOG_DENSITY, 0.007f);				// How Dense Will The Fog Be
glHint(GL_FOG_HINT,  GL_NICEST);			// Fog Hint Value
glFogf(GL_FOG_START, 10.0f);				// Fog Start Depth
glFogf(GL_FOG_END, 0.2f);				// Fog End Depth
//glEnable(GL_FOG);					// Enables GL_FOG

}
//--------------------------------------------------------------------------------------
//void World::store_normals(void){
    
//    Coordinates cross;
    
//    //top face z+L for all do minus sign of this for z face
    
//    cross.x = 0.0;
//    cross.y = 0.0;
//    cross.z = 1.0;
//    store_cube_normals.push_back(cross);
    
//    cross.x = 1.0;
//    cross.y = 0.0;
//    cross.z = 0.0;
//    //left face x+L for all
    
//    store_cube_normals.push_back(cross);
    
//    cross.x = 0.0;
//    cross.y = 1.0;
//    cross.z = 0.0;
//    //back face y+L for all
    
//    store_cube_normals.push_back(cross);
//}
//--------------------------------------------------------------------------------------
Coordinates* World::calcNormal(Coordinates one,Coordinates two, Coordinates three){

    Coordinates* A = new Coordinates();
    Coordinates* B= new Coordinates();
    Coordinates* cross;
    
    A->x = two.x-one.x;
    A->y = two.y-one.y;
    A->z = two.z-one.z;
    
    B->x= three.x-one.x;
    B->y = three.y-one.y;
    B->z = three.z-one.z;
    
    
    cross = crossProduct(A,B);
    
    
    delete A;
    delete B;
    return(cross);

}
//-----------------------------------------------------------
void DrawCube(float x, float y, float z, float red, float green, float blue, float translu, float L)						// Here's Where We Do All The Drawing
{
    
    Coordinates cross;
    GLfloat X;
    GLfloat Y;
    GLfloat Z;
    
    //if(translu<1){}
    //glColor4f(red, green, blue, translu);
    //else{
    
    
    GLfloat array[] = {red, green, blue, translu};
    
 
    //glMaterialfv(GL_FRONT, GL_DIFFUSE,array);
    //}
    
     glColor4fv(array);
     
    glBegin(GL_QUADS);
    
     X =WORLDpointer->store_cube_normals[2].x;
    Y = WORLDpointer->store_cube_normals[2].y;
    Z = WORLDpointer->store_cube_normals[2].z;
    
    
    glNormal3f(X,Y,Z);
    
    //glColor3f(1 ,0.5f,0 );			// Set The Color To Orange
    glVertex3f( x , y+L , z );			// Top Right Of The Quad (Bottom)
    glVertex3f(x+L , y+L , z );			// Top Left Of The Quad (Bottom)
    glVertex3f(x+L , y+L , z+L );			// Bottom Left Of The Quad (Bottom)
    glVertex3f( x , y+L , z+L );			// Bottom Right Of The Quad (Bottom)
     glEnd();
     
     glColor4fv(array);
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
    
     glBegin(GL_QUADS);					// Start Drawing The Cube
    
    X = -WORLDpointer->store_cube_normals[2].x;
    Y = -WORLDpointer->store_cube_normals[2].y;
    Z = -WORLDpointer->store_cube_normals[2].z;
    
    glNormal3f(X,Y,Z);
    //glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
    glVertex3f( x , y , z+L );			// Top Right Of The Quad (Top)
    glVertex3f(x+L , y , z+L );			// Top Left Of The Quad (Top)
    glVertex3f(x+L , y , z );			// Bottom Left Of The Quad (Top)
    glVertex3f( x , y , z );			// Bottom Right Of The Quad (Top)
     glEnd();
     
     glColor4fv(array);
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
    
    glBegin(GL_QUADS);
    
    X = -WORLDpointer->store_cube_normals[0].x;
    Y = -WORLDpointer->store_cube_normals[0].y;
    Z = -WORLDpointer->store_cube_normals[0].z;
    
    glNormal3f(X,Y,Z);
    
    //glColor3f(1 ,0 ,0 );			// Set The Color To Red
    glVertex3f( x , y , z );			// Top Right Of The Quad (Front)
    glVertex3f(x+L , y , z );			// Top Left Of The Quad (Front)
    glVertex3f(x+L , y+L , z );			// Bottom Left Of The Quad (Front)
    glVertex3f( x , y+L , z );			// Bottom Right Of The Quad (Front)
     glEnd();
     
     glColor4fv(array);
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
    
     glBegin(GL_QUADS);
     X = WORLDpointer->store_cube_normals[0].x;
    Y = WORLDpointer->store_cube_normals[0].y;
    Z = WORLDpointer->store_cube_normals[0].z;
    
    glNormal3f(X,Y,Z);
    
    //glColor3f(1 ,1 ,0 );			// Set The Color To Yellow
    glVertex3f( x , y+L , z+L );			// Bottom Left Of The Quad (Back)
    glVertex3f(x+L , y+L , z+L );			// Bottom Right Of The Quad (Back)
    glVertex3f(x+L , y , z+L );			// Top Right Of The Quad (Back)
    glVertex3f( x , y , z+L );			// Top Left Of The Quad (Back)
     glEnd();
     
     glColor4fv(array);
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
    
     glBegin(GL_QUADS);
     X = WORLDpointer->store_cube_normals[1].x;
    Y = WORLDpointer->store_cube_normals[1].y;
    Z = WORLDpointer->store_cube_normals[1].z;
    
    glNormal3f(X,Y,Z);
   

    glVertex3f(x+L , y , z );			// Top Right Of The Quad (Left)
    glVertex3f(x+L , y , z+L );			// Top Left Of The Quad (Left)
    glVertex3f(x+L , y+L , z+L );			// Bottom Left Of The Quad (Left)
    glVertex3f(x+L , y+L , z );			// Bottom Right Of The Quad (Left)
     glEnd();
     
     glColor4fv(array);
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
    
     glBegin(GL_QUADS);
     X = -WORLDpointer->store_cube_normals[1].x;
    Y = -WORLDpointer->store_cube_normals[1].y;
    Z = -WORLDpointer->store_cube_normals[1].z;
    glNormal3f(X,Y,Z);
    
    glVertex3f( x , y , z+L );			// Top Right Of The Quad (Right)
    glVertex3f( x , y , z );			// Top Left Of The Quad (Right)
    glVertex3f( x , y+L , z );			// Bottom Left Of The Quad (Right)
    glVertex3f( x , y+L , z+L );			// Bottom Right Of The Quad (Right)
    
    
    glEnd();						// Done Drawing The Quad
    
}
//---------------------------------------------------------------------------------------------------------------------------------------
void World::viewGridLines(void){
    
    int i, k, j;
    glLineWidth(1.0f);
    if(sectX<0) sectX=0;
    if(sectY<0) sectY=0;
    if(sectZ<0) sectZ=0;
    
    if(sectX2>xMAX) sectX2=xMAX;
    if(sectY2>yMAX) sectY2=yMAX;
    if(sectZ2>zMAX) sectZ2=zMAX;
    
    int recentreX = (int)(sectX+(sectX2-sectX)/2.00);
    
    int recentreY = (int)(sectY+(sectY2-sectY)/2.00);
    
    int recentreZ = (int)(sectZ+(sectZ2-sectZ)/2.00);
    
    
    float red, green, blue;
    
    //draw the grid lines-----------------------------------------------------------------
    
    for(k=sectZ;k<sectZ2;k++){
        for(i=sectX;i<=sectX2;i++){
            
            red=0.9f; green = 0.9f; blue =0.9f;
            glColor4f(red, green, blue, 1.0);
            
            glBegin(GL_LINES);
            glVertex3f(i-recentreX, sectY-recentreY, k-recentreZ);
            glVertex3f(i-recentreX, sectY2-recentreY, k-recentreZ);
            glEnd();
        }
        for(j=sectY;j<=sectY2;j++){
            
            red=0.9f; green = 0.9f; blue =0.9f;

            glColor4f(red, green, blue, 1.0);
            
            glBegin(GL_LINES);
            glVertex3f(sectX-recentreX, j-recentreY, k-recentreZ);
            glVertex3f(sectX2-recentreX, j-recentreY, k-recentreZ);
            glEnd();
        }
    }
    
    for(k=sectX;k<sectX2;k++){
        for(i=sectZ;i<=sectZ2;i++){
            
            red=0.9f; green = 0.9f; blue =0.9f;

            glColor4f(red, green, blue, 1.0);
            
            glBegin(GL_LINES);
            glVertex3f(k-recentreX, sectY-recentreY, i-recentreZ);
            glVertex3f(k-recentreX, sectY2-recentreY, i-recentreZ);
            glEnd();
        }
        for(j=sectY;j<=sectY2;j++){
            
            red=0.0f; green = 1.0f; blue =0.0f;
            glColor4f(red, green, blue, 1.0);
            
            glBegin(GL_LINES);
            glVertex3f(k-recentreX, j-recentreY, sectZ-recentreZ);
            glVertex3f(k-recentreX, j-recentreY, sectZ2-recentreZ);
            glEnd();
        }
    }
    
}
//-------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
void World::drawFilAgentsCylinders(float a,  float b,  float c,  int a2,  int b2,  int c2, float red, float green, float blue, float diam, int X){
    
    //a2, b2, c2 are the recentre values not the second point that the cylinder is drawn between!
    
    glColor4f(red, green, blue, 1);
    //glColor4f(1.0f, 1.0f, 1.0f, 1);
    
    GLfloat posx=(float)(a-a2);
    GLfloat posy=(float)(b-b2);
    GLfloat posz=(float)(c-c2);
    
    float minX=(grid[(int)a][(int)b][(int)c].Fids[X]->filNeigh->Mx) - (float)a2;
    float minY=(grid[(int)a][(int)b][(int)c].Fids[X]->filNeigh->My) - (float)b2;
    float minZ=(grid[(int)a][(int)b][(int)c].Fids[X]->filNeigh->Mz) -(float) c2;
    
    GLfloat posx2=minX;
    GLfloat posy2=minY;
    GLfloat posz2=minZ;
    
    GLfloat diameter=diam;
    
    GLdouble height=sqrt(pow((posx2-posx), 2)+pow((posy2-posy), 2)+pow((posz2-posz), 2));
    
    //if(((minX+a2)!=0)&&((minX+a2)!=xMAX)&&(a!=0)&&(a!=yMAX)){
    if(fabs(grid[(int)a][(int)b][(int)c].Fids[X]->filNeigh->Mx-a)<(float)xMAX/2.0f){
        glPushMatrix();
        
        glTranslatef(posx, posy, posz);
        glRotatef((180.0f/M_PI)*(GLdouble)2*asin(0.5*sqrt(pow((posx2-posx), 2)+pow((posy2-posy), 2)+pow(posz2-posz-height, 2))/height), -(posy2-posy)/sqrt(pow((posx2-posx), 2)+pow((posy2-posy), 2)), (posx2-posx)/sqrt(pow((posx2-posx), 2)+pow((posy2-posy), 2)), 0.0f);
        
        GLUquadric* quads = gluNewQuadric();
        
        gluSphere(quads, diameter, 6, 6);
        
        gluDeleteQuadric(quads);
        
        GLUquadric* quad = gluNewQuadric();
        
        gluCylinder((GLUquadric*)quad, (GLdouble)diameter, (GLdouble)diameter, (GLdouble)height, 12, 6);
        
        gluDeleteQuadric(quad);
        
        glPopMatrix();
        
    }
}

//---------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
void World::viewWorld(void){
    
    
    
    if(gridView==1){
        viewGrid();
    }
    if(meshView==1){
        //cout<<"trying.."<<endl;
        viewMesh();
        //cout<<"tried"<<endl;
    }
    if(linesView==1){
        viewGridLines();
    }
   

    //if(timeStep>TIME_DIFFAD_STARTS)
   //viewCOMpath();
    
    int recentreX = (int)(sectX+(sectX2-sectX)/2.00);
    
    int recentreY = (int)(sectY+(sectY2-sectY)/2.00);
    
    int recentreZ = (int)(sectZ+(sectZ2-sectZ)/2.00);
    
    int i;
   
    
    if(viewLight){
    DrawCube((float)light0_position[0]-recentreX , (float)light0_position[1]-recentreY,(float)light0_position[2]-recentreZ, 0.0, 1.0, 0.0, 1, 10);
    }
    
    if((MACROS>0)){
    //macro1_pos[0] = macrophages[0]->coords.x; macro1_pos[1] = macrophages[0]->coords.y; macro1_pos[2] = macrophages[0]->coords.z;
        DrawCube(macro1_pos[0]-recentreX, macro1_pos[1]-recentreY, macro1_pos[2]-recentreZ, 1.0, 1.0, 1.0, 1, 1);
        macrophages[0]->coords.x=macro1_pos[0];
        macrophages[0]->coords.y=macro1_pos[1];
        macrophages[0]->coords.z=macro1_pos[2];
    }
        
    
    
    if(MACROS>1){
        DrawCube(macro2_pos[0]-recentreX, macro2_pos[1]-recentreY, macro2_pos[2]-recentreZ, 1.0, 1.0, 1.0, 1, 1);
        macrophages[1]->coords.x=macro2_pos[0];
        macrophages[1]->coords.y=macro2_pos[1];
        macrophages[1]->coords.z=macro2_pos[2];
    }
    if(MACROS>2){
        DrawCube(macro3_pos[0]-recentreX, macro3_pos[1]-recentreY, macro3_pos[2]-recentreZ, 1.0, 1.0, 1.0, 1, 1);
        macrophages[2]->coords.x=macro3_pos[0];
        macrophages[2]->coords.y=macro3_pos[1];
        macrophages[2]->coords.z=macro3_pos[2];
    }
    if(MACROS>3){
        DrawCube(macro4_pos[0]-recentreX, macro4_pos[1]-recentreY, macro4_pos[2]-recentreZ, 1.0, 1.0, 1.0, 1, 1);
        macrophages[3]->coords.x=macro4_pos[0];
        macrophages[3]->coords.y=macro4_pos[1];
        macrophages[3]->coords.z=macro4_pos[2];
    }
    
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
void World::drawSprings(MemAgent* mp, float red, float green, float blue){
    
    int j;
    
    float PN[NEIGH][3];
    
    if(sectX<0) sectX=0;
    if(sectY<0) sectY=0;
    if(sectZ<0) sectZ=0;
    
    if(sectX2>xMAX) sectX2=xMAX;
    if(sectY2>yMAX) sectY2=yMAX;
    if(sectZ2>zMAX) sectZ2=zMAX;
    
    int recentreX = (int)(sectX+(sectX2-sectX)/2.00);
    
    int recentreY = (int)(sectY+(sectY2-sectY)/2.00);
    
    int recentreZ = (int)(sectZ+(sectZ2-sectZ)/2.00);
    
    float fog;
    
    for(j=0;j<mp->neighs;j++){
        //draw
        if(mp->neigh[j]!=NULL){
            
            
            
            if(viewType==8){
                
                
                    if(mp->Mz>10)
                        fog = (mp->Mz-10)/20.0f;
                    else
                        fog = 0.0f;
                                       
                
                
            }
            glColor4f(red, green, blue, 1.0);
            glLineWidth(3.0f);
            if((mp->FIL!=NONE)&&((mp->neigh[j]->FIL==STALK)||(mp->neigh[j]->FIL==TIP))) glLineWidth(2.0f);
            
            if(viewType==8){
                red= 0.1;green=0.1; blue = 0.1;
            }
            if((rightView==1)&&(mp->SpringNeigh[j]->right==true)) glColor4f(1.03, 1.8, 1.2, 1.0);
            if((horizontalView==1)&&(mp->SpringNeigh[j]->horizontal==true)) glColor4f(1.03, 1.8, 1.2, 1.0);
            if((junctionView==1)&&(mp->SpringNeigh[j]->Junction==true)){
                glColor4f(1.03, 1.8, 1.2, 1.0);
                
                    
                    
                    if(viewType==8){
                    red = 210.0f/255.0f; green =58.0f/255.0f; blue = 230.0f/255.0f;
                    }
            
                }
            if(viewType==8){
            glColor4f(red+fog, green+fog, blue+fog, 1.0);
            }  //if(meshSurfaceBlack==1)glLineWidth(3.0f);
                //else glLineWidth(2.0f);
            
                       //if((mp->deleteFlag==true)||(mp->neigh[j]->deleteFlag==true)) glColor4f(1.0, 1.0, 1.0, 1.0);
            PN[j][0] = mp->Mx - mp->neigh[j]->Mx;
            PN[j][1] = mp->My - mp->neigh[j]->My;
            PN[j][2] =mp-> Mz - mp->neigh[j]->Mz;
            
            //if(mp->SpringNeigh[j]==ECagents[1]->Springs[280]){ glColor4f(0.03,1.8, 1.2,1.0);
            if((junctionOnlyView==1)&&(mp->SpringNeigh[j]->Junction==true)){
            if(sqrt(PN[j][0]*PN[j][0])<xMAX/2.0f){
                glBegin(GL_LINES);
                glVertex3f(mp->Mx-recentreX, mp->My-recentreY, mp->Mz-recentreZ);
                glVertex3f(mp->neigh[j]->Mx-recentreX, mp->neigh[j]->My-recentreY, mp->neigh[j]->Mz-recentreZ);
                glEnd();
            }
            }
            else if(junctionOnlyView==0){
                if(sqrt(PN[j][0]*PN[j][0])<xMAX/2.0f){
                glBegin(GL_LINES);
                glVertex3f(mp->Mx-recentreX, mp->My-recentreY, mp->Mz-recentreZ);
                glVertex3f(mp->neigh[j]->Mx-recentreX, mp->neigh[j]->My-recentreY, mp->neigh[j]->Mz-recentreZ);
                glEnd();
            }
            }
            
        }
    }
    
    
}
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

void World:: viewMesh(void){


    //glEnable(GL_LIGHTING);
    Coordinates* cross;
    int upto;
    int i, j,k;
    MemAgent * mp;
    float red, green, blue;
    vector<Coordinates> coords;
    vector <MemAgent*> triangleNodes;
Coordinates one;
    Coordinates two;
    Coordinates three;
    if(sectX<0) sectX=0;
    if(sectY<0) sectY=0;
    if(sectZ<0) sectZ=0;

    if(sectX2>xMAX) sectX2=xMAX;
    if(sectY2>yMAX) sectY2=yMAX;
    if(sectZ2>zMAX) sectZ2=zMAX;

    int recentreX = (int)(sectX+(sectX2-sectX)/2.00);

    int recentreY = (int)(sectY+(sectY2-sectY)/2.00);

    int recentreZ = (int)(sectZ+(sectZ2-sectZ)/2.00);
    int d;
    bool up = true;
    bool down = false;
    //size of nodes
    float L=0.2;
    GLfloat X;
    GLfloat Y;
    GLfloat Z;
    bool dontDo;
    bool flag3er=false;
    

    for(j=0;j<ECagents.size();j++){

        upto =  ECagents[j]->nodeAgents.size();

        //if((pickedCell==ECELLS)||(j==pickedCell)){
        //if(((cell0view==1)&&(j==0))||(cell0view==0)){
            for(i=0;i<upto;i++){

                mp= ECagents[j]->nodeAgents[i];

             //   if((pickAgent==0)||((pickAgent==1)&&(i==pickedAgent))){
            /*if(surfaceTriangles){
            //cout<<ECagents[j]->trianglesUPDOWN.size()<<endl;
            for(k=0;k<(float)ECagents[j]->triangles.size()/3.0f;k++){

                one = ECagents[j]->triangles[k*3];
                two= ECagents[j]->triangles[k*3+1];
                three = ECagents[j]->triangles[k*3+2];


                if(viewType==1){ red=((float)ECagents[j]->VEGFRtot/(float)VEGFRnorm); green=0.2245098; blue = 0.545098;}
                else if(viewType==3){ red=(float)5.0f;green=0.2245098; blue = 0.545098;}
                else if(viewType==2){ red= 0.534f;green=0.0623f+ECagents[j]->Dll4tot/5000.0f; blue = 0.5923f;}
                else if(viewType==4){ red= 0.534f;green=0.0623f+ECagents[j]->Dll4tot/50.0f; blue = 0.5923f;}
                else if(viewType==5){ red=0.3; green=0.2245098; blue = 0.545098;}
                else if(viewType==6){ red=0.3; green=0.2245098; blue = 0.545098;}
                else if(viewType==7){ red= 0.534f;green=0.0623f; blue = 0.5923f;}
                else if(viewType==8){ red= 0.0f;green=0.0f; blue = 0.0f;}

                GLfloat array[] = {red, green, blue, 1.0};

                cross=WORLDpointer->calcNormal(two, one, three);

                if(ECagents[j]->trianglesUPDOWN[k]==1){
                    X=-cross.x;
                    Y=-cross.y;
                    Z=-cross.z;
                }
                else{
                    X=cross.x;
                    Y=cross.y;
                    Z=cross.z;
                }

                glNormal3f(X, Y, Z);

                if(((one.x>=sectX)&&(one.x<sectX2))&&((one.y>=sectY)&&(one.y<sectY2))&&((one.z>=sectZ)&&(one.z<sectZ2))){

                    if((fabs(one.x-two.x)<xMAX/2.0f)&&(fabs(one.x-three.x)<xMAX/2.0f)&&(fabs(two.x-three.x)<xMAX/2.0f)){
                        glBegin(GL_TRIANGLES);		// Drawing Using Triangles
                        glVertex3f(one.x-recentreX, one.y-recentreY, one.z-recentreZ);		// Top
                        glVertex3f(two.x-recentreX, two.y-recentreY, two.z-recentreZ);		// Bottom Left
                        glVertex3f(three.x-recentreX, three.y-recentreY, three.z-recentreZ);		// Bottom Right
                        glEnd();
                    }
                    glColor4fv(array);
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);

                }
            }
        }*/
                if(surfaceTriangles){
                    if(junctionOnlyView==0){
                    if(((mp->Mx>=sectX)&&(mp->Mx<sectX2))&&((mp->My>=sectY)&&(mp->My<sectY2))&&((mp->Mz>=sectZ)&&(mp->Mz<sectZ2))){

                        for(d=0;d<3;d++){
                            dontDo=false;
                            //only for hexagonal cells
                            //if((mp->labelled2==true)&&(d==1)) dontDo=true;
                            //if((mp->labelled==true)&&(d==0)) dontDo=true;

                            flag3er=false;
                            if(dontDo==false){
                            if(d==0) triangleNodes = mp->getTriangle(up);
                            else if(d==1) triangleNodes = mp->getTriangle(down);
                            else{
                                if(mp->mesh3SpringsOnly==true){
                                    triangleNodes = mp->three_erTriangleFix();
                                    flag3er=true;
                                }

                            }

                           if(triangleNodes.size()==3){

                                //red=0.4; green=0.2245098; blue = 0.545098;

                                if(viewType==1){ red=(float)mp->Cell->VEGFRtot/((float)VEGFRNORM); green=0.2245098; blue = 0.545098;
                                //if(red<0.5) red = 0.5;
                                }
                                else if(viewType==3){ red=(float)mp->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                                else if(viewType==2){ red= 0.534f;green=0.0623f+mp->Cell->Dll4tot/(float)MAX_dll4; blue = 0.5923f;}
                                else if(viewType==4){ red= 0.534f;green=0.0623f+mp->Dll4/50.0f; blue = 0.5923f;}
                                else if(viewType==5){ red=(mp->VEGFR/30.0f); green=0.2245098; blue = 0.545098;}
                                else if(viewType==6){ red=mp->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                                else if(viewType==7){ red= 0.534f;green=0.0623f+(float)mp->Cell->activeNotchtot/800.0f; blue = 0.5923f;}
                                else if(viewType==8){ red= 0.0f;green=0.0f; blue = 0.0f;}
                                else if(viewType==9){ red= 0.2f;green=0.8f; blue = mp->Force[0]+mp->Force[1]+mp->Force[2];}
                                else if(viewType==10){ red= mp->Cell->red;green=mp->Cell->green; blue = mp->Cell->blue;}

                                //if(flag3er==true){ red = 1.0; green = 1.0; blue = 0.8;}

                    if(mutantView==1){
                        if(mp->Cell->mutant==true){
                            green = 0.8;
                        }
                    }

                                 GLfloat array[] = {red, green, blue, 1.0};
                                 one.x = mp->Mx;
                                 one.y = mp->My;
                                 one.z = mp->Mz;

                                 two.x = triangleNodes[1]->Mx;
                                 two.y = triangleNodes[1]->My;
                                 two.z = triangleNodes[1]->Mz;

                                 three.x = triangleNodes[2]->Mx;
                                 three.y = triangleNodes[2]->My;
                                 three.z = triangleNodes[2]->Mz;


                                cross=WORLDpointer->calcNormal(two,one, three);


                                if((d==1)||(d==2)){
                                X=-cross->x;
                                Y=-cross->y;
                                Z=-cross->z;
                                }
                                else{
                                X=cross->x;
                                Y=cross->y;
                                Z=cross->z;
                                }

                                glNormal3f(X,Y,Z);
                                delete cross;
                                

                                glNormal3f(X,Y,Z);

                                if((fabs(mp->Mx-two.x)<xMAX/2.0f)&&(fabs(mp->Mx-three.x)<xMAX/2.0f)&&(fabs(two.x-three.x)<xMAX/2.0f)){
                                    glColor4fv(array);
                                    glBegin(GL_TRIANGLES);		// Drawing Using Triangles
                                    glVertex3f(mp->Mx-recentreX, mp->My-recentreY, mp->Mz-recentreZ);		// Top
                                    glVertex3f(two.x-recentreX, two.y-recentreY, two.z-recentreZ);		// Bottom Left
                                    glVertex3f(three.x-recentreX, three.y-recentreY, three.z-recentreZ);		// Bottom Right
                                    glEnd();

                                    glColor4fv(array);
                                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
                                }


                            }
                            triangleNodes.clear();
                        }
                        }
                    }
                    }
                }
                if(((mp->Mx>=sectX)&&(mp->Mx<sectX2))&&((mp->My>=sectY)&&(mp->My<sectY2))&&((mp->Mz>=sectZ)&&(mp->Mz<sectZ2))){

                    //if(j%2==1){
                    //red=1.0f; green = 0.0f; blue = 1.0f;

                    if(viewType==1){ red=(float)mp->Cell->VEGFRtot/((float)VEGFRNORM); green=0.2245098; blue = 0.545098;}//if(red<0.5) red = 0.5;}
                    else if(viewType==3){ red=(float)mp->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                    else if(viewType==2){ red= 0.534f;green=0.0623f+mp->Cell->Dll4tot/(float)MAX_dll4; blue = 0.5923f;}
                    else if(viewType==4){ red= 0.534f;green=0.0623f+mp->Dll4/50; blue = 0.5923f;}
                    else if(viewType==5){ red=(mp->VEGFR/30.0f); green=0.2245098; blue = 0.545098;}
                    else if(viewType==6){ red=mp->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                    else if(viewType==7){ red= 0.534f;green=0.0623f+(float)mp->Cell->activeNotchtot/800.0f; blue = 0.5923f;}
                    else if(viewType==8){ red= 0.0f;green=0.0f; blue = 0.0f;}
                    else if(viewType==9){ red= 0.2f;green=0.8f; blue = (mp->Force[0]+mp->Force[1]+mp->Force[2])*10;}
                    else if(viewType==10){ red= mp->Cell->red;green=mp->Cell->green; blue = mp->Cell->blue;}


                   
                    if(viewType==8){
                    if((mp->FIL==BASE)&&(baseView)){red = 0.8; green = 0.8; blue = 0.8;}
                    else if((mp->FIL==STALK)&&(stalkView)){red = 0.5; green = 0.5; blue = 0.5;}
                    else if((mp->FIL==TIP)&&(tipView)){red = 0.2; green = 0.2; blue = 0.2;}
                    else if((mp->FIL==NONE)&&(noneView)){ red= 0.0f;green=0.0f; blue = 0.0f;}

                    
                    }
                    else{
                    if((mp->FIL==BASE)&&(baseView))red = 0.0;
                    else if((mp->FIL==STALK)&&(stalkView))green = 0.6;
                    else if((mp->FIL==TIP)&&(tipView))green = 0.8;
                    else if((mp->FIL==NONE)&&(noneView)) blue= 0.0;
                    else if((mp->labelled==true)&&(labelledView)){red = 1.0f; green = 1.0f; blue = 1.0f;}
                    else if((mp->labelled2==true)&&(labelledView)){ red = 0.6; green = 0.6;}
                    }

                    if((mp->FIL!=STALK)&&(mp->FA==true)&&(FAview))green = 1.8;

                    if(mutantView==1){
                        if(mp->Cell->mutant==true){
                            green = 0.8;
                        }
                    }

                    if(junctionOnlyView==false)
                      //  cout<<"drawing cube "<<(mp->Mx)-recentreX<< " "<<(mp->My)-recentreY<<" "<<(mp->Mz)-recentreZ<<endl;
                    DrawCube((mp->Mx)-recentreX, (mp->My)-recentreY, (mp->Mz)-recentreZ, red, green, blue, 1, L);
                    

                    
                   // cout<<"drawing spring"<<endl;
                    drawSprings(mp, red, green, blue);


                }
            }
        }
    }
        
    



//-------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

void World:: viewMesh_diagramSpheres(void){
    
    
    //glEnable(GL_LIGHTING);
    Coordinates* cross;
    int upto;
    int i, j,k;
    MemAgent * mp;
    float red, green, blue;
    vector<Coordinates> coords;
     vector <MemAgent*> triangleNodes;
Coordinates one;
    Coordinates two;
    Coordinates three;
    if(sectX<0) sectX=0;
    if(sectY<0) sectY=0;
    if(sectZ<0) sectZ=0;
    
    if(sectX2>xMAX) sectX2=xMAX;
    if(sectY2>yMAX) sectY2=yMAX;
    if(sectZ2>zMAX) sectZ2=zMAX;
    
    int recentreX = (int)(sectX+(sectX2-sectX)/2.00);
    
    int recentreY = (int)(sectY+(sectY2-sectY)/2.00);
    
    int recentreZ = (int)(sectZ+(sectZ2-sectZ)/2.00);
    int d;
    bool up = true;
    bool down = false;
    //size of nodes
    float L=0.2;
    GLfloat X;
    GLfloat Y;
    GLfloat Z;
    
     float posx, posy, posz;
    
    for(j=0;j<ECagents.size();j++){
       
        upto =  ECagents[j]->nodeAgents.size();

        if((pickedCell==ECELLS)||(j==pickedCell)){
        if(((cell0view==1)&&(j==0))||(cell0view==0)){
            for(i=0;i<upto;i++){
                
                mp= ECagents[j]->nodeAgents[i];


                
            /*if(surfaceTriangles){
            //cout<<ECagents[j]->trianglesUPDOWN.size()<<endl;
            for(k=0;k<(float)ECagents[j]->triangles.size()/3.0f;k++){
                
                one = ECagents[j]->triangles[k*3];
                two= ECagents[j]->triangles[k*3+1];
                three = ECagents[j]->triangles[k*3+2];
                
                
                if(viewType==1){ red=((float)ECagents[j]->VEGFRtot/(float)VEGFRnorm); green=0.2245098; blue = 0.545098;}
                else if(viewType==3){ red=(float)5.0f;green=0.2245098; blue = 0.545098;}
                else if(viewType==2){ red= 0.534f;green=0.0623f+ECagents[j]->Dll4tot/5000.0f; blue = 0.5923f;}
                else if(viewType==4){ red= 0.534f;green=0.0623f+ECagents[j]->Dll4tot/50.0f; blue = 0.5923f;}
                else if(viewType==5){ red=0.3; green=0.2245098; blue = 0.545098;}
                else if(viewType==6){ red=0.3; green=0.2245098; blue = 0.545098;}
                else if(viewType==7){ red= 0.534f;green=0.0623f; blue = 0.5923f;}
                else if(viewType==8){ red= 0.0f;green=0.0f; blue = 0.0f;}
                
                GLfloat array[] = {red, green, blue, 1.0};
                
                cross=WORLDpointer->calcNormal(two, one, three);
                
                if(ECagents[j]->trianglesUPDOWN[k]==1){
                    X=-cross.x;
                    Y=-cross.y;
                    Z=-cross.z;
                }
                else{
                    X=cross.x;
                    Y=cross.y;
                    Z=cross.z;
                }
                
                glNormal3f(X, Y, Z);
                delete cross;
                if(((one.x>=sectX)&&(one.x<sectX2))&&((one.y>=sectY)&&(one.y<sectY2))&&((one.z>=sectZ)&&(one.z<sectZ2))){
                    
                    if((fabs(one.x-two.x)<xMAX/2.0f)&&(fabs(one.x-three.x)<xMAX/2.0f)&&(fabs(two.x-three.x)<xMAX/2.0f)){
                        glBegin(GL_TRIANGLES);		// Drawing Using Triangles
                        glVertex3f(one.x-recentreX, one.y-recentreY, one.z-recentreZ);		// Top
                        glVertex3f(two.x-recentreX, two.y-recentreY, two.z-recentreZ);		// Bottom Left
                        glVertex3f(three.x-recentreX, three.y-recentreY, three.z-recentreZ);		// Bottom Right
                        glEnd();
                    }
                    glColor4fv(array);
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
                    
                }
            }
        }*/
                if(surfaceTriangles){
                    
                    if(((mp->Mx>=sectX)&&(mp->Mx<sectX2))&&((mp->My>=sectY)&&(mp->My<sectY2))&&((mp->Mz>=sectZ)&&(mp->Mz<sectZ2))){

                        for(d=0;d<2;d++){
                        if(d==0) triangleNodes = mp->getTriangle(up);
                            else triangleNodes = mp->getTriangle(down);


                           if(triangleNodes.size()==3){

                                
                                //red=0.4; green=0.2245098; blue = 0.545098;
                                
                                if(viewType==1){ red=(float)mp->Cell->VEGFRtot/((float)VEGFRNORM); green=0.2245098; blue = 0.545098;
                                //if(red<0.5) red = 0.5;
                                }
                                else if(viewType==3){ red=(float)mp->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                                else if(viewType==2){ red= 0.534f;green=0.0623f+mp->Cell->Dll4tot/(float)MAX_dll4; blue = 0.5923f;}
                                else if(viewType==4){ red= 0.534f;green=0.0623f+mp->Dll4/50.0f; blue = 0.5923f;}
                                else if(viewType==5){ red=(mp->VEGFR/30.0f); green=0.2245098; blue = 0.545098;}
                                else if(viewType==6){ red=mp->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                                else if(viewType==7){ red= 0.534f;green=0.0623f+(float)mp->Cell->activeNotchtot/(float)NotchNorm; blue = 0.5923f;}
                                
                               
                                 GLfloat array[] = {red, green, blue, 1.0};
                                 one.x = mp->Mx;
                                 one.y = mp->My;
                                 one.z = mp->Mz;

                                 two.x = triangleNodes[1]->Mx;
                                 two.y = triangleNodes[1]->My;
                                 two.z = triangleNodes[1]->Mz;

                                 three.x = triangleNodes[2]->Mx;
                                 three.y = triangleNodes[2]->My;
                                 three.z = triangleNodes[2]->Mz;

                                cross=WORLDpointer->calcNormal(two,one, three);
                               
                               
                                if(d==1){
                                X=-cross->x;
                                Y=-cross->y;
                                Z=-cross->z;
                                }
                                else{
                                X=cross->x;
                                Y=cross->y;
                                Z=cross->z;
                                }
                               
                                glNormal3f(X,Y,Z);
                                delete cross;
                                
                                if((fabs(mp->Mx-two.x)<xMAX/2.0f)&&(fabs(mp->Mx-three.x)<xMAX/2.0f)&&(fabs(two.x-three.x)<xMAX/2.0f)){
                                    glColor4fv(array);
                                    glBegin(GL_TRIANGLES);		// Drawing Using Triangles
                                    glVertex3f(mp->Mx-recentreX, mp->My-recentreY, mp->Mz-recentreZ);		// Top
                                    glVertex3f(two.x-recentreX, two.y-recentreY, two.z-recentreZ);		// Bottom Left
                                    glVertex3f(three.x-recentreX, three.y-recentreY, three.z-recentreZ);		// Bottom Right
                                    glEnd();

                                    glColor4fv(array);
                                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, array);
                                }
                                
                                
                            }
                            coords.clear();
                        }
                    }
                }
                if(((mp->Mx>=sectX)&&(mp->Mx<sectX2))&&((mp->My>=sectY)&&(mp->My<sectY2))&&((mp->Mz>=sectZ)&&(mp->Mz<sectZ2))){
                    
                    //if(j%2==1){
                    //red=1.0f; green = 0.0f; blue = 1.0f;
                    
                    if(viewType==1){ red=((float)mp->Cell->VEGFRtot/((float)VEGFRNORM/(float)ECcross)); green=0.2245098; blue = 0.545098;}//if(red<0.5) red = 0.5;}
                    else if(viewType==3){ red=(float)mp->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                    else if(viewType==2){ red= 0.534f;green=0.0623f+mp->Cell->Dll4tot/(float)MAX_dll4; blue = 0.5923f;}
                    else if(viewType==4){ red= 0.534f;green=0.0623f+mp->Dll4/50.0f; blue = 0.5923f;}
                    else if(viewType==5){ red=(mp->VEGFR/30.0f); green=0.2245098; blue = 0.545098;}
                    else if(viewType==6){ red=mp->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                    else if(viewType==7){ red= 0.534f;green=0.0623f+(float)mp->Cell->activeNotchtot/(float)NotchNorm; blue = 0.5923f;}
                    
            if(viewType==8){
                    /*if((mp->FIL==BASE)&&(baseView)){red = 0.8; green = 0.8; blue = 0.8;}
                    else if((mp->FIL==STALK)&&(stalkView)){red = 0.5; green = 0.5; blue = 0.5;}
                    else if((mp->FIL==TIP)&&(tipView)){red = 0.2; green = 0.2; blue = 0.2;}*/
                    red = 0.2; green = 0.2; blue = 0.2;
                    float fog;
                    if(mp->Mz>10)
                        fog = (mp->Mz-10)/20.0f;
                    else
                        fog = 0.0f;
                    
				
                    /*if((mp->FIL==BASE)&&(baseView))red = 0.0;
                    else if((mp->FIL==STALK)&&(stalkView))green = 0.6;
                    else if((mp->FIL==TIP)&&(tipView))green = 0.8;
                    else if((mp->FIL==NONE)&&(noneView)) blue= 0.0;*/
                    if((mp->labelled==true)&&(labelledView)){
                        red = 210.0f/255.0f; green =58.0f/255.0f; blue = 230.0f/255.0f;
                    }
                    
                   
                    red+=fog;green+=fog; blue+=fog;
                    }
                    
					
                    //if((mp->FA==true)&&(FAview))green = 1.8;
                    
                    //DrawCube((mp->Mx)-recentreX, (mp->My)-recentreY, (mp->Mz)-recentreZ, red, green, blue, 1, L);
                       GLfloat array[] = {red, green, blue, 1.0};
                       glColor4fv(array);
                    posx = (mp->Mx)-recentreX;
                    posy = (mp->My)-recentreY;
                    posz = (mp->Mz)-recentreZ;
                    
                  glTranslatef(posx,posy,posz);
            GLUquadric* quad = gluNewQuadric();
            gluSphere(quad, 0.45f,18,18);
            glTranslatef(-posx,-posy,-posz);
            gluDeleteQuadric(quad);
                    
                    /*if(viewType==1){ red=((float)mp->Cell->VEGFRtot/((float)VEGFRnorm/(float)ECcross)); green=0.2245098; blue = 0.545098;}//if(red<0.5) red = 0.5;}
                    else if(viewType==3){ red=(float)mp->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                    else if(viewType==2){ red= 0.534f;green=0.0623f+mp->Cell->Dll4tot/5000.0f; blue = 0.5923f;}
                    else if(viewType==4){ red= 0.534f;green=0.0623f+mp->Dll4/50.0f; blue = 0.5923f;}
                    else if(viewType==5){ red=(mp->VEGFR/30.0f); green=0.2245098; blue = 0.545098;}
                    else if(viewType==6){ red=mp->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                    else if(viewType==7){ red= 0.534f;green=0.0623f+mp->Cell->activeNotchtot/10000.0f; blue = 0.5923f;}
                    */
                    
                    drawSprings(mp, red, green, blue);
                    
                    
                }
            }
            }
        }
    }
    
    
}
//-------------------------------------------------------------------------------------------------------------------
void World::viewGrid(void){
    
    int i, j, k, m, uptoM;
    float L = 1.0f;
    float red, green, blue;
    int flag;
    
    if(sectX<0) sectX=0;
    if(sectY<0) sectY=0;
    if(sectZ<0) sectZ=0;
    
    if(sectX2>xMAX) sectX2=xMAX;
    if(sectY2>yMAX) sectY2=yMAX;
    if(sectZ2>zMAX) sectZ2=zMAX;
    
    int recentreX = (int)(sectX+(sectX2-sectX)/2.00);
    
    int recentreY = (int)(sectY+(sectY2-sectY)/2.00);
    
    int recentreZ = (int)(sectZ+(sectZ2-sectZ)/2.00);
    
    if(viewType==8){
     glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
}
    //solid cubes
    for(i=sectX;i<sectX2;i++)
        for(j=sectY;j<sectY2;j++)
            for(k=sectZ;k<sectZ2;k++){
                flag=0;
                
                //if(grid[i][j][k].Eid!=NULL)
                        //if(junctionView==1)DrawCube(((float)i)-recentreX, ((float)j)-recentreY, ((float)k)-recentreZ, 0.1, 0.1, 1.0, 0.6, L);
                               if(grid[i][j][k].type==E){


                        if(viewInsideEnv==1){
                            if(grid[i][j][k].Eid->inside==true){
                            red=0.9f; green = 0.0f; blue =0.0;
                            DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, 1.0,L);//(grid[i][j][k].Eid->VEGF)*0.01, L);
                       }
                        }
                        
                                //........................................................................................................................................................................
                //astrocytes blue
                if(((grid[i][j][k].Eid->Astro==true)||(grid[i][j][k].Eid->OldAstro==true))&&(astro)){
                    red=0.067; green=0.1245098; blue = 0.445098;
                    DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, 1.0, L);

                }
                //........................................................................................................................................................................

                        
                               }
                //........................................................................................................................................................................
                //view filopodia
                if((grid[i][j][k].Fids.size()!=0)){
                    
                    if(membrane==1){
                        for(m=0;m<(int)grid[i][j][k].Fids.size();m++){
                            if((pickedCell==ECELLS)||(grid[i][j][k].Fids[m]->Cell==ECagents[pickedCell])){
                                if((nodeAgentView==1)&&(grid[i][j][k].Fids[m]->node==true)) flag=1;
                                if((springAgentsView==1)&&(grid[i][j][k].Fids[m]->node==false)) flag=1;
                                if(flag==1){
                                    if(viewType==1){ red=((float)grid[i][j][k].Fids[m]->Cell->VEGFRtot/((float)VEGFRNORM/(float)ECcross)); green=0.2245098; blue = 0.545098;}//if(red<0.5) red = 0.5;}
                                    else if(viewType==3){ red=(float)grid[i][j][k].Fids[m]->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                                    else if(viewType==2){ red= 0.534f;green=0.0623f+grid[i][j][k].Fids[m]->Cell->Dll4tot/(float)MAX_dll4; blue = 0.5923f;}
                                    else if(viewType==4){ red= 0.534f;green=0.0623f+grid[i][j][k].Fids[m]->Dll4/50.0f; blue = 0.5923f;}
                                    else if(viewType==5){ red=(grid[i][j][k].Fids[m]->VEGFR/20.0f); green=0.2245098; blue = 0.545098;}
                                    else if(viewType==6){ red=grid[i][j][k].Fids[m]->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                                    else if(viewType==7){ red= 0.534f;green=0.0623f+(float)grid[i][j][k].Fids[m]->activeNotch/(float)NotchNorm; blue = 0.5923f;}
                                    else if(viewType==8){
									
                                    if(grid[i][j][k].Fids[m]->node==false){red= 0.7f;green=0.7f; blue = 0.7f;}
                                    else{red= 0.0f;green=0.0f; blue = 0.0f;}
}else if(viewType==10){ red= grid[i][j][k].Fids[m]->Cell->red;green=grid[i][j][k].Fids[m]->Cell->green; blue =grid[i][j][k].Fids[m]->Cell->blue;}

                                    if((junctionView==1)&&(grid[i][j][k].Fids[m]->junction ==true)) {red=1.03f; green = 1.8f; blue = 1.2f;}
                                    if((FAview==1)&&(grid[i][j][k].Fids[m]->FA ==true)) {red=0.03f; green = 1.8f; blue = 0.2f;}
                                    
                                    //drawFilAgentsCylinders(grid[i][j][k].Fids[m]->Mx,grid[i][j][k].Fids[m]->My, grid[i][j][k].Fids[m]->Mz, recentreX, recentreY, recentreZ, red, green, blue, 0.2, m);
                                    DrawCube(((float)i)-recentreX, ((float)j)-recentreY, ((float)k)-recentreZ, red, green, blue, 0.6, L);
                                }
                            }
                        }
                        
                    }
                }
                //........................................................................................................................................................................
                //........................................................................................................................................................................
                //there is no cytoplasm in this model version!
                if(grid[i][j][k].type==MED){
                    
                    //if(cytoView==1){
                        red=0.8f; green = 0.5f; blue = 0.7f;
                        DrawCube(((float)i)-recentreX, ((float)j)-recentreY, ((float)k)-recentreZ, red, green, blue, 1, L);
                    //}
                }
                //........................................................................................................................................................................
                //........................................................................................................................................................................
                //membrane
                if(grid[i][j][k].type==M){
                    /*tester
                     * int LER;
                     * int fleg=0;
                     *
                     * if(grid[i][j][k].Mids.size()>1){
                     * for(LER=0;LER<grid[i][j][k].Mids.size();LER++){
                     * if((grid[i][j][k].Mids[LER]->FIL==BASE)||(grid[i][j][k].Mids[LER]->FIL==NONE)){
                     * if(fleg==1) cout<<"node = "<<grid[i][j][k].Mids[LER]->node<<endl<<"doubled up!"<<grid[i][j][k].Mids.size()<<endl;
                     * if(fleg==0){ fleg=1; cout<<"node = "<<grid[i][j][k].Mids[LER]->node<<endl;}
                     *
                     * }
                     * }
                     * }*/
                    uptoM = grid[i][j][k].Mids.size();
                    
                    if(membrane==1){
                        for(m=0;m<uptoM;m++){
                            if((pickedCell==ECELLS)||(grid[i][j][k].Mids[m]->Cell==ECagents[pickedCell])){
                                /*if((springAgentsView==1)&&(grid[i][j][k].Mids[m]->node==false)&&(grid[i][j][k].Mids[m]->triangle.size()==0)){
                                    
                                    //if(grid[i][j][k].Mids[m]->FIL==NONE) cout<<"left over"<<endl;
                                    red=0.30f; green = 0.40f; blue = 1.0f;
                                    //if(grid[i][j][k].Mids.size()>1)cout<<"doubled up!"<<grid[i][j][k].Mids.size()<<endl;
                                    if(viewType==1){ red=((float)grid[i][j][k].Mids[m]->Cell->VEGFRtot/(float)VEGFRnorm); green=0.2245098; blue = 0.545098;}
                                    else if(viewType==3){ red=(float)grid[i][j][k].Mids[m]->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                                    else if(viewType==2){ red= 0.534f;green=0.0623f+grid[i][j][k].Mids[m]->Cell->Dll4tot/5000.0f; blue = 0.5923f;}
                                    else if(viewType==4){ red= 0.534f;green=0.0623f+grid[i][j][k].Mids[m]->Dll4/50.0f; blue = 0.5923f;}
                                    else if(viewType==5){ red=(grid[i][j][k].Mids[m]->VEGFR/20.0f); green=0.2245098; blue = 0.545098;}
                                    else if(viewType==6){ red=grid[i][j][k].Mids[m]->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                                    else if(viewType==7){ red= 0.534f;green=0.0623f+grid[i][j][k].Mids[m]->Notch1/50.0f; blue = 0.5923f;}
                                    else if(viewType==8){ red= 0.5f;green=0.5f; blue = 0.5f;}

                                    if((junctionView==1)&&(grid[i][j][k].Mids[m]->junction ==true)) {red=1.03f; green = 1.8f; blue = 1.2f;}
                                    if((FAview==1)&&(grid[i][j][k].Mids[m]->FA ==true)) {red=0.03f; green = 1.8f; blue = 0.2f;}
                                    DrawCube(i-recentreX-L/2.0f, j-recentreY-L/2.0f, k-recentreZ-L/2.0f, red, green, blue, 0.5, L);
                                }*/
                                
                                if((surfaceAgentsView==1)&&(grid[i][j][k].Mids[m]->triangle.size()!=0)){
                                    
                                    //if(grid[i][j][k].Mids[m]->FIL==NONE) cout<<"left over"<<endl;
                                    red=0.30f; green = 0.40f; blue = 1.0f;
                                    //if(grid[i][j][k].Mids.size()>1)cout<<"doubled up!"<<grid[i][j][k].Mids.size()<<endl;
                                    if(viewType==1){ red=((float)grid[i][j][k].Mids[m]->Cell->VEGFRtot/((float)VEGFRNORM/(float)ECcross)); green=0.2245098; blue = 0.545098;}//if(red<0.5) red = 0.5;}
                                    else if(viewType==3){ red=(float)grid[i][j][k].Mids[m]->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                                    else if(viewType==2){ red= 0.534f;green=0.0623f+grid[i][j][k].Mids[m]->Cell->Dll4tot/(float)MAX_dll4; blue = 0.5923f;}
                                    else if(viewType==4){ red= 0.534f;green=0.0623f+grid[i][j][k].Mids[m]->Dll4/50.0f; blue = 0.5923f;}
                                    else if(viewType==5){ red=(grid[i][j][k].Mids[m]->VEGFR/20.0f); green=0.2245098; blue = 0.545098;}
                                    else if(viewType==6){ red=grid[i][j][k].Mids[m]->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                                    else if(viewType==7){ red= 0.534f;green=0.0623f+(float)grid[i][j][k].Mids[m]->activeNotch/(float)NotchNorm; blue = 0.5923f;}
                                    else if(viewType==8){ red= 0.3f;green=0.3f; blue = 0.3f;}
                                    else if(viewType==10){ red= grid[i][j][k].Mids[m]->Cell->red;green=grid[i][j][k].Mids[m]->Cell->green; blue =grid[i][j][k].Mids[m]->Cell->blue;}


									
                                    if((junctionView==1)&&(grid[i][j][k].Mids[m]->junction ==true)) {red=1.03f; green = 1.8f; blue = 1.2f;}
                                    if((FAview==1)&&(grid[i][j][k].Mids[m]->FA ==true)) {red=0.03f; green = 1.8f; blue = 0.2f;}

                                    if((grid[i][j][k].Mids[m]->labelled==true)&&(labelledView))green = 0.8;
                                    if((grid[i][j][k].Mids[m]->labelled2==true)&&(labelledView))green = 0;

                                    DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, 0.6, L);
                                }
                                
                                if((nodeAgentView==1)&&(grid[i][j][k].Mids[m]->node==true)){
                                    
                                    if(viewType==1){ red=((float)grid[i][j][k].Mids[m]->Cell->VEGFRtot/((float)VEGFRNORM/(float)ECcross)); green=0.2245098; blue = 0.545098;}//if(red<0.5) red = 0.5;}
                                    else if(viewType==3){ red=(float)grid[i][j][k].Mids[m]->filTokens/5.0f;green=0.2245098; blue = 0.545098;}
                                    else if(viewType==2){ red= 0.534f;green=0.0623f+grid[i][j][k].Mids[m]->Cell->Dll4tot/(float)MAX_dll4; blue = 0.5923f;}
                                    else if(viewType==4){ red= 0.534f;green=0.0623f+grid[i][j][k].Mids[m]->Dll4/50.0f; blue = 0.5923f;}
                                    else if(viewType==5){ red=(grid[i][j][k].Mids[m]->VEGFR/20.0f); green=0.2245098; blue = 0.545098;}
                                    else if(viewType==6){ red=grid[i][j][k].Mids[m]->VEGFRactive/5.0f; green=0.2245098; blue = 0.545098;}
                                    else if(viewType==7){ red= 0.534f;green=0.0623f+(float)grid[i][j][k].Mids[m]->activeNotch/(float)NotchNorm; blue = 0.5923f;
                                    //cout<<grid[i][j][k].Mids[m]->activeNotch<<endl;
                                    }
                                    else if(viewType==8){ red= 0.0f;green=0.0f; blue = 0.0f;}
                    else if(viewType==10){ red= grid[i][j][k].Mids[m]->Cell->red;green=grid[i][j][k].Mids[m]->Cell->green; blue =grid[i][j][k].Mids[m]->Cell->blue;}
				
                                    if((junctionView==1)&&(grid[i][j][k].Mids[m]->junction ==true)){ red=1.03f; green = 1.8f; blue = 1.2f;}
                                    if((FAview==1)&&(grid[i][j][k].Mids[m]->FA ==true)) {red=0.03f; green = 1.8f; blue = 0.2f;}
                                    if((grid[i][j][k].Mids[m]->labelled==true)&&(labelledView))green = 0.8;
                                    if((grid[i][j][k].Mids[m]->labelled2==true)&&(labelledView))green = 0;
                                    
                                    DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, 0.6, L);
                                    
                                    
                                }
                            }
                        }
                        
                    }
                }
            }
    //........................................................................................................................................................................
    
    //if(viewType!=10){
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
    
    if((bloodView==1)||(VEGFview==1)){
        for(i=sectX;i<sectX2;i++)
            for(j=sectY;j<sectY2;j++)
                for(k=sectZ;k<sectZ2;k++){
                    
                
                    //........................................................................................................................................................................
                    //environment -VEGF or blood  *translucent*
                    if(grid[i][j][k].type==BLOOD){
                        
                        if(bloodView==1){
                            red=1.0f; green = 0.0f; blue = 0.0f;
                            
                            DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, 0.1, L);
                        }
                    }
                    if(grid[i][j][k].type==E){

                        //if(grid[i][j][k].Eid->inside==true){
                        //    red=0.2f; green = 0.57f; blue =0.3;
                        //    DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, 1,L);//(grid[i][j][k].Eid->VEGF)*0.01, L);

                        //}
                        if((VEGFview==1)&&(grid[i][j][k].Eid->VEGF>0.0f)){
                            
                                                        //red=1.0f; green = 1.0f; blue =(grid[i][j][k].Eid->VEGF)*0.01f;
                            red=0.2f; green = 0.57f; blue =0.3+(grid[i][j][k].Eid->VEGF)*0.01f;
                            DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, (grid[i][j][k].Eid->VEGF)*0.01,L);//(grid[i][j][k].Eid->VEGF)*0.01, L);
                            
                            /*glColor4f(1.0f,1.0f,(grid[i][j][k].Eid->VEGF)*0.01, (grid[i][j][k].Eid->VEGF)*0.01);
                             * // Specify the point size before the primitive is specified
                             * glPointSize(5.0f);
                             *
                             * // Draw the point
                             * glBegin(GL_POINTS);
                             * glVertex3f(i-recentreX,j-recentreY,k-recentreZ );
                             * glEnd();
                            */
                            
                        }
                    }
                    //........................................................................................................................................................................
                }
        
    }
     glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
  
    
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
//test out the grids line function
void testGridAgents(void){
    
    float P[3] = {0.1, 2.9, 1.0};
    float N[3] = {2.9, 1.1, 1.0};
    int array[10][10][10] = {{{0}}};
    float steps=0.5f;
    //array[(int)P[0]][(int)P[1]][(int)P[2]] = 1;
    //array[(int)N[0]][(int)N[1]][(int)N[2]] = 1;
    array[(int)P[0]][(int)P[1]][(int)P[2]] = 1;
    array[(int)N[0]][(int)N[1]][(int)N[2]] = 1;
    
    int A, B, D;
    float x, y, z;
    float PN[3];
    int i, j, k;
    float x1, y1, z1, x2, y2, z2;
    float L = 0.3;
    float red, green, blue;
    
    if(sectX<0) sectX=0;
    if(sectY<0) sectY=0;
    if(sectZ<0) sectZ=0;
    
    if(sectX2>xMAX) sectX2=xMAX;
    if(sectY2>yMAX) sectY2=yMAX;
    if(sectZ2>zMAX) sectZ2=zMAX;
    
    int recentreX = (int)(sectX+(sectX2-sectX)/2.00);
    
    int recentreY = (int)(sectY+(sectY2-sectY)/2.00);
    
    int recentreZ = (int)(sectZ+(sectZ2-sectZ)/2.00);
    
    red=1.0f; green = 0.0f; blue = 1.0f;
    
    if(meshView==1){
        L=0.2;
        red=1.0f; green = 0.0f; blue = 1.0f;
        DrawCube((P[0])-recentreX, (P[1])-recentreY, (P[2])-recentreZ, red, green, blue, 1, L);
        red=1.0f; green = 1.0f; blue = 1.0f;
        DrawCube((N[0])-recentreX, (N[1])-recentreY, (N[2])-recentreZ, red, green, blue, 1, L);
        
        red=1.0f; green = 0.0f; blue = 1.0f;
        glBegin(GL_LINES);
        glVertex3f(P[0]-recentreX, P[1]-recentreY, P[2]-recentreZ);
        glVertex3f(N[0]-recentreX, N[1]-recentreY, N[2]-recentreZ);
        glEnd();
    }
    
    
    
    //glEnable(GL_LIGHT0);
    A=((int)P[0]-(int)N[0])*((int)P[0]-(int)N[0]);
    B= ((int)P[1]-(int)N[1])*((int)P[1]-(int)N[1]);
    D=((int)P[2]-(int)N[2])*((int)P[2]-(int)N[2]);
    
    //if the spring stretches past its nearest neighs
    if((sqrt((double)A)>1)||(sqrt((double)B)>1)||(sqrt((double)D)>1)){
        
        //check if the grid sites inbetween along that spring are empty,
        PN[0] = P[0]-N[0];
        PN[1] = P[1]-N[1];
        PN[2] = P[2]-N[2];
        
        x1=P[0];
        y1=P[1];
        z1=P[2];
        
        x2=N[0];
        y2=N[1];
        z2=N[2];
        
        if(x2>x1+steps){
            x=x1+steps;
            //we know this is the axis along which the vector increases
            do{
                y =(((x-x1)/PN[0])*PN[1])+y1;
                z = (((x-x1)/PN[0])*PN[2])+z1;
                array[(int)x][(int)y][(int)z]=1;
                x+=steps;
                cout<<"x2>x1 ";
            }while((int)x!=(int)x2);
            
        }
        else if(x2<x1-steps) {
            x=x1-steps;
            //we know this is the axis along which the vector increases
            do{
                y =(((x-x1)/PN[0])*PN[1])+y1;
                z = (((x-x1)/PN[0])*PN[2])+z1;
                array[(int)x][(int)y][(int)z]=1;
                x-=steps;
                cout<<"x2<x1 ";
            }while((int)x!=(int)x2);
        }
        
        if(y2>y1+steps){
            
            y=y1+steps;
            //we know this is the axis along which the vector increases
            do{
                x =(((y-y1)/PN[1])*PN[0])+x1;
                z = (((y-y1)/PN[1])*PN[2])+z1;
                array[(int)x][(int)y][(int)z]=1;
                y+=steps;
                cout<<"y2>y1 ";
            }while((int)x!=(int)x2);
        }
        else if(y2<y1-steps){
            
            y=y1-steps;
            //we know this is the axis along which the vector increases
            do{
                x =(((y-y1)/PN[1])*PN[0])+x1;
                z = (((y-y1)/PN[1])*PN[2])+z1;
                array[(int)x][(int)y][(int)z]=1;
                y-=steps;
                cout<<"y2<y1 ";
                
            }while((int)x!=(int)x2);
        }
        
        if(z2>z1+steps){
            z=z1+steps;
            //we know this is the axis along which the vector increases
            do{
                x =(((z-z1)/PN[2])*PN[0])+x1;
                y = (((z-z1)/PN[2])*PN[1])+y1;
                array[(int)x][(int)y][(int)z]=1;
                z+=steps;
                cout<<"z2>z1 ";
                
            }while((int)x!=(int)x2);
        }
        else if(z2<z1-steps){
            
            z=z1-steps;
            //we know this is the axis along which the vector increases
            do{
                x =(((z-z1)/PN[2])*PN[0])+x1;
                y = (((z-z1)/PN[2])*PN[1])+y1;
                array[(int)x][(int)y][(int)z]=1;
                z-=steps;
                cout<<"z2<z1 ";
                
                
            }while((int)x!=(int)x2);
        }
        
    }
    
    
    //draw the grid lines-----------------------------------------------------------------
    
    for(k=sectZ;k<sectZ2;k++){
        for(i=sectX;i<=sectX2;i++){
            
            red=0.0f; green = 1.0f; blue =0.0f;
            glColor4f(red, green, blue, 1.0);
            
            glBegin(GL_LINES);
            glVertex3f(i-recentreX, 0-recentreY, k-recentreZ);
            glVertex3f(i-recentreX, 10-recentreY, k-recentreZ);
            glEnd();
        }
        for(j=sectY;j<=sectY2;j++){
            
            red=0.0f; green = 1.0f; blue =0.0f;
            glColor4f(red, green, blue, 1.0);
            
            glBegin(GL_LINES);
            glVertex3f(0-recentreX, j-recentreY, k-recentreZ);
            glVertex3f(10-recentreX, j-recentreY, k-recentreZ);
            glEnd();
        }
    }
    
    //-------------------------------------------------------------------------------------------
    if(gridView==1){
        L=1.0;
        for(i=sectX;i<sectX2;i++)
            for(j=sectY;j<sectY2;j++)
                for(k=sectZ;k<sectZ2;k++){
                    if(array[i][j][k]==1){
                        
                        red=1.0f; green = 0.0f; blue = 1.0f;
                        DrawCube(i-recentreX, j-recentreY, k-recentreZ, red, green, blue, 1, L);
                    }
                    /*else{
                     *
                     * red=0.01f; green = 0.03f; blue = 0.20f;
                     * DrawCube((i+L/2.0f)-recentreX, (j+L/2.0f)-recentreY, (k+L/2.0f)-recentreZ, red, green, blue, 1, L);
                     * }*/
                    
                }
    }
    
}
//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
void simulate_multiple(void){

 /*   World* world;
    
    if(RUN==3) RUN=2;
    if(WORLDpointer->timeStep==MAXtime+1){
        RUN=1;
        delete WORLDpointer;
    }
    if(RUN==1){
    
        world= new World();
        
        WORLDpointer=world;
        RUN=2;
    }
    if(RUN==0) RUN=3;
    
    WORLDpointer->simulate(0);
    
    */
}
//--------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
void myGlutReshape( int x, int y ) {
    int tx, ty, tw, th;
    int initialZoom=-(int)((xMAX/2)*100.5);
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );

    glViewport (0, 0, (GLsizei) x, (GLsizei) y);
    
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    // Calculate The Aspect Ratio And Set The Clipping Volume
    if (y == 0) y = 1;
    //glOrtho(-((float)w / 2.0f), (float)w / 2.0f, -((float)h / 2.0f), (float)h / 2.0f, -400.0f, 400.0f);

    gluPerspective(45.0f, (float)x/(float)y, 0.1, 10000.0);
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
    //glLoadIdentity(); // Reset The Modelview Matrix
    glTranslatef(xMAX/4, yMAX/4, initialZoom);

    xy_aspect = (float)tw / (float)th;

    glutPostRedisplay();
}
//----------------------------------------------------------------------------------------------------

void screenRecording()
{
    switch (screenRecordState)
    {
        case NotRecording:
            break;
        case Recording:
            ScreenRecording::screenDump(string("movie/" + to_string(frameNumber) + ".tga").c_str());
            frameNumber++;
            break;
        case SaveRecording:
            ScreenRecording::saveAndCreateVideo();
            frameNumber = 0;
            screenRecordState = NotRecording;
            break;
    }
}

GLUI_CB startRecording()
{
    statText->set_text("Recording...");
    startRecordButton->disable();
    endRecordButton->enable();
    screenRecordState = Recording;
}

GLUI_CB endRecording()
{
    statText->set_text("");
    endRecordButton->disable();
    startRecordButton->enable();
    screenRecordState = SaveRecording;
    screenRecording();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void display(void) {
    
    
    
    
    //on the fly movie making
    if((startMovie==1)&&(movie==1)){ movie=2; startMovie=0;}
    else if((startMovie==1)&&(movie==0)) movie=1;
    else if(endMovie==1){ movie=3; endMovie=0;}
    else if((endMovie==0)&&(movie==3)) movie=0;

    
    if(WORLDpointer->Pause==0){
        
       
        //simulate_multiple();
        //WORLDpointer->simulate(movie);
        if (WORLDpointer->timeStep < MAXtime)
        {
            WORLDpointer->simulateTimestep();
            screenRecording();
            counter_edittext->set_int_val(WORLDpointer->timeStep);
            if (ANALYSIS_HYSTERESIS)
                WORLDpointer->hysteresisAnalysis();
            if (ANALYSIS_TIME_TO_PATTERN)
                WORLDpointer->evaluateSandP();
        }

        if (WORLDpointer->timeStep >= MAXtime && screenRecordState == Recording)
        {
            endRecording();
        }
        
       //if(WORLDpointer->timeStep==0)
          //WORLDpointer->Pause=1;
        
    }
    if(MACROS==2){
    if(WORLDpointer->timeStep==0){
        macro1_pos[0] = WORLDpointer->macrophages[0]->coords.x;
        macro1_pos[1] = WORLDpointer->macrophages[0]->coords.y;
        macro1_pos[2] = WORLDpointer->macrophages[0]->coords.z;
        
        macro2_pos[0] = WORLDpointer->macrophages[1]->coords.x;
        macro2_pos[1] = WORLDpointer->macrophages[1]->coords.y;
        macro2_pos[2] = WORLDpointer->macrophages[1]->coords.z;
    }
    }
   // glui->sync_live();
   glEnable( GL_LIGHTING );
    //glEnable(GL_LIGHT0);
    //------------------------
    if(viewType==8)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode( GL_MODELVIEW);
   
    glLoadIdentity();
    glTranslatef( obj_pos[0], obj_pos[1], obj_pos[2] );
    glMultMatrixf( view_rotate );
    //glScalef( scale, scale, scale );
    
    //glFlush();
    //glPushMatrix();
    //glMultMatrixf( object_rotate );
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat array[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, array);
    glMaterialf(GL_FRONT, GL_SHININESS, 55.0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
  

   
    //cout<< "starting viewWorld"<<endl;
    WORLDpointer->viewWorld();
    //cout<< "done viewWorld"<<endl;
    
    glLoadIdentity();
    glMultMatrixf( view_rotate );
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    
    //testGridAgents();
    
    //glPopMatrix();
    
    
    
    glutSwapBuffers();
    
    glFlush();
    
    
    
}
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
/*void myGlutReshape( int x, int y ) {
    int tx, ty, tw, th;
    int initialZoom=-(int)((xMAX/2)*1.5);
   // GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    glViewport( tx, ty, tw, th );
    
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    // Calculate The Aspect Ratio And Set The Clipping Volume
    if (y == 0) y = 1;
    //glOrtho(-((float)w / 2.0f), (float)w / 2.0f, -((float)h / 2.0f), (float)h / 2.0f, -400.0f, 400.0f);
    
    gluPerspective(45.0f, (float)x/(float)y, 0.1, 10000.0);
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
    //glLoadIdentity(); // Reset The Modelview Matrix
    glTranslatef(xMAX/4, yMAX/4, initialZoom);
    
    xy_aspect = (float)tw / (float)th;
    
    glutPostRedisplay();
}*/
//----------------------------------------------------------------------------------------------------
void displayGlui(int * argc, char  ** argv) {
    
    glutInit(argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( 50, 0 );
    glutInitWindowSize( WinWidth, WinHeight);
    
    
    main_window = glutCreateWindow( "MemAgent Spring Model: K. Bentley - Single Cell Bistability test ALife 14 version: 2019" );
    
    init() ;
    
    glutDisplayFunc(display);
    //screendump(WinWidth, WinHeight);
   
    glutReshapeFunc(myGlutReshape);
    glutIdleFunc(myGlutIdle);
    GLUI_Master.set_glutReshapeFunc( myGlutReshape);
    GLUI_Master.set_glutSpecialFunc( NULL );
    
    glEnable(GL_LIGHTING);
    glEnable( GL_NORMALIZE );
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    
    /****************************************/
    /*          Enable z-buferring          */
    /****************************************/
    
    glEnable(GL_DEPTH_TEST);
    
    
    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/
    
    //create the GLUI
    //glui = GLUI_Master.create_glui( "GLUI" );
    

    /*** Create the side subwindow ***/
    glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT );
    
    //quit button
    glui->add_button( "Quit", 0, (GLUI_Update_CB)exit );
    
    GLUI_Panel *movies= glui->add_panel("Movie", true);
    statText = glui->add_statictext_to_panel(movies, "");
    startRecordButton = glui->add_button_to_panel(movies, "Start Recording", 1, (GLUI_Update_CB)startRecording);
    endRecordButton = glui->add_button_to_panel(movies, "End Recording", 2, (GLUI_Update_CB)endRecording);

    statText->set_text("Recording...");
    startRecordButton->disable();
    screenRecordState = Recording;
    
    GLUI_Panel *viewer= glui->add_panel("View", true);
    
    GLUI_Listbox *listbox = glui->add_listbox("View", &viewType);
    listbox->add_item(1, "EC VEGFR");
    listbox->add_item(2, "EC Dll4");
    listbox->add_item(3, "Mem filTokens");
    listbox->add_item(4, "local Dll4");
    listbox->add_item(5, "local VEGFR");
    listbox->add_item(6, "local active VEGFR");
    listbox->add_item(7, "active Notch");
    listbox->add_item(8, "black and white");
    listbox->add_item(9, "force");
    listbox->add_item(10, "cell colours");
    

	
    GLUI_Panel *vessels= glui->add_panel("Display elements", true);
    glui->add_checkbox_to_panel( vessels, "Mesh", &meshView);
    glui->add_checkbox_to_panel( vessels, "grid", &gridView);
    //glui->add_checkbox_to_panel( vessels, "Cytoplasm", &cytoView);
    glui->add_checkbox_to_panel( vessels, "Junction", &junctionView);
    glui->add_checkbox_to_panel( vessels, "horizontal springs", &horizontalView);
    glui->add_checkbox_to_panel( vessels, "right springs", &rightView);
    glui->add_checkbox_to_panel( vessels, "Focal Adhesions", &FAview);
    glui->add_checkbox_to_panel( vessels, "Membrane", &membrane);
    glui->add_checkbox_to_panel( vessels, "springAgents", &springAgentsView );
    glui->add_checkbox_to_panel( vessels, "nodeAgents", &nodeAgentView );
    glui->add_checkbox_to_panel( vessels, "surface agents", &surfaceAgentsView );
    glui->add_checkbox_to_panel( vessels, "surface triangles", &surfaceTriangles );
    //glui->add_checkbox_to_panel( vessels, "lines", &linesView );
    glui->add_checkbox_to_panel( vessels, "astrocytes", &astro );
    //glui->add_checkbox_to_panel( vessels, "view single agent", &pickAgent);
    glui->add_checkbox_to_panel( vessels, "VEGF", &VEGFview);
    glui->add_checkbox_to_panel( vessels, "view light pos", &viewLight);
    glui->add_checkbox_to_panel( vessels, "view vessel axes", &vesselView);
    glui->add_checkbox_to_panel( vessels, "view vessel axes", &vesselView);
    glui->add_checkbox_to_panel( vessels, "view inside vessel", &viewInsideEnv);
    glui->add_checkbox_to_panel( vessels, "junctions only", &junctionOnlyView);
    glui->add_checkbox_to_panel( vessels, "view Basement Membrane", &viewBM);

    glui->add_checkbox_to_panel( vessels, "BASE", &baseView);
    glui->add_checkbox_to_panel( vessels, "NONE", &noneView);
    glui->add_checkbox_to_panel( vessels, "STALK", &stalkView);
    glui->add_checkbox_to_panel( vessels, "TIP", &tipView);
    glui->add_checkbox_to_panel( vessels, "labelled", &labelledView);
    glui->add_checkbox_to_panel( vessels, "mutants", &mutantView);
    
    GLUI_Rollout *cells = glui->add_rollout_to_panel( viewer, "cell", true);
    GLUI_Spinner *cellsspinner = glui->add_spinner_to_panel(cells, "choose cell", GLUI_SPINNER_INT, &pickedCell);
    cellsspinner->set_int_limits( 0, ECELLS+3);
    GLUI_Spinner *agentspinner = glui->add_spinner_to_panel(cells, "choose agent", GLUI_SPINNER_INT, &pickedAgent);
    agentspinner->set_int_limits( 0, 3000);
    //add rollout
    GLUI_Rollout *view = glui->add_rollout_to_panel( viewer, "Row Slice:", false);
    GLUI_Spinner *row1_spinner = glui->add_spinner_to_panel( view, "From Row:", GLUI_SPINNER_INT, &sectX);
    row1_spinner->set_int_limits( 0, xMAX+1);
    GLUI_Spinner *row2_spinner = glui->add_spinner_to_panel( view, "To Row:", GLUI_SPINNER_INT, &sectX2);
    row2_spinner->set_int_limits( 0, xMAX+1);
    GLUI_Rollout *view2 = glui->add_rollout_to_panel( viewer, "Col Slice:", false);
    GLUI_Spinner *col1_spinner = glui->add_spinner_to_panel( view2, "From Column:", GLUI_SPINNER_INT, &sectY);
    col1_spinner->set_int_limits( 0, yMAX);
    GLUI_Spinner *col2_spinner = glui->add_spinner_to_panel( view2, "To Column:", GLUI_SPINNER_INT, &sectY2);
    col2_spinner->set_int_limits( 0, yMAX);
    GLUI_Rollout *view3 = glui->add_rollout_to_panel( viewer, "Layer Slice:", false);
    GLUI_Spinner *layer1_spinner = glui->add_spinner_to_panel( view3, "From Layer:", GLUI_SPINNER_INT, &sectZ);
    layer1_spinner->set_int_limits( 0, zMAX);
    GLUI_Spinner *layer2_spinner = glui->add_spinner_to_panel( view3, "To Layer:", GLUI_SPINNER_INT, &sectZ2);
    layer2_spinner->set_int_limits( 0, zMAX);
    //add rollout
    GLUI_Rollout *programOutputs = glui->add_rollout( "Program outputs", true );
    counter_edittext = glui->add_edittext_to_panel(programOutputs, "Time step:", GLUI_EDITTEXT_INT, &WORLDpointer->timeStep);
    //counter_edittext->disable();
    
    World* worldP=WORLDpointer;
    glui->add_checkbox("Pause", &worldP->Pause);
    
    
    /**** Link windows to GLUI, and register idle callback ******/
    glui->set_main_gfx_window( main_window );
    
    /*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window,
            GLUI_SUBWINDOW_BOTTOM );
    glui2->set_main_gfx_window( main_window );
    
    glui2->add_column( false );
    GLUI_Rotation *view_rot = glui2->add_rotation( "Rotate view", view_rotate );
    view_rot->set_spin( 1.0);
    
    glui2->add_column( false );
    /*** Add another rollout ***/
    
    glui2->add_column( false );
    GLUI_Translation *trans_xy =
            glui2->add_translation( "move", GLUI_TRANSLATION_XY, obj_pos );
    trans_xy->set_speed( .5 );
    glui2->add_column( false );
    GLUI_Translation *trans_z =
            glui2->add_translation( "zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .5 );
    glui2->add_column( false );
    GLUI_Rotation *lights_rot = glui2->add_rotation( "Light", lights_rotation );
    lights_rot->set_spin( .82 );
    glui2->add_column( false );
    if(MACROS>0){GLUI_Translation *transMac_xy =
            glui2->add_translation( "macrophage 1", GLUI_TRANSLATION_XY, macro1_pos );
    transMac_xy->set_speed( .5 );
    }
    glui2->add_column( false );
    if(MACROS>1){GLUI_Translation *transMac2_xy =
            glui2->add_translation( "macrophage 2", GLUI_TRANSLATION_XY, macro2_pos );
    transMac2_xy->set_speed( .5 );
    }
    
    glui2->add_column( false );
    GLUI_Translation *trans_Lig =
            glui2->add_translation( "Lights XY", GLUI_TRANSLATION_XY, light0_position);
    trans_Lig->set_speed( .5 );
    glui2->add_column( false );
    GLUI_Translation *trans_Liz =
            glui2->add_translation( "Lights", GLUI_TRANSLATION_Z, &light0_position[2] );
    trans_Liz->set_speed( .5 );
    
    /* We register the idle callback with GLUI, *not* with GLUT */
    GLUI_Master.set_glutIdleFunc( myGlutIdle );
}

#endif
