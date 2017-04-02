/**
 * scene.cpp
 * Trabalho CG - Estrutura de base para a informação retirada do/s ficheiros
 * struct sScene {

 * @author (Pirata)
 * @date (2017.03.27)
 * @version (v2)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.

#include "point3d.h"
#include "scene.h"

struct sModel {
	char name[256];
	int nPontos;
	Ponto3D pontos;
	struct sModel *next;
};
// Comand types:
// 'T' - translate
// 'S' - Scale
// 'R' - Rotate
// 'M' - model
// 'C' - color

Scene sceneInit()
{
	Scene s = (Scene) malloc(sizeof(struct sScene));
	s->groups = NULL;
	s->models = NULL;
	return s;
}

void printDebug(int tt, struct sGroup* g)
{
	int i = 0;
	struct sCommand* auxc;

	if(g != NULL) {
		auxc = g->commands;
		while((i < 5) && (auxc != NULL)) {
			i++;
			//printf("%d + %d - Comando: %c com %p e next = %p\n",tt,i,auxc->type,auxc, auxc->next);
			
			auxc = auxc->next;
		}
		//printDebug(tt+1,g->subgroup);
		//printf("\n");
		//printDebug(tt,g->next);
	} else {
		//printf("O g é nulo - %d\n",tt);
	}

}


// Scene previously allocated and initialized.
// Not verifying repetitions.
void add_read_Model(char* filename, struct sModel** mod)
{
	int n = 0, i = 0;
	float sX, sY, sZ;
	struct sPonto3D pt;
	struct sModel* novo = NULL;
	FILE* f = fopen(filename, "r");

	//printf("add read model\n");

	if(f != NULL)
	{
		if(fscanf(f, "%d\n", &n))
		{
			novo = (struct sModel*) malloc(sizeof(struct sModel));
			strcpy(novo->name,filename);
			novo->nPontos = n;
			novo->pontos = (Ponto3D) malloc(n * sizeof(struct sPonto3D));
			for(i=0;(i < n) && (fscanf(f,"%f; %f; %f\n",&sX,&sY,&sZ));i++)
			{
				pt = setPoint(sX,sY,sZ);
				novo->pontos[i] = pt;
			}
			novo->next = *mod;
			*mod = novo;
		} else {
			perror("file corrupted");
		}
	} else {
		perror("cannot open file");
	}
}

void drawModel(char* model, Scene s)
{
	int i = 0;

	struct sModel* auxmodels1 = NULL;
	auxmodels1 = s->models;
	while((auxmodels1 != NULL) && (strcmp(auxmodels1->name,model) != 0))
	{
		auxmodels1 = auxmodels1->next;
	}

	if(auxmodels1 == NULL)
	{
		add_read_Model(model,&(s->models));
		drawModel(model,s);
	} else {
		glBegin(GL_TRIANGLES);
		for(i = 0;i < auxmodels1->nPontos;i++)
		{
			glVertex3f(auxmodels1->pontos[i].x,auxmodels1->pontos[i].y,auxmodels1->pontos[i].z);
		}
		glEnd();
	}
}

void drawCommand(struct sCommand* c, Scene s)
{
	float* auxfloats = NULL;
	char* auxtext = NULL;

	if(c->type == 'T')
	{
		auxfloats = (float*) c->info;
		glTranslatef(auxfloats[0],auxfloats[1],auxfloats[2]);
	}
	if(c->type == 'R')
	{
		auxfloats = (float*) c->info;
		glRotatef(auxfloats[0],auxfloats[1],auxfloats[2],auxfloats[3]);
	}
	if(c->type == 'S')
	{
		auxfloats = (float*) c->info;
		glScalef(auxfloats[0],auxfloats[1],auxfloats[2]);
	}
	if(c->type == 'C')
	{
		auxfloats = (float*) c->info;
		glColor3f(auxfloats[0],auxfloats[1],auxfloats[2]);
	}
	if(c->type == 'M')
	{
		auxtext = (char*) c->info;
		drawModel(auxtext, s);
	}

}

void drawGroup(struct sGroup* g, Scene s)
{
	struct sCommand *c = NULL;

	if(g) {
		
		glPushMatrix();
		for(c = g->commands; c; c = c->next)
			drawCommand(c, s);
		drawGroup(g->subgroup, s);
		drawGroup(g->next, s);
		glPopMatrix();
	}
}

int search_Model(Scene s, char* model, struct sModel* m)
{
	struct sModel *aux = NULL;
	for(aux = s->models; (aux != NULL) && (strcmp(model,aux->name) != 0); aux = aux->next);
	m = aux;
	if(aux == NULL)
	{
		return -1;
	} else {
		return 0;
	}
}

void print_Model(char* model, Scene *s)
{
	int i = 0;
	struct sModel* aux = NULL;
	int n = 0;
	for(aux = (*s)->models;(aux != NULL) && (strcmp(aux->name,model));aux = aux->next);
	if(aux)
	{
		glBegin(GL_TRIANGLES);
		for(i = 0;i < aux->nPontos;i++)
			glVertex3f(aux->pontos[i].x,aux->pontos[i].y,aux->pontos[i].z);
		glEnd();
	} else {
		perror("could not read model");
	}
}

void set3Floats(float a, float b, float c, struct sCommand *cm)
{
	float* aux = (float*) malloc(3*sizeof(float));
	aux[0] = a;
	aux[1] = b;
	aux[2] = c;
	cm->info = (void*) aux;
}

void set4Floats(float a, float b, float c, float d, struct sCommand *cm)
{
	float* aux = (float*) malloc(4*sizeof(float));
	aux[0] = a;
	aux[1] = b;
	aux[2] = c;
	aux[3] = d;
	cm->info = (void*) aux;
}

void setStringInfo(const char* str, struct sCommand* c)
{
	char* aux = (char*) malloc(10 + (strlen(str)));
	strcpy(aux,str);
	c->info = (void*) aux;
}

void xml_group_read(struct sGroup* g,TiXmlNode* node)
{
	TiXmlNode *child = NULL;
	TiXmlAttribute *at = NULL;
	struct sGroup* gNow = g;
	const char* value;

	struct sCommand *auxcomand = NULL;

	float x, y, z, angle, *auxfloats;
	bool flag = 0;

	for(child = node->FirstChild(); child; child = child->NextSibling())
	{
		flag = 0;
		value = child->Value();

		if(!strcmp(value,"group"))
		{
			gNow->subgroup = (struct sGroup*) malloc(sizeof(struct sGroup));
			gNow->subgroup->next = NULL;
			gNow->subgroup->commands = NULL;
			gNow->subgroup->subgroup = NULL;
			xml_group_read(gNow->subgroup,child);
			gNow->next = (struct sGroup*) malloc(sizeof(struct sGroup));
			gNow->next->next = NULL;
			gNow->next->commands = NULL;
			gNow->next->subgroup = NULL;
			gNow = gNow->next;
			flag = 1;
		}

		if((!flag) && (!strcmp(value,"translate")))
		{
			for(at=child->ToElement()->FirstAttribute(); at; at = at->Next())
			{
				if(!strcmp(at->Name(),"x"))
					x = atof(at->Value());
				if(!strcmp(at->Name(),"y"))
					y = atof(at->Value());
				if(!strcmp(at->Name(),"z"))
					z = atof(at->Value());
			}
			
			if(gNow->commands == NULL)
			{
				gNow->commands = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = gNow->commands;
			} else {
				for(auxcomand = gNow->commands;auxcomand->next ;auxcomand = auxcomand->next);
				auxcomand->next = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = auxcomand->next;
			}
			auxcomand->type = 'T';
			set3Floats(x,y,z,auxcomand);

			flag = 1;
		}

		if((!flag) && (!strcmp(value,"rotate")))
		{
			for(at=child->ToElement()->FirstAttribute(); at; at = at->Next())
			{
				if(!strcmp(at->Name(),"x"))
					x = atof(at->Value());
				if(!strcmp(at->Name(),"y"))
					y = atof(at->Value());
				if(!strcmp(at->Name(),"z"))
					z = atof(at->Value());
				if(!strcmp(at->Name(),"angle"))
					angle = atof(at->Value());
			}

			if(gNow->commands == NULL)
			{
				gNow->commands = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = gNow->commands;
			}
			else {
				for(auxcomand = gNow->commands;auxcomand->next ;auxcomand = auxcomand->next);
				auxcomand->next = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = auxcomand->next;
			}
			auxcomand->type = 'R';
			set4Floats(angle,x,y,z,auxcomand);

			flag = 1;
		}

		if((!flag) && (!strcmp(value,"scale")))
		{
			for(at=child->ToElement()->FirstAttribute(); at; at = at->Next())
			{
				if(!strcmp(at->Name(),"x"))
					x = atof(at->Value());
				if(!strcmp(at->Name(),"y"))
					y = atof(at->Value());
				if(!strcmp(at->Name(),"z"))
					z = atof(at->Value());
			}
			

			if(gNow->commands == NULL)
			{
				gNow->commands = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = gNow->commands;
			}
			else {
				for(auxcomand = gNow->commands;auxcomand->next ;auxcomand = auxcomand->next);
				auxcomand->next = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = auxcomand->next;
			}
			auxcomand->type = 'S';
			set3Floats(x,y,z,auxcomand);

			flag = 1;
		}

		if((!flag) && (!strcmp(value,"color")))
		{
			//printf("color\n");
			for(at=child->ToElement()->FirstAttribute(); at; at = at->Next())
			{
				if(!strcmp(at->Name(),"r"))
					x = atof(at->Value());
				if(!strcmp(at->Name(),"g"))
					y = atof(at->Value());
				if(!strcmp(at->Name(),"b"))
					z = atof(at->Value());
			}
			
			if(gNow->commands == NULL)
			{
				gNow->commands = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = gNow->commands;
			}
			else {
				for(auxcomand = gNow->commands;auxcomand->next ;auxcomand = auxcomand->next);
				auxcomand->next = (struct sCommand*) malloc(sizeof(struct sCommand));
				auxcomand = auxcomand->next;
			}
			auxcomand->type = 'C';
			set3Floats(x,y,z,auxcomand);

			flag = 1;
		}

		if((!flag) && (!strcmp(value,"model")))
		{
			//printf("model\n");
			at = child->ToElement()->FirstAttribute();
			if(!strcmp(at->Name(),"file"))
			{
				if(gNow->commands == NULL)
				{
					gNow->commands = (struct sCommand*) malloc(sizeof(struct sCommand));
					auxcomand = gNow->commands;
				}
				else {
					for(auxcomand = gNow->commands;auxcomand->next ;auxcomand = auxcomand->next);
					auxcomand->next = (struct sCommand*) malloc(sizeof(struct sCommand));
					auxcomand = auxcomand->next;
				}
				auxcomand->type = 'M';
				setStringInfo(at->Value(),auxcomand);
			}
		}
		//printf("fim ciclo -  - g=%p; g-sub=%p; g-next:%p e g-cmmands=%p - gNow:%p; gNow->next:%p, gNow->subgroup:%p\n",g,g->subgroup,g->next,g->commands,gNow,gNow->next,gNow->subgroup);
	}
}

void xml_read_engine(const char* filename,Scene s)
{
	TiXmlDocument d;
	TiXmlElement *root = NULL;
	TiXmlNode *scene = NULL;

	if(d.LoadFile(filename))
	{

		root = d.RootElement();
		scene = root->FirstChild("scene");
		if(scene)
		{
			//printf("cheguei ao s->groups malloc\n");
			s->groups = (struct sGroup*) malloc(sizeof(struct sGroup));
			s->groups->next = NULL;
			s->groups->commands = NULL;
			s->groups->subgroup = NULL;


			xml_group_read(s->groups,scene);
		}
	}
}