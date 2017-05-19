

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define ANG2RAD M_PI/180.0

#define COWBOYS 8
#define RAIO_COWBOYS 5
#define INDIOS 16
#define RAIO_INDIOS 25
#define ARVORES 1000
#define STEP_COWBOY 1.0f
#define STEP_INDIO 0.5f


float step = 0.0;
float resN[3]={0,1,0};
unsigned int t,texID, tw, th;
int n_pontos;

float height = 2.0f;
float x = 0.0f;
float z = 0.0f;


float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

unsigned int texture;
int imageWidth;
unsigned char *imageData;

// vectors to store vertex data temporarily
std::vector<float> position, normal, texCoord;
GLuint buffers[3];

void changeSize(int w, int h) {
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;
    
    // compute window's aspect ratio
    float ratio = w * 1.0 / h;
    
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set the correct perspective
    gluPerspective(45,ratio,1,1000);
    
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


float h(int i, int j) {
    
    return(imageData[i*imageWidth + j] * 0.2f);
}


float h2(float x, float z) {
    
    
    int x1, x2, z1, z2;
    float xGrid, zGrid;
    float fracX, fracZ, ha, hb;
    
    xGrid = x + imageWidth / 2;
    zGrid = z + imageWidth / 2;
    
    x1 = (int)floor(xGrid);
    x2 = x1 + 1;
    fracX = xGrid - x1;
    
    z1 = (int)floor(zGrid);
    z2 = z1 + 1;
    fracZ = zGrid - z1;
    
    ha = h(x2, z1) * fracX + (1 - fracX) * h(x1, z1);
    hb = h(x2, z2) * fracX + (1 - fracX) * h(x1, z2);
    
    return(hb * fracZ + (ha * (1 - fracZ)));
}

void computeNormalFeios(int i, int j) {
    
    // fill the normal vector with the normal for vertex at grid location (i,j)
    
}


void computeNormal(int i, int j){
    
    float v1[3],v2[3], modulo;
    v1[0]=v2[2]=0;
    v1[2]=v2[0]=2;
    
    if(i==0)
        i++;
    else if(i==tw)
        i--;
    
    if(j==0)
        j++;
    else if(j==tw)
        j--;
    
    v1[1]=h(i+1,j) - h(i-1, j);
    v2[1]=h(i,j+1) - h(i, j-1);
    
    
    //Vector Normal
    resN[0]= (v1[1] * v2[2]) - (v1[2] * v2[1]);
    resN[1]= (v1[2] * v2[0]) - (v1[0] * v2[2]);
    resN[2]= (v1[0] * v2[1]) - (v1[1] * v2[0]);
    
    //Modulo do vector
    modulo= sqrtf(powf(resN[0], 2) + powf(resN[1], 2)+ powf(resN[2], 2));
    
    //Vector Unit�rio
    resN[0]/=modulo;
    resN[1]/=modulo;
    resN[2]/=modulo;
    
}



void prepareTerrain() {
    int n=0;
    int f=0;
    
    for (int i = 1; i < imageWidth - 2; i++) {
        for (int j = 1; j < imageWidth - 1; j++) {
            
            computeNormal(i + 1, j);
            
            texCoord[n++]=resN[0];
            texCoord[n++]=resN[1];
            texCoord[n++]=resN[2];
            
            
            // fill texCoord vector with the texture coordinates for grid location (i+1,j)
            
            position.push_back(i - imageWidth*0.5f + 1);
            position.push_back(h(i + 1, j));
            position.push_back(j - imageWidth*0.5f);
            
            computeNormal(i, j);
            normal[f++]=resN[0];
            normal[f++]=resN[1];
            normal[f++]=resN[2];
            
            texCoord[n++]=resN[0];
            texCoord[n++]=resN[1];
            texCoord[n++]=resN[2];
    
            
            // fill texCoord vector with the texture coordinates for grid location (i,j)
            
            position.push_back(i - imageWidth*0.5f);
            position.push_back(h(i, j));
            position.push_back(j - imageWidth*0.5f);
        }
    }
     
    glGenBuffers(3, buffers);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), &(position[0]), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), &(normal[0]), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(float), &(texCoord[0]), GL_STATIC_DRAW);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
}


