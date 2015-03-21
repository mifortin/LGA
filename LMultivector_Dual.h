#pragma once//

/*!
 *	@file	LMultivector_Dual		Code needed to compute the dual
 */

#include "LMultivector.h"

/*!	@brief	Computes the dual of a given multivector
 
	@tparam		MV		The multivector.  Should be inferred.  ie. e1^e2^e3
	@tparam		T		The type.  Should be inferred.  Typically float.
 
	@param		in_		The multivector to take the dual of
	@return				The dual.
 
	@warning	For this to work, ensure the GATuple's MV template parameter
				is the multivector.
 */
template<GABasis MV, class T>
GATuple<MV, T> Dual(GATuple<MV, T> in_)
{
	// Compute the exponent
	CompilerEval<(GAGrade(MV) * (GAGrade(MV)-1)) / 2> grade;
	
	// -1 to an even number is positive, else negative.
	GA<MV,T> inverse(grade.result()%2 == 0 ? 1.0 : -1.0);
	
	return in_ * inverse;
}


/*! @brief	Computes the cross product
	
	@param		left_	Left-hand side parameter for the cross product
	@param		right_	Right-hand side paramter for the cross product
 
	@return				The cross product
 
	@warning	We define cross product in terms of the geometric product.
 */
template<GABasis MV, class T>
GATuple<MV, T> Cross(const GATuple<MV,T> left_, const GATuple<MV,T> right_)
{
	GA<MV, T> psuedoscalar(1);
	
	return - psuedoscalar | (left_ ^ right_);
}
