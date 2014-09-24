#pragma once

class CAbstractContract;

//namespace RM{

	class AccuralBasis{
	public:
		double get_term(double prev, double at){
			double prv_ = 0., at_ = 0.;
			modf(prev, &prv_);
			modf(at, &at_);
			return at_ - prv_;
		};
	};

	enum Interpolation{
		NO_INTERPOLATION = 0,
		FLAT_FORWARD_INTERPOLATION = 1,
		FLAT_BACKWARD_INTERPOLATION = 2,
		LINEAR_INTERPOLATION = 3
	};

	enum Extrapolation{
		NO_EXTRAPOLATION = 0,
		FLAT_EXTRAPOLATION = 1, 
		LINEAR_EXTRAPOLATION = 2
	};

	class Curve{
	public:
		virtual 
			bool get_value(	long term, 
			Interpolation interpolation_type, 
			Extrapolation extrapolation_type, 
			double& value) = 0;

		virtual
			long recalculate() = 0;
	};

	typedef long coordinate;
	typedef double point_value;

	template<class Coord = long, class Value = double>
	class Point{
	public:
		Point(){};

		Point(const Coord& coord, const Value& value){
			this->coord_ = coord;
			this->value_ = value;
		};

		Point(const Coord& coord){
			this->coord_ = coord;
		};

		bool operator < (const Point<Coord, Value>& rh) const{
			return coord_ < rh.coord_;
		};

		bool operator == (const Point<Coord, Value>& rh) const{
			return coord_ == rh.coord_;
		};

		Coord coord_;
		Value value_;
	};

	class ForwardCurve: public Curve{
	public:

		typedef std::vector<CAbstractContract*> source_container;

		typedef Point<coordinate, point_value> my_point;
		typedef std::vector<my_point> my_curve;

		AccuralBasis acb_;
		ForwardCurve();

		virtual
		~ForwardCurve();

		virtual
		bool get_value(	coordinate coord, 
			Interpolation interpolation_type, 
			Extrapolation extrapolation_type, 
			double& value);

		void add_contract(CAbstractContract& contract);

		virtual
		long recalculate();

		inline
		long add_point(coordinate coord, point_value value);

		inline
		void delete_point(coordinate coord);

		inline 
		long get_index(coordinate coord);

		inline
		long find(coordinate coord, long& left_idx, long& right_idx);

		void clear_curve();

		void copy_to(my_curve& dst);

	private:
		source_container data_source_;
		my_curve curve_;
	};

//};