void renderTerrain() {
    
    float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glNormalPointer(GL_FLOAT, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
    
    for (int i = 1; i < imageWidth - 2; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, (imageWidth - 2) * 2 * i, (imageWidth - 2) * 2);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
}


void drawTree() {
    
    glPushMatrix();
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    float brown[4] = { 1.0f,1.0f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, brown);
    glutSolidCone(0.25f, 4, 5, 1);
    
    float green[4] = { 0.0f, 0.5f + rand() * 0.5f / RAND_MAX,0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    glTranslatef(0.0f, 0.0f, 2.0f);
    glutSolidCone(2.0f, 5.0f, 5.0f, 1.0f);
    glPopMatrix();
}


void placeTrees() {
    
    float r = 35.0;
    float alpha;
    float rr;
    float x,z;
    
    srand(31457);
    int arvores = 0;
    
    while (arvores < ARVORES) {
        
        rr = rand() * 150.0/ RAND_MAX;
        alpha = rand() * 6.28 / RAND_MAX;
        
        x = cos(alpha) * (rr + r);
        z = sin(alpha) * (rr + r);
        
        if (fabs(x) < 100 && fabs(z) < 100) {
            
            glPushMatrix();
            glTranslatef(x, h2(x, z),z);
            drawTree();
            glPopMatrix();
            arvores++;
        }
    }
}


void drawDonut() {
    
    float purple[4] = { 1.0f,0.0f,1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purple);
    glPushMatrix();
    glTranslatef(0.0,0.5,0.0);
    glutSolidTorus(0.5,1.25,8,16);
    glPopMatrix();
}


void drawIndios() {
    
    float angulo;
    float red[4] = { 1.0f,0.0f,0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    for (int i = 0; i < INDIOS; i++) {
        
        angulo = i * 360.0/INDIOS + step * STEP_INDIO;
        glPushMatrix();
        glRotatef(angulo,0.0,1.0,0.0);
        glTranslatef(0.0,0.0,RAIO_INDIOS);
        glutSolidTeapot(1);
        glPopMatrix();
    }
}


void drawCowboys() {
    
    float angulo;
    float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
    
    for (int i = 0; i < COWBOYS; i++) {
        
        angulo = i * 360.0/COWBOYS + step * STEP_COWBOY;
        glPushMatrix();
        glRotatef(-angulo,0.0,1.0,0.0);
        glTranslatef(RAIO_COWBOYS,0.0,0.0);
        glutSolidTeapot(1);
        glPopMatrix();
    }
}


void drawScene() {
    
    renderTerrain();
    placeTrees();
    drawDonut();
    glPushMatrix();
    // move teapots up so that they are placed on top of the ground plane
    glTranslatef(0.0,1.0,0.0);
    drawCowboys();
    drawIndios();
    glPopMatrix();
}


void renderScene(void) {
    
    float pos[4] = {-1.0, 1.0, 1.0, 0.0};
    
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);
    
    float lpos[4] = { 1,1,1,0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    
    drawScene();
    step++;
    
    // End of frame
    glutSwapBuffers();
}



// escrever fun��o de processamento do teclado

void processKeys(unsigned char key, int xx, int yy) {
    
}



void processMouseButtons(int button, int state, int xx, int yy) {
    
    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {
            
            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}


void processMouseMotion(int xx, int yy) {
    
    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;
    
    if (!tracking)
        return;
    
    deltaX = xx - startX;
    deltaY = yy - startY;
    
    if (tracking == 1) {
        
        
        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;
        
        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;
        
        rAux = r;
    }
    else if (tracking == 2) {
        
        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}


void loadTexture() {
    
    unsigned int t, tw, th;
    unsigned char *texData;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)"relva.jpg");
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();
    
    glGenTextures(1, &texture);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);
}


void init() {
    
    alpha = 0.0;
    beta = 0.0;
    
    unsigned int ima[1];
    
    ilInit();
    ilGenImages(1, ima);
    ilBindImage(ima[0]);
    ilLoadImage((ILstring)"terreno.jpg");
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    
    imageWidth = ilGetInteger(IL_IMAGE_HEIGHT);
    imageData = ilGetData();
    printf("%d\n", imageWidth);
    
    prepareTerrain();
    
    loadTexture();
    
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
}


int main(int argc, char **argv) {
    
    // inicializa��o
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("CG@DI-UM");
    
    
    // registo de fun��es
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    
    // por aqui registo da fun��es do teclado e rato
    
    glutKeyboardFunc(processKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    
#ifndef __APPLE__	
    glewInit();
#endif	
    
    init();
    
    // entrar no ciclo do GLUT 
    glutMainLoop();
    
    return 0;
}

