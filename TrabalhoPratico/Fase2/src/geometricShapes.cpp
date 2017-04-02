/**
 * geometricShapes.cpp
 * Trabalho CG - Formas Geometricas
 * 
 * @author (Pirata)
 * @date (2017.03.25)
 * @version (v2)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.

#include <stdlib.h>
#include "geometricShapes.h"

/* _USE_MATH_DEFINES
 * Definitions of useful mathematical constants
 * M_E        - e
 * M_LOG2E    - log2(e)
 * M_LOG10E   - log10(e)
 * M_LN2      - ln(2)
 * M_LN10     - ln(10)
 * M_PI       - pi
 * M_PI_2     - pi/2
 * M_PI_4     - pi/4
 * M_1_PI     - 1/pi
 * M_2_PI     - 2/pi
 * M_2_SQRTPI - 2/sqrt(pi)
 * M_SQRT2    - sqrt(2)
 * M_SQRT1_2  - 1/sqrt(2)
 */

/** Todas as funções a partir deste ponto escrevem para um array os pontos, ordenados prontos a ser lidos e desenhados. */

/** Plano: Um dos tamnahos tem de ser 0 para a função fazer um plano direito (Lógico hein? Um plano tem um lado que é flat), paralelos a XY, XZ ou YZ.
 *  sizes: largura e comprimento do plano;
 *  center: o ponto cartesiano onde queremos que se situe o centro do plano;
 *  divisions: o numero de cortes (se for 0 faz o plano numa peça);
 *  rev: se queremos o plano invertido ou não. (0 desenha o plano a ver-se do lado positivo);
 *  pontos: array para onde vamos mandar os pontos calculados.
 *  
 *  Return: devolve um inteiro com o numero de pontos que calculou.
 */
int drawPlane_Points(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions, bool rev, Ponto3D points)
{
	struct sPonto3D pt1, pt2, pt3, pt4, pt5, pt6;
	int i, j, n = divisions + 2, ind = 0;
	int *k = NULL;
	float ptsX[n], ptsY[n], ptsZ[n];
	float divX = sizeX / (float) (n - 1), divY = sizeY / (float) (n - 1), divZ = sizeZ / (float) (n - 1);
	ptsX[0] = ((-1.0f) * sizeX / 2.0f) + centerX, ptsY[0] = ((-1.0f) * sizeY / 2.0f) + centerY, ptsZ[0] = ((-1.0f) * sizeZ / 2.0f) + centerZ;

	// atribui-o a k o endereço da variavel não usada nos ciclos. Ciclos feitos de modo a prever.
	if(!sizeY)
		k = &j;
	else
		k = &i;
	// calcular os valores dos pontos com as divisoes. O array correspondente ao size que está a 0 fica cheio de 0s dentro.
	for(i = 1;i < n;i++)
	{
		ptsX[i] = ptsX[i-1] + divX;
		ptsY[i] = ptsY[i-1] + divY;
		ptsZ[i] = ptsZ[i-1] + divZ;
	}
	// ciclo da triangulação dos planos, com opção para se queremos o plano a imprimir para o lado positivo ou negativo;
	if(!rev) {
		for(j = 0;j < (n - 1); j++)
			for(i = 0;i < (n - 1);i++) {
				// atribui os valores a um ponto interno e depois copia-o para o array.
				pt1 = setPoint(ptsX[i],ptsY[j],ptsZ[*k+1]);
				pt2 = setPoint(ptsX[i+1],ptsY[j],ptsZ[*k]);
				pt3 = setPoint(ptsX[i],ptsY[j+1],ptsZ[*k]);
				pt4 = setPoint(ptsX[i],ptsY[j+1],ptsZ[*k+1]);
				pt5 = setPoint(ptsX[i+1],ptsY[j],ptsZ[*k+1]);
				pt6 = setPoint(ptsX[i+1],ptsY[j+1],ptsZ[*k]);
				points[ind+0] = pt1;
				points[ind+1] = pt2;
				points[ind+2] = pt3;
				points[ind+3] = pt4;
				points[ind+4] = pt5;
				points[ind+5] = pt6;
				ind += 6;
			}
	} else {
		for(j = 0;j < (n - 1); j++)
			for(i = 0;i < (n - 1);i++) {
				pt6 = setPoint(ptsX[i],ptsY[j],ptsZ[*k+1]);
				pt5 = setPoint(ptsX[i+1],ptsY[j],ptsZ[*k]);
				pt4 = setPoint(ptsX[i],ptsY[j+1],ptsZ[*k]);
				pt3 = setPoint(ptsX[i],ptsY[j+1],ptsZ[*k+1]);
				pt2 = setPoint(ptsX[i+1],ptsY[j],ptsZ[*k+1]);
				pt1 = setPoint(ptsX[i+1],ptsY[j+1],ptsZ[*k]);
				points[ind+0] = pt1;
				points[ind+1] = pt2;
				points[ind+2] = pt3;
				points[ind+3] = pt4;
				points[ind+4] = pt5;
				points[ind+5] = pt6;
				ind += 6;
			}
	}
	return ind;		//numero de pontos contados; passados para o array.
}

