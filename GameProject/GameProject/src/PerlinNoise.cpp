#include "Header Files/PerlinNoise.h"

float PerlinNoise::GenNoise(int x)
{
	x = (x << 13) ^ x;

	return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float PerlinNoise::GenNoise(float x, float y)
{
	int n = static_cast<int>(x + y * 599); //599 NAJHSSSS // 42069  //690 //1338
	n = (n << 13) ^ n;

	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float PerlinNoise::Lerp(float a, float b, float x)
{
	return a * (1 - x) + b * x;
}

float PerlinNoise::CosineInterpolate(float a, float b, float x)
{
	float ft = x * 3.141592f;
	float f = (1 - static_cast<float>(cos(ft))) * 0.5f;

	return a * (1 - f) + b * f;
}

float PerlinNoise::SmoothNoise(float x, float y)
{
	x = static_cast<int>(x);

	float corners = (GenNoise(x - 1, y - 1) + GenNoise(x + 1, y - 1) + GenNoise(x - 1, y + 1) + GenNoise(x + 1, y + 1))
		/ 16.0f;
	float sides = (GenNoise(x - 1, y) + GenNoise(x + 1, y) + GenNoise(x, y - 1) + GenNoise(x, y + 1)) / 8.0f;
	float center = GenNoise(x, y) / 4.0f;

	return corners + sides + center;
}

float PerlinNoise::InterpolatedNoise(float x, float y)
{
	int intX = static_cast<int>(x);
	float fractionX = x - intX;

	int intY = static_cast<int>(y);
	float fractionY = y - intY;

	float v1 = SmoothNoise(intX, intY);
	float v2 = SmoothNoise(intX + 1, intY);
	float v3 = SmoothNoise(intX, intY + 1);
	float v4 = SmoothNoise(intX + 1, intY + 1);

	float i1 = CosineInterpolate(v1, v2, fractionX);
	float i2 = CosineInterpolate(v3, v4, fractionX);

	return CosineInterpolate(i1, i2, fractionY);
}

float PerlinNoise::CreatePerlinNoise(float x, float y)
{
	float total = 0;

	// Persistance 
	float p = 0.4f;

	// Nr of octaves 
	int n = 8;

	for (int i = 0; i < n; i++)
	{
		float frequency = static_cast<float>(pow(2, i)) / 16.0f;
		float amplitude = static_cast<float>(pow(p, i));

		total += InterpolatedNoise(x * frequency, y * frequency) * amplitude;
	}

	return total;
}

float PerlinNoise::CreateMultiFractal(float x, float y)
{
	float total = 0;

	// Persistance 
	float p = 0.4f;

	// Nr of octaves 
	int n = 8;

	for (int i = 0; i < n; i++)
	{
		float frequency = static_cast<float>(pow(2.0f, i)) / 16.0f;
		float amplitude = static_cast<float>(pow(p, i));

		float signal = InterpolatedNoise(x * frequency, y * frequency);
		signal /= signal + 1.0f;
		total += abs(signal * amplitude);
	}

	return total;
}

float PerlinNoise::GenSpecificNoise(float x, float y, int fractal)
{
	int n = static_cast<int>(x + y * fractal);
	n = (n << 13) ^ n;

	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float PerlinNoise::SmoothSpecificNoise(float x, float y, int fractal)
{
	x = static_cast<int>(x);

	float corners = (GenSpecificNoise(x - 1, y - 1, fractal) + GenSpecificNoise(x + 1, y - 1, fractal) +
		GenSpecificNoise(x - 1, y + 1, fractal) + GenSpecificNoise(x + 1, y + 1, fractal)) / 16.0f;
	float sides = (GenSpecificNoise(x - 1, y, fractal) + GenSpecificNoise(x + 1, y, fractal) +
		GenSpecificNoise(x, y - 1, fractal) + GenSpecificNoise(x, y + 1, fractal)) / 8.0f;
	float center = GenSpecificNoise(x, y, fractal) / 4.0f;

	return corners + sides + center;
}

float PerlinNoise::InterpolatedSpecificNoise(float x, float y, int fractal)
{
	int intX = static_cast<int>(x);
	float fractionX = x - intX;

	int intY = static_cast<int>(y);
	float fractionY = y - intY;

	float v1 = SmoothSpecificNoise(intX, intY, fractal);
	float v2 = SmoothSpecificNoise(intX + 1, intY, fractal);
	float v3 = SmoothSpecificNoise(intX, intY + 1, fractal);
	float v4 = SmoothSpecificNoise(intX + 1, intY + 1, fractal);

	float i1 = CosineInterpolate(v1, v2, fractionX);
	float i2 = CosineInterpolate(v3, v4, fractionX);

	return CosineInterpolate(i1, i2, fractionY);
}

float PerlinNoise::CreateSpecificPerlinNoise(float x, float y, int fractal)
{
	float total = 0;

	// Persistance 
	float p = 0.4f;

	// Nr of octaves 
	int n = 8;

	for (int i = 0; i < n; i++)
	{
		float frequency = static_cast<float>(pow(2, i)) / 16.0f;
		float amplitude = static_cast<float>(pow(p, i));

		total += InterpolatedSpecificNoise(x * frequency, y * frequency, fractal) * amplitude;
	}

	return total;
}

float PerlinNoise::CreateSpecificMultiFractal(float x, float y, int fractal)
{
	float total = 0;

	// Persistance 
	float p = 0.4f;

	// Nr of octaves 
	int n = 8;

	for (int i = 0; i < n; i++)
	{
		float frequency = static_cast<float>(pow(2.0f, i)) / 16.0f;
		float amplitude = static_cast<float>(pow(p, i));

		float signal = InterpolatedSpecificNoise(x * frequency, y * frequency, fractal);
		signal /= signal + 1.0f;
		total += abs(signal * amplitude);
	}

	return total;
}
