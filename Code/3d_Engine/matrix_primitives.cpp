//matrix functions (primitives) and low level trig
/*
$Log: matrix_primitives.cpp,v $
Revision 1.7  2005/12/17 10:48:10  rob
Zex2005: Added full nested braces for nested structure/array initialisations. Not strictly necessary, but it only took me a few minutes and I did find one minor error in the movie data.

Revision 1.6  2005/10/10 21:35:56  rob
Zex2005: Added newline at end to stop compiler whinging.

Revision 1.5  2005/09/30 00:01:31  stu
Zex2005: Code:x86 - fixed crashes to do with statics (they appear to have been overridden by extern declarations within the file - hmmm)

Revision 1.4  2005/09/29 00:50:48  stu
Zex2005: Code: Basic x86 port

Revision 1.3  2005/05/02 10:30:22  rob
Zex2005: More Updates (code)

Revision 1.2  2005/04/23 19:55:17  rob
ZEX: First set of changes for SDL build. (3d_engine)

Revision 1.2  1999/05/06 17:40:04  stu
*** empty log message ***

*/

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
#include <math.h>

//oldmac//#include <fp.h>
#include "3d_structures_def.h"
#include "matrix_primitives_int.h"
#include "matrix_primitives.h"
//Our module static dat
static float zmat[4][4]={ 
{ 0,0,0,0 },
{ 0,0,0,0 },
{ 0,0,1,0 },
{ 0,0,0,1 } };

static float ymat[4][4]={
{ 0,0,0,0 },
{ 0,1,0,0 },
{ 0,0,0,0 },
{ 0,0,0,1 } };
                         
static float xmat[4][4]={ 
{ 1,0,0,0 },
{ 0,0,0,0 },
{ 0,0,0,0 },
{ 0,0,0,1 } };
static float tmat[4][4]={ 
{ 1,0,0,0 },
{ 0,1,0,0 },
{ 0,0,1,0 },
{ 0,0,0,1 } };
                         
//   tmat[0][0]=1;  tmat[0][1]=0;  tmat[0][2]=0;  tmat[0][3]=0;
//   tmat[1][0]=0;  tmat[1][1]=1;  tmat[1][2]=0;  tmat[1][3]=0;
//   tmat[2][0]=0;  tmat[2][1]=0;  tmat[2][2]=1;  tmat[2][3]=0;
//   tmat[3][0]=tx; tmat[3][1]=ty; tmat[3][2]=tz; tmat[3][3]=1;
              
//   xmat[0][0]=1;        xmat[0][1]=0;        xmat[0][2]=0;        xmat[0][3]=0;
//   xmat[1][0]=0;        xmat[1][1]=Cax;      xmat[1][2]=Sax;      xmat[1][3]=0;
//   xmat[2][0]=0;        xmat[2][1]=-Sax;     xmat[2][2]=Cax;      xmat[2][3]=0;
//   xmat[3][0]=0;        xmat[3][1]=0;        xmat[3][2]=0;	    xmat[3][3]=1;
                   
//   ymat[0][0]=Cay;      ymat[0][1]=0;        ymat[0][2]=-Say;     ymat[0][3]=0;
//   ymat[1][0]=0;        ymat[1][1]=1;        ymat[1][2]=0;        ymat[1][3]=0;
//   ymat[2][0]=Say;      ymat[2][1]=0;        ymat[2][2]=Cay;      ymat[2][3]=0;
//   ymat[3][0]=0;        ymat[3][1]=0;        ymat[3][2]=0;        ymat[3][3]=1;

//   zmat[0][0]=Caz;      zmat[0][1]=Saz;      zmat[0][2]=0;        zmat[0][3]=0;
//   zmat[1][0]=-Saz;     zmat[1][1]=Caz;      zmat[1][2]=0;        zmat[1][3]=0;
//   zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=1;        zmat[2][3]=0;
//   zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;        zmat[3][3]=1;


//SB 6/11/98
void MAT_Copy(float source[4][4], float dest[4][4])
{
    int i,j;
    for(i=0; i<4; i++)
      for(j=0; j<4; j++)
         dest[i][j]=source[i][j];
}

//mult 2 matricies and store in dest matrix
void MAT_Mult(float mat1[4][4], float mat2[4][4], float dest[4][4])
{
   int i,j;

   for(i=0; i<4; i++)
      for(j=0; j<4; j++)
         dest[i][j]=mat1[i][0]*mat2[0][j]+
                    mat1[i][1]*mat2[1][j]+
                    mat1[i][2]*mat2[2][j]+
                    mat1[i][3]*mat2[3][j];
}


//matrix product
void VEC_MultMatrix(_3D *Source,float mat[4][4],_3D *Dest)
{
    Dest->x=Source->x*mat[0][0]+
            Source->y*mat[1][0]+
            Source->z*mat[2][0]+
                      mat[3][0];
    Dest->y=Source->x*mat[0][1]+
            Source->y*mat[1][1]+
            Source->z*mat[2][1]+
                      mat[3][1];
    Dest->z=Source->x*mat[0][2]+
            Source->y*mat[1][2]+
            Source->z*mat[2][2]+
                      mat[3][2];
}


