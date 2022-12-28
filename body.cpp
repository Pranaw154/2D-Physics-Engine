#include "precompiled.h"

Body::Body(Shape *shape_, int x, int y)
    : shape(shape_->Clone())
{
    shape->body = this;
    position.Set((double)x, (double)y);
    velocity.Set(0, 0);
    angularVelocity = 0;
    torque = 0;
    orient = Random(-PI, PI);
    force.Set(0, 0);
    staticFriction = 0.5;
    dynamicFriction = 0.5;
    restitution = 1.0;
    shape->Initialize();
    r = Random(0.2, 1.0);
    g = Random(0.2, 1.0);
    b = Random(0.2, 1.0);
}

void Body::SetOrient(double radians)
{
    orient = radians;
    shape->SetOrient(radians);
}