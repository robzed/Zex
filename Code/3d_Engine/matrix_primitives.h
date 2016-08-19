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


