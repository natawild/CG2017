/**
 * point3d.cpp
 * Trabalho CG - Ponto 3D
 * 
 * @author (Pirata)
 * @date (2017.03.25)
 * @version (v2)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.


#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"

// Global Struture
Scene global_scene = sceneInit();


//float raio=180,raioAux=0,angCam_h=0,angCam_v=0.5,angAux_h=0,angAux_v=0,x_tela,y_tela,look[]={20,0,0},avanco=5;

float auxCH=0.0, auxCV=0.1, auxCR=180.0;
// angle of rotation for the camera direction
float angleX=0.0, angleY=0.0, angleZ=180.0;
// actual vector representing the camera's direction
float lx=0.0f, ly=0.0f, lz=20.0f;
// XZ position of the camera
float xcam=0.0f, ycam=0.0f, zcam=180.0f;
// Camara globals
//int _polygon_mode = GL_LINE;

//look[0]+(raio+raioAux)*sin(angCam_h+angAux_h)*cos(angCam_v+angAux_v)

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT :
			auxCH += fraction;
			xcam = lx + (auxCR * sin(auxCH) * cos(auxCV));
			zcam = lz + (auxCR * cos(auxCH) * cos(auxCV));
			break;
		case GLUT_KEY_RIGHT :
			auxCH -= fraction;
			xcam = lx + (auxCR * sin(auxCH) * cos(auxCV));
			zcam = lz + (auxCR * cos(auxCH) * cos(auxCV));
			break;
		case GLUT_KEY_UP :
			xcam += lx * fraction;
			ycam += ly * fraction;
			zcam += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			xcam -= lx * fraction;
			ycam -= ly * fraction;
			zcam -= lz * fraction;
			break;
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	
	float fraction = 0.1f;

	switch (key) {
		case 'd' :
			angleX += fraction;
			lx = sin(angleX);
			lz = -cos(angleX);
			break;
		case 'a' :
			angleX -= fraction;
			lx = sin(angleX);
			lz = -cos(angleX);
			break;
		case 'w' :
			if(angleY+fraction<90) {
				angleY += fraction;
				ly = sin(angleY);
			}
			break;
		case 's' :
			if(angleY-fraction>-90) {
				angleY -= fraction;
				ly = sin(angleY);
			}
			break;
		case 'q' :
			auxCV += fraction;
			ycam = ly + (auxCR * sin(auxCV));
			break;
		case 'e' :
			auxCV -= fraction;
			ycam = ly + (auxCR * sin(auxCV));
			break;
	}
	glutPostRedisplay();
}

void changeSize(int w, int h){
    
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

	// set the camera
	glLoadIdentity();
	gluLookAt(xcam, ycam, zcam,
				lx,	ly,	lz,
			0.0f, 1.0f,	0.0f);

	// put drawing instructions here

	/*
    if (tipo_camera==1)
        modo_explorador();
    else
        if (tipo_camera==2)
            modo_fps();
        else{
            gluLookAt(0,3,5,
                      0.0, 0.0, 0.0,
                      0.0f, 1.0f, 0.0f);
        }
    */
    glColor3f(0.0f,0.0f,0.5f);
	// put drawing instructions here
	
    drawGroup(global_scene->groups, global_scene);
    
	// End of frame
	glutSwapBuffers();
}

void front_menu(int op){
    switch (op) {
        case 1:
            glPolygonMode(GL_FRONT,GL_POINT);
            break;
        case 2:
            glPolygonMode(GL_FRONT,GL_LINE);
            break;
        case 3:
            glPolygonMode(GL_FRONT,GL_FILL);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
	
	if(argc != 2){
        perror("input data arguments number not compatible\n");
        return 1;
    }

    global_scene = sceneInit();

    //printf("scene: %p\n",global_scene);

	xml_read_engine(argv[1],global_scene);
	// só fez load do XML, só faz load dos modelos ao chama-los para draw - UMA VEZ APENAS

	printDebug(1,global_scene->groups);

	//printf("3 - depois ler XML Scene\n");

	printDebug(1,global_scene->groups);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CGDI");

	//printf("creating windows\n");

	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutCreateMenu(front_menu);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enter GLUT's main loop
	glutMainLoop();

	return 0;
}
