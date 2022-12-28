#ifndef SCENE_H
#define SCENE_H

#include "precompiled.h"

struct Scene
{

    double m_dt;
    int m_iterations;
    std::vector<Body *> bodies;
    std::vector<Manifold> contacts;

    Scene(double dt, int iterations)
        : m_dt(dt), m_iterations(iterations)
    {
        std::cout<<"FGG"<<"\n";
    }

    void Step(void);
    void Render(void);
    Body *Add(Shape *shape, int x, int y);
    void Clear(void);
};

#endif // SCENE_H