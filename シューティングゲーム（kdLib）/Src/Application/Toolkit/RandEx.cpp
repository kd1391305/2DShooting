#include "RandEx.h"

bool HitGacha(const float probability)
{
	float r = rand() / (float)RAND_MAX;
	if (r < probability)return true;
	return false;
}

float randRange(float min, float max)
{  
	return  min + (rand() / (float)RAND_MAX) * (max - min);
}
