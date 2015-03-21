#pragma once//

#include <cassert>
#include <string.h>

/*!	\file	LMultivector.h		Multivector routing
	
	This file has the routines used to make multi-vectors a reality.  We
	provide templates to perform the various products on the basis.
 
	@code
		// Define an oriented plane with area of 10.
		GA<e1^e2> xy = 10;
	@endcode
 
	The operators are as follows:  Pipe (|) for the geometric product,
	asterisk (*) for the inner product, and carot (^) for the outer product.
 
	@warning	We use features from C++14, and have only tested on Clang.
 */


//! Enumeration that consists of the basis.
/*! Each basis is orthonormal to all the others.
 
	To create a subspace, combine more than one basis using ^.
 
	For example, the plane rotating from e1 to e2 would be e1^e2.
 
	@warning	These are or'd together.  The type is used by the compiler
				to chose the proper overloaded operators.
 */
enum GABasis
{
	scalar	= 0x0,	//!< Special value for the scalar
	e1		= 0x001,
	e2		= 0x002,
	e3		= 0x004,
	e4		= 0x008,
	e5		= 0x010,
	e6		= 0x020,
	e7		= 0x040,
	e8		= 0x080,
	e9		= 0x100,
};


//! An operation that does the geometric product.
struct GA_GeometricProduct
{
	template<class X, class Y, class Z>
	constexpr static void action(X &o, Y &lhs, Z &rhs)
	{ o += (lhs | rhs); }
};


//! An operation that does the inner product.
struct GA_InnerProduct
{
	template<class X, class Y, class Z>
	constexpr static void action(X &o, Y &lhs, Z &rhs)
	{ o += (lhs * rhs); }
};


//! An operation that does the outer product.
struct GA_OuterProduct
{
	template<class X, class Y, class Z>
	constexpr static void action(X &o, Y &lhs, Z &rhs)
	{ o += (lhs ^ rhs); }
};


//! Determine the grade of a basis
/*! We define the grade as the number of basis vectors for the given subspace.
	
	The order is used for the inner and outer products.
 
	@warning	Only meant to be used at compile time
	@param		t a constant that tells of the type.
 */
constexpr int GAGrade(const GABasis t)
{
	int o = 0;
	unsigned int it = (unsigned int)t;
	while (it > 0)
	{
		if ((it & 0x1) == 0x1)
			o++;
		it >>= 1;
	}
	return o;
}


//! Combines multiple basis by multiplying left to right.
/*!	Consider e1^e2 and e2^e3, the combination is e1^e3 since the e2s cancel out.
 */
constexpr GABasis operator^(const GABasis left, const GABasis right)
{
	return GABasis((unsigned int)left^(unsigned int)right);
}

//! Takes two basis and finds the maximum
/*!	Rather than the minimal combination, what is the largest combination.
	In other words, find the most logical psedo-vector of the left and right.
 */
constexpr GABasis operator|(const GABasis left, const GABasis right)
{
	return GABasis((unsigned int)left|(unsigned int)right);
}


static_assert(GAGrade(scalar) == 0, "GAGrade: Order(scalar) = 0");
static_assert(GAGrade(e1) == 1, "GAGrade: Order(e1) = 1");
static_assert(GAGrade(e3) == 1, "GAGrade: Order(e3) = 1");
static_assert(GAGrade(e2^e4^e6) == 3, "GAGrade: Order(e2^e4^e6) = 3");


//! Handles the case where the geometric product is not associative.
/*! Recall, e1^e2 = -e2^e1.  Each time there is a shift of two elements in the
	basis, we flip sign.  e1^e2^e3 = -e2^e1^e3.  And this works as dimensions
	go up.
 
	We assume that e1...en come in order, as such put right after left, and
	see how many elements have to be shifted to sort the e1, e2, e3...
	elements.
 */
