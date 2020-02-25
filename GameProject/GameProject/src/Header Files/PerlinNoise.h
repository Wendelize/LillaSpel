#pragma once

#pragma once 
#include "Include.h" 
#include <algorithm> 

class PerlinNoise
{
private:

public:
	float GenNoise(int x);
	float GenNoise(float x, float y);
	float Lerp(float a, float b, float x);
	float CosineInterpolate(float a, float b, float x);
	float SmoothNoise(float x, float y);
	float InterpolatedNoise(float x, float y);
	float CreatePerlinNoise(float x, float y);
	float CreateMultiFractal(float x, float y);
};
