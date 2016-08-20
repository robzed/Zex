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

