#include "KalmanFilter.hpp"

namespace Model {

KalmanFilter::KalmanFilter(wxPoint startPosition) {
  belief.at(0).at(0) = startPosition.x;
  belief.at(1).at(0) = startPosition.y;

  error = {{100, 0}, {0, 100}};
}

KalmanFilter::~KalmanFilter() {
  // TODO Auto-generated destructor stub
}

wxPoint KalmanFilter::iterateFilter(
                                    double compassMeasurement,
                                    double odometerMeasurement) {

  double measuredX =
      odometerMeasurement * cos(compassMeasurement) + belief.at(0).at(0);
  double measuredY =
      odometerMeasurement * sin(compassMeasurement) + belief.at(1).at(0);

  Matrix<double, 2, 1> control = {measuredX - belief.at(0).at(0),
                                  measuredY - belief.at(1).at(0)};
  Matrix<double, 2, 1> measuredPosition = {measuredX, measuredY};
  Matrix<double, 2, 1> predictedStateVector = belief + control;
  Matrix<double, 2, 2> predictedProcessCovariance = A * error * A;
  Matrix<double, 2, 2> kalmanGain =
      (predictedProcessCovariance * A) *
      (A * predictedProcessCovariance * A + R).inverse();

  belief = predictedStateVector +
           kalmanGain * (measuredPosition - predictedStateVector);

  error = predictedProcessCovariance * (A - kalmanGain * A);

  int believedX = static_cast<int>(belief.at(0).at(0));
  int believedY = static_cast<int>(belief.at(1).at(0));

  return wxPoint(believedX, believedY);
}

} // namespace Model
