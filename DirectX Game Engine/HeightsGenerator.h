#include <stdlib.h>

#include <GLMath.h>

#pragma once
class HeightsGenerator
{
private:
	int seed;
	float GetNoise(int x, int z);
	float GetSmoothNoise(int x, int z);
	float GetInterpolatedNoise(float x, float z);
	float Interpolate(float a, float b, float blend);

public:
	HeightsGenerator(int seed);
	~HeightsGenerator();


};

