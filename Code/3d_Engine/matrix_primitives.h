// Zex - 2D Space Trading Game with 3D graphics
// Copyright (C) 1998-2005  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

//matrix_primitives.h
#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif

void MAT_Copy(float source[4][4], float dest[4][4]);
void MAT_Mult(float mat1[4][4], float mat2[4][4], float dest[4][4]);
void VEC_MultMatrix(_3D *Source,float mat[4][4],_3D *Dest);
void MAT_Identity(float mat[4][4]);
void TR_Translate(float matrix[4][4],float tx,float ty,float tz);
void TR_Scale(float matrix[4][4],float sx,float sy, float sz);
void TR_Rotate(float matrix[4][4],int ax,int ay,int az);
void Z3D_TrigInit();


