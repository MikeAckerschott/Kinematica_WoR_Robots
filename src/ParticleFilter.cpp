#include "ParticleFilter.hpp"
#include "DistancePercept.hpp"
#include "Shape2DUtils.hpp"

namespace Model {

ParticleFilter::ParticleFilter(int numberOfParticles,
                               LidarDistanceSensor *lidar)
    : lidar(lidar) {
  // TODO Auto-generated constructor stub
  srand((unsigned)time(NULL));
  for (int i = 0; i < numberOfParticles; i++) {
    int x = rand() % 1000;
    int y = rand() % 1000;

    // std::cout << "x: " << x << " y: " << y << std::endl;

    std::shared_ptr<AbstractStimulus> stimulus =
        lidar->getStimulus((wxPoint(x, y)));

    DistanceStimuli *distanceStimuli =
        dynamic_cast<DistanceStimuli *>(stimulus.get());

    this->particles.push_back(Particle(*distanceStimuli, 0.0, x, y));
  }
}

ParticleFilter::~ParticleFilter() {
  // TODO Auto-generated destructor stub
}

ParticleFilter::ParticleFilter() {
  // TODO Auto-generated constructor stub
}

ParticleFilter &ParticleFilter::operator=(const ParticleFilter &other) {
  // TODO Auto-generated constructor stub
}

std::vector<wxPoint> ParticleFilter::getParticlePositions() {
  std::vector<wxPoint> positions;

  for (int i = 0; i < particles.size(); ++i) {
    positions.push_back(wxPoint(particles.at(i).x, particles.at(i).y));
  }

  return positions;
}

std::vector<Particle> ParticleFilter::getUpdatedParticles() {
  std::vector<Particle> updatedParticles;

  for (int i = 0; i < particles.size(); ++i) {

    particles.at(i).x += rand() % 10 - 5;
    particles.at(i).y += rand() % 10 - 5;
  }

  return updatedParticles;
}

void ParticleFilter::calculateWeight(std::vector<DistancePercept> &lidarScan) {

  std::cout << "lidarScan.size(): " << lidarScan.size() << std::endl;
  std::cout << "particles.size(): " << particles.size() << std::endl;

  for (int i = 0; i < particles.size(); ++i) {
    double weight = 0.0;

    int iterationSize =
        std::min(particles.at(i).lidarScan.stimuli.size(), lidarScan.size());
    // std::cout << iterationSize << std::endl;

    particles[i].weight = 0.0;

    for (int j = 0; j < iterationSize; ++j) {
      //       //   double distance =
      //       //
      //       Utils::Shape2DUtils::distance(particles.at(i).lidarScan.stimuli.at(j).position,
      //       // lidarScan[j].position);

      DistanceStimulus distanceStimulus =
          particles.at(i).lidarScan.stimuli.at(j);

      wxPoint endpoint{static_cast<int>(particles.at(i).x +
                                        std::cos(distanceStimulus.angle) *
                                            distanceStimulus.distance),
                       static_cast<int>(particles.at(i).y +
                                        std::sin(distanceStimulus.angle) *
                                            distanceStimulus.distance)};

      double distance =
          Utils::Shape2DUtils::distance(endpoint, lidarScan.at(j).point);

      // calculate weight where less difference equals more weight
      particles[i].weight += (sqrt(distance));

      std::cout << "weight: " << particles[i].weight << std::endl;
    }
    // particles[i].weight += i;
    // std::cout << "weight particle " << i << ": " << particles[i].weight
    //           << std::endl;
  }
}

} // namespace Model