#include "precompiled.h"

void Manifold::Solve(void)
{
  Dispatch[A->shape->GetType()][B->shape->GetType()](this, A, B);
}

void Manifold::Initialize(void)
{
  // Calculate average restitution
  e = std::min(A->restitution, B->restitution);

  // Calculate static and dynamic friction
  sf = std::sqrt(A->staticFriction * B->staticFriction);
  df = std::sqrt(A->dynamicFriction * B->dynamicFriction);

  for (int i = 0; i < contact_count; ++i)
  {
    // Calculate radii from COM to contact
    Vec ra = contacts[i] - A->position;
    Vec rb = contacts[i] - B->position;

    Vec rv = B->velocity + Cross(B->angularVelocity, rb) -
             A->velocity - Cross(A->angularVelocity, ra);

    // Determine if we should perform a resting collision or not
    // The idea is if the only thing moving this object is gravity,
    // then the collision should be performed without any restitution
    // if (rv.squared_vec_length() < (dt * gravity).squared_vec_length() + EPSILON){
    //   e = 0.0f;
    //   std::cout<<"RIP"<<"\n";
    // }
  }
}

void Manifold::ApplyImpulse(void)
{
  // Early out and positional correct if both objects have infinite mass
  if (Equal(A->im + B->im, 0))
  {
    std::cout<<A->im<<" "<<B->im<<"\n";
    InfiniteMassCorrection();
    std::cout<<"Hello\n";
    return;
  }

  for (int i = 0; i < contact_count; ++i)
  {
    // Calculate radii from COM to contact
    Vec ra = contacts[i] - A->position;
    Vec rb = contacts[i] - B->position;

    // Relative velocity
    Vec rv = B->velocity + Cross(B->angularVelocity, rb) -
             A->velocity - Cross(A->angularVelocity, ra);

    // Relative velocity along the normal
    double contactVel = Dot(rv, normal);

    // Do not resolve if velocities are separating
    if (contactVel > 0)
      return;

    double raCrossN = Cross(ra, normal);
    double rbCrossN = Cross(rb, normal);
    double invMassSum = A->im + B->im + Sqr(raCrossN) * A->iI + Sqr(rbCrossN) * B->iI;

    // Calculate impulse scalar
    double j = -(1.0f + e) * contactVel;
    j /= invMassSum;
    j /= (double)contact_count;

    // Apply impulse
    Vec impulse = normal * j;
    A->ApplyImpulse(-impulse, ra);
    B->ApplyImpulse(impulse, rb);

    // Friction impulse
    rv = B->velocity + Cross(B->angularVelocity, rb) -
         A->velocity - Cross(A->angularVelocity, ra);

    Vec t = rv - (normal * Dot(rv, normal));
    t.Normalize();

    // j tangent magnitude
    double jt = -Dot(rv, t);
    jt /= invMassSum;
    jt /= (double)contact_count;

    // Don't apply tiny friction impulses
    if (Equal(jt, 0.0f))
      return;

    // Coulumb's law
    Vec tangentImpulse;
    if (std::abs(jt) < j * sf)
      tangentImpulse = t * jt;
    else
      tangentImpulse = t * -j * df;

    // Apply friction impulse
    A->ApplyImpulse(-tangentImpulse, ra);
    B->ApplyImpulse(tangentImpulse, rb);
  }
}

void Manifold::PositionalCorrection(void)
{
  const double k_slop = 0.05f; // Penetration allowance
  const double percent = 0.4f; // Penetration percentage to correct
  Vec correction = (std::max(penetration - k_slop, 0.0) / (A->im + B->im)) * normal * percent;
  A->position -= correction * A->im;
  B->position += correction * B->im;
}

void Manifold::InfiniteMassCorrection(void)
{
  A->velocity.Set(0, 0);
  B->velocity.Set(0, 0);
}
