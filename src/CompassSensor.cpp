#include "CompassSensor.hpp"

#include "AbstractSensor.hpp"
#include "CompassPercept.hpp"
#include "CompassStimulus.hpp"
#include "Logger.hpp"
#include "MathUtils.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Shape2DUtils.hpp"
#include "Wall.hpp"

#include <random>

namespace Model {
/**
 *
 */
/* static */ double CompassSensor::stddev = 2.0;
/**
 *
 */
CompassSensor::CompassSensor(Robot &aRobot) : AbstractSensor(aRobot) {}
/**
 *
 */
std::shared_ptr<AbstractStimulus> CompassSensor::getStimulus() const {
  Robot *robot = dynamic_cast<Robot *>(agent);
  if (robot) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> noise{0, CompassSensor::stddev};

    auto test = robot->getFront();
    auto testAsAngle = Utils::Shape2DUtils::getAngle(test);

    double angle = Utils::MathUtils::toRadians(
        Utils::MathUtils::toDegrees(
            Utils::Shape2DUtils::getAngle(robot->getFront())) +
        (noise(gen))); // TODO fix noise as degrees not as radians!

    return std::make_shared<CompassStimulus>(angle);
  }
}
/**
 *
 */
std::shared_ptr<AbstractPercept> CompassSensor::getPerceptFor(
    std::shared_ptr<AbstractStimulus> anAbstractStimulus) const {
  Robot *robot = dynamic_cast<Robot *>(agent);
  if (robot) {
    CompassStimulus *stimulus =
        dynamic_cast<CompassStimulus *>(anAbstractStimulus.get());
    if (stimulus) {
      double angle = stimulus->angle;
      return std::make_shared<CompassPercept>(angle);
    }
  }
}
/**
 *
 */
std::string CompassSensor::asString() const { return "CompassSensor"; }
/**
 *
 */
std::string CompassSensor::asDebugString() const { return asString(); }
} // namespace Model
