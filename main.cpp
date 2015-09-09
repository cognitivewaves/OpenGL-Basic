#include "GL/freeglut.h"

#include <stdio.h>
#include <math.h>   // fabs

static int slices = 16;
static int stacks = 16;

static double zoom = .01;
static int width = 0;
static int height = 0;
static float thetax=0, thetay=0, thetaz=0;

GLUquadric* pOrigin;
GLUquadric* pAxisX;
GLUquadric* pArrowX;
GLUquadric* pAxisY;
GLUquadric* pArrowY;
GLUquadric* pAxisZ;
GLUquadric* pArrowZ;

bool double_equal(double a, double b)
{
    if (fabs(a-b) < 1e-3)
        return true;
    return false;
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'Q':
    case 'q': glutLeaveMainLoop () ;      break;

    case '=':
    case '+':
        zoom=zoom-0.001; 
        if (double_equal(zoom, 0))
           zoom = 0.001;
        break;

    case '-':
    case '_': 
        zoom = zoom+0.001;
        if (double_equal(zoom, 0))
            zoom = 0.001;
        break;

    case ' ':
        zoom = 0.01;
        thetax=0;
        thetay=0;
        thetaz=0;
        printf("%g zoom minus\n", zoom); 
        break;

    case 'x':
        thetax -= 10;
        break;
    case 'X':
        thetax += 10;
        break;

    case 'y':
        thetay -= 10;
        break;
    case 'Y':
        thetay += 10;
        break;

    case 'z':
        thetaz -= 10;
        break;
    case 'Z':
        thetaz += 10;
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

static void resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, width, height);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho((-width/2)*zoom, (width/2)*zoom, (-height/2)*zoom, (height/2)*zoom, -10, 10);

    gluLookAt(0, 0, 1,  0, 0, 0,  0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(thetax, 1, 0, 0);
    glRotatef(thetay, 0, 1, 0);
    glRotatef(thetaz, 0, 0, 1);

    // origin
    glColor4d(1, 1, 1, 0.5);
    gluSphere(pOrigin, 0.1, slices, stacks);

    // x-axis
    glColor3d(1, 0, 0);
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    gluCylinder(pAxisX, 0.05f, 0.05f, 1.0f, slices, stacks);
    glPopMatrix();

    // x-axis arrow
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, 1);
    gluCylinder(pArrowX, 0.1f,  0.0f,  0.2f, slices, stacks);
    glPopMatrix();

    // y-axis
    glColor3d(0, 1, 0);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(pAxisX, 0.05f, 0.05f, 1.0f, slices, stacks);
    glPopMatrix();

    // y-axis arrow
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, 1);
    gluCylinder(pArrowX, 0.1f,  0.0f,  0.2f, slices, stacks);
    glPopMatrix();

    // z-axis 
    glColor3d(0, 0, 1);
    glPushMatrix();
    gluCylinder(pAxisZ, 0.05f, 0.05f, 1.0f, slices, stacks);
    glPopMatrix();

    // z-axis arrow
    glPushMatrix();
    glTranslatef(0, 0, 1);
    gluCylinder(pArrowZ, 0.1f,  0.0f,  0.2f, slices, stacks);
    glPopMatrix();

    glutSwapBuffers();
}

void AppInit()
{
    pOrigin = gluNewQuadric();

    pAxisX = gluNewQuadric();
    pArrowX = gluNewQuadric();

    pAxisY = gluNewQuadric();
    pArrowY = gluNewQuadric();

    pAxisZ = gluNewQuadric();
    pArrowZ = gluNewQuadric();
}

void AppCleanup()
{
    gluDeleteQuadric(pOrigin);

    gluDeleteQuadric(pAxisX);
    gluDeleteQuadric(pArrowX);

    gluDeleteQuadric(pAxisY);
    gluDeleteQuadric(pArrowY);

    gluDeleteQuadric(pAxisZ);
    gluDeleteQuadric(pArrowZ);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(40,40);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutCreateWindow("Basic OpenGL using FreeGLUT");

    AppInit();

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    glClearColor(0,0,0,.5);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST); // z-buffer test
    //glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);

    const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    const GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();

    AppCleanup();

	return 0;
}

