#include "OdometerSensor.hpp"

#include "Logger.hpp"
#include "MathUtils.hpp"
#include "OdometerPercept.hpp"
#include "OdometerStimulus.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Shape2DUtils.hpp"
#include "Wall.hpp"

#include <random>
#include <wx/gdicmn.h>

namespace Model {
/**
 *
 */
/* static */ double OdometerSensor::stddev = 1.0;
/**
 *
 */
OdometerSensor::OdometerSensor(Robot &aRobot) : AbstractSensor(aRobot) {}
/**
 *
 */
std::shared_ptr<AbstractStimulus> OdometerSensor::getStimulus() const {
  Robot *robot = dynamic_cast<Robot *>(agent);
  if (robot) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> noise{0, OdometerSensor::stddev};

    wxPoint currentPos = robot->getPosition();
    wxPoint previousPos = robot->getPreviousPosition();

    // calculate distance between 2 positions without shape2dutils
    double distanceX = currentPos.x - previousPos.x;
    double distanceY = currentPos.y - previousPos.y;
    double distanceDriven =
        (std::sqrt(std::pow(distanceX, 2) + std::pow(distanceY, 2)));
    distanceDriven += noise(gen) * distanceDriven / 10.0;

    std::cout << "distanceX: " << distanceX << " | distanceY: " << distanceY
              << " | distanceDriven: " << distanceDriven << std::endl;

    return std::make_shared<OdometerStimulus>(distanceDriven);
  }
}
/**
 *
 */
std::shared_ptr<AbstractPercept> OdometerSensor::getPerceptFor(
    std::shared_ptr<AbstractStimulus> anAbstractStimulus) const {
  Robot *robot = dynamic_cast<Robot *>(agent);
  if (robot) {
    OdometerStimulus *stimulus =
        dynamic_cast<OdometerStimulus *>(anAbstractStimulus.get());
    if (stimulus) {
      double distanceDriven = stimulus->distance;
      return std::make_shared<OdometerPercept>(distanceDriven);
    }
  }
}
/**
 *
 */
std::string OdometerSensor::asString() const { return "OdometerSensor"; }
/**
 *
 */
std::string OdometerSensor::asDebugString() const { return asString(); }
} // namespace Model
