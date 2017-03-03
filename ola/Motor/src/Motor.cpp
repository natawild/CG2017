#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "headers\tinyxml2.h"
#include "..\..\utils\CoordsEsfericas.h"
#include "..\..\utils\Ponto3D.h"

vector<Primitiva> primitivas;

using namespace tinyxml2;
using namespace std;

GLenum modoPoligonos;

void f_special_keys(int key, int x, int y);
void f_teclas_normais(unsigned char key, int x, int y);
void changeSize(int w, int h);
void renderScene(void);
vector<const char *> leXML();
void le3D(const char *);
void menuDrawing(int opt);

vector<Ponto3D> pontos;
CoordsEsfericas camara;

int main(int argc, char **argv) {
	modoPoligonos = GL_LINE;
	vector<const char *> ficheiros = leXML();
	camara = CoordsEsfericas(10.0, 0.0, M_PI / 3.0f);
	for (auto it = ficheiros.begin(); it != ficheiros.end(); ++it) {
		std::cout << "Ficheiro encontrado: " << *it << std::endl;
		le3D(*it);
	}

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutSpecialFunc(f_special_keys);
	glutKeyboardFunc(f_teclas_normais);

	int menu = glutCreateMenu(menuDrawing);
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
	gluLookAt(camara.cCartesianas.x, camara.cCartesianas.y, camara.cCartesianas.z,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// put drawing instructions here

	glPolygonMode(GL_FRONT_AND_BACK, modoPoligonos);

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1.0);
	for (auto it = pontos.begin(); it != pontos.end(); ++it) {
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

	glEnd();

	// End of frame
	glutSwapBuffers();
}


void parseGrupo(XMLElement* grupo, Transformacao transf) {
	Transformacao trans;
	Translacao tr;
	Rotacao ro;
	Escala es = Escala::Escala(1, 1, 1);
	float ang1, rotX, rotY, rotZ, transX, transY, transZ, escX, escY, escZ;
	ang1 = rotX = rotY = rotZ = transX = transY = transZ = escX = escY = escZ = 1;

	if (strcmp(grupo->FirstChildElement()->Value(), "grupo") == 0)
		grupo = grupo->FirstChildElement();

	//transformações para um grupo
	XMLElement* transformacao = grupo->FirstChildElement();

	for (transformacao; (strcmp(transformacao->Value(), "modelos") != 0); transformacao = transformacao->NextSiblingElement()) {
		if (strcmp(transformacao->Value(), "translacao") == 0) {
			if (transformacao->Attribute("X")) transX = stof(transformacao->Attribute("X"));
			else transX = 0;
			if (transformacao->Attribute("Y")) transY = stof(transformacao->Attribute("Y"));
			else transY = 0;
			if (transformacao->Attribute("Z")) transZ = stof(transformacao->Attribute("Z"));
			else transZ = 0;
			tr = Translacao::Translacao(transX, transY, transZ);
		}
		if (strcmp(transformacao->Value(), "rotacao") == 0) {
			if (transformacao->Attribute("angulo")) ang1 = stof(transformacao->Attribute("angulo"));
			else ang1 = 0;
			if (transformacao->Attribute("eixoX")) rotX = stof(transformacao->Attribute("eixoX"));
			else rotX = 0;
			if (transformacao->Attribute("eixoY")) rotY = stof(transformacao->Attribute("eixoY"));
			else rotY = 0;
			if (transformacao->Attribute("eixoZ")) rotZ = stof(transformacao->Attribute("eixoZ"));
			else rotZ = 0;
			ro = Rotacao::Rotacao(ang1, rotX, rotY, rotZ);
		}
		if (strcmp(transformacao->Value(), "escala") == 0) {
			if (transformacao->Attribute("X")) escX = stof(transformacao->Attribute("X"));
			else escX = 1;
			if (transformacao->Attribute("Y")) escY = stof(transformacao->Attribute("Y"));
			else escY = 1;
			if (transformacao->Attribute("Z")) escZ = stof(transformacao->Attribute("Z"));
			else escZ = 1;
			es.setX(escX);
			es.setY(escY);
			es.setZ(escZ);
		}
	}
	//Actualizacao dos valores em relação ao nodo pai
	tr.setTransx(tr.getTransx() + transf.getTranslacao().getTransx());
	tr.setTransy(tr.getTransy() + transf.getTranslacao().getTransy());
	tr.setTransz(tr.getTransz() + transf.getTranslacao().getTransz());
	ro.setAngulo(ro.getAngulo() + transf.getRotacao().getAngulo());
	ro.setEixoX(ro.geteixoX() + transf.getRotacao().geteixoX());
	ro.setEixoY(ro.geteixoY() + transf.getRotacao().geteixoY());
	ro.setEixoZ(ro.geteixoZ() + transf.getRotacao().geteixoZ());
	es.setX(es.getX() * transf.getEscala().getX());
	es.setY(es.getY() * transf.getEscala().getY());
	es.setZ(es.getZ() * transf.getEscala().getZ());
	trans = Transformacao::Transformacao(tr, ro, es);

	//para o mesmo grupo, quais os modelos(ficheiros) que recebem as transformações
	for (XMLElement* modelo = grupo->FirstChildElement("modelos")->FirstChildElement("modelo"); modelo; modelo = modelo->NextSiblingElement("modelo")) {

		Primitiva p;
		p.setNome(modelo->Attribute("ficheiro"));
		cout << p.getNome() << endl;
		readFile(p.getNome());
		p.setPontos(pontos);
		pontos.clear();

		p.setTransformacao(trans);

		cout << "Translacao: " << trans.getTranslacao().getTransx() << " - " << trans.getTranslacao().getTransy() << " - " << trans.getTranslacao().getTransz() << endl;
		cout << "Rotacao   : " << trans.getRotacao().getAngulo() << " - " << trans.getRotacao().geteixoX() << " - " << trans.getRotacao().geteixoY() << " - " << trans.getRotacao().geteixoZ() << endl;
		cout << "Escala    : " << trans.getEscala().getX() << " - " << trans.getEscala().getY() << " - " << trans.getEscala().getZ() << endl;

		primitivas.push_back(p);
	}

	//faz o mesmo de cima para grupos filhos
	if (grupo->FirstChildElement("grupo")) {
		parseGrupo(grupo->FirstChildElement("grupo"), trans);
	}

	//faz o mesmo de cima para grupos irmãos
	if (grupo->NextSiblingElement("grupo")) {
		parseGrupo(grupo->NextSiblingElement("grupo"), transf);
	}
}




vector<const char *> leXML() {
	vector<const char *> ficheiros;
	XMLDocument doc;
	XMLError erro = doc.LoadFile("Modelos/scene.xml");


	if (erro == XML_NO_ERROR) {
		XMLElement* modelElem = doc.FirstChildElement("scene")->FirstChildElement("model");
		while (modelElem != nullptr) {
			const char *fich = modelElem->FirstAttribute()->Value();
			ficheiros.push_back(fich);
			modelElem = modelElem->NextSiblingElement();
		}
	}
	else {
		std::cout << "Erro ao ler ficheiro" << std::endl;
	}

	return ficheiros;
}

void le3D(const char * ficheiro) {
	char linha[1024];
	float nums[3];
	std::vector<double> numeros;

	int i = 1;
	ifstream leitura(ficheiro);
	while (leitura.getline(linha, 1024)) {
		std::istringstream iss(linha);
		Ponto3D ponto;

		for (int n = 0; n < 3; n++) iss >> nums[n];
		ponto = { nums[0], nums[1], nums[2] };

		pontos.push_back(ponto);
	}

}

void f_special_keys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: camara.paraCima(2 * M_PI / 360.0);  break;
	case GLUT_KEY_DOWN: camara.paraBaixo(2 * M_PI / 360.0); break;
	case GLUT_KEY_LEFT: camara.paraEsquerda(2 * M_PI / 360.0);  break;
	case GLUT_KEY_RIGHT: camara.paraDireita(2 * M_PI / 360.0); break;
	}

	//DEBUG:
	//std::cout << "raio: " << camara.raio << " az: " << camara.azimuth_ang << " polar: " << camara.polar_ang << " | x:" << camara._x << " y: " << camara._y << " z: " << camara._z << std::endl;
	glutPostRedisplay();
}

void f_teclas_normais(unsigned char key, int x, int y) {
	switch (tolower(key)) {
	case 'w': camara.paraCima(2 * M_PI / 360.0);  break;
	case 's': camara.paraBaixo(2 * M_PI / 360.0); break;
	case 'a': camara.paraEsquerda(2 * M_PI / 360.0);  break;
	case 'd': camara.paraDireita(2 * M_PI / 360.0); break;
	case 'e': camara.aproximar(0.5); break;
	case 'q': camara.afastar(0.5); break;
	}

	//DEBUG:
	//std::cout << "raio: " << camara.raio << " az: " << camara.azimuth_ang << " polar: " << camara.polar_ang << " | x:" << camara._x << " y: " << camara._y << " z: " << camara._z << std::endl;
	glutPostRedisplay();
}

void menuDrawing(int opt) {
	switch (opt) {
	case 0: modoPoligonos = GL_FILL; break;
	case 1:  modoPoligonos = GL_LINE; break;
	case 2:  modoPoligonos = GL_POINT; break;
	}
	glutPostRedisplay();
}

