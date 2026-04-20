#include "RandEx.h"

bool HitGacha(const float probability)
{
	float r = rand() / (float)RAND_MAX;
	if (r < probability)return true;
	return false;
}

float rand_Range(float min, float max)
{  
	return (rand() / (float)RAND_MAX) * (max - min + 1) + min;
}
