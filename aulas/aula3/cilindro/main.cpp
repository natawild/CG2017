#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

float px = 0, py = 5, pz = 5, ord = 0.5;
float x = 0, rot = 25, rotf = 0;
float r = 2, al = 3, alpha = 2, delta = 1;
int slices = 5; //quantidade de fatias 

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
	gluLookAt(px, py, pz,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);


	glRotatef(rot, 0, 1, 0);
	glTranslatef(0, 0, rotf);
	//gluLookAt(r, 0, 0, 0, 0,0, 0, 1, 0);

	// pôr instruções de desenho aqui

	float ang = (2 * 3, 14159 / slices), p, q = 0;


	for (int i = 0; i < slices; i++) {
		p = q;
		q += ang;

		glBegin(GL_TRIANGLES);
		// Base superior

		glColor3f(0.8, 0.5, 0.1); //meio castanho 
		glVertex3f(0, al, 0);
		glVertex3f(r*sin(p), al, r*cos(p));
		glVertex3f(r*sin(q), al, r*cos(q));

		// Base inferior

		glColor3f(1, 0, 1);
		glVertex3f(0, -al, 0);
		glVertex3f(r*sin(p), -al, r*cos(p));
		glVertex3f(r*sin(q), -al, r*cos(q));

		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(r*sin(p), al, r*cos(p));
		glVertex3f(r*sin(q), al, r*cos(q));
		glVertex3f(r*sin(q), -al, r*cos(q));

		glColor3f(0, 0, 1);
		glVertex3f(r*sin(p), -al, r*cos(p));
		glVertex3f(r*sin(q), -al, r*cos(q));
		glVertex3f(r*sin(p), al, r*cos(p));

		glEnd();
	}

	// End of frame
	glutSwapBuffers();
}

void mykeyEvents(unsigned char key, int x, int y) {
	switch (key) {

	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}

};


void teclas(unsigned char tecla, int x, int y) {

	switch (tecla) {
	case 'w':
		rotf += 5.5;
		break;

	case 's':
		rotf -= 1.25;
		break;
	case 'd':
		rot += 5.6;
		break;
	case 'a':
		rot -= 5;
		break;
	default:
		break;
	}

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



void main(int argc, char **argv) {

	// pôr inicialização aqui
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow(" O Cilindro");

	// pôr registo de funções aqui

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(teclas);
	glutKeyboardFunc(mykeyEvents);
	glutKeyboardFunc(keys); 
	

	// alguns settings para OpenGL

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// entrar no ciclo do GLUT aqui
	glutMainLoop();


}