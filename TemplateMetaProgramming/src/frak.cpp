/*
 * frak.cpp
 *
 *  Created on: 29.10.2018
 *      Author: CToma
 */

template <unsigned int n>
struct factorial {
	enum { result = n * factorial<n - 1>::result };
};

template <>
struct factorial<0> {
	enum { result = 1 };
};

template <long N, long D> struct Frak {
	static const long Num = N;
	static const long Den = D;
};

template <int N, typename X> struct ScalarMultiplication
{
	static const long Num = N * X::Num;
	static const long Den = N * X::Den;

};

template <int N, typename X> struct ScalarMultiplication2{
	typedef Frak<N*X::Num, N*X::Den> result;
};

template <int X, int Y> struct MCD {
	static const long result = MCD<Y, X % Y>::result;
};

template <int X> struct MCD<X, 0> {
	static const long result = X;
};

template <class F> struct Simpl {
	static const long mcd = MCD<F::Num, F::Den>::result;
	static const long new_num = F::Num / mcd;
	static const long new_den = F::Den / mcd;
	typedef Frak<new_num, new_den> New_Frak;
	typedef New_Frak result;
};

template <typename X1, typename Y1> struct SameBase {
	typedef typename ScalarMultiplication2< Y1::Den, X1>::result X;
	typedef typename ScalarMultiplication2< X1::Den, Y1>::result Y;
};

template <typename X, typename Y> struct Sum {
	typedef SameBase<X, Y> S;
	static const long Num = S::X::Num + S::Y::Num;
	static const long Den = S::Y::Den;

	typedef typename Simpl< Frak<Num, Den> >::result res;
};

template <int ArgValue>
struct TLog2Floor
{
   enum
   {   /// calculation result
	   value = TLog2Floor< (ArgValue >> 1) >::value + 1
   };

   int x : 3;
};

/// @cond VFC_DOXY_SPECIALIZATIONS

/// specialization for ArgValue == 1 (recursion termination)
template <>
struct TLog2Floor<1>
{
   enum { value = 0 };
};

/// specialization for undefined case ArgValue == 0 (generates compiler error)
template <>
struct TLog2Floor<0>
{
};



