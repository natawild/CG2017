/**
 * point3d.cpp
 * Trabalho CG - Ponto 3D
 * 
 * @author (Pirata)
 * @date (2017.03.25)
 * @version (v2)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.

#include "point3d.h"

struct sPonto3D setPoint (float x, float y, float z)
{
	struct sPonto3D here;
	here.x = x;
	here.y = y;
	here.z = z;
	return here;
}