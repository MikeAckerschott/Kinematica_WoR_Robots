#ifndef PARTICLEFILTER_HPP_
#define PARTICLEFILTER_HPP_

#include "Config.hpp"
#include "DistancePercept.hpp"
#include "DistancePercepts.hpp"
#include "DistanceStimuli.hpp"
#include "LidarDistanceSensor.hpp"
#include "Particle.hpp"

#include <cstdlib>
#include <iostream>
#include <random>

namespace Model {

// class to calculate particle filter from Lidar data
class ParticleFilter {

public:
  ParticleFilter();

  ParticleFilter(ParticleFilter &particleFilter);

  ParticleFilter(int numberOfParticles, LidarDistanceSensor *lidar);

  ParticleFilter &operator=(const ParticleFilter &other);

  virtual ~ParticleFilter();

  // calculate the particle filter
  void calculateWeight(std::vector<DistancePercept> &lidarScan, int x, int y);

  std::vector<Particle> getUpdatedParticles(int speedX, int speedY);

  std::vector<Particle> particles;

  std::vector<wxPoint> getParticlePositions();

  std::vector<unsigned long long> getParticleWeights();

  wxPoint getBelievedPosition();

  LidarDistanceSensor *lidar;

  double totalWeight;

private:
};

} // namespace Model
#endif /* namespace Model */
