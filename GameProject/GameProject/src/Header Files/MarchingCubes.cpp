#include "MarchingCubes.h"

/* MARCHING CUBES RELATED DATA FOR TRIANGULATION */
vec3 cornerTable[8] = 
{
	vec3(0, 0, 0),
	vec3(1, 0, 0),
	vec3(1, 1, 0),
	vec3(0, 1, 0),
	vec3(0, 0, 1),
	vec3(1, 0, 1),
	vec3(1, 1, 1),
	vec3(0, 1, 1)
};

int edgeIndices[12][2] =
{
	{0, 1}, 
	{1, 2},
	{3, 2},
	{0, 3},
	{4, 5},
	{5, 6},
	{7, 6},
	{4, 7},
	{0, 4},
	{1, 5},
	{2, 6},
	{3, 7}
};

vec3 edgeTable[12][2] = 
{
	{ vec3(0, 0, 0), vec3(1, 0, 0) },
	{ vec3(1, 0, 0), vec3(1, 1, 0) },
	{ vec3(0, 1, 0), vec3(1, 1, 0) },
	{ vec3(0, 0, 0), vec3(0, 1, 0) },
	{ vec3(0, 0, 1), vec3(1, 0, 1) },
	{ vec3(1, 0, 1), vec3(1, 1, 1) },
	{ vec3(0, 1, 1), vec3(1, 1, 1) },
	{ vec3(0, 0, 1), vec3(0, 1, 1) },
	{ vec3(0, 0, 0), vec3(0, 0, 1) },
	{ vec3(1, 0, 0), vec3(1, 0, 1) },
	{ vec3(1, 1, 0), vec3(1, 1, 1) },
	{ vec3(0, 1, 0), vec3(0, 1, 1) }
};

