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
  KalmanFilter();
  virtual ~KalmanFilter();

  // μt = At μt-1 + Bt ut + εt

                                                   // 
  Matrix<float, 2, 1> currentStateVector = {0, 0}; // de vorige state vector
  Matrix<float, 2, 2> B = {{1, 0}, {0, 1}};   // de transitie matrix met update
  Matrix<float, 2, 1> controlUpdate = {0, 0}; // de control update
  Matrix<float, 2, 1> predictedNoiseVector = {0, 0}; // predicted noise vector

  // Pkp = A * Pkmin1 * At + Qk
  Matrix<float, 2, 2>
      predictedStateCovarianceMatrix; // de voorspelde process covarantie matrix
  Matrix<float, 2, 2> R; //  onzekerheid mbt de process covarantie matrix

  // K = (Pkp * H) / (H * Pkp * Ht + R)
  Matrix<float, 2, 2> identity = {
      {1, 0}, {0, 1}}; // matrix om grootte-verschillen op te lossen ( I )
  Matrix<float, 2, 2> sensorCovarianceMatrix = {
      {1, 0}, {0, 4}}; // de sensor coverantie matrix

  // Yk = C * Xkm + Zk
  Matrix<float, 2, 1> measurementVector; // de gemeten vector
  Matrix<float, 2, 1> Z = {0, 0};

  Matrix<float, 2, 2> currentCovarianceMatrix = {
      {1, 0}, {0, 4}}; // Matrix that stores current covariance of
                       // driving speed and turning speed;

  int time;

  // //--------------------PREDICTED STATE VECTOR--------------------
  // Matrix<float, 2, 2> predictedStateVector;
  // Matrix<float, 2, 2> currentStateVector;

  // Matrix<float, 2, 2> At; // transitie matrix zonder update
  // Matrix<float, 2, 2> Bt; // transitie matrix met update
  // float Ut;               // control update
  // Matrix<float, 2, 2> Et; // predicted noise vector
  // //--------------------------------------------------------------

  // //--------------------PREDICTED COVARIANCE MATRIX--------------------
  // Matrix<float, 2, 2> sigmaT;
  // Matrix<float, 2, 2> Rt; // uncertainty in sigmaT
  // //-------------------------------------------------------------------

  // //--------------------Kalman Gain------------------------------------
  // float Kt;               // kalman gain
  // Matrix<float, 2, 2> Qt; // sensor covariance matrix
  // //-------------------------------------------------------------------

  // //--------------------Measurement Vector-----------------------------
  // Matrix<float, 1, 2> stateVector;
  // Matrix<float, 1, 2> measurementNoise;
  // //-------------------------------------------------------------------

  Matrix<float, 2, 1> getPredictedStateVector(double frontAngle);
  Matrix<float, 2, 2> getPredictedProcessCovariance();
  Matrix<float, 2, 2> calculateKalmanGain();
  void calculateAdjustedStateVector(float measuredOdometer,
                                    float measuredCompass);

  void calculateKalman(double actualAngle, double measuredAngle, double measuredOdom);

private:
};

} // namespace Model
#endif /* KALMANFILTER_HPP_ */
