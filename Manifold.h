#ifndef MANIFOLD_H
#define MANIFOLD_H

#include "PMath.h"
struct Body;

struct Manifold
{
  Manifold( Body *a, Body *b )
    : A( a )
    , B( b )
  {
  }

  void Solve( void );                 // Generate contact information
  void Initialize( void );            // Precalculations for impulse solving
  void ApplyImpulse( void );          // Solve impulse and apply
  void PositionalCorrection( void );  // Naive correction of positional penetration
  void InfiniteMassCorrection( void );

  Body *A;
  Body *B;

  double penetration;     // Depth of penetration from collision
  Vec normal;          // From A to B
  Vec contacts[2];     // Points of contact during collision
  int contact_count; // Number of contacts that occured during collision
  double e;               // Mixed restitution
  double df;              // Mixed dynamic friction
  double sf;              // Mixed static friction
};

#endif // MANIFOLD_H