constexpr int GAProductMultiplyBy(const GABasis left, const GABasis right)
{
	const unsigned int uil = (unsigned int)left;
	const unsigned int uir = (unsigned int)right;
	
	// Find the last bit in uil
	int lastBit = 9;
	while ( lastBit >= 0 && ((uil >> lastBit) & 0x1) == 0 )
		lastBit--;
	
	// Find the bits in right...
	int sign = 1;
	for (int rb=0; rb<=9; rb++)
	{
		if ( (uir >> rb) & 0x1)
		{
			// Count every bit we must jump over...
			for (int j=lastBit; j > rb; j--)
			{
				if ((uil >> j) & 0x1)
					sign *= -1;
			}
		}
	}
	
	return sign;
}

static_assert(GAProductMultiplyBy(e1, e2) == 1, "GAProductMultiplyBy: In order, simple");
static_assert(GAProductMultiplyBy(e2, e1) == -1, "GAProductMultiplyBy: Reverse order, simple");
static_assert(GAProductMultiplyBy(e2, e1^e3) == -1, "GAProductMultiplyBy: Middle, with negation");



//! A geometric algebra single-variable object
/*!
	The GA object simply wraps a scalar and allows it to be annotated with a
	type suffix.
 
	@tparam	T		The type, which behaves like a float, for most purposes
	@tparam	MV		The integral id.  For scalars, this is 0.  For vectors,
					this is 1,2,3,...F.  For bivectors, it is 11...FF.
					For example, e1e2e3 is written as 123.
 */
template<GABasis MV, class T = float>
class GA
{
public:
	GA(T in_t = 0) : t(in_t) {}
	
	//! Assigning operator
	GA<MV, T>&operator=(T in_) { t = in_; return *this; }
	
	//! Cast operator
	operator T() const { return t; }
	
	T &operator()() { return t; }
	
	//! Utility method to get the basis vectors associated with the given multiplier t.
	/*! @warning For performance, we use the template argument directly when available. */
	static constexpr GABasis type() { return MV; }
	
protected:
	T t;	//!< The type, multiplier in front of the basis.
};


//! Utility object to force the compiler to evaluate something.
template<int X>
class CompilerEval
{
public:
	static constexpr int result() { return X; }
};


//! Product of two GA objects.
template<class T, GABasis M1, GABasis M2>
constexpr GA<M1^M2, T> operator| (GA<M1, T> l, GA<M2, T> r)
{
	CompilerEval<GAProductMultiplyBy(M1, M2)> sign;
	
	GA<M1^M2, T> result = l() * r() * (T)sign.result();
	
	return result;
}


//! Product of a GA object with a scalar
template<class T, GABasis M1>
constexpr GA<M1, T> operator| (GA<M1, T> l, float r)
{
	GA<M1, T> result = l() * r;
	
	return result;
}


//! Product of a scalar with a GA object
template<class T, GABasis M1>
constexpr GA<M1, T> operator| ( float l, GA<M1, T> r)
{
	GA<M1, T> result = l * r();
	
	return result;
}


//! Outer product of two GA objects.
template<class T, GABasis M1, GABasis M2>
constexpr GA<M1^M2, T> operator^ (GA<M1, T> l, GA<M2, T> r)
{
	CompilerEval<GAGrade(M1^M2) == GAGrade(M1) + GAGrade(M2) ? 1:0> sign;

	GA<M1^M2, T> result = l() * r() * (T)sign.result();
	
	return result;
}


//! Inner product of two GA objects.
template<class T, GABasis M1, GABasis M2>
constexpr GA<M1^M2, T> operator* (GA<M1, T> l, GA<M2, T> r)
{
	CompilerEval<GAGrade(M1^M2) == GAGrade(M2) - GAGrade(M1) ? 1:0> sign;
	
	GA<M1^M2, T> result = l() * r() * (T)sign.result();
	
	return result;
}


//! Geometric algebra tuple
/*! Provides a means of holding summations of scalar GA objects
	@tparam PS	Psuedo-scalar.  Or largest possible type needed...
	@tparam T	The type (default float)
 */
template<GABasis PS, class T = float>
class GATuple
{
public:
	
	//! Default...
	GATuple() {}
	
	//! Copy from another tuple...
	template<GABasis M1>
	GATuple(GATuple<M1, T> &in_)
	{
		static_assert(M1 <= PS, "Data loss would ensue");
		memcpy(_data, in_._data, sizeof(T) * (M1+1));
	}
	
