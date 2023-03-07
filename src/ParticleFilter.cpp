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

    this->particles.push_back(Particle(*distanceStimuli, 0, x, y));
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

std::vector<unsigned long long> ParticleFilter::getParticleWeights() {
  std::vector<unsigned long long> weights;

  for (int i = 0; i < particles.size(); ++i) {
    weights.push_back(particles.at(i).weight);
  }

  return weights;
}

std::vector<Particle> ParticleFilter::getUpdatedParticles(int speedX,
                                                          int speedY) {
  std::vector<Particle> updatedParticles;
  std::cout << "speedX: " << speedX << " speedY: " << speedY << std::endl;
  std::vector<unsigned long long> weights = getParticleWeights();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<unsigned long long> distribution(weights.begin(),
                                                              weights.end());

  std::uniform_int_distribution<std::mt19937::result_type> randomX(-25, 25);
  std::uniform_int_distribution<std::mt19937::result_type> randomY(-25, 25);

  for (int i = 0; i < particles.size(); ++i) {
    int index = distribution(gen);

    int x = particles.at(index).x + randomX(gen);
    int y = particles.at(index).y + randomY(gen);

    std::shared_ptr<AbstractStimulus> stimulus =
        lidar->getStimulus((wxPoint(x, y)));

    DistanceStimuli *distanceStimuli =
        dynamic_cast<DistanceStimuli *>(stimulus.get());

    particles.at(index).x = x;
    particles.at(index).y = y;
    particles.at(index).lidarScan = *distanceStimuli;

    updatedParticles.push_back(particles.at(index));
  }

  this->particles = updatedParticles;
  return updatedParticles;
}

void ParticleFilter::calculateWeight(std::vector<DistancePercept> &lidarScan,
                                     int x, int y) {

  // std::cout << "lidarScan.size(): " << lidarScan.size() << std::endl;
  // std::cout << "particles.size(): " << particles.size() << std::endl;

  totalWeight = 0.0;

  for (int i = 0; i < particles.size(); ++i) {
    double weight = 0.0;

    int iterationSize =
        std::min(particles.at(i).lidarScan.stimuli.size(), lidarScan.size());
    // std::cout << iterationSize << std::endl;

    particles[i].weight = 0;

    for (int j = 0; j < iterationSize; ++j) {
      //       //   double distance =
      //       //
      //       Utils::Shape2DUtils::distance(particles.at(i).lidarScan.stimuli.at(j).position,
      //       // lidarScan[j].position);

      DistanceStimulus distanceStimulus =
          particles.at(i).lidarScan.stimuli.at(j);

      wxPoint beginpoint(particles.at(i).x, particles.at(i).y);

      wxPoint endpoint{static_cast<int>(particles.at(i).x +
                                        std::cos(distanceStimulus.angle) *
                                            distanceStimulus.distance),
                       static_cast<int>(particles.at(i).y +
                                        std::sin(distanceStimulus.angle) *
                                            distanceStimulus.distance)};

      double particleDistance =
          Utils::Shape2DUtils::distance(beginpoint, endpoint);

      double lidarDistance =
          Utils::Shape2DUtils::distance(wxPoint(x, y), lidarScan.at(j).point);

      particles[i].weight +=
          1 / sqrt(pow(particleDistance - lidarDistance, 2)) * 10000;

      // std::cout << "weight: " << particles[i].weight << std::endl;
    }

    totalWeight += particles[i].weight;
    // particles[i].weight += i;
    // std::cout << "weight particle " << i << ": " << particles[i].weight
    //           << std::endl;
  }
}

} // namespace Model