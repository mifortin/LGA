#pragma once//

#include "LMultivector.h"
#include <ostream>
#include <cmath>

/*! \file LMultivector_ostream.h	Basic output stream support.
	
 Support for the standard ostream << operator.  It was getting cluttered in
 LMultivector.h
 
 Including this file allows the use of cout and other C++ output operators
 to emit rudimentary debug information.
*/


//! Output function for a GABasis
std::ostream &operator<<(std::ostream &ostr, GABasis t)
{
	int x;
	for (x=0; x<9; x++)
	{
		if (t & (1 << x))
		{
			ostr << " e" << (x+1);
		}
	}
	
	return ostr;
}


//! Output function for GA
template<GABasis BASIS, class TYPE>
std::ostream &operator<<(std::ostream &o, GA<BASIS, TYPE> &v)
{
	o << v() << BASIS;
	return o;
}


//! Utility to write out strings...
struct GAOStreamUtil
{
	GAOStreamUtil(std::ostream &in_oRef)
	: _oRef(in_oRef) {}
	
	template<GABasis BASIS, class TYPE>
	void action(GA<BASIS, TYPE> &o)
	{
		if (std::abs(o()) <= 0.00001)
			return;
		
		if (o() < 0)
			_oRef << " - ";
		else if (!_firstRun)
			_oRef << " + ";
		
		_oRef << std::abs(o()) << BASIS;
		
		_firstRun = false;
	}
	
private:
	bool _firstRun = true;
	std::ostream &_oRef;
};


//! Output for a tuple (for debugging as there is an implicit copy!)
template<GABasis BASIS, class TYPE>
std::ostream& operator<<(std::ostream &o, GATuple<BASIS, TYPE> t)
{
	GAOStreamUtil osu(o);
	GAMetaHelper<BASIS, BASIS, GAOStreamUtil, TYPE> gmh(t, osu);
	
	return o;
}
