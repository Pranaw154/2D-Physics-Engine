#ifndef BODY_H
#define BODY_H

struct Shape;

struct Body
{
    Vec position;
    Vec velocity;

    double angularVelocity;
    double torque;
    double orient; // radians

    Vec force;

    // Set by shape
    double I;  // moment of inertia
    double iI; // inverse inertia
    double m;  // mass
    double im; // inverse mass

    double staticFriction;
    double dynamicFriction;
    double restitution;

    // Shape interface
    Shape *shape;

    // Store a color in RGB format
    double r, g, b;

    Body(Shape *shape_, int x, int y);

    void ApplyForce(const Vec &f)
    {
        force += f;
    }

    void ApplyImpulse(const Vec &impulse, const Vec &contactVector)
    {
        velocity += im * impulse;
        angularVelocity += iI * Cross(contactVector, impulse);
    }

    void SetStatic(void)
    {
        I = 0.0;
        iI = 0.0;
        m = 0.0;
        im = 0.0;
    }

    void SetOrient(double radians);
};

#endif // BODY_H