//fills in an identity matrix
void MAT_Identity(float mat[4][4])
{
//using blockmove might offer a slightt speed increase
extern float the_identity[4][4];	//a predefined identity so hopefully the compiler can do a quick copy
//BlockMoveData(&the_identity,mat,sizeof(the_identity));

    mat[0][0]=1; mat[0][1]=0; mat[0][2]=0; mat[0][3]=0;
    mat[1][0]=0; mat[1][1]=1; mat[1][2]=0; mat[1][3]=0;
    mat[2][0]=0; mat[2][1]=0; mat[2][2]=1; mat[2][3]=0;
    mat[3][0]=0; mat[3][1]=0; mat[3][2]=0; mat[3][3]=1;

 
//    mat[0][0]=1; mat[0][1]=0; mat[0][2]=0; mat[0][3]=0;
//    mat[1][0]=0; mat[1][1]=1; mat[1][2]=0; mat[1][3]=0;
//    mat[2][0]=0; mat[2][1]=0; mat[2][2]=1; mat[2][3]=0;
//    mat[3][0]=0; mat[3][1]=0; mat[3][2]=0; mat[3][3]=1;
}


void TR_Translate(float matrix[4][4],float tx,float ty,float tz)
{
//   tmat[0][0]=1;  tmat[0][1]=0;  tmat[0][2]=0;  tmat[0][3]=0;
//   tmat[1][0]=0;  tmat[1][1]=1;  tmat[1][2]=0;  tmat[1][3]=0;
//   tmat[2][0]=0;  tmat[2][1]=0;  tmat[2][2]=1;  tmat[2][3]=0;
   tmat[3][0]=tx; tmat[3][1]=ty; tmat[3][2]=tz; //tmat[3][3]=1;
   MAT_Mult(matrix,tmat,mat1);
   MAT_Copy(mat1,matrix);
}

void TR_Scale(float matrix[4][4],float sx,float sy, float sz)
{
   float smat[4][4];
   smat[0][0]=sx; smat[0][1]=0;  smat[0][2]=0;  smat[0][3]=0;
   smat[1][0]=0;  smat[1][1]=sy; smat[1][2]=0;  smat[1][3]=0;
   smat[2][0]=0;  smat[2][1]=0;  smat[2][2]=sz; smat[2][3]=0;
   smat[3][0]=0;  smat[3][1]=0;  smat[3][2]=0;  smat[3][3]=1;
   MAT_Mult(matrix,smat,mat1);
   MAT_Copy(mat1,matrix);
}

void TR_Rotate(float matrix[4][4],int ax,int ay,int az)
{
   float Cax,Sax,Cay,Say,Caz,Saz;
//   float xmat[4][4];

Sax=SIN(ax);
Say=SIN(ay);
Saz=SIN(az);

Cax=COS(ax);
Cay=COS(ay);
Caz=COS(az);

//   xmat[0][0]=1;        xmat[0][1]=0;        xmat[0][2]=0;        xmat[0][3]=0;
//   xmat[1][0]=0;        
   xmat[1][1]=Cax;      xmat[1][2]=Sax;      //xmat[1][3]=0;
//   xmat[2][0]=0;        
   xmat[2][1]=-Sax;     xmat[2][2]=Cax;      //xmat[2][3]=0;
//   xmat[3][0]=0;        xmat[3][1]=0;        xmat[3][2]=0;	    xmat[3][3]=1;

   ymat[0][0]=Cay;      
//   ymat[0][1]=0;        
   ymat[0][2]=-Say;
//   ymat[0][3]=0;
//   ymat[1][0]=0;        ymat[1][1]=1;        ymat[1][2]=0;        ymat[1][3]=0;
     ymat[2][0]=Say;      //ymat[2][1]=0;        
     ymat[2][2]=Cay;      //ymat[2][3]=0;
//   ymat[3][0]=0;        ymat[3][1]=0;        ymat[3][2]=0;        ymat[3][3]=1;

   zmat[0][0]=Caz;      zmat[0][1]=Saz;      //zmat[0][2]=0;        zmat[0][3]=0;
   zmat[1][0]=-Saz;     zmat[1][1]=Caz;      //zmat[1][2]=0;        zmat[1][3]=0;
//   zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=1;        zmat[2][3]=0;
//   zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;        zmat[3][3]=1;

   MAT_Mult(matrix,ymat,mat1);
   MAT_Mult(mat1,xmat,mat2);
   MAT_Mult(mat2,zmat,matrix);
}

//init functions

//fill sin and cos tables

void Z3D_TrigInit()
{
extern	float SinTable[360], CosTable[360];
   int d;
   for(d=0; d<360; d++)
   {
       SinTable[d]=sin(d*PI/180.0);
       CosTable[d]=cos(d*PI/180.0);
   }
}


