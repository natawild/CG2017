#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Passar para um .h
typedef struct sPonto3D {
	float x;
	float y;
	float z;
} *Ponto3D;

struct sPonto3D setPoint (float x, float y, float z)
{
	struct sPonto3D here;
	here.x = x;
	here.y = y;
	here.z = z;
	return here;
}

Ponto3D _buffer_pontos = NULL;
int _buffer_size = 0;

// angle of rotation for the camera direction
float angleX=0.0, angleY=0.0;
// actual vector representing the camera's direction
float lx=0.0f, ly=0.0f, lz=-1.0f;
// XZ position of the camera
float xcam=0.0f, ycam=0.5f, zcam=5.0f;
// Gestão da imagem
int _polygon_mode = GL_LINE;

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

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT : //straff arrasta a camara para a esquerda 
			xcam = (xcam - 0.5);
			break;
		case GLUT_KEY_RIGHT : //arrasta a camara para a direita
			xcam = (xcam + 0.5);
			break;
		case GLUT_KEY_UP : //zoom in 
			xcam += lx * fraction;
			zcam += lz * fraction;
			break;
		case GLUT_KEY_DOWN : // zoom out 
			xcam -= lx * fraction;
			zcam -= lz * fraction;
			break;
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	
	float fraction = 0.1f;

	switch (key) {
		case 'd' : // muda o angulo para onde estamos a olhar, não muda a camara de sitio 
			angleX += 0.01f;
			lx = sin(angleX);
			lz = -cos(angleX);
			break;
		case 'a' :
			angleX -= 0.01f;
			lx = sin(angleX);
			lz = -cos(angleX);
			break;
		case 'w' :
			angleY += 0.01f;
			ly = sin(angleY);
			break;
		case 's' :
			angleY -= 0.01f;
			ly = sin(angleY);
			break;
		case 'q' : //arrasta a camara para cima
			ycam = (ycam + 0.5);
			break;
		case 'e' : //arrasta a camara para baixo 
			ycam = (ycam - 0.5);
			break;
		case 'f' :
			_polygon_mode = GL_FILL;
			break;
		case 'g' :
			_polygon_mode = GL_LINE;
			break;
		case 'h' :
			_polygon_mode = GL_POINT;
			break;
	}
	glutPostRedisplay();
}



void renderScene(void) {

	int i;
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(xcam, ycam, zcam,
			xcam+lx, ycam+ly,  zcam+lz,
			0.0f, 1.0f,  0.0f);

	// put drawing instructions here

	glPolygonMode(GL_FRONT_AND_BACK, _polygon_mode);

	/*for (auto it = pontos.begin(); it != pontos.end(); ++it) {
		glVertex3f(it->x, it->y, it->z);
	}
	int fy = 1;
	for (size_t j = 0; j < primitivas.size(); j++) {
		glPushMatrix();
		Transformacao t = primitivas[j].getTransformacao();

		glRotatef(t.getRotacao().getAngulo(), t.getRotacao().geteixoX(), t.getRotacao().geteixoY(), t.getRotacao().geteixoZ());
		glTranslatef(t.getTranslacao().getTransx(), t.getTranslacao().getTransy(), t.getTranslacao().getTransz());
		glScalef(t.getEscala().getX(), primitivas[j].getTransformacao().getEscala().getY(), primitivas[j].getTransformacao().getEscala().getZ());
		pontos.clear();
		pontos = primitivas[j].getPontos();
		glBegin(GL_TRIANGLES);
		if (fy) { glColor3f(1.0f, 1.0f, 0.0f); fy = 0; }
		else glColor3f(0.0f, 0.9f, 1.0f);
		for (size_t i = 0; i < pontos.size(); i++)
			glVertex3f(pontos[i].getX(), pontos[i].getY(), pontos[i].getZ());
*/
	glEnd();

	// End of frame
	glutSwapBuffers();
}

void straux(char* in, char* out)
{
	for(int i = 1; i < strlen(in)-1; i++) {
		out[i-1] = in[i];
	}
}

int main(int argc, char **argv) {
	
	bool flagOut = false;
	float sX, sY, sZ;
	int i = 0, j = 0, verts, k = 0, c = 0;
	char tmp[1024];
	char nm[100];
	char files[100][100];
	FILE* xml = NULL;
	FILE* f = NULL;
	struct sPonto3D pt;


	if(xml = fopen(argv[1],"r"))
	{
		//printf("open\n");
		if(!fscanf(xml,"<scene>\n")){
			while(!flagOut && (feof(xml) == 0)) {
				if(fscanf(xml,"<model file=%s />\n",tmp) == 1) {
					sscanf(tmp,"\"%[^\n\t\"]\"",nm);
					strcpy(files[i],nm);
					//printf("%s %d %s\n",tmp,i,files[i]);
					i++;
				} else
					flagOut = true;
			}
			fclose(xml);
		} else {
			perror("read file error");
		}
	}
	//le os pontos e passa para um array de pontos global 
	while(j < i) {
		if(f = fopen(files[j],"r")) {
			if(fscanf(f,"%d",&verts)) {
				if (_buffer_pontos = (Ponto3D) realloc(_buffer_pontos, (_buffer_size + verts) * sizeof(struct sPonto3D))) {
					k = _buffer_size;
					_buffer_size += verts;
					for(;(k < _buffer_size) && (fscanf(f,"%f; %f; %f\n",&sX,&sY,&sZ));k++)
					{
						pt = setPoint(sX,sY,sZ);
						_buffer_pontos[k] = pt;
					}
				} else {
					perror("buffer reallocating error");
					return -1;
				}
			} else {
				perror("file damaged, forcing termination");
				return -1;
			}
			fclose(f);
		} else {
			perror("cannot open file");
		}
		j++;
	}

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);

	//int menu = glutCreateMenu(menuDrawing);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddMenuEntry("FILL", 0);
	glutAddMenuEntry("LINE", 1);
	glutAddMenuEntry("POINT", 2);
	// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// enter GLUT's main loop
	glutMainLoop();

	return 1;
}

