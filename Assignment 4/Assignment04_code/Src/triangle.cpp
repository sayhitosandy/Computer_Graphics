//
// Created by Sanidhya on 10-10-2017.
//

#include "triangle.h"
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

using namespace std;

bool Triangle::intersect(Ray& r) const
{
    double xa, ya, za, xb, yb, zb, xc, yc, zc, xe, ye, ze, xd, yd, zd, beta, gamma, t;
    xa = a.X();
    ya = a.Y();
    za = a.Z();
    xb = b.X();
    yb = b.Y();
    zb = b.Z();
    xc = c.X();
    yc = c.Y();
    zc = c.Z();

    Vector3D e = r.getOrigin();
    Vector3D d = r.getDirection();
    xe = e.X();
    ye = e.Y();
    ze = e.Z();
    xd = d.X();
    yd = d.Y();
    zd = d.Z();

    glm::mat3 A, A1, A2, A3;
    A[0][0] = xa-xb;
    A[0][1] = ya-yb;
    A[0][2] = za-zb;
    A[1][0] = xa-xc;
    A[1][1] = ya-yc;
    A[1][2] = za-zc;
    A[2][0] = xd;
    A[2][1] = yd;
    A[2][2] = zd;

    A1 = A;
    A2 = A;
    A3 = A;

    double detA = glm::determinant(A);

    A1[0][0] = xa-xe;
    A1[0][1] = ya-ye;
    A1[0][2] = za-ze;

    A2[1][0] = xa-xe;
    A2[1][1] = ya-ye;
    A2[1][2] = za-ze;

    A3[2][0] = xa-xe;
    A3[2][1] = ya-ye;
    A3[2][2] = za-ze;

    beta = glm::determinant(A1);
    beta = beta/detA;

    gamma = glm::determinant(A2);
    gamma = gamma/detA;

    t = glm::determinant(A3);
    t = t/detA;

    if (beta > 0 && gamma > 0 && beta+gamma < 1) {
        bool res = r.setParameter(t, this);
        if (res) {
            Vector3D n = crossProduct(c-a, b-a);
            r.setNormal(unitVector(-n));
        }
        return true;
    }

    return false;
}