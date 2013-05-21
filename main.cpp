/****************************************************************************
 * Animation Examples
 * CS248 Final Project
 ****************************************************************************/

//#include "st.h"
//#include "stglut.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "math.h"

#include "obj.h"
#include "mesh.h"
#include "loadbmp.h"
#include "CatmullRom.h"


const float PI = 3.14159265;

//room dimensions
float roomWidth = 60;
float roomHeight = 10;
float roomLength = 60;

//imported Rusko dimensions
float maxx, minx, maxy, miny, maxz, minz;
mesh* objMesh;

//CatmullRom stuff
bool jumpOn;
int jumpTimer;
int numControlPoints;
CatmullRom* cr;

//animation
bool upKeyPressed, downKeyPressed, rightKeyPressed, leftKeyPressed;

//OpenGL display-related
int windowWidth  = 640;
int windowHeight = 640;

//camera/world positions, initialized at init
STVector3 camPos, worldPos, lastJump;
float worldAngle;

// Lighting and shading attributes
bool flat;

//Light properties
static float light_specular[] = {1.0, 1.0, 1.0, 1.0};
static float light_ambient[]  = {0.10, 0.10, 0.10, 1.0};
static float light_diffuse[]  = {0.8, 0.8, 0.8, 1.0};
static float light_position[] = {10, 45, 20, 0};

//Material properties
static float material_ambient0[]  = { .4, .6, 1.0, 1.0 };
static float material_diffuse0[]  = { .4, .6, 1.0, 1.0 };
static float material_specular0[] = { 1.0000, 1.0000, 1.0000, 1.0 };
static float shininess = 80.0;  // # between 1 and 128.



/**
 *Functions that draw parts of the room
 */
void drawGround(float width, float height, float length)
{
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f,0.3f); //black

    glNormal3f(0, 0, 1.0);
    glVertex3f(0, 0, length);
        
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(0, 0, 0);
        
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, 0, 0);
        
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, 0, length);

	glEnd();
}

void drawRoof(float width, float height, float length)
{
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f,0.9f); //black
    
    glNormal3f(0, 0, 1.0);
    glVertex3f(0, height, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(0, height, 0);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, height, 0);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, height, length);
    
	glEnd();
}

void drawWalls(float width, float height, float length)
{
    //first wall
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.3f,0.3f); //redish
    
    glNormal3f(0, 0, 1.0);
    glVertex3f(0, 0, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(0, height, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(0, height, 0);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(0, 0, 0);
    
	glEnd();
    
    //second wall
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.3f,0.3f); //redish
    
    glNormal3f(0, 0, 1.0);
    glVertex3f(width, 0, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, height, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, height, 0);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, 0, 0);
    
	glEnd();

    //back wall
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.1f,0.1f); //redish
    
    glNormal3f(0, 0, 1.0);
    glVertex3f(0, 0, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(0, height, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, height, length);
    
    if (!flat)glNormal3f(0, 0, 1.0);
    glVertex3f(width, 0, length);
    
	glEnd();

}

void drawRoom()
{
    drawGround(roomWidth,roomHeight,roomLength);
    //drawRoof(roomWidth,roomHeight,roomLength);
    drawWalls(roomWidth,roomHeight,roomLength);
}

/**
 * Draws the object
 * applies smooth shading, normal per vertex
 */
void drawSmoothObj()
{
    vector<STPoint3>* vertices = objMesh->getVertices();
    vector<STPoint3>* vertices_n = objMesh->getVertices_n();
    
    vector<STPoint3>* faces = objMesh->getFaces();
    vector<STPoint3>* faces_vn = objMesh->getFaces_vn();
    glEnable(GL_NORMALIZE);
    glBegin(GL_TRIANGLES);
    glColor3f(0.2, 0.6, 0.2);
    for (int i=0; i < faces->size(); i++){
        STPoint3 fac = faces->at(i);
        STPoint3 fac_vn = faces_vn->at(i);
        
        STPoint3 v1 = vertices->at((int)fac.x);
        STPoint3 v2 = vertices->at((int)fac.y);
        STPoint3 v3 = vertices->at((int)fac.z);
        
        STPoint3 vn1 = vertices_n->at((int)fac_vn.x);
        STPoint3 vn2 = vertices_n->at((int)fac_vn.y);
        STPoint3 vn3 = vertices_n->at((int)fac_vn.z);
        
        glNormal3f(vn1.x, vn1.y, vn1.z);
        glVertex3f(v1.x,v1.y,v1.z);
        
        glNormal3f(vn2.x, vn2.y, vn2.z);
        glVertex3f(v2.x,v2.y,v2.z);
        
        glNormal3f(vn3.x, vn3.y, vn3.z);
        glVertex3f(v3.x,v3.y,v3.z);
    }
    glEnd ();
    glFlush();
}

