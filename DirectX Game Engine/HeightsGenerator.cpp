#include "HeightsGenerator.h"


HeightsGenerator::HeightsGenerator(int seed){
	this->seed = seed;
}

HeightsGenerator::~HeightsGenerator(){

}

float HeightsGenerator::GetNoise(int x, int z) {
	srand(x * 49632 + z * 325176 + seed);
	return (float)(rand() % 1000001) / (float)1000000;
}

float HeightsGenerator::GetSmoothNoise(int x, int z) {
	return 0.0f;
}

float HeightsGenerator::GetInterpolatedNoise(float x, float z) {
	return 0.0f;
}

float HeightsGenerator::Interpolate(float a, float b, float blend) {
	double theta = blend * PI;
	float f = (float)(1.0f - cos(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}