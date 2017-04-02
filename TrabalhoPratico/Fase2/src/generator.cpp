/**
 * generator.cpp
 * Trabalho CG - Gerador
 * 
 * @author (Pirata)
 * @date (2017.03.25)
 * @version (v2)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "geometricShapes.h"

int main(int argc, char **argv)
{
	Ponto3D buffer = NULL;
	bool flag = false;
	int r, i, div, slices, stacks;
	float sizeX, sizeY, sizeZ, height, radius;
	FILE* f = NULL;

	if (argc > 1)
	{
		if(!strcmp(argv[1], "plane") || !strcmp(argv[1],"plano"))
		{
			buffer = (Ponto3D) malloc(6*sizeof(struct sPonto3D));
			//argv => gerador plano sizeX sizeZ ficheiro
			if(argc == 5)
			{
				if(sscanf(argv[2],"%f",&sizeX) && sscanf(argv[3],"%f",&sizeZ))
					r = drawPlane_Points(sizeX, 0.0f, sizeZ, 0.0f, 0.0f, 0.0f, 0, 0, buffer);
				else {
					perror("Arguments types not compatible");
					return -1;
				}
				if(f = fopen(argv[4],"w"))
				{
					fprintf(f, "%d\n", r);	// talvez não necessário
					for(i = 0;i < r;i++)
						fprintf(f, "%.12f; %.12f; %.12f\n", buffer[i].x,buffer[i].y,buffer[i].z);
					flag = true;
					fclose(f);
				} else {
					perror("Unable to open file");
					return -1;
				}
			}
			// argv => gerador plano size ficheiro
			// so um tamanho entrado, faz um quadrado
			if(!flag && (argc == 4))
			{
				if(sscanf(argv[2],"%f",&sizeX))
					r = drawPlane_Points(sizeX, 0.0f, sizeX, 0.0f, 0.0f, 0.0f, 0, 0, buffer);
				else {
					perror("Arguments types not compatible");
					return -1;
				}
				if(f = fopen(argv[3],"w"))
				{
					fprintf(f, "%d\n", r);	// talvez não necessário
					for(i = 0;i < r;i++)
						fprintf(f, "%.12f; %.12f; %.12f\n", buffer[i].x,buffer[i].y,buffer[i].z);
					flag = true;
					fclose(f);
				} else {
					perror("Unable to open file");
					return -1;
				}
			}
			// argv => gerador plano ficheiro
			// sem tamanhos definidos, os pré definidos serão 100
			if(!flag && (argc == 3))
			{
				r = drawPlane_Points(10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0, 0, buffer);
				if(f = fopen(argv[2],"w"))
				{
					fprintf(f, "%d\n", r);	// talvez não necessário
					for(i = 0;i < r;i++)
						fprintf(f, "%.12f; %.12f; %.12f\n", buffer[i].x,buffer[i].y,buffer[i].z);
					flag = true;
					fclose(f);
				} else {
					perror("Unable to open file");
					return -1;
				}
			}
			if(!flag) {
				perror("arguments missing");
				return -1;
			}
		}
		if(!flag && (!strcmp(argv[1],"box") || !strcmp(argv[1],"caixa")))
		{
			//argv => gerador caixa sizeX sizeY sizeZ divisoes ficheiro
			if(argc == 7)
			{
				if(sscanf(argv[2],"%f",&sizeX) && sscanf(argv[3],"%f",&sizeY) && sscanf(argv[4],"%f",&sizeZ) && sscanf(argv[5],"%d",&div)) {
					buffer = (Ponto3D) malloc((6 * (6 * (div + 1) * (div + 1))) * sizeof(struct sPonto3D));	 // 6 lados * 6 pontos por quadrado * (div+1)² divisoes
					r = drawBox_Points(sizeX, sizeY, sizeZ, 0.0f, 0.0f, 0.0f, div, buffer);
				} else {
					perror("Arguments types not compatible");
					return -1;
				}
				if(f = fopen(argv[6],"w"))
				{
					fprintf(f, "%d\n", r);	// talvez não necessário
					for(i = 0;i < r;i++)
						fprintf(f, "%.12f; %.12f; %.12f\n", buffer[i].x,buffer[i].y,buffer[i].z);
					flag = true;
					fclose(f);
				} else {
					perror("Unable to open file");
					return -1;
				}
			}
			//argv => gerador caixa sizeX sizeY sizeZ ficheiro
			if(!flag && (argc == 6))
			{
				if(sscanf(argv[2],"%f",&sizeX) && sscanf(argv[3],"%f",&sizeY) && sscanf(argv[4],"%f",&sizeZ)) {
					buffer = (Ponto3D) malloc(36 * sizeof(struct sPonto3D));
					r = drawBox_Points(sizeX, sizeY, sizeZ, 0.0f, 0.0f, 0.0f, div, buffer);
				} else {
					perror("Arguments types not compatible");
					return -1;
				}
				if(f = fopen(argv[5],"w"))
				{
					fprintf(f, "%d\n", r);	// talvez não necessário
					for(i = 0;i < r;i++)
						fprintf(f, "%.12f; %.12f; %.12f\n", buffer[i].x,buffer[i].y,buffer[i].z);
					flag = true;
					fclose(f);
				} else {
					perror("Unable to open file");
					return -1;
				}
			}
			if(!flag) {
				perror("arguments missing");
				return -1;
			}
		}
		if(!flag && (!strcmp(argv[1],"cone")))
		{
			//argv => gerador cone raio altura slices stacks ficheiro
			if(argc == 7)
			{
				if(sscanf(argv[2],"%f",&radius) && sscanf(argv[3],"%f",&height) && sscanf(argv[4],"%d",&slices) && sscanf(argv[5],"%d",&stacks)) {
					buffer = (Ponto3D) malloc(((6 * slices) + (6 * slices * (stacks - 1))) * sizeof(struct sPonto3D));	 // 3 * slices (topo) + 6 * slices * stacks-1 (cone) + 3 * slices (base)
					r = drawCone_Points(radius, height, 0.0f, 0.0f, 0.0f, slices, stacks, buffer);
				} else {
					perror("Arguments types not compatible");
					return -1;
				}
				if(f = fopen(argv[6],"w"))
				{
					fprintf(f, "%d\n", r);	// talvez não necessário
					for(i = 0;i < r;i++)
						fprintf(f, "%.12f; %.12f; %.12f\n", buffer[i].x,buffer[i].y,buffer[i].z);
					flag = true;
					fclose(f);
				} else {
					perror("Unable to open file");
					return -1;
				}
			} else {
				perror("arguments missing");
				return -1;
			}
		}
		if(!flag && (!strcmp(argv[1],"sphere") || !strcmp(argv[1],"esfera")))
		{
			// argv => gerador esfera raio slices stacks ficheiro
			if(argc == 6)
			{
				if(sscanf(argv[2],"%f",&radius) && sscanf(argv[3],"%d",&slices) && sscanf(argv[4],"%d",&stacks)) {
					buffer = (Ponto3D) malloc(((6 * slices) + (6 * slices * (stacks - 2))) * sizeof(struct sPonto3D));	 // 3 * slices (topo) + 6 * slices * stacks-2 (esfera) + 3 * slices (base)
					r = drawSphere_Points(radius, 0.0f, 0.0f, 0.0f, slices, stacks, buffer);
				} else {
					perror("Arguments types not compatible");
					return -1;
				}
				if(f = fopen(argv[5],"w"))
				{
					fprintf(f, "%d\n", r);	// talvez não necessário
					for(i = 0;i < r;i++){
						fprintf(f, "%.12f; %.12f; %.12f\n", buffer[i].x,buffer[i].y,buffer[i].z);
					}
					flag = true;
					fclose(f);
				} else {
					perror("Unable to open file");
					return -1;
				}
			} else {
				perror("arguments missing");
				return -1;
			}
		}
	}
	if(!flag) {
		perror("no comands compatible");
		return -1;
	} else {
		return 0;
	}
}