/**
 * Draws the object
 * applies flat shading
 */
void drawFlatObj()
{
    vector<STPoint3>* vertices = objMesh->getVertices();
    vector<STPoint3>* vertices_n = objMesh->getVertices_n();
    
    vector<STPoint3>* faces = objMesh->getFaces();
    vector<STPoint3>* faces_vn = objMesh->getFaces_vn();
    
    glBegin(GL_TRIANGLES);
    glColor3f(0.2, 0.6, 0.2);
    for (int i=0; i < faces->size(); i++){
        STPoint3 fac = faces->at(i);
        STPoint3 fac_vn = faces_vn->at(i);
        
        STPoint3 v1 = vertices->at((int)fac.x);
        STPoint3 v2 = vertices->at((int)fac.y);
        STPoint3 v3 = vertices->at((int)fac.z);
        
        STVector3 vn1 = STVector3(vertices_n->at((int)fac_vn.x));
        STVector3 vn2 = STVector3(vertices_n->at((int)fac_vn.y));
        STVector3 vn3 = STVector3(vertices_n->at((int)fac_vn.z));
        
        STVector3 faceNormal = STVector3((vn1+vn2+vn3)/3);
        faceNormal.Normalize();
        
        glNormal3f(faceNormal.x, faceNormal.y, faceNormal.z);
        glVertex3f(v1.x,v1.y,v1.z);
        glVertex3f(v2.x,v2.y,v2.z);
        glVertex3f(v3.x,v3.y,v3.z);
    }
    glEnd ();
    glFlush();
}

/**
 * Sets up light properties for "rusko"
 */
void LightSetup()
{
    //light properties
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    if (flat) glShadeModel(GL_FLAT);
    else glShadeModel(GL_SMOOTH);
    
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT,   light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,   light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
     
    //material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT,   material_ambient0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   material_diffuse0);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  material_specular0);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
}

/**
 * Window resize callback function
 */
void ReshapeCallback(int w, int h)
{
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, windowWidth, windowHeight);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
	//Set up a perspective projection
    float aspectRatio = (float) windowWidth / (float) windowHeight;
    
    gluPerspective(30, aspectRatio, 1, 1000.0f);

    //since object is at the origin, looks 30steps ahead
    gluLookAt(camPos.x, camPos.y, camPos.z, 0, 0, 30, 0, 1, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * Applies transformation to room as "Rusko" moves around
 */
void drawWorld()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity();
    
    glRotated(worldAngle, 0, 1, 0);  //rotates world with given angle
    glTranslatef(worldPos.x, worldPos.y, worldPos.z);  //translates world to new position
    
    drawRoom();
    LightSetup(); //applies transform to lighting as well
    glPopMatrix ();
}

/**
 * Applies necessary transformations to Rusko
 */
void drawRusko()
{
    //transform Rusko
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity();
    
    glScaled(1/maxx, 1/maxx, 1/maxx); //makes Rusko unit size
    
    glColor3f(1.0, 1.0, 1.0);
    if (flat) drawFlatObj();
    else drawSmoothObj();
    glPopMatrix ();
}

/**
 * Display callback function
 */
void DisplayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawWorld(); //transforms and draws the world as Rusko moves around
    drawRusko();  //transforms and draws Rusko
       
    glDisable(GL_LIGHTING);  //disables lighting
    
    ReshapeCallback(windowWidth, windowHeight);

    glutSwapBuffers();
}


void init(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}


/**
 * Jump function
 * Users catmull rom to simulate curve of jump
 */
void jump()
{
    int u_temp = (jumpTimer+10) %10;
    float u = float(u_temp)/10 + .1f; //adds the .1f to make sure that we go through the last control point

    int i = jumpTimer/10;
    STPoint3 fu = cr->curveAt(u, i);

    //calculates where the world needs to be after a jump
    worldPos.y = -fu.y;
    worldPos.z -= (fu.z - lastJump.z)*cos(PI/180*worldAngle);
    worldPos.x += (fu.z - lastJump.z)*sin(PI/180*worldAngle);

    lastJump.z = fu.z; //makes sure to record lastJump.z position
    jumpTimer++;
    if (int(jumpTimer/10) >= numControlPoints-1) {
        jumpOn = false;
        lastJump.x = lastJump.y = lastJump.z = 0;
    }
}


