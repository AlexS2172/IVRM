//**************************************************************************************************************//
// abstract:	interface & implementation of thread wrapper class
//
// content:		EgLib::Math
//
// copyright:	(c) 2004 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once
#define _EGFLOATOPS_H_DEFINED

#include <limits>
#include <math.h>
namespace EgLib
{
	namespace EgNumeric
	{
		//-------------------------------------------------------------------------------------------------------------//
		// Rounds Val to the nearest acceptable value
		template <typename T>
			T EgRoundDown(T tVal, T tLessSignificantValue)
		{
			return ((__int64)(tVal / tLessSignificantValue)) * (T)tLessSignificantValue;
		}

		template <typename T>
		T EgRound(T tVal, T tLessSignificantValue)
		{
			return ((__int64)((tVal+tLessSignificantValue*.5) / tLessSignificantValue)) * (T)tLessSignificantValue;
		}

		//-------------------------------------------------------------------------------------------------------------//
		// Returns the integer result of any type comparison (double, float, int etc.)
		// taking into account the rounding rules
		// Returns: 
		//    -1   if t1 is less than t2; 
		//    1    if t1 is greater than t2;
		//    0    if t1 equals to t2
		template <typename T>
			int EgCmp(T t1, T t2, T tLessSignificantValue)
		{
			// We should deal with floating point accuracy
			// ( Let compare 20.56 (double presentation is 20.55(9))
			//   and 20.55 (double presentation is 20.55(0)1
			//   when LESS_SIGNIFICANT_VALUE is 0.01)
			T fRes = 
				EgRoundDown(t1 + tLessSignificantValue / 2, tLessSignificantValue) - 
				EgRoundDown(t2 + tLessSignificantValue / 2, tLessSignificantValue);

			return 
				(fRes > tLessSignificantValue / 2) ? 1 :
			(fRes < -tLessSignificantValue / 2) ? -1 :
			0;
		}

		//-------------------------------------------------------------------------------------------------------------//
		template <typename T>
			bool EQZero(T vtVal)
		{
			if(std::numeric_limits<T>::is_exact)
				return (0 == vtVal); 

			if(std::numeric_limits<T>::is_signed)
				return fabs(vtVal) <  std::numeric_limits<T>::epsilon();		
			else
				return vtVal <  std::numeric_limits<T>::epsilon();		
		}

		//-------------------------------------------------------------------------------------------------------------//
		template <typename T>
		bool NEQZero(T vtVal)
		{
			return !EQZero(vtVal);
		}

		//-------------------------------------------------------------------------------------------------------------//
		template <typename T>
			inline bool EQ(T dVal1, T dVal2)
		{
			if(std::numeric_limits<T>::is_exact)
				return dVal1 == dVal2;

			if(std::numeric_limits<T>::is_signed)
				return EQZero(dVal1 - dVal2);
			else
			{
				long double dDelta = dVal1 - dVal2;
				return EQZero<T>(static_cast<T>(fabs(dDelta)));
			}
		}

		//-------------------------------------------------------------------------------------------------------------//
		template <typename T>
			inline bool Less (T dVal1, T dVal2)
		{
			if(std::numeric_limits<T>::is_exact)
				return dVal1 < dVal2;

			if(std::numeric_limits<T>::is_signed)
			{
				T dCmp = dVal1 - dVal2;
				if(EQZero(dCmp))
					return false;
				return dCmp<0;
			}
			else
			{
				long double dCmp = dVal1 - dVal2;
				if(fEQZero<T>(static_cast<T>(fabs(dCmp))))
					return false;
				return dVal1 < dVal2;
			}
		}

		//-------------------------------------------------------------------------------------------------------------//
		template <typename T>
			inline bool LessOrEq (T dVal1, T dVal2 )
		{
			if(std::numeric_limits<T>::is_exact)
				return dVal1 <= dVal2;

			if(std::numeric_limits<T>::is_signed)
			{
				T dCmp = dVal1 - dVal2;
				if(EQZero(dCmp))
					return true;
				return dCmp<0;
			}
			else
			{
				long double dCmp = dVal1 - dVal2;
				if(EQZero<T>(static_cast<T>(fabs(dCmp))))
					return true;
				return dVal1 < dVal2;
			}
		}
		//-------------------------------------------------------------------------------------------------------------//
		template <typename T>
			inline bool Greater (T dVal1, T dVal2 )
		{
			return Less(dVal2, dVal1);
		}

		//-------------------------------------------------------------------------------------------------------------//
		template <typename T>
			inline bool GreaterOrEq (T dVal1, T dVal2 )
		{
			return LessOrEq(dVal2, dVal1);
		}
	};
};