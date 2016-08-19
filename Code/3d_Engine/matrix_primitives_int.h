//matrix_primitives_int.h (internal header)
float SinTable[360], CosTable[360];
float mat1[4][4], mat2[4][4];	//these are module global matricies used to rotate and
                                //transform an object.

//identity defined in global storage for a quick copy...
float the_identity [4][4] =
{
{ 1,0,0,0 },
{ 0,1,0,0 },
{ 0,0,1,0 },
{ 0,0,0,1 } };

