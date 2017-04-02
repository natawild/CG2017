/**
 * geometricShapes.h
 * Trabalho CG - Formas Geometricas
 * 
 * @author (Pirata)
 * @date (2017.03.25)
 * @version (v2)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.

#ifndef _GEOMETRIC_SHAPES_CG_G18_
#define _GEOMETRIC_SHAPES_CG_G18_


#define _USE_MATH_DEFINES
#include <math.h>

#include "point3d.h"


int drawPlane_Points(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions, bool rev, Ponto3D points);

int drawBox_Points(float sizeX, float sizeY, float sizeZ, float centerX, float centerY, float centerZ, int divisions, Ponto3D points);

int drawCone_Points(float radius, float height, float centerX, float centerY, float centerZ, int slices, int stacks, Ponto3D points);

int drawSphere_Points(float radius, float centerX, float centerY, float centerZ, int stacks, int slices, Ponto3D points);

#endif