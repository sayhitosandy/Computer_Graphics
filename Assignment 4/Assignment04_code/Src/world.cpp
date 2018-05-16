#include <iostream>
#include "world.h"

using namespace std;

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++)
		objectList[i]->intersect(ray);
	return ray.getParameter();
}

Color World::shade_ray(Ray& ray, int count)
{
	firstIntersection(ray);
	if(ray.didHit()) {
        if (count <= 5) {
            Vector3D r = unitVector(ray.getDirection() - (2 * dotProduct(ray.getDirection(), ray.getNormal()) * ray.getNormal()));
            Ray reflectedRay(ray.getPosition() + 0.1 * r, r);
//            std::cout << shade_ray(reflectedRay, count + 1).b << shade_ray(reflectedRay, count + 1).g << shade_ray(reflectedRay, count + 1).r;
            return (ray.intersected())->shade(ray) + 0.7*shade_ray(reflectedRay, count + 1);
        }
        return (ray.intersected()->shade(ray));
    }
	return background;
}