/*** Functions to turn objects right/left, and move up/down ***/
void turnRight()
{
    worldAngle += 5;
}

void turnLeft()
{
    worldAngle -= 5;
}

void moveForward()
{
    worldPos.x += 1*sin(PI/180*worldAngle);
    worldPos.z -= 1*cos(PI/180*worldAngle);
}

void moveBack()
{
    worldPos.x -= 1*sin(PI/180*worldAngle);
    worldPos.z += 1*cos(PI/180*worldAngle);
}


/**
 * Timer function for moving forward/back, and turning
 */
static void Timer(int value)
{
    if (upKeyPressed) moveForward();
    if (downKeyPressed) moveBack();
    if (rightKeyPressed) turnRight();
    if (leftKeyPressed) turnLeft();
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 0); // 10 milliseconds
 }

/**
 * Timer function for jumping, it moves faster
 */
static void TimerJump(int value){
    if (jumpOn) jump();
    glutPostRedisplay();
    glutTimerFunc(10, TimerJump, 0); // 10 milliseconds

}

/**
 * Keyboard callback function
 */
void KeyboardCallback(unsigned char key, int x, int y)
{
    switch (key)
    {
        // exit program on escape press
        case 27:
            exit(0);
            break;
        case 'f':
            if (flat) flat = false;
            else flat = true;
            glutPostRedisplay();
            break;
        case ' ':  //activates jumping
            if (jumpOn) jumpOn = false;
            else {
                jumpOn = true;
                jumpTimer = 0;
            }
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


/**
 * special cases for up/down/left/right arrows
 **/
void KeySpecial(int key, int x, int y)
{
    if (key == GLUT_KEY_RIGHT) { //turns to the right
        rightKeyPressed = true;
    } else if (key == GLUT_KEY_LEFT) {  //turns to the left
        leftKeyPressed = true;
    } else if (key == GLUT_KEY_UP){  //moves a step forward
        upKeyPressed = true;
    } else if (key == GLUT_KEY_DOWN){ //moves a step back
        downKeyPressed = true;
    }
    glutPostRedisplay();
}

/**
 * special cases for releasing up/down/left/right arrows
 **/
void KeySpecialUp(int key, int x, int y)
{
    if (key == GLUT_KEY_RIGHT){
        rightKeyPressed = false;
    } else if (key == GLUT_KEY_LEFT){
        leftKeyPressed = false;
    }if (key == GLUT_KEY_UP){
        upKeyPressed = false;
    } else if (key == GLUT_KEY_DOWN){
        downKeyPressed = false;
    }
}

/**
 * Get's bounding box of object, probably will change once start using openMesh
 **/
void getBoundingBox(){
    maxx = objMesh->maxx;
    minx = objMesh->minx;
    maxy = objMesh->maxy;
    miny = objMesh->miny;
    maxz = objMesh->maxz;
    minz = objMesh->minz;
}


/**
 * Does initial setup being going into main loop
 **/
void setup(string objFile, string splineFile){
    
    //Rusko's mesh
    obj* object = new obj(objFile);
    objMesh = object->getMesh();
    getBoundingBox();

    //Light properties
    flat = false;
    
    //World position
    worldPos.x = -roomWidth/2;
    worldPos.y = 0;
    worldPos.z = -8;
    worldAngle = 0;

    //Rusko position
    camPos.x = 0;
    camPos.y = 3;
    camPos.z = -15;

    //Jump stuff
    jumpOn = false;
    jumpTimer = 0;
    lastJump.x = lastJump.y = lastJump.z = 0;
    cr = new CatmullRom();
    cr->readFile(splineFile);
    numControlPoints = cr->numControlPoints;
    
    //animation stuff
    upKeyPressed = downKeyPressed = false;
}



void usage()
{
	printf("usage: objFile jumpSplinePoints\n");
	exit(0);
}


/**
 * Program entry point
 */
int main(int argc, char* argv[])
{
    if (argc < 3) usage();
    string objFile = string(argv[1]);
    string splineFile = string(argv[2]);
    setup(objFile, splineFile);

    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition(10, 10);  //where the pop up window appears relative to monitor
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Rusko's Adventures");
    init();
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    
    glutTimerFunc(100, Timer, 0); //timer for moving up/down/turning
    glutTimerFunc(10, TimerJump, 0); //timer for jumping
    
    glutKeyboardFunc(KeyboardCallback);
    glutSpecialFunc(KeySpecial);
    glutSpecialUpFunc(KeySpecialUp);
    
    // enter the GLUT main loop
    glutMainLoop();

    return 0;
}