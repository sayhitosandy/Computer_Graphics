//sphere.cpp

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
#include "sphere.h"

bool Sphere::intersect(Ray& r) const
{
	Vector3D centerVector = r.getOrigin() - position;
	double a = 1.0;
	double b = 2*dotProduct(r.getDirection(), centerVector);
	double c = dotProduct(centerVector, centerVector) - radius*radius;
	double discriminant = b*b - 4.0*a*c;

    //now check if discr. is posivtive or zero, then only we have an intersection!
	if(discriminant >=0.0)
	{

		if(discriminant == 0)
		{
			double t;
			t = -b/(2.0*a);
			r.setParameter(t, this);
            r.setNormal(unitVector(r.getOrigin() + t*r.getDirection() - position));
            return true;
		}
		else
		{
			//Calculate both roots
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);

			bool b1 = r.setParameter(t1, this);
			bool b2 = r.setParameter(t2, this);

            if (b1 == true) {
                r.setNormal(unitVector(r.getOrigin() + t1*r.getDirection() - position));
            }
            if (b2 == true) {
                r.setNormal(unitVector(r.getOrigin() + t2*r.getDirection() - position));
            }

			return b1||b2;    
		}
	}
	return false;

}
