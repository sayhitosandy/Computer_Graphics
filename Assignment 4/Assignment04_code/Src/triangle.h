//
// Created by Sanidhya on 10-10-2017.
//

#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class Triangle : public Object
{
private:
    Vector3D a, b, c;

public:
    Triangle(const Vector3D& _a, const Vector3D& _b, const Vector3D& _c, Material* mat):
            Object(mat), a(_a), b(_b), c(_c)
    {
        isSolid = true;
    }

    virtual bool intersect(Ray& r) const;
};

#endif //TRIANGLE_H
