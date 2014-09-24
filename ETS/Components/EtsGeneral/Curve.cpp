#include "StdAfx.h"
#include "Curve.h"
#include "AbstractContract.h"
#include "FutureContract.h"

//namespace RM{



	ForwardCurve::ForwardCurve(){
		curve_.reserve(128);
	};

	/*virtual*/
	ForwardCurve::~ForwardCurve(){
		curve_.clear();
		data_source_.clear();
	};

	/*virtual*/
	bool ForwardCurve::get_value(	coordinate coord, 
									Interpolation interpolation_type, 
									Extrapolation extrapolation_type, 
									double& value){

		long idx = -1L;
		long left_idx = -1L;
		long right_idx = -1L;

		idx = find(coord, left_idx, right_idx);

		if (idx >= 0){ // point found
		   value = curve_[idx].value_;
		   return true;
		}
		else{ // point not found
		
			if (left_idx == -1L && right_idx == -1L){ //empty curve
				return false; 
			}
			else if (left_idx == -1L && right_idx != -1L){ // left side extrapolation

				if (extrapolation_type == NO_EXTRAPOLATION){
					return false;
				}
				else if(extrapolation_type == FLAT_EXTRAPOLATION){
					value = curve_[right_idx].value_;
					return true;
				}
				else if(extrapolation_type == LINEAR_EXTRAPOLATION){
					return false;
				}
			}
			else if (left_idx != -1L && right_idx == -1L){ // right side extrapolation

				if (extrapolation_type == NO_EXTRAPOLATION){
					return false;
				}
				else if(extrapolation_type == FLAT_EXTRAPOLATION){
					value = curve_[left_idx].value_;
					return true;
				}
				else if(extrapolation_type == LINEAR_EXTRAPOLATION){
					return false;
				}
			}
			else { // interpolate 
			
				if (interpolation_type == NO_INTERPOLATION){
					return false;
				}
				else if (interpolation_type == FLAT_FORWARD_INTERPOLATION){
					value = curve_[right_idx].value_;
					return true;
				}
				else if (interpolation_type == FLAT_BACKWARD_INTERPOLATION){
					value = curve_[left_idx].value_;
					return true;
				}
				else if (interpolation_type == LINEAR_INTERPOLATION){
					double dx = double(curve_[right_idx].coord_ - curve_[left_idx].coord_);
					double dy = curve_[right_idx].value_ - curve_[left_idx].value_;
					double x0 = (curve_[left_idx].coord_);
					double y0 = curve_[left_idx].value_;

					value = ((double(coord) - x0) * dy / dx) + y0;

					return true;
				}
			}
		};
		return false;
	};

	void ForwardCurve::add_contract(CAbstractContract& contract){
		data_source_.push_back(&contract);
	};

	/*virtual*/
	long ForwardCurve::recalculate(){

		source_container::iterator it = data_source_.begin();
		source_container::iterator it_end = data_source_.end();

		CAbstractContract* _contract = 0;

		while (it != it_end){

			_contract	= (*it);

			CFutureContract* _future = dynamic_cast<CFutureContract*>(_contract);
			
			it++;
			if (!_future){
				continue;
			};
			
			double current_date = CCalculationSettingsSingleton::Instance()->getCurrentDate();
			coordinate coord = coordinate(acb_.get_term(current_date, _future->m_dtMaturityDate));
			long point_index = -1;

			double rate_value = _contract->getPriceMid();

			if (rate_value != 0. && coord >= 0){
				if ((point_index = get_index(coord)) < 0 ){
					add_point(coord, rate_value);						
				}
				else{
					curve_[point_index].value_ = rate_value;
				}
			}
		};

		std::sort(curve_.begin(), curve_.end());

		return 0L;
	};

	inline
	long ForwardCurve::add_point(coordinate coord, point_value value){
		curve_.push_back(my_point(coord, value));
		return static_cast<long>(curve_.size()-1);
	};

	inline
	void ForwardCurve::delete_point(coordinate coord){
		my_curve::iterator it = std::find(curve_.begin(), curve_.end(), my_point(coord));
		if (curve_.end() != it){
			curve_.erase(it);
		};
	};

	inline 
	long ForwardCurve::get_index(coordinate coord){
		for (long i = 0; i < static_cast<long>(curve_.size()); i++){
			if (curve_[i].coord_ == coord) return i;
		};
		return -1L;
	};

	inline
	long ForwardCurve::find(coordinate coord, long& left_idx, long& right_idx){

		if (curve_.empty()) return -1L;

		long found_index = -1L;
		long curve_length = static_cast<long>(curve_.size());

		left_idx = right_idx = -1L;

		if ((found_index = get_index(coord)) < 0){
			if (coord < curve_[0].coord_){
				right_idx = 0;
			}
			else if (coord > curve_[curve_length - 1].coord_){
				left_idx = curve_length - 1;
			}
			else {
				for (long i = 0; i < curve_length - 1; i++){
					if (coord > curve_[i].coord_ && coord < curve_[i + 1].coord_){
						left_idx = i;
						right_idx = i + 1;
						break;
					};
				}
			}

		};
		return found_index;
	};

	void ForwardCurve::clear_curve(){
		curve_.clear();	
	};

	void ForwardCurve::copy_to(my_curve& dst){
		if (!curve_.empty()){
			//std::copy(curve_.begin(), curve_.end(), dst.begin());
			dst = curve_;
		}
	};
//};