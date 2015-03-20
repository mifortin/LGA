#pragma once//

#include "LMultivector.h"

/*!	@file LMultivector_Operator.h		Series of literals used to make things nicer
 */

// R1
GA<e1,float> operator "" _e1 (long double _)		{ return GA<e1,float>(_);}

// R2
GA<e2,float> operator "" _e2 (long double _)		{ return GA<e2,float>(_);}
GA<e1^e2,float> operator "" _e1_e2 (long double _)		{ return GA<e1^e2,float>(_);}

// R3
GA<e3,float> operator "" _e3 (long double _)		{ return GA<e3,float>(_);}
GA<e1^e3,float> operator "" _e1_e3 (long double _)		{ return GA<e1^e3,float>(_);}
GA<e2^e3,float> operator "" _e2_e3 (long double _)		{ return GA<e2^e3,float>(_);}
GA<e1^e2^e3,float> operator "" _e1_e2_e3 (long double _)		{ return GA<e1^e2^e3,float>(_);}

// R4
GA<e4,float> operator "" _e4 (long double _)		{ return GA<e4,float>(_);}
GA<e1^e4,float> operator "" _e1_e4 (long double _)		{ return GA<e1^e4,float>(_);}
GA<e2^e4,float> operator "" _e2_e4 (long double _)		{ return GA<e2^e4,float>(_);}
GA<e1^e2^e4,float> operator "" _e1_e2_e4 (long double _)		{ return GA<e1^e2^e4,float>(_);}
GA<e3^e4,float> operator "" _e3_e4 (long double _)		{ return GA<e3^e4,float>(_);}
GA<e1^e3^e4,float> operator "" _e1_e3_e4 (long double _)		{ return GA<e1^e3^e4,float>(_);}
GA<e2^e3^e4,float> operator "" _e2_e3_e4 (long double _)		{ return GA<e2^e3^e4,float>(_);}
GA<e1^e2^e3^e4,float> operator "" _e1_e2_e3_e4 (long double _)		{ return GA<e1^e2^e3^e4,float>(_);}

// R5
GA<e5,float> operator "" _e5 (long double _)		{ return GA<e5,float>(_);}
GA<e1^e5,float> operator "" _e1_e5 (long double _)		{ return GA<e1^e5,float>(_);}
GA<e2^e5,float> operator "" _e2_e5 (long double _)		{ return GA<e2^e5,float>(_);}
GA<e1^e2^e5,float> operator "" _e1_e2_e5 (long double _)		{ return GA<e1^e2^e5,float>(_);}
GA<e3^e5,float> operator "" _e3_e5 (long double _)		{ return GA<e3^e5,float>(_);}
GA<e1^e3^e5,float> operator "" _e1_e3_e5 (long double _)		{ return GA<e1^e3^e5,float>(_);}
GA<e2^e3^e5,float> operator "" _e2_e3_e5 (long double _)		{ return GA<e2^e3^e5,float>(_);}
GA<e1^e2^e3^e5,float> operator "" _e1_e2_e3_e5 (long double _)		{ return GA<e1^e2^e3^e5,float>(_);}
GA<e4^e5,float> operator "" _e4_e5 (long double _)		{ return GA<e4^e5,float>(_);}
GA<e1^e4^e5,float> operator "" _e1_e4_e5 (long double _)		{ return GA<e1^e4^e5,float>(_);}
GA<e2^e4^e5,float> operator "" _e2_e4_e5 (long double _)		{ return GA<e2^e4^e5,float>(_);}
GA<e1^e2^e4^e5,float> operator "" _e1_e2_e4_e5 (long double _)		{ return GA<e1^e2^e4^e5,float>(_);}
GA<e3^e4^e5,float> operator "" _e3_e4_e5 (long double _)		{ return GA<e3^e4^e5,float>(_);}
GA<e1^e3^e4^e5,float> operator "" _e1_e3_e4_e5 (long double _)		{ return GA<e1^e3^e4^e5,float>(_);}
GA<e2^e3^e4^e5,float> operator "" _e2_e3_e4_e5 (long double _)		{ return GA<e2^e3^e4^e5,float>(_);}
GA<e1^e2^e3^e4^e5,float> operator "" _e1_e2_e3_e4_e5 (long double _)		{ return GA<e1^e2^e3^e4^e5,float>(_);}
