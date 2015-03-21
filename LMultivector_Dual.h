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
