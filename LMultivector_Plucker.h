#pragma once//

#include "LMultivector.h"

/*! @file LMultivector_Plucker.h	Rudimentary support for Plucker coordinates
	
	We represent a multivector as a GATuple in homogeneous coordinates.
 
	This file is primarily a wrapper around the GATuple providing, for most
	cases, a wrapper for 3-space support.
 
	Methods in this file occur in homogeneous space.  Conformal space will be
	looked at later as space requirements go up and do not add what we need...
 */

namespace Plucker
{
	//!	Generates a point in 3-space.
	GATuple<e1^e2^e3^e4> Point(const float x_, const float y_, const float z_)
	{
		return GA<e1>(x_) + GA<e2>(y_) + GA<e3>(z_) + 1.0_e4;
	}
	

	//!	Generates the representation of a line using Plucker coordinates.
	/*!
		@tparam MV1		The multivector for the first tuple.  (Should be inferred)
		@tparam MV2		The multivector for the second tuple.  (Should be inferred)
		@tparam TYPE	The type used for arithmetic (Should be inferred).
	 
		@param	u		First homogeneous coordinate where e1 is basis at infinity
		@param	v		Second homogeneous coordinate where e1 is basis at infinity
	 
		@warning		Use the Plucker::Point method for both u and v.
	 */
	template<GABasis MV1, class TYPE=float>
	constexpr GATuple<MV1, TYPE> Line(GATuple<MV1, TYPE> u, GATuple<MV1, TYPE> v)
	{
		return (u ^ v);
	}
	
	
	//! Generates the representation of a plane using Plucker coordinates
	/*!	The plane is generated from the 3 points, p1, p2, and p3.
	 
		@warning	Use Plucker::Point to generate the points.
	 */
	template<GABasis MV1, class TYPE>
	constexpr GATuple<MV1, TYPE> Plane(GATuple<MV1, TYPE> p1, GATuple<MV1, TYPE> p2, GATuple<MV1, TYPE> p3)
	{
		return (p1 ^ p2 ^ p3);
	}
	
	
	//! Meet - collide two objects.
	/*! The meet will result in the intersection.  Like a line and plane for a point. */
	template<GABasis MV1, class TYPE>
	constexpr GATuple<MV1, TYPE> Meet(GATuple<MV1, TYPE> o1, GATuple<MV1, TYPE> o2)
	{
		return Dual(o1) * o2;
	}
}