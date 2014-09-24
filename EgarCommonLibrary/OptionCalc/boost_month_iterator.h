#pragma once

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

//modified month iterator

namespace boost {
	namespace date_time {
		template<class date_type>
		class month_fnctr 
		{
		public:
			typedef typename date_type::duration_type duration_type;
			typedef typename date_type::calendar_type cal_type;
			typedef typename cal_type::ymd_type ymd_type;
			typedef typename cal_type::day_type day_type;

			month_fnctr(int f) : f_(f), origDayOfMonth_(0) {}
			duration_type get_offset(const date_type& d) const 
			{
				ymd_type ymd(d.year_month_day());
				if (origDayOfMonth_ == 0) {
					origDayOfMonth_ = ymd.day;
					day_type endOfMonthDay(cal_type::end_of_month_day(ymd.year,ymd.month));
					//*if (endOfMonthDay == ymd.day) {
					//*	origDayOfMonth_ = -1; //force the value to the end of month
					//*}
				}
				typedef date_time::wrapping_int2<short,1,12> wrap_int2;
				typedef typename wrap_int2::int_type int_type;
				wrap_int2 wi(ymd.month);
				//calc the year wrap around, add() returns 0 or 1 if wrapped
				int_type year = wi.add(static_cast<int_type>(f_)); 
				year = static_cast<int_type>(year + ymd.year); //calculate resulting year
				//       std::cout << "trace wi: " << wi.as_int() << std::endl;
				//       std::cout << "trace year: " << year << std::endl;
				//find the last day for the new month
				day_type resultingEndOfMonthDay(cal_type::end_of_month_day(year, wi.as_int()));
				//original was the end of month -- force to last day of month
				if (origDayOfMonth_ == -1) {
					return date_type(year, wi.as_int(), resultingEndOfMonthDay) - d;
				}
				day_type dayOfMonth = origDayOfMonth_;
				if (dayOfMonth > resultingEndOfMonthDay) {
					dayOfMonth = resultingEndOfMonthDay;
				}
				return date_type(year, wi.as_int(), dayOfMonth) - d;
			}
			//! Returns a negative duration_type
			duration_type get_neg_offset(const date_type& d) const 
			{
				ymd_type ymd(d.year_month_day());
				if (origDayOfMonth_ == 0) {
					origDayOfMonth_ = ymd.day;
					day_type endOfMonthDay(cal_type::end_of_month_day(ymd.year,ymd.month));
					//*if (endOfMonthDay == ymd.day) {
					//*	origDayOfMonth_ = -1; //force the value to the end of month
					//*}
				}
				typedef date_time::wrapping_int2<short,1,12> wrap_int2;
				typedef typename wrap_int2::int_type int_type;
				wrap_int2 wi(ymd.month);
				//calc the year wrap around, add() returns 0 or 1 if wrapped
				int_type year = wi.subtract(static_cast<int_type>(f_)); 
				year = static_cast<int_type>(year + ymd.year); //calculate resulting year
				//find the last day for the new month
				day_type resultingEndOfMonthDay(cal_type::end_of_month_day(year, wi.as_int()));
				//original was the end of month -- force to last day of month
				if (origDayOfMonth_ == -1) {
					return date_type(year, wi.as_int(), resultingEndOfMonthDay) - d;
				}
				day_type dayOfMonth = origDayOfMonth_;
				if (dayOfMonth > resultingEndOfMonthDay) {
					dayOfMonth = resultingEndOfMonthDay;
				}
				return date_type(year, wi.as_int(), dayOfMonth) - d;
			}
		private:
			int f_;
			mutable short origDayOfMonth_;
		};

	}
	namespace gregorian{
		typedef date_time::date_itr<date_time::month_fnctr<date>, date> month_itr;
		typedef date_time::month_fnctr<date> month_adder;
	}
}