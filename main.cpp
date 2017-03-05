#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>

typedef struct sPonto3D {
	float x;
	float y;
	float z;
} *Ponto3D;

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

void drawSnowMan() {

	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body

	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);
}
/*
void drawBox(float xx, float yy, float zz, int divisions)
{
	float pontosX[10], pontosY[10], pontosZ[10];
	float divX = xx / (divisions + 1), divY = yy / (divisions + 1), divZ = zz / (divisions + 1);

	if(divisions >= 10)
	{
		pontosX = realloc((divisions + 2) * sizeof(float));
		pontosY = realloc((divisions + 2) * sizeof(float));
		pontosZ = realloc((divisions + 2) * sizeof(float));
	}

	pontosX[0] = (-1) * (xx / 2);
	pontosY[0] = (-1) * (yy / 2);
	pontosZ[0] = (-1) * (zz / 2);

	for(int i = 1; i < (divisions + 2); i++)
	{
		pontosX[i] = pontosX[i - 1] + divX;
		pontosY[i] = pontosY[i - 1] + divY;
		pontosZ[i] = pontosZ[i - 1] + divZ;
	}
	glBegin(GL_TRIANGLES);
}
*/

// So desenha planos paralelos aos planos cartesianos
// Um dos tamanhos tem de ser 0;
void drawPlane(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions, bool rev)
{
	int i, j, *k = NULL, n = divisions + 2;
	float ptsX[n], ptsY[n], ptsZ[n];
	float divX = sizeX / (float) (n - 1), divY = sizeY / (float) (n - 1), divZ = sizeZ / (float) (n - 1);
	ptsX[0] = ((-1.0f) * sizeX / 2.0f) + centerX, ptsY[0] = ((-1.0f) * sizeY / 2.0f) + centerY, ptsZ[0] = ((-1.0f) * sizeZ / 2.0f) + centerZ;

	if(!sizeY)
		k = &j;
	else
		k = &i;
	for(i = 1;i < n;i++)
	{
		ptsX[i] = ptsX[i-1] + divX;
		ptsY[i] = ptsY[i-1] + divY;
		ptsZ[i] = ptsZ[i-1] + divZ;
	}
	if(!rev) {
		glBegin(GL_TRIANGLES);
		for(j = 0;j < (n - 1); j++)
			for(i = 0;i < (n - 1);i++) {
				glVertex3f(ptsX[i],ptsY[j],ptsZ[*k+1]);
				glVertex3f(ptsX[i+1],ptsY[j],ptsZ[*k]);
				glVertex3f(ptsX[i],ptsY[j+1],ptsZ[*k]);
				glVertex3f(ptsX[i],ptsY[j+1],ptsZ[*k+1]);
				glVertex3f(ptsX[i+1],ptsY[j],ptsZ[*k+1]);
				glVertex3f(ptsX[i+1],ptsY[j+1],ptsZ[*k]);
			}
		glEnd();
	} else {
		glBegin(GL_TRIANGLES);
		for(j = 0;j < (n - 1); j++)
			for(i = 0;i < (n - 1);i++) {
				glVertex3f(ptsX[i+1],ptsY[j+1],ptsZ[*k]);
				glVertex3f(ptsX[i+1],ptsY[j],ptsZ[*k+1]);
				glVertex3f(ptsX[i],ptsY[j+1],ptsZ[*k+1]);
				glVertex3f(ptsX[i],ptsY[j+1],ptsZ[*k]);
				glVertex3f(ptsX[i+1],ptsY[j],ptsZ[*k]);
				glVertex3f(ptsX[i],ptsY[j],ptsZ[*k+1]);
			}
		glEnd();	
	}
}

void drawBox(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions)
{
	int res = 0;
	// Plano de Cima e Baixo:
	drawPlane(sizeX, 0, sizeZ, centerX, centerY + (sizeY / 2.0f), centerZ, divisions, 0);
	drawPlane(sizeX, 0, sizeZ, centerX, centerY - (sizeY / 2.0f), centerZ, divisions, 1);

	// Plano de "Esquerda e Direita":
	drawPlane(0, sizeY, sizeZ, centerX + (sizeX / 2.0f), centerY, centerZ, divisions, 0);
	drawPlane(0, sizeY, sizeZ, centerX - (sizeX / 2.0f), centerY, centerZ, divisions, 1);

	//Plano de Frente e Trás:
	drawPlane(sizeX, sizeY, 0, centerX, centerY, centerZ + (sizeZ / 2.0f), divisions, 0);
	drawPlane(sizeX, sizeY, 0, centerX, centerY, centerZ - (sizeZ / 2.0f), divisions, 1);

}

