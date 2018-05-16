#include "world.h"
#include "material.h"

#include <iostream>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Color Material::shade(const Ray& incident, const bool isSolid) const
{
	Color diff(0), spec(0), amb(0);
	Vector3D v, l, l2, nor, h;
	v = -unitVector(incident.getDirection());
	nor = unitVector(incident.getNormal());
//    printf("%lf %lf %lf",nor.X(),nor.Y(),nor.Z());
//    printf("%d",world->getLightSource().size());
	l = unitVector(world->getLightSource()[0]->getPosition() + incident.getPosition());
    l2 = unitVector(world->getLightSource()[0]->getPosition() - incident.getPosition());

    Color intensity = world->getLightSource()[0]->getIntensity();

	double ans = dotProduct(nor,l2);
	diff = color*std::fmax(0,ans)*kd*intensity;

    Ray shadowfeeler(incident.getPosition()+0.1*l2, l2);
    world->firstIntersection(shadowfeeler);
    if (shadowfeeler.didHit()) {
        diff = Color(0);
    }

	h = unitVector(v+l2);
	ans = dotProduct(nor,h);
	spec = pow(ans, n);
	spec = color*spec*intensity;

	amb = color*ka;

//    return color;
	return diff+spec+amb;
}