int triTable[256][16] =
{ 
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
	{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
	{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
	{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
	{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
	{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
	{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
	{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
	{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
	{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
	{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
	{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
	{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
	{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
	{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
	{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
	{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
	{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
	{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
	{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
	{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
	{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
	{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
	{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
	{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
	{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
	{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
	{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
	{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
	{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
	{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
	{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
	{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
	{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
	{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
	{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} 
};

MarchingCubes::MarchingCubes()
{
	VertexData tempVertex;
	tempVertex.pos = vec3(0, 0, 0);
	tempVertex.color = vec3(0, 0, 0);
	tempVertex.normal = vec3(0, 0, 0);
	tempVertex.uv = vec2(0, 0);
	m_vertices.push_back(tempVertex);

	unsigned int tempIndex = 0;
	m_indices.push_back(tempIndex);

	Material tempMaterial;
	tempMaterial.Diffuse = vec3(0.8);
	tempMaterial.Specular = vec3(1);
	tempMaterial.Ambient = vec3(1);
	tempMaterial.Shininess = 625;
	m_materials.push_back(tempMaterial);

	TextureData tempTexture;
	tempTexture.id = 0;
	tempTexture.type = " ";
	m_textures.push_back(tempTexture);

	m_mesh = new Mesh(m_vertices, m_indices, m_textures, m_materials);
	m_transform = new Transform;
	m_transform->Translate(vec3(-m_middle, 0, -m_middle));
	ClearMeshData();
	PopulateTerrainMap(m_currentLvl);

	CreateMeshData();
	BuildMesh();

	CreatePhysics();
}

MarchingCubes::~MarchingCubes()
{
	delete m_mesh;

	delete m_transform;
	delete m_platformShape;
	delete m_btTransform;
}

void MarchingCubes::Init()
{
}

void MarchingCubes::Update(GLFWwindow* window, vector<vec3> bombPos, bool shrink)
{
	//Multithread PART
	ClearMeshData();
	if (shrink) {
		if (m_way == true)
		{
			m_shrink++;
			if (m_shrink > 9)
				m_way = false;
		}
		else if (m_way == false) {
			m_shrink--;
			if (m_shrink == 0)
				m_way = true;
		}
	}
	PopulateTerrainMap(m_currentLvl);

	if (bombPos.size() > 0)
	{
		for (int i = 0; i < bombPos.size(); i++)
		{
			MakeHole(bombPos.at(i));
		}
	}

	if (m_holes.size() > 0)
		UpdateHoles();

	CreateMeshData();

}

void MarchingCubes::MarchCube(vec3 position)
{
	float cube[8];
	vec3 currentTriangle[3];

	for (int i = 0; i < 8; i++)
	{
		cube[i] = SampleTerrain(position + cornerTable[i]);
	}

	int lookupIndex = GetLookupIndex(cube);

	if (lookupIndex == 0 || lookupIndex == 255)
		return;

	VertexData temp;

	temp.normal = vec3(0, 1, 0); // Calculate per vertex for better result
	temp.color = vec3(1, 0, 0);
	temp.uv = vec2(1);
	
	int edgeIndex = 0;
	for (int i = 0; i < 5; i++) // Maximum 5 triangles per cell
	{
		for (int j = 0; j < 3; j++) // Maximum 3 points per triangle
		{
			int indice = triTable[lookupIndex][edgeIndex];

			if (indice == -1)
				return;

			vec3 v1 = position + edgeTable[indice][0]; // Start 
			vec3 v2 = position + edgeTable[indice][1]; // and end of a cube-edge. 
			vec3 vPos;

			if (m_smoothTerrain)
			{
				float vert1Sample = cube[edgeIndices[indice][0]];
				float vert2Sample = cube[edgeIndices[indice][1]];

				float difference = vert2Sample - vert1Sample;

				if (difference == 0)
					difference = m_terrainSurface;
				else
					difference = (m_terrainSurface - vert1Sample) / difference;

				// vPos = v1 + ((v2 - v1) * difference);
				vPos = v1 * (1 - difference) + v2 * difference;
			}
			else
			{
				vPos = (v1 + v2) * 0.5f; // Middle point of edge
			}

			temp.pos = vPos;
			m_vertices.push_back(temp);
			m_indices.push_back(m_vertices.size() - 1);

			edgeIndex++;
		}

		int currentVert = m_vertices.size() - 1;
		m_vertices[currentVert].normal = normalize(cross(m_vertices[currentVert - 2].pos - m_vertices[currentVert - 1].pos, m_vertices[currentVert - 1].pos - m_vertices[currentVert].pos));
		m_vertices[currentVert - 1].normal = normalize(cross(m_vertices[currentVert - 2].pos - m_vertices[currentVert - 1].pos, m_vertices[currentVert - 1].pos - m_vertices[currentVert].pos));
		m_vertices[currentVert - 2].normal = normalize(cross(m_vertices[currentVert - 2].pos - m_vertices[currentVert - 1].pos, m_vertices[currentVert - 1].pos - m_vertices[currentVert].pos));
	}
}

void MarchingCubes::ClearMeshData()
{
	m_vertices.clear();
	m_indices.clear();
}

void MarchingCubes::BuildMesh()
{
	m_mesh->UpdateMesh(m_vertices, m_indices); 
}

void MarchingCubes::Draw(Shader* shader)
{
	shader->UseShader();
	shader->SetUniform("u_Model", m_transform->GetMatrix());
	shader->SetUniform("u_PlayerColor", vec3(1));
	shader->SetUniform("u_Glow", false);
	m_mesh->Draw(shader);
}

void MarchingCubes::PopulateTerrainMap(int level)
{
	for (int x = 0; x < m_width + 1; x++)
	{
		for (int y = 0; y < m_height + 1; y++)
		{
			for (int z = 0; z < m_width + 1; z++)
			{
				switch (level)
				{
				case 0 :
					if ((x < m_middle - 4 && z < m_middle - 4) || (x > m_middle + 4 && z < m_middle - 4) || (x > m_middle + 4 && z > m_middle + 4) || (x < m_middle - 4 && z > m_middle + 4)) // MAKE A MIDDLE VARIABLE
					{
						vec2 distVec = (vec2(x, z) - vec2(m_middle, m_middle));
						float dist = sqrt(pow(distVec.x, 2) + pow(distVec.y, 2));
						if (dist > 17)
						{
							m_terrainMap[x][y][z] = 1;
						}
						else
						{
							m_terrainMap[x][y][z] = (float)y;
						}
					}
					else
					{
						float thisHeight = (float)m_height * ((float)rand() / (RAND_MAX)) / 16.0f * 1.5f + 0.001f;
						m_terrainMap[x][y][z] = (float)y - thisHeight;

					}

					if (x >= m_width - m_shrink || z >= m_width - m_shrink || x <= m_shrink || z <= m_shrink)
					{
						m_terrainMap[x][y][z] = 1;

					}
					break;

				case 1 :
					if (x > m_middle - 6 && x < m_middle + 6 && z > m_middle - 6 && z < m_middle + 6)
					{
						m_terrainMap[x][y][z] = 1;
					}
					else
					{
						m_terrainMap[x][y][z] = (float)y;
					}

					if (x >= m_width - m_shrink || z >= m_width - m_shrink || x <= m_shrink || z <= m_shrink)
					{
						m_terrainMap[x][y][z] = 1;
					}
					break;

				case 2: // TETRIS + MULTIFRACTAL
					if (x <= m_middle && z <= m_middle)
					{
						m_terrainMap[x][y][z] = 1;
					}
					else
					{
						float thisHeight = (float)m_height * m_noise->CreateMultiFractal((float)x, (float)z);
						m_terrainMap[x][y][z] = (float)y - thisHeight;

						if (thisHeight < 0.0f)
							m_terrainMap[x][y][z] = (float)y;
					}

					if (x >= m_width || z >= m_width || x <= 0 || z <= 0)
					{
						m_terrainMap[x][y][z] = 1;
					}
					break;

				case 3:
					if (x <= m_middle + 5 && x >= m_middle - 5 && z <= m_middle + 5 && z >= m_middle - 5)
					{
						float thisHeight = (float)m_height * ((float)rand() / (RAND_MAX)) / 16.0f * 1.5f + 3.0f;
						m_terrainMap[x][y][z] = (float)y - thisHeight;
						if (x <= m_middle + 3 && x >= m_middle - 3 && z <= m_middle + 3 && z >= m_middle - 3)
						{
							m_terrainMap[x][y][z] = 1;
						}
					}
					else
					{
						m_terrainMap[x][y][z] = (float)y;
					}

					if (distance(vec2(m_middle, m_middle), vec2((float)x, (float)z)) >= 24.0 - m_shrink)
						m_terrainMap[x][y][z] = 1.0f;

					break;
					
				case 4:
					if (x > m_middle - 11 && x < m_middle + 11 && z > m_middle - 11 && z < m_middle + 11)
					{
						if (x > m_middle - 6 && x < m_middle + 6 && z > m_middle - 6 && z < m_middle + 6)
						{
							m_terrainMap[x][y][z] = (float)y - 2.0f;
						}
						else
						{						
							m_terrainMap[x][y][z] = (float)y - 1.0f;
						}
					}
					else
					{		
						m_terrainMap[x][y][z] = (float)y;
					}


					if (x >= m_width - m_shrink || z >= m_width - m_shrink || x <= m_shrink || z <= m_shrink)
					{
						m_terrainMap[x][y][z] = 1;
					}
					break;

				case 5:
					if (x <= m_holeSize && z <= 2 || x >= m_width - m_holeSize && z <= 2 || 
						x <= 2 && z <= m_holeSize || z >= m_width - m_holeSize && x <= 2 ||
						z >= m_width - 2 && x <= m_holeSize || z >= m_width - 2 && x >= m_width - m_holeSize || 
						x >= m_width - 2 && z <= m_holeSize || z >= m_width - m_holeSize && x >= m_width - 2)
					{
						float thisHeight = (float)m_height * ((float)rand() / (RAND_MAX)) / 16.0f * 1.5f + 3.0f;
						m_terrainMap[x][y][z] = (float)y - thisHeight;
					}
					else
					{
						m_terrainMap[x][y][z] = (float)y;
					}

					if (x >= m_width || z >= m_width || x <= 0 || z <= 0)
					{
						m_terrainMap[x][y][z] = 1;
						m_heightArray[x][y] = 1;
					}
					break;
					
				default:
					float thisHeight = (float)m_height * m_noise->CreatePerlinNoise((float)x, (float)z); // ((float)rand() / (RAND_MAX)) / 16.0f * 1.5f + 0.001f; 
					m_terrainMap[x][y][z] = (float)y - thisHeight;
					m_heightArray[x][z] = (float)y - thisHeight + 1;

					if (thisHeight < 0.0f)
						m_terrainMap[x][y][z] = (float)y;
						//m_heightArray[x][z] = (float)y + 1;


					if (distance(vec2(m_middle, m_middle), vec2((float)x, (float)z)) >= (m_middle - 1) - m_shrink) 
						m_terrainMap[x][y][z] = 1.0f; 
						//m_heightArray[x][z] = 1.0f;
					break;
				}
			}
		}
	}
}

int MarchingCubes::GetLookupIndex(float* cube)
{
	int lookupIndex = 0;

	for (int i = 0; i < 8; i++)
	{
		if (cube[i] > m_terrainSurface)
		{
			lookupIndex |= 1 << i;
		}
	}

	return lookupIndex;
}

void MarchingCubes::CreateMeshData()
{
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int z = 0; z < m_width; z++)
			{
				MarchCube(vec3(x, y, z));
			}
		}
	}
}

float MarchingCubes::SampleTerrain(vec3 point)
{
	return m_terrainMap[(int)point.x][(int)point.y][(int)point.z];
}

void MarchingCubes::DrawWarning(int x, int y, int z)
{
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	{
			glVertex3f(
				(float)x,
				(float)y,
				(float)z
			);
	}
	glEnd();
}

btRigidBody* MarchingCubes::GetBody()
{
	return m_body;
}

void MarchingCubes::CreatePhysics()
{
	vector<btVector3> points;
	btTriangleMesh tetraMesh;
	m_tetraMesh = tetraMesh;
	for (int j = 0; j < m_mesh->m_vertices.size(); j++)
	{

		points.push_back(btVector3(m_mesh->m_vertices[j].pos.x, m_mesh->m_vertices[j].pos.y, m_mesh->m_vertices[j].pos.z));
	}
	if (points.size() > 3) {
		for (int j = 0; j < points.size() - 3; j = j+3)
		{
			m_tetraMesh.addTriangle(points[j], points[j+1], points[j+2], false);
			m_tetraMesh.addTriangleIndices(m_mesh->m_indices[j], m_mesh->m_indices[j+1], m_mesh->m_indices[j+2]);
		}
	}
	m_newPlatformShape = new btBvhTriangleMeshShape(&m_tetraMesh, true, true);

	m_newBtTransform = new btTransform;
	m_newBtTransform->setIdentity();
	m_newBtTransform->setOrigin(btVector3(-m_middle, -0.f, -m_middle));
	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);
	m_newMotionState = new btDefaultMotionState(*m_newBtTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_newMotionState, m_newPlatformShape, localInertia);
	m_body = new btRigidBody(rbInfo);

	m_body->setFriction(3);
}

void MarchingCubes::UpdatePhysics()
{
	vector<btVector3> points;

	btTriangleMesh tetraMesh;
	m_tetraMesh = tetraMesh;
	for (int j = 0; j < m_mesh->m_vertices.size(); j++)
	{

		points.push_back(btVector3(m_mesh->m_vertices[j].pos.x, m_mesh->m_vertices[j].pos.y, m_mesh->m_vertices[j].pos.z));
	}
	if (points.size() > 3) {
		for (int j = 0; j < points.size() - 3; j = j + 3)
		{
			m_tetraMesh.addTriangle(points[j], points[j + 1], points[j + 2], false);
			m_tetraMesh.addTriangleIndices(m_mesh->m_indices[j], m_mesh->m_indices[j + 1], m_mesh->m_indices[j + 2]);
		}
	}
	m_newPlatformShape = new btBvhTriangleMeshShape(&m_tetraMesh, true, true);
}

void MarchingCubes::MapUpdate()
{
	//Sync after mutli and update physics.
	BuildMesh();
	UpdatePhysics();

	delete m_platformShape;
	m_platformShape = m_newPlatformShape;
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.f, m_newMotionState, m_platformShape, localInertia);
	delete m_body;
	m_body = new btRigidBody(rbInfo);
}

vector<VertexData> MarchingCubes::GetVertices()
{
	return m_vertices;
}

void MarchingCubes::SetCurrentLevel(int level)
{
	m_currentLvl = level;
}

int MarchingCubes::GetCurrentLevel()
{
	return m_currentLvl;
}

void MarchingCubes::SetHoleSize(int holeSize)
{
	m_holeSize = holeSize;
}

int MarchingCubes::GetHoleSize()
{
	return m_holeSize;
}

int MarchingCubes::VertForIndice(vec3 vertex)
{
	VertexData temp;
	temp.normal = vec3(0, 1, 0); // Calculate per vertex for better result 
	temp.color = vec3(1, 0, 0);
	temp.uv = vec2(1);

	for (int i = 0; i < m_vertices.size(); i++)
	{
		if (m_vertices[i].pos == vertex)
			return -1;
	}

	temp.pos = vertex;
	m_vertices.push_back(temp);

	int currentVert = m_vertices.size() - 1;
	m_indices.push_back(currentVert);

	return currentVert;
}

void MarchingCubes::CalculateNormals()
{
	if (m_vertices.size() > 3)
	{
		int currentVert = m_vertices.size() - 1;
		m_vertices[currentVert].normal = normalize(cross(m_vertices[currentVert - 2].pos - m_vertices[currentVert - 1].pos, m_vertices[currentVert - 1].pos - m_vertices[currentVert].pos));
		m_vertices[currentVert - 1].normal = normalize(cross(m_vertices[currentVert - 2].pos - m_vertices[currentVert - 1].pos, m_vertices[currentVert - 1].pos - m_vertices[currentVert].pos));
		m_vertices[currentVert - 2].normal = normalize(cross(m_vertices[currentVert - 2].pos - m_vertices[currentVert - 1].pos, m_vertices[currentVert - 1].pos - m_vertices[currentVert].pos));
	}
}

void MarchingCubes::MakeHole(vec3 position)
{
	for (int x = 0; x < m_width + 1; x++)
	{
		for (int y = 0; y < m_height + 1; y++)
		{
			for (int z = 0; z < m_width + 1; z++)
			{
				vec3 posToTerrain = vec3(position.x + m_middle, position.y, position.z + m_middle);
				if (distance(posToTerrain, vec3(x, y, z)) < 3.0f)
				{
					Hole temp;
					 
					temp.position = vec3(x, y, z);
					temp.depth = 3.0 / (distance(posToTerrain, vec3(x, y, z)) + 0.1);
					m_holes.push_back(temp);
				}
			}
		}
	}
}

void MarchingCubes::UpdateHoles()
{
	for (int i = 0; i < m_holes.size(); i++)
	{
		m_terrainMap[(int)m_holes[i].position.x][(int)m_holes[i].position.y][(int)m_holes[i].position.z] = m_terrainMap[(int)m_holes[i].position.x][(int)m_holes[i].position.y][(int)m_holes[i].position.z] + m_holes[i].depth;
	}
}

void MarchingCubes::ClearHoles()
{
	m_holes.clear();
}