	//! Fetch - use templates to force computations
	template<GABasis I>
	GA<I, T> at() { static_assert(I >= 0 && I <= PS, "range check"); return GA<I,T>(_data[I]); }
	
	//! Assign - to set a value in the tuple.
	template<GABasis I>
	GATuple<PS,T> &operator=(GA<I,T> in_g)
	{
		static_assert(I >= 0 && I <= PS, "range check");
		_data[I] = in_g();
		return *this;
	}
	
	//! Add a value
	template<GABasis I>
	GATuple<PS,T> &operator+=(GA<I,T> in_g)
	{
		static_assert(I >= 0 && I <= PS, "range check");
		_data[I] += in_g();
		return *this;
	}
	
	
public:
	//! Data, the e1... act as an index
	/*! Should be private, but the compiler is giving me a hard time */
	T _data[PS+1] = {0};
};


//! Recursive helper structure to help in doing operations over tuples
template<GABasis X, GABasis MV, class Y, class T>
struct GAMetaHelper
{
	GAMetaHelper<GABasis(X-1), MV, Y, T> _t;
	
	GAMetaHelper(GATuple<MV, T> &tpl, Y &operand)
	: _t(tpl, operand)
	{
		GA<X, T> gObj(tpl._data[X]);
		operand.action(gObj);
	}
};


//! Terminating case within the structure.
template<GABasis MV, class Y, class T>
struct GAMetaHelper<scalar, MV, Y, T>
{
	GAMetaHelper(GATuple<MV, T> &tpl, Y &operand)
	{
		GA<scalar, T> gObj(tpl._data[scalar]);
		operand.action(gObj);
	}
};


//! Provide a rudimentary summation.
/*! This allows us to define a GATuple using the sum of GA objects.
	As should be the case. */
template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator+ ( GA<M1, T> l, GA<M2, T> r)
{
	GATuple<M1|M2, T> ret;
	
	ret += l;
	ret += r;
	
	return ret;
}


//! Case where we wish to add an element to a multivector
/*! For performance, use += instead, as we must make copies! */
template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator+( GATuple<M1, T> l, GA<M2, T> r)
{
	GATuple<M1|M2, T> ret(l);
	ret += r;
	return ret;
}


//! Case where we wish to add an element to a multivector
/*! For performance, use += instead, as we must make copies! */
template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator+( GA<M1, T> l, GATuple<M2, T> r)
{
	GATuple<M1|M2, T> ret(r);
	ret += l;
	return ret;
}


//! Utility to add tuples together
template<class T, GABasis MV>
struct GATupleSummationUtil
{
	GATupleSummationUtil(GATuple<MV, T> &in_d)
	: _dest(in_d)
	{}
	
	template<GABasis X, class Y>
	void action(GA<X, Y> &o)
	{
		_dest += o;
	}
	
private:
	GATuple<MV, T> &_dest;
};


//! Adding to tuples together
template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1,T>& operator+=(GATuple<M1,T>& src, GATuple<M2,T> r)
{
	GATupleSummationUtil<T, M1> tsu(src);
	GAMetaHelper<M2, M2, GATupleSummationUtil<T, M1>, T> gmh(r, tsu);
	return src;
}


//! Utility to post-multiply by a GA...
template<class T, GABasis MV, GABasis M2, class OP>
struct GAPostMultiplyUtil
{
	GAPostMultiplyUtil(GATuple<MV, T> &in_d, GA<M2, T> &rhs)
	: _dest(in_d)
	, _rhs(rhs)
	{}
	
	template<GABasis X, class Y>
	void action(GA<X, Y> &o)
	{
		OP::action(_dest, o, _rhs);
	}
	
private:
	GATuple<MV, T> &_dest;
	GA<M2, T> &_rhs;
};


//! Multiply a tuple to a GA...
template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator|( GATuple<M1, T> l, GA<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GAPostMultiplyUtil<T, M1|M2, M2, GA_GeometricProduct> pmu(toRet, r);
	GAMetaHelper<M1, M1, GAPostMultiplyUtil<T, M1|M2, M2, GA_GeometricProduct>, T> gmh(l, pmu);
	
	return toRet;
}

