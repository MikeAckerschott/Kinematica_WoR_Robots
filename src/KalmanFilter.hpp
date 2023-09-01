#ifndef KALMANFILTER_HPP_
#define KALMANFILTER_HPP_

#include "Point.hpp"
#include <cstdlib>
#include <iostream>
#include <random>
#include <wx/gdicmn.h>

#include "Matrix.hpp"

namespace Model {

// class to calculate particle filter from Lidar data
class KalmanFilter {

public:
  explicit KalmanFilter(wxPoint startPosition);
  virtual ~KalmanFilter();

      // We use the real position for starters, not an estimated position.
    Matrix<double, 2, 1> belief;
    Matrix<double, 2, 2> error ;

    const Matrix<double, 2, 2> A = {{1, 0}, {0, 1}};
    const Matrix<double, 2, 2> R = {{1, 0}, {0, 1}}; // sensor covariance matrix

    wxPoint iterateFilter(wxPoint position, wxPoint previousPosition, double compassMeasurement, double odometerMeasurement);

  
};

} // namespace Model
#endif /* KALMANFILTER_HPP_ */
