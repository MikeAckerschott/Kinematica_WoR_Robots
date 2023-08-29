#include "KalmanFilter.hpp"

namespace Model {

KalmanFilter::KalmanFilter() { currentStateVector = {10, 0}; }

KalmanFilter::~KalmanFilter() {
  // TODO Auto-generated destructor stub
}

Matrix<float, 2, 1> KalmanFilter::getPredictedStateVector(double actualAngle) {
  Matrix<float, 2, 2> A = {{1, 0}, {0, 1}};
  Matrix<float, 2, 2> B = {{1, 0}, {0, 1}};

  Matrix<float, 2, 1> Uk = {10 - currentStateVector.at(0).at(0),
                            actualAngle - currentStateVector.at(1).at(0)};

std::cout<<"1"<<"currentStateVector: "<<currentStateVector.at(0).at(0)<<" - " << currentStateVector.at(1).at(0)<<std::endl;

  return A * currentStateVector + B * Uk;
}

Matrix<float, 2, 2> KalmanFilter::getPredictedProcessCovariance() {
  // std::cout<<"2"<<"currentStateVector: "<<currentStateVector.at(0).at(0)<<" - " << currentStateVector.at(1).at(0)<<std::endl;
  // return A * currentCovarianceMatrix * A.transpose();
  return {{1, 0}, {0, 1}};
}

Matrix<float, 2, 2> KalmanFilter::calculateKalmanGain() {
  auto predictedProcessCovariance = getPredictedProcessCovariance();

  // get the sensor covariance matrix
  Matrix<float, 2, 2> sensorCovarianceMatrix = {
      {1, 0}, {0, 4}}; // de sensor coverantie matrix

  Matrix<float, 2, 2> temp =  predictedProcessCovariance *
         ((sensorCovarianceMatrix * predictedProcessCovariance *
           sensorCovarianceMatrix.transpose()) +
          sensorCovarianceMatrix);
std::cout<<"3"<<"currentStateVector: "<<currentStateVector.at(0).at(0)<<" - " << currentStateVector.at(1).at(0)<<std::endl;
          return temp.inverse();
}

void KalmanFilter::calculateAdjustedStateVector(float measuredOdometer,
                                                float measuredCompass) {

  // cast measuredOdometer and measuredCompass to float
  float odomMeasurement = float(measuredOdometer);
  float compassMeasurement = float(measuredCompass);

  Matrix<float, 2, 1> measurement = {odomMeasurement, compassMeasurement};
  Matrix<float, 2, 2> tempIdentity = {{1, 0}, {0, 1}};
  auto measurementIdentity = tempIdentity * measurement;
  Matrix<float, 2, 1> predictedStateVector =
      getPredictedStateVector(measuredCompass);
  Matrix<float, 2, 2> kalmanGain = calculateKalmanGain();

  currentStateVector =
      predictedStateVector +
      (kalmanGain * (measurementIdentity - predictedStateVector));

  currentCovarianceMatrix = (tempIdentity - kalmanGain) * currentCovarianceMatrix;
}

void KalmanFilter::calculateKalman(double actualAngle, double measuredAngle, double measuredOdom){

std::cout<<"angle: " <<actualAngle<<std::endl;

  Matrix<float, 2, 1> predictedStateVector = getPredictedStateVector(actualAngle);
  std::cout<<"predictedStateVector: "<<predictedStateVector<<std::endl;
  Matrix<float, 2, 2> predictedProcessCovariance = getPredictedProcessCovariance();
  std::cout<<"predictedProcessCovariance: "<<predictedProcessCovariance<<std::endl;
  Matrix<float, 2, 2> kalmanGain = calculateKalmanGain();
  std::cout<<"kalmanGain: "<<kalmanGain<<std::endl;

  std::cout<<"predictedStateVector: "<<predictedStateVector<<std::endl;
    measurementVector = {measuredOdom, measuredAngle};
  std::cout<<"measurementVector: "<<measurementVector<<std::endl;



  currentStateVector = predictedStateVector + (kalmanGain * (measurementVector - predictedStateVector));
  std::cout<<"4"<<"currentStateVector: "<<currentStateVector.at(0).at(0)<<" - " << currentStateVector.at(1).at(0)<<std::endl;
  
  if(currentStateVector.at(0).at(0) < 0){
    while(true);
  }
  
  currentCovarianceMatrix = (identity - kalmanGain) * predictedProcessCovariance;

}

} // namespace Model