// MINIMO SLICES = 3
void drawCircle (float radius, float centerX, float centerY, float centerZ, int slices, int rev)
{
	int i;
	float alpha = 0.0f, div = 360 / (float) slices;
	float ptsX[slices + 1], ptsZ[slices + 1];

	for(i = 0; i <= slices; i++)
	{
		ptsX[i] = (radius * cos(alpha)) + centerX;
		ptsZ[i] = (radius * sin(alpha)) + centerZ;
		alpha += (div * M_PI / (float) 180);
	}

	glBegin(GL_TRIANGLES);
	for(i = 0; i < slices; i++)
	{
		glVertex3f(centerX, centerY, centerZ);
		glVertex3f(ptsX[i+1], centerY, ptsZ[i+1]);
		glVertex3f(ptsX[i], centerY, ptsZ[i]);
	}
	glEnd();
}

/* Stacks minimum is 1, slices minimum is 2 (triangle),*/
void drawCone(float height, float radius, float centerX, float centerY, float centerZ, int slices, int stacks)
{
	//float toDel;
	float sinAlpha, betas[slices+1], divBeta;
	float d, stHeightUp, stHeightDown, radUp, radDown;
	int i, j;

	sinAlpha = radius / height;
	divBeta = (360.0f / (float) slices) * M_PI / 180.0f;
	d = height / (float) stacks;

	// array com os angulos todos das circunferencias;
	betas[0] = 0;
	betas[slices] = 0;
	for(i = 1;i < slices;i++)
		betas[i] = betas[i-1] + divBeta;

	// Triangulos do bico do cone
	stHeightDown = height - d;		// altura da circunferencia de baixo;
	radDown = sinAlpha * d;
	glBegin(GL_TRIANGLES);
	for(i = 0;i < slices; i++)
	{
		//toDel = (float) i / (float) slices;
		//glColor3f(toDel,toDel,toDel);
		glVertex3f(centerX,centerY + height,centerZ);
		glVertex3f((radDown * cos(betas[i+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i+1])) + centerZ);
		glVertex3f((radDown * cos(betas[i])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i])) + centerZ);
	}
	glEnd();

	// Stacks até à base.
	glBegin(GL_TRIANGLES);
	for(i = 2;i <= stacks;i++)
	{
		stHeightUp = stHeightDown;
		radUp = radDown;
		// If apenas no caso de ser a base da stack de baixo, sendo assim, fica coms os valores originais;
		if(i == stacks) {
			radDown = radius;
			stHeightDown = 0;
		}
		else {
			stHeightDown -= d;
			radDown = sinAlpha * (i * d);
		}
		for(j = 0;j < slices;j++)
		{

			//toDel = (float) (j + i) / (float) (slices + stacks);
			//glColor3f(toDel,toDel,toDel);
			glVertex3f((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
			glVertex3f((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			glVertex3f((radDown * cos(betas[j])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j])) + centerZ);
			glVertex3f((radUp * cos(betas[j+1])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j+1])) + centerZ);
			glVertex3f((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			glVertex3f((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
		}
	}
	glEnd();


	// Circulo da base:
	glBegin(GL_TRIANGLES);
	for(i = 0; i < slices; i++)
	{
		glVertex3f(centerX, centerY, centerZ);
		glVertex3f((radius * cos(betas[i])) + centerX, centerY, (radius * sin(betas[i])) + centerZ);
		glVertex3f((radius * cos(betas[i+1])) + centerX, centerY, (radius * sin(betas[i+1])) + centerZ);
	}
	glEnd();

	// Tambem se podia usar a função drawCircle mas acabei por reparar que a função não consegue ser aproveitada para a esfera e serve so para a base do cone. (pode ser apagada);
}

void drawSphere(float radius, float centerX, float centerY, float centerZ, int stacks, int slices)
{
	float toDel;
	float alphas[stacks+1], divAlpha, betas[slices+1], divBeta;
	float stHeightUp, stHeightDown, radUp, radDown;
	int i, j;

	divBeta = (360.0f / (float) slices) * M_PI / 180.0f;
	divAlpha = M_PI / (float) stacks; 		// == (180.0f / (float) stacks) * M_PI / 180.0f;

	alphas[0] = M_PI / 2.0f;
	alphas[stacks] = - M_PI / 2.0f;
	betas[0] = 0;
	betas[slices] = 0;

	for(i = 1;i < slices;i++)
		betas[i] = betas[i-1] + divBeta;
	for(i = 1;i < stacks;i++)
		alphas[i] = alphas[i-1] - divAlpha;

	// Triangulos do topo e base da esfera
	stHeightDown = radius * sin(alphas[1]);
	stHeightUp = radius * sin(alphas[stacks - 1]);		// tambem podia ser stHeightDown = stHeightUp; Mas fica assim por razao do algoritmo.
	radDown = radius * cos(alphas[1]);
	radUp = radius * cos(alphas[stacks - 1]);			// tambem aqui podia ser radDown = radUp;

	glBegin(GL_TRIANGLES);
	for(i = 0;i < slices; i++)
	{
		glVertex3f(centerX, centerY + radius, centerZ);
		glVertex3f((radDown * cos(betas[i+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i+1])) + centerZ);
		glVertex3f((radDown * cos(betas[i])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i])) + centerZ);

		glVertex3f(centerX,centerY - radius,centerZ);
		glVertex3f((radUp * cos(betas[i])) + centerX, centerY + stHeightUp, (radUp * sin(betas[i])) + centerZ);
		glVertex3f((radUp * cos(betas[i+1])) + centerX, centerY + stHeightUp, (radUp * sin(betas[i+1])) + centerZ);
	}
	glEnd();

	// resto da esfera
	glBegin(GL_TRIANGLES);
	for(i = 2;i <= stacks;i++)
	{
		stHeightUp = stHeightDown;
		radUp = radDown;
		// If apenas no caso de ser a base da stack de baixo, sendo assim, fica coms os valores originais;
		stHeightDown = radius * sin(alphas[i]);
		radDown = radius * cos(alphas[i]);
		
		for(j = 0;j < slices;j++)
		{

			toDel = (float) (j + i) / (float) (slices + stacks);
			glColor3f(toDel,toDel,toDel);
			glVertex3f((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
			glVertex3f((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			glVertex3f((radDown * cos(betas[j])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j])) + centerZ);
			glVertex3f((radUp * cos(betas[j+1])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j+1])) + centerZ);
			glVertex3f((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			glVertex3f((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
		}
	}
	glEnd();
}


float size = 0.5f;
// angle of rotation for the camera direction
float angleX=0.0, angleY=0.0;
// actual vector representing the camera's direction
float lx=0.0f, ly=0.0f, lz=-1.0f;
// XZ position of the camera
float xcam=0.0f, ycam=0.5f, zcam=5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT :
			xcam = (xcam - 0.5);
			break;
		case GLUT_KEY_RIGHT :
			xcam = (xcam + 0.5);
			break;
		case GLUT_KEY_UP :
			xcam += lx * fraction;
			zcam += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			xcam -= lx * fraction;
			zcam -= lz * fraction;
			break;
	}
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	
	float fraction = 0.1f;

	switch (key) {
		case 'd' :
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
		case 'q' :
			ycam = (ycam + 0.5);
			break;
		case 'e' :
			ycam = (ycam - 0.5);
			break;
	}
}

void computePos(float deltaMove) {

	xcam += deltaMove * lx * 0.1f;
	zcam += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	angleX += deltaAngle;
	lx = sin(angleX);
	lz = -cos(angleX);
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(xcam, ycam, zcam,
			xcam+lx, ycam+ly,  zcam+lz,
			0.0f, 1.0f,  0.0f);

	// put drawing instructions here
	//drawSnowMan();

	//glColor3f(0,0,0.5);
	//glutWireTeapot(3*size);
	/*glBegin(GL_TRIANGLES);
		glColor3f(0,0,0.5);
		glVertex3f(1,0,0);
		glVertex3f(-1,0,0);
		glVertex3f(0,-2,0);
	glEnd();*/
	//glTranslatef(0,1,0);
	//drawCircle(1.5, 0, 0,0,20,0);
	//drawCone(2,1,0,0,0,10,10);
	drawSphere(1,0,0,0,100,100);
	//glTranslatef(0,-1,0);
	//drawPlane(2,0.5,0,0,0,0,4,0);

	//size += 0.001;

	// End of frame
	glutSwapBuffers();
}


int main(int argc, char **argv) {

// put GLUT init here
glutInit(&argc,argv);
glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
glutInitWindowPosition(100,100);
glutInitWindowSize(800,800);
glutCreateWindow("CG@DI");



// put callback registration here
glutDisplayFunc(renderScene);
glutReshapeFunc(changeSize);
glutIdleFunc(renderScene);
//glutSpecialFunc(processSpecialKeys);
glutSpecialFunc(processSpecialKeys);
glutKeyboardFunc(processNormalKeys);




// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glPolygonMode(GL_FRONT, GL_FILL);

// enter GLUT's main loop
	glutMainLoop();
	
	return 1;
}

/* PARTE DO PROJECTO - IMPRESSÃO PARA FICHEIRO */

int drawPlane_Points(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions, bool rev, Ponto3D pontos)
{
	int i, j, *k = NULL, n = divisions + 2, ind = 0;
	float ptsX[n], ptsY[n], ptsZ[n];
	float divX = sizeX / (float) (n - 1), divY = sizeY / (float) (n - 1), divZ = sizeZ / (float) (n - 1);
	ptsX[0] = ((-1.0f) * sizeX / 2.0f) + centerX, ptsY[0] = ((-1.0f) * sizeY / 2.0f) + centerY, ptsZ[0] = ((-1.0f) * sizeZ / 2.0f) + centerZ;

	if(!sizeY)
		k = &j;
	else
		k = &i;
	for(i = 1;i < n;i++)
	{
		ptsX[i] = ptsX[i-1] + divX;
		ptsY[i] = ptsY[i-1] + divY;
		ptsZ[i] = ptsZ[i-1] + divZ;
	}
	if(!rev) {
		//glBegin(GL_TRIANGLES);
		for(j = 0;j < (n - 1); j++)
			for(i = 0;i < (n - 1);i++) {
				pontos[ind+0].x = ptsX[i];
				pontos[ind+0].y = ptsY[j];
				pontos[ind+0].z = ptsZ[*k+1];
				pontos[ind+1].x = ptsX[i+1];
				pontos[ind+1].y = ptsY[j];
				pontos[ind+1].z = ptsZ[*k];
				pontos[ind+2].x = ptsX[i];
				pontos[ind+2].y = ptsY[j+1];
				pontos[ind+2].z = ptsZ[*k];
				pontos[ind+3].x = ptsX[i];
				pontos[ind+3].y = ptsY[j+1];
				pontos[ind+3].z = ptsZ[*k+1];
				pontos[ind+4].x = ptsX[i+1];
				pontos[ind+4].y = ptsY[j];
				pontos[ind+4].z = ptsZ[*k+1];
				pontos[ind+5].x = ptsX[i+1];
				pontos[ind+5].y = ptsY[j+1];
				pontos[ind+5].z = ptsZ[*k];
				ind += 6;
			}
		//glEnd();
	} else {
		//glBegin(GL_TRIANGLES);
		for(j = 0;j < (n - 1); j++)
			for(i = 0;i < (n - 1);i++) {
				pontos[ind+0].x = ptsX[i+1];
				pontos[ind+0].y = ptsY[j+1];
				pontos[ind+0].z = ptsZ[*k];
				pontos[ind+1].x = ptsX[i+1];
				pontos[ind+1].y = ptsY[j];
				pontos[ind+1].z = ptsZ[*k+1]; 
				pontos[ind+2].x = ptsX[i];
				pontos[ind+2].y = ptsY[j+1];
				pontos[ind+2].z = ptsZ[*k+1];
				pontos[ind+3].x = ptsX[i];
				pontos[ind+3].y = ptsY[j+1];
				pontos[ind+3].z = ptsZ[*k];
				pontos[ind+4].x = ptsX[i+1];
				pontos[ind+4].y = ptsY[j];
				pontos[ind+4].z = ptsZ[*k];
				pontos[ind+5].x = ptsX[i];
				pontos[ind+5].y = ptsY[j];
				pontos[ind+5].z = ptsZ[*k+1];
				ind += 6;
			}
		//glEnd();	
	}
	return ind;
}

int drawBox_Points(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions, Ponto3D pontos)
{
	int res = 0;
	// Plano de Cima e Baixo:
	res += drawPlane_Points(sizeX, 0, sizeZ, centerX, centerY + (sizeY / 2.0f), centerZ, divisions, 0, pontos+res);
	res += drawPlane_Points(sizeX, 0, sizeZ, centerX, centerY - (sizeY / 2.0f), centerZ, divisions, 1, pontos+res);

	// Plano de "Esquerda e Direita":
	res += drawPlane_Points(0, sizeY, sizeZ, centerX + (sizeX / 2.0f), centerY, centerZ, divisions, 0, pontos+res);
	res += drawPlane_Points(0, sizeY, sizeZ, centerX - (sizeX / 2.0f), centerY, centerZ, divisions, 1, pontos+res);

	//Plano de Frente e Trás:
	res += drawPlane_Points(sizeX, sizeY, 0, centerX, centerY, centerZ + (sizeZ / 2.0f), divisions, 0, pontos+res);
	res += drawPlane_Points(sizeX, sizeY, 0, centerX, centerY, centerZ - (sizeZ / 2.0f), divisions, 1, pontos+res);

	return res;
}

/**
int main(int argc, char **argv)
{
	sPonto3D buffer[1000];
	int r;

	r = drawBox_Points(2,2,2,0,0,0,0,&buffer[0]);
	for(int j = 0;j < r; j++)
		printf("%2.5f; %2.5f; %2.5f\n",buffer[j].x,buffer[j].y,buffer[j].z);

	return 0;
}
*/