template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator^( GATuple<M1, T> l, GA<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GAPostMultiplyUtil<T, M1|M2, M2, GA_OuterProduct> pmu(toRet, r);
	GAMetaHelper<M1, M1, GAPostMultiplyUtil<T, M1|M2, M2, GA_OuterProduct>, T> gmh(l, pmu);
	
	return toRet;
}

template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator*( GATuple<M1, T> l, GA<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GAPostMultiplyUtil<T, M1|M2, M2, GA_InnerProduct> pmu(toRet, r);
	GAMetaHelper<M1, M1, GAPostMultiplyUtil<T, M1|M2, M2, GA_InnerProduct>, T> gmh(l, pmu);
	
	return toRet;
}


//! Utility to pre-multiply by a GA...
template<class T, GABasis MV, GABasis M2, class OP>
struct GAPreMultiplyUtil
{
	GAPreMultiplyUtil(GATuple<MV, T> &in_d, GA<M2, T> &lhs)
	: _dest(in_d)
	, _lhs(lhs)
	{}
	
	template<GABasis X, class Y>
	void action(GA<X, Y> &o)
	{
		OP::action(_dest, _lhs, o);
	}
	
private:
	GATuple<MV, T> &_dest;
	GA<M2, T> &_lhs;
};


//! Multiply a GA to a tuple...
template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator|( GA<M1, T> l, GATuple<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GAPreMultiplyUtil<T, M1|M2, M1, GA_GeometricProduct> pmu(toRet, l);
	GAMetaHelper<M2, M2, GAPreMultiplyUtil<T, M1|M2, M1, GA_GeometricProduct>, T> gmh(r, pmu);
	
	return toRet;
}

template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator^( GA<M1, T> l, GATuple<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GAPreMultiplyUtil<T, M1|M2, M1, GA_OuterProduct> pmu(toRet, l);
	GAMetaHelper<M2, M2, GAPreMultiplyUtil<T, M1|M2, M1, GA_OuterProduct>, T> gmh(r, pmu);
	
	return toRet;
}

template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator*( GA<M1, T> l, GATuple<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GAPreMultiplyUtil<T, M1|M2, M1, GA_InnerProduct> pmu(toRet, l);
	GAMetaHelper<M2, M2, GAPreMultiplyUtil<T, M1|M2, M1, GA_InnerProduct>, T> gmh(r, pmu);
	
	return toRet;
}


//! Utility to multiply tuples
template<class T, GABasis MV, GABasis M2, class OP>
struct GATupleMultiplyUtil
{
	GATupleMultiplyUtil(GATuple<MV, T> &in_d, GATuple<M2, T> &rhs)
	: _dest(in_d)
	, _rhs(rhs)
	{}
	
	template<GABasis X, class Y>
	void action(GA<X, Y> &o)
	{
		OP::action(_dest, o, _rhs);
	}
	
private:
	GATuple<MV, T> &_dest;
	GATuple<M2, T> &_rhs;
};


//! Multiply a tuple by a tuple...
template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator|( GATuple<M1, T> l, GATuple<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GATupleMultiplyUtil<T, M1|M2, M2, GA_GeometricProduct> pmu(toRet, r);
	GAMetaHelper<M1, M1, GATupleMultiplyUtil<T, M1|M2, M2, GA_GeometricProduct>, T> gmh(l, pmu);
	
	return toRet;
}

template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator^( GATuple<M1, T> l, GATuple<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GATupleMultiplyUtil<T, M1|M2, M2, GA_OuterProduct> pmu(toRet, r);
	GAMetaHelper<M1, M1, GATupleMultiplyUtil<T, M1|M2, M2, GA_OuterProduct>, T> gmh(l, pmu);
	
	return toRet;
}

template<class T, GABasis M1, GABasis M2>
constexpr GATuple<M1|M2, T> operator*( GATuple<M1, T> l, GATuple<M2, T> r)
{
	GATuple<M1|M2, T> toRet;
	GATupleMultiplyUtil<T, M1|M2, M2, GA_InnerProduct> pmu(toRet, r);
	GAMetaHelper<M1, M1, GATupleMultiplyUtil<T, M1|M2, M2, GA_InnerProduct>, T> gmh(l, pmu);
	
	return toRet;
}
