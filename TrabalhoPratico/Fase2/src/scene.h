/**
 * scene.h
 * Trabalho CG - Estrutura base
 * 
 * @author (Pirata)
 * @date (2017.03.28)
 * @version (v1)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.

#ifndef _SCENE_CG_G18_
#define _SCENE_CG_G18_

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "tinyxml/tinyxml.h"

struct sCommand {
	char type;
	void* info;
	struct sCommand *next;
};

struct sGroup {
	struct sCommand* commands;
	struct sGroup* subgroup;
	struct sGroup* next;
};

typedef struct sScene {
	struct sModel *models;
	struct sGroup *groups;
} *Scene;


Scene sceneInit();

void printDebug(int t,struct sGroup* gg);

void xml_read_engine(const char* filename, Scene s);

void drawGroup(struct sGroup* g, Scene s);

#endif