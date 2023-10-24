#include "ParticleFilter.hpp"
#include "DistancePercept.hpp"
#include "MathUtils.hpp"
#include "Particle.hpp"
#include "Shape2DUtils.hpp"

namespace Model
{

  ParticleFilter::ParticleFilter(int numberOfParticles,
                                 LidarDistanceSensor *lidar)
      : lidar(lidar), totalWeight(0.0)
  {
    // TODO Auto-generated constructor stub
    srand(static_cast<unsigned>(time(NULL)));
    for (int i = 0; i < numberOfParticles; i++)
    {
      int x = rand() % 1024;
      int y = rand() % 1024;

      std::shared_ptr<AbstractStimulus> stimulus =
          lidar->getStimulus((wxPoint(x, y)));

      DistanceStimuli *distanceStimuli =
          dynamic_cast<DistanceStimuli *>(stimulus.get());

      this->particles.push_back(Particle(*distanceStimuli, 0, x, y));
    }
  }

  ParticleFilter::~ParticleFilter()
  {
    // TODO Auto-generated destructor stub
  }

  std::vector<wxPoint> ParticleFilter::getParticlePositions()
  {
    std::vector<wxPoint> positions;

    for (long unsigned int i = 0; i < particles.size(); ++i)
    {
      positions.push_back(wxPoint(particles.at(i).x, particles.at(i).y));
    }

    return positions;
  }

  std::vector<unsigned long long> ParticleFilter::getParticleWeights()
  {
    std::vector<unsigned long long> weights;

    for (long unsigned int i = 0; i < particles.size(); ++i)
    {
      weights.push_back(particles.at(i).weight);
    }

    return weights;
  }

  std::vector<Particle> ParticleFilter::getUpdatedParticles()
  {
    std::vector<Particle> updatedParticles;
    std::vector<unsigned long long> weights = getParticleWeights();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<unsigned long long> distribution(weights.begin(),
                                                                weights.end());

    std::uniform_int_distribution<std::mt19937::result_type> addedX(-10, 10);
    std::uniform_int_distribution<std::mt19937::result_type> addedY(-10, 10);

    for (long unsigned int i = 0; i < particles.size(); ++i)
    {
      long long unsigned int index = distribution(gen);

      // weightMultiplier * particles.size() = max weight
      //    int x = (particles.at(index).x + addedX(gen) +
      //            (1 - (particles.at(index).weight /
      //                  (weightMultiplier * particles.size()))) *
      //                addedX(gen));
      //    int y = (particles.at(index).y + addedY(gen) +
      //            (1 - (particles.at(index).weight /
      //                  (weightMultiplier * particles.size()))) *
      //                addedY(gen));

      int x = particles.at(index).x + static_cast<int>(addedX(gen)); // move particle around
      int y = particles.at(index).y + static_cast<int>(addedY(gen)); // move particle around

      // move particle more or less based on current weight
      double totalParticles = static_cast<double>(particles.size());
      double particleWeight = static_cast<double>(particles.at(index).weight);

      x += 1 - (static_cast<int>(particleWeight /
                                 (weightMultiplier * totalParticles)) *
                static_cast<int>(addedX(gen)));
      y += 1 - (static_cast<int>(particleWeight /
                                 (weightMultiplier * totalParticles)) *
                static_cast<int>(addedY(gen)));

      // get current Compass scan
      double currentCompassScan = Model::RobotWorld::getRobotWorld().getRobot("Robot")->getCurrentCompassMeasurement();
      x += static_cast<int>(std::cos(currentCompassScan) * 8);
      y += static_cast<int>(std::sin(currentCompassScan) * 8);

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

  void ParticleFilter::calculateWeight(std::vector<DistancePercept> &lidarScan)
  {

    totalWeight = 0.0;

    for (unsigned long long i = 0; i < particles.size(); ++i)
    {

      long unsigned int iterationSize =
          std::min(particles.at(i).lidarScan.stimuli.size(), lidarScan.size());

      particles[i].weight = 0;

      for (long unsigned int j = 0; j < iterationSize; ++j)
      {

        DistanceStimulus distanceStimulus =
            particles.at(i).lidarScan.stimuli.at(j);

        wxPoint beginpoint(particles.at(i).x, particles.at(i).y);

        double x = static_cast<double>(particles.at(i).x);
        double y = static_cast<double>(particles.at(i).y);

        wxPoint endpoint{static_cast<int>(x +
                                          std::cos(distanceStimulus.angle) *
                                              distanceStimulus.distance),
                         static_cast<int>(y +
                                          std::sin(distanceStimulus.angle) *
                                              distanceStimulus.distance)};

        double particleDistance =
            Utils::Shape2DUtils::distance(beginpoint, endpoint);

        double lidarDistance =
            Utils::Shape2DUtils::distance(wxPoint(particles.at(i).x, particles.at(i).y), lidarScan.at(j).point);

        double distanceDifference = particleDistance - lidarDistance;

        // Define the standard deviation for the Gaussian-like function
        double stdDeviation = 2.0; // Adjust this value as needed

        // Calculate the weight using the Gaussian-like function
        double gaussianWeight =
            std::exp(-0.5 * (distanceDifference * distanceDifference) /
                     (stdDeviation * stdDeviation));

        particles[i].weight += static_cast<unsigned long long>(gaussianWeight * weightMultiplier);
      }

      totalWeight += particles[i].weight;
    }
  }

  wxPoint ParticleFilter::getBelievedPosition()
  {
    // get the average position of all particles
    std::vector<unsigned long long> weights = getParticleWeights();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<unsigned long long> distribution(weights.begin(),
                                                                weights.end());

    long unsigned int totalX = 0;
    long unsigned int totalY = 0;
    for (long long unsigned int i = 0; i < weights.size(); ++i)
    {
      long long unsigned int index = distribution(gen);
      totalX += particles.at(index).x;
      totalY += particles.at(index).y;
    }

    int totalParticles = static_cast<int>(weights.size());
    int believedX = static_cast<int>(totalX / totalParticles);
    int believedY = static_cast<int>(totalY / totalParticles);

    return wxPoint(believedX, believedY);
  }

} // namespace Model
