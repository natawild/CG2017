#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>


#define _USE_MATH_DEFINES
#include <math.h>

double i = 0;

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);

    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    glTranslatef(0.0,0.0,-3.0);
    glutSolidTeapot(0.5);
    glTranslatef(0.0,0.0,3.0);

    gluLookAt(0.0, 0.0, -7.0,
              0.0, 0.0, -1.0,
              0.0f, 1.0f, 0.0f);
    glTranslatef(0.0,0.0,-10.0);
    glutWireTeapot (fabs(sin (i)));
    i += .001;

    // put drawing instructions here

    //glutSolidTeapot(.5);


    // End of frame
    glutSwapBuffers();
}

int main(int argc, char **argv) {

    float i = 0;

    // put init here
    glutInit(&argc, argv);


    // put callback registration here

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("HelloTeapot");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.5f, 0.0f, 0.0f, 0.0f);

    // enter GLUT's main loop
    glutMainLoop();

    return 0;
}
