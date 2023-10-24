#include "LidarDistanceSensor.hpp"
#include "DistanceStimuli.hpp"
#include "DistanceStimulus.hpp"

#include "Logger.hpp"
#include "MathUtils.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Shape2DUtils.hpp"
#include "Wall.hpp"

#include <random>

namespace Model
{
  /**
   *
   */
  /* static */ double LidarDistanceSensor::stddev = 10.0;
  /**
   *
   */
  LidarDistanceSensor::LidarDistanceSensor(Robot &aRobot)
      : AbstractSensor(aRobot) {}
  /**
   *
   */
  std::shared_ptr<AbstractStimulus> LidarDistanceSensor::getStimulus() const
  {
    Robot *robot = dynamic_cast<Robot *>(agent);
    std::vector<DistanceStimulus> stimuli;

    if (robot)
    {
      std::random_device rd{};
      std::mt19937 gen{rd()};
      std::normal_distribution<> noise{0, LidarDistanceSensor::stddev};

      double angle = 0;

      for (int i = 0; i < 180; ++i)
      {
        double distance = 2000;

        angle += Utils::MathUtils::toRadians(2);

        std::vector<WallPtr> walls = RobotWorld::getRobotWorld().getWalls();
        for (std::shared_ptr<Wall> wall : walls)
        {
          wxPoint wallPoint1 = wall->getPoint1();
          wxPoint wallPoint2 = wall->getPoint2();
          wxPoint robotLocation = robot->getPosition();
          wxPoint laserEndpoint{
              static_cast<int>(robotLocation.x +
                               std::cos(angle) * lidarBeamLength),
              static_cast<int>(robotLocation.y +
                               std::sin(angle) * lidarBeamLength)};

          wxPoint interSection = Utils::Shape2DUtils::getIntersection(
              wallPoint1, wallPoint2, robotLocation, laserEndpoint);

          if (interSection != wxDefaultPosition &&
              Utils::Shape2DUtils::distance(robotLocation, interSection) <
                  distance)
          {
            distance = Utils::Shape2DUtils::distance(robotLocation, interSection);
            // stimuli.push_back(DistanceStimulus(angle, distance));
          }
        }
        if (distance == 2000)
        {
          distance = lidarBeamLength;
        }

        distance += noise(gen);
        stimuli.push_back(DistanceStimulus(angle, distance));
      }
      return std::make_shared<DistanceStimuli>(stimuli);
    }
    //   return std::make_shared<DistanceStimuli>(stimulus);
    return std::make_shared<DistanceStimuli>(stimuli);
  }

  std::shared_ptr<AbstractStimulus>
  LidarDistanceSensor::getStimulus(wxPoint position) const
  {

    // Robot *robot = dynamic_cast<Robot *>(agent);
    std::vector<DistanceStimulus> stimuli;

    // if (robot) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> noise{0, LidarDistanceSensor::stddev};

    double angle = 0;

    for (int i = 0; i < 180; ++i)
    {
      double distance = 2000;

      angle += Utils::MathUtils::toRadians(2);

      std::vector<WallPtr> walls = RobotWorld::getRobotWorld().getWalls();
      for (std::shared_ptr<Wall> wall : walls)
      {
        wxPoint wallPoint1 = wall->getPoint1();
        wxPoint wallPoint2 = wall->getPoint2();
        wxPoint laserEndpoint{
            static_cast<int>(position.x +
                             std::cos(angle) * lidarBeamLength + noise(gen)),
            static_cast<int>(position.y +
                             std::sin(angle) * lidarBeamLength + noise(gen))};

        wxPoint interSection = Utils::Shape2DUtils::getIntersection(
            wallPoint1, wallPoint2, position, laserEndpoint);

        if (interSection != wxDefaultPosition &&
            Utils::Shape2DUtils::distance(position, interSection) <
                distance)
        {
          distance = Utils::Shape2DUtils::distance(position, interSection);
          // stimuli.push_back(DistanceStimulus(angle, distance));
        }
      }
      if (distance == 2000)
      {
        distance = lidarBeamLength;
      }
      stimuli.push_back(DistanceStimulus(angle, distance));
    }
    return std::make_shared<DistanceStimuli>(stimuli);
    // }
  }
  /**
   *
   */
  std::shared_ptr<AbstractPercept> LidarDistanceSensor::getPerceptFor(
      std::shared_ptr<AbstractStimulus> anAbstractStimulus) const
  {
    Robot *robot = dynamic_cast<Robot *>(agent);
    std::vector<DistancePercept> percepts;
    if (robot)
    {
      wxPoint robotLocation = robot->getPosition();

      DistanceStimuli *distanceStimuli =
          dynamic_cast<DistanceStimuli *>(anAbstractStimulus.get());
      if (distanceStimuli)
      {
        for (DistanceStimulus stimulus : distanceStimuli->stimuli)
        {
          if (stimulus.distance == noDistance)
          {
            //   return std::make_shared<DistancePercept>(wxPoint(noObject,
            //   noObject));
            percepts.push_back(DistancePercept(wxPoint(noObject, noObject)));
          }
          wxPoint endpoint{
              static_cast<int>(robotLocation.x +
                               std::cos(stimulus.angle) * stimulus.distance),
              static_cast<int>(robotLocation.y +
                               std::sin(stimulus.angle) * stimulus.distance)};

          // return std::make_shared<DistancePercept>(endpoint);
          percepts.push_back(DistancePercept(endpoint));
        }
      }
    }
    return std::make_shared<DistancePercepts>(percepts);
    //   return std::make_shared<DistancePercept>(
    //   wxPoint(invalidDistance, invalidDistance));
  }
  /**
   *
   */
  std::string LidarDistanceSensor::asString() const
  {
    return "LidarDistanceSensor";
  }
  /**
   *
   */
  std::string LidarDistanceSensor::asDebugString() const { return asString(); }
} // namespace Model
