#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

float px = 0, py = 5, pz = 5, ord = 0.5;
int menu;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(px,py,pz, //posicão da camara
		      0.0,0.0,0.0, // para onde estamos a olhar 
			  0.0f,1.0f,0.0f); // orientação da camara

// put the geometric transformations here


// put drawing instructions here

	glBegin(GL_TRIANGLES);

	// Front
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-1.0f, -1.0f, 1.0f);

	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(0.0f, 1.0f, 0.0f);
	
	glVertex3f(1.0f, -1.0f, 1.0f);
	
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Back
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, -1.0f);
	
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left
	glColor3f(1.0f, 0.9f, 0.1f);       // 
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-1.0f, -1.0f, -1.0f);
	
	glVertex3f(-1.0f, -1.0f, 1.0f);


	//desenhar a base 

	glColor3f(0.0f, 1.0f, 0.0f); //green

	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();
	
	
	// Done drawing the pyramid



	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events

void mykeyEvents(unsigned char key, int x, int y) {
	switch (key) {

	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}

};

void keys(int key_code, int x, int y) {
	switch (key_code) {
	case GLUT_KEY_LEFT:
		if (px>-5 && px <= 0 && pz >= 0 && pz <= 5) {
			px -= ord;
			pz -= ord;
		}
		else if (px >= -5 && px<0 && pz >= -5 && pz <= 0) {
			px += ord;
			pz -= ord;
		}
		else if (px >= 0 && px<5 && pz >= -5 && pz <= 0) {
			px += ord;
			pz += ord;
		}
		else if (px>0 && px <= 5 && pz >= 0 && pz <= 5) {
			px -= ord;
			pz += ord;
		}

		break;
	case GLUT_KEY_RIGHT:
		if (px >= 0 && px<5 && pz >= 0 && pz <= 5) {
			px += ord;
			pz -= ord;
		}
		else if (px>0 && px <= 5 && pz >= -5 && pz <= 0) {
			px -= ord;
			pz -= ord;
		}
		else if (px>-5 && px <= 0 && pz >= -5 && pz <= 0) {
			px -= ord;
			pz += ord;
		}
		else if (px >= -5 && px<0 && pz >= 0 && pz <= 5) {
			px += ord;
			pz += ord;
		}
		break;
	case GLUT_KEY_UP: py += ord; break;
	case GLUT_KEY_DOWN: py -= ord; break;

	}
	glutPostRedisplay();
}

void menu_op(int id_op) {
	switch (id_op) {
	case 1:
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	case 2:
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 3:
		glPolygonMode(GL_FRONT, GL_POINT);
		break;
	}
	glutPostRedisplay();
}





int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("A pirâmide");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(mykeyEvents);
	glutSpecialFunc(keys);


	menu = glutCreateMenu(menu_op);
	glutAddMenuEntry("FILL", 1);
	glutAddMenuEntry("LINE", 2);
	glutAddMenuEntry("POINT", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
