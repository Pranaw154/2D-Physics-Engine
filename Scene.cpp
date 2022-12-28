#include "precompiled.h"

void IntegrateForces(Body *b, double dt)
{
    if (b->im == 0.0f)
        return;

    b->velocity += (b->force * b->im + gravity) * (dt / 2.0f);
    b->angularVelocity += b->torque * b->iI * (dt / 2.0f);
}

void IntegrateVelocity(Body *b, double dt)
{
    if (b->im == 0.0f)
        return;

    b->position += b->velocity * dt;
    b->orient += b->angularVelocity * dt;
    b->SetOrient(b->orient);
    IntegrateForces(b, dt);
}

void Scene::Step(void)
{
    // Generate new collision info
    contacts.clear();
    for (int i = 0; i < bodies.size(); ++i)
    {
        Body *A = bodies[i];

        for (int j = i + 1; j < bodies.size(); ++j)
        {
            Body *B = bodies[j];
            if (A->im == 0 && B->im == 0)
                continue;
            Manifold m(A, B);
            m.Solve();
            if (m.contact_count)
                contacts.emplace_back(m);
        }
    }

    // Integrate forces
    for (int i = 0; i < bodies.size(); ++i)
        IntegrateForces(bodies[i], m_dt);

    // Initialize collision
    for (int i = 0; i < contacts.size(); ++i)
        contacts[i].Initialize();

    // Solve collisions
    for (int j = 0; j < m_iterations; ++j)
        for (int i = 0; i < contacts.size(); ++i)
            contacts[i].ApplyImpulse();

    // Integrate velocities
    for (int i = 0; i < bodies.size(); ++i)
        IntegrateVelocity(bodies[i], m_dt);

    // Correct positions
    for (int i = 0; i < contacts.size(); ++i)
        contacts[i].PositionalCorrection();

    // Clear all forces
    for (int i = 0; i < bodies.size(); ++i)
    {
        Body *b = bodies[i];
        b->force.Set(0, 0);
        b->torque = 0;
    }
}

void Scene::Render(void)
{
    for (int i = 0; i < bodies.size(); ++i)
    {
        Body *b = bodies[i];
        b->shape->Draw();
    }

    for (int i = 0; i < contacts.size(); ++i)
    {
        Manifold &m = contacts[i];
        Vec n = m.normal;
        for (int j = 0; j < m.contact_count; ++j)
        {
            Vec c = m.contacts[j];
            int x1 = c.x, y1 = c.y;
            n *= 0.75f;
            c += n;
            int x2 = c.x, y2 = c.y;
            S2D_DrawLine(x1, y1, x2, y2,
                         40,
                         1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0);

        }
    }
}

Body *Scene::Add(Shape *shape, int x, int y)
{
    assert(shape);
    Body *b = new Body(shape, x, y);
    bodies.push_back(b);
    return b;
}

