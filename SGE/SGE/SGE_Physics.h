#ifndef INCLUDED_SGE_PHYSICS_H
#define INCLUDED_SGE_PHYSICS_H

//====================================================================================================
// Filename:	SGE_Physics.h
// Description:	Header containing some useful physics helper functions.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_Math.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Helper Functions
//====================================================================================================

bool Physics_BoundingCircleTest(const Circle& srcCircle, const Circle& intersectCircle);
Vector2 Physics_Response(const float dt, const Vector2& src, const Vector2& contact, const Vector2& velocity, const float restitution = 2.0f);

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_PHYSICS_H