/** Caixa: Faz uma caixa de qualquer tamanho, as "paredes" sao paralelas aos planos cartesianos.
 *  sizes: largura, comprimento e altura da caixa;
 *  center: o ponto cartesiano onde queremos que se situe o centro da caixa;
 *  divisions: o numero de cortes dos planos (se for 0 faz o plano numa peça);
 *  pontos: array para onde vamos mandar os pontos calculados.
 *  
 *  Return: devolve um inteiro com o numero de pontos que calculou.
 */
int drawBox_Points(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions, Ponto3D points)
{
	int res = 0;
	// Plano de Cima e Baixo:
	res += drawPlane_Points(sizeX, 0, sizeZ, centerX, centerY + (sizeY / 2.0f), centerZ, divisions, 0, points+res);
	res += drawPlane_Points(sizeX, 0, sizeZ, centerX, centerY - (sizeY / 2.0f), centerZ, divisions, 1, points+res);

	// Plano de "Esquerda e Direita":
	res += drawPlane_Points(0, sizeY, sizeZ, centerX + (sizeX / 2.0f), centerY, centerZ, divisions, 0, points+res);
	res += drawPlane_Points(0, sizeY, sizeZ, centerX - (sizeX / 2.0f), centerY, centerZ, divisions, 1, points+res);

	//Plano de Frente e Trás:
	res += drawPlane_Points(sizeX, sizeY, 0, centerX, centerY, centerZ + (sizeZ / 2.0f), divisions, 0, points+res);
	res += drawPlane_Points(sizeX, sizeY, 0, centerX, centerY, centerZ - (sizeZ / 2.0f), divisions, 1, points+res);

	return res;
}

/** Cone: Faz um cone de qualquer tamanho, a base paralela ao plano XZ e apontado para cima.
 *  height: altura do cone;
 *  radius: raio da base do cone;
 *  center: o ponto cartesiano onde queremos que se situe o centro da base do cone;
 *  slices: o numero de fatias verticais do cone (minimo 3 para fazer uma piramide triangular, mas corre sem erros com 2 para a função (faz um triangulo filled dos dois lados);
 *  stacks: o numero de cortes horizontais do cone (minimo 1 - faz a peça de uma vez);
 *  pontos: array para onde vamos mandar os pontos calculados.
 *  
 *  Return: devolve um inteiro com o numero de pontos que calculou.
 */
int drawCone_Points(float radius, float height, float centerX, float centerY, float centerZ, int slices, int stacks, Ponto3D points)
{
	float sinAlpha, betas[slices+1], divBeta;
	float d, stHeightUp, stHeightDown, radUp, radDown;
	struct sPonto3D pt1, pt2, pt3, pt4, pt5, pt6;
	int i, j, ind = 0;

	sinAlpha = radius / height;								// seno de metade do angulo do bico do cone - calcula os raios das diferentes stacks;
	divBeta = (360.0f / (float) slices) * M_PI / 180.0f;	// tamanho do angulo para cada slice
	d = height / (float) stacks;							// altura de cada stack do cone

	// array com os angulos todos das circunferencias - angulos no plano paralelo a XZ (chao);
	betas[0] = 0;
	betas[slices] = 0;
	for(i = 1;i < slices;i++)
		betas[i] = betas[i-1] + divBeta;

	// Triangulos do bico do cone
	stHeightDown = height - d;		// altura da circunferencia de baixo;
	radDown = sinAlpha * d;			// raio da circunferencia
	
	for(i = 0;i < slices; i++)
	{
		// tal como no plano. Atribuição dos pontos a um ponto local e depois copiados para o array;
		pt1 = setPoint(centerX,centerY + height,centerZ);
		pt2 = setPoint((radDown * cos(betas[i+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i+1])) + centerZ);
		pt3 = setPoint((radDown * cos(betas[i])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i])) + centerZ);
		points[ind+0] = pt1;
		points[ind+1] = pt2;
		points[ind+2] = pt3;
		ind += 3;
	}

	// construçao das stacks até à base.
	for(i = 2;i <= stacks;i++)
	{
		stHeightUp = stHeightDown;		// a altura de cima passa a ser a de baixo e a de baixo vai baixar 
		radUp = radDown;				// ao mudar a altura, muda o raio tambem;
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

			pt1 = setPoint((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
			pt2 = setPoint((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			pt3 = setPoint((radDown * cos(betas[j])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j])) + centerZ);
			pt4 = setPoint((radUp * cos(betas[j+1])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j+1])) + centerZ);
			pt5 = setPoint((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			pt6 = setPoint((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
			points[ind+0] = pt1;
			points[ind+1] = pt2;
			points[ind+2] = pt3;
			points[ind+3] = pt4;
			points[ind+4] = pt5;
			points[ind+5] = pt6;
			ind += 6;
		}
	}

	// Circulo da base:
	for(i = 0; i < slices; i++)
	{
		pt1 = setPoint(centerX, centerY, centerZ);
		pt2 = setPoint((radius * cos(betas[i])) + centerX, centerY, (radius * sin(betas[i])) + centerZ);
		pt3 = setPoint((radius * cos(betas[i+1])) + centerX, centerY, (radius * sin(betas[i+1])) + centerZ);
		points[ind+0] = pt1;
		points[ind+1] = pt2;
		points[ind+2] = pt3;
		ind += 3;
	}
	// Tambem se podia usar a função drawCircle mas acabei por reparar que a função não consegue ser aproveitada para a esfera e serve so para a base do cone. (pode ser apagada);
	return ind;
}

/** Esfera: Faz uma esfera de qualquer tamanho, o centro paralelo ao plano XZ e os topos na direção do eixo dos YY.
 *  radius: raio da esfera;
 *  center: o ponto cartesiano onde queremos que se situe o centro da esfera;
 *  slices: o numero de fatias verticais da esfera (minimo 3 para fazer estilo piramide triangular, mas corre sem erros com 2 para a função (faz uma especie de plano filled dos dois lados);
 *  stacks: o numero de cortes horizontais da esfera (minimo 2 - faz a peça em dois, topo e baixo);
 *  pontos: array para onde vamos mandar os pontos calculados.
 *  
 *  Return: devolve um inteiro com o numero de pontos que calculou.
 */
int drawSphere_Points(float radius, float centerX, float centerY, float centerZ, int stacks, int slices, Ponto3D points)
{
	float alphas[stacks+1], divAlpha, betas[slices+1], divBeta;
	float stHeightUp, stHeightDown, radUp, radDown;
	int i, j, ind = 0;
	struct sPonto3D pt1, pt2, pt3, pt4, pt5, pt6;

	// calculo do angulo de cada slice e stack, pasado para radianos
	divBeta = (360.0f / (float) slices) * M_PI / 180.0f;
	divAlpha = M_PI / (float) stacks; 	// == (180.0f / (float) stacks) * M_PI / 180.0f;

	// tal como no cone, criação dos arrays com os angulos todos os slices e stacks sendo que os iniciais sao predefinidos
	// os angulos alpha (do Norte ao sul) começam em 90 e vao até -90, i.e., PI/2 e vão até -PI/2
	alphas[0] = M_PI / 2.0f;
	alphas[stacks] = - M_PI / 2.0f;
	// os angulos beta (horizontal) começam no 0 até 360, i.e., de 0 a 2PI
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

	for(i = 0;i < slices; i++)
	{
		pt1 = setPoint(centerX, centerY + radius, centerZ);
		pt2 = setPoint((radDown * cos(betas[i+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i+1])) + centerZ);
		pt3 = setPoint((radDown * cos(betas[i])) + centerX, centerY + stHeightDown, (radDown * sin(betas[i])) + centerZ);

		pt4 = setPoint(centerX,centerY - radius,centerZ);
		pt5 = setPoint((radUp * cos(betas[i])) + centerX, centerY + stHeightUp, (radUp * sin(betas[i])) + centerZ);
		pt6 = setPoint((radUp * cos(betas[i+1])) + centerX, centerY + stHeightUp, (radUp * sin(betas[i+1])) + centerZ);
		points[ind+0] = pt1;
		points[ind+1] = pt2;
		points[ind+2] = pt3;
		points[ind+3] = pt4;
		points[ind+4] = pt5;
		points[ind+5] = pt6;
		ind += 6;
	}

	// resto da esfera
	for(i = 2;i < stacks;i++)
	{
		// a altura de cima passa a ser a de baixo assim como o raio e os de baixo sao recalculados
		stHeightUp = stHeightDown;
		radUp = radDown;
		stHeightDown = radius * sin(alphas[i]);
		radDown = radius * cos(alphas[i]);
		
		for(j = 0;j < slices;j++)
		{
			pt1 = setPoint((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
			pt2 = setPoint((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			pt3 = setPoint((radDown * cos(betas[j])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j])) + centerZ);
			pt4 = setPoint((radUp * cos(betas[j+1])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j+1])) + centerZ);
			pt5 = setPoint((radDown * cos(betas[j+1])) + centerX, centerY + stHeightDown, (radDown * sin(betas[j+1])) + centerZ);
			pt6 = setPoint((radUp * cos(betas[j])) + centerX, centerY + stHeightUp, (radUp * sin(betas[j])) + centerZ);
			points[ind+0] = pt1;
			points[ind+1] = pt2;
			points[ind+2] = pt3;
			points[ind+3] = pt4;
			points[ind+4] = pt5;
			points[ind+5] = pt6;
			ind += 6;
		}
	}
	return ind;
}
/*
int ring(float outter_radious, float inner_radious,int slices, int rings, int ori, Ponto3D points){
    float angulo=(2*M_PI)/slices,x,y=0,l_aux,raio=(outter_radious-inner_radious)/rings,alt=0;
    
    fprintf(f,"%d\n",2*slices*(rings)*9);
    outter_radious=inner_radious;
    if(ori){
        for(;rings>0;rings--){
            inner_radious=outter_radious;
            outter_radious+=raio;
            
            for(l_aux=0;l_aux<slices;l_aux++){
                x=y;
                y+=angulo;
                fprintf(f,"%f %f %f\n",inner_radious*sin(x), alt, inner_radious*cos(x));
                fprintf(f,"%f %f %f\n",outter_radious*sin(x), alt, outter_radious*cos(x));
                fprintf(f,"%f %f %f\n",inner_radious*sin(y), alt, inner_radious*cos(y));
                
                fprintf(f,"%f %f %f\n",inner_radious*sin(y), alt, inner_radious*cos(y));
                fprintf(f,"%f %f %f\n",outter_radious*sin(x), alt, outter_radious*cos(x));
                fprintf(f,"%f %f %f\n",outter_radious*sin(y), alt, outter_radious*cos(y));
            }
        }
    }else{
        for(;rings>0;rings--){
            inner_radious=outter_radious;
            outter_radious+=raio;
            
            for(l_aux=0;l_aux<slices;l_aux++){
                x=y;
                y+=angulo;inner_radious
                fprintf(f,"%f %f %f\n",inner_radious*sin(x), alt, inner_radious*cos(x));
                fprintf(f,"%f %f %f\n",inner_radious*sin(y), alt, inner_radious*cos(y));
                fprintf(f,"%f %f %f\n",outter_radious*sin(x), alt, outter_radious*cos(x));
                
                fprintf(f,"%f %f %f\n",inner_radious*sin(y), alt, inner_radious*cos(y));
                fprintf(f,"%f %f %f\n",outter_radious*sin(y), alt, outter_radious*cos(y));
                fprintf(f,"%f %f %f\n",outter_radious*sin(x), alt, outter_radious*cos(x));
            }
        }
    }
}*/