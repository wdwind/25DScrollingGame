#ifndef LIGHT
#define LIGHT
#include "Vec3D.h"
#include <stdlib.h>

class Light{
public:
	Light() {};
	Light(Vec3Df Lp, Vec3Df Lc, float Lpower, float S) : lightPos(Lp), lightColor(Lc), lightPower(Lpower), s(S) {};

	Vec3Df lightPos = Vec3Df(0, 0, 0);
	Vec3Df lightColor = Vec3Df(1, 1, 1);
	float lightPower = 8;
	float s = 5;
};
#endif