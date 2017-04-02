/**
 * point3d.h
 * Trabalho CG - Ponto 3D
 * 
 * @author (Pirata)
 * @date (2017.03.25)
 * @version (v2)
 */
//  Copyright (c) 2017 Luís Pedro Fonseca. All rights reserved.

#ifndef _POINT_3D_CG_G18_
#define _POINT_3D_CG_G18_

/** \brief Estrutura Ponto3D
 *  
 *  Structure to store the information of a point in a tri-dimensional mapping.
 *  Data is stored in floats.
 */
typedef struct sPonto3D {
	float x;	/// Abcissa
	float y;	/// Ordenada
	float z;	/// ------ Esqueci-me
} *Ponto3D;

/** \brief Set the values passed into a Ponto3D tuple.
 *
 *  Mostly used to save space/time coding. Merely creates a Ponto3D with the given values stored inside it.
 */
struct sPonto3D setPoint (float x, float y, float z);

#endif