#include "Robot.hpp"
#include <cmath>

#include "Client.hpp"
#include "CommunicationService.hpp"
#include "CompassPercept.hpp"
#include "CompassSensor.hpp"
#include "DistancePercept.hpp"
#include "Goal.hpp"
#include "LaserDistanceSensor.hpp"
#include "LidarDistanceSensor.hpp"
#include "Logger.hpp"
#include "MainApplication.hpp"
#include "MathUtils.hpp"
#include "Matrix.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"
#include "OdometerPercept.hpp"
#include "OdometerSensor.hpp"
#include "ParticleFilter.hpp"
#include "RobotWorld.hpp"
#include "Server.hpp"
#include "Shape2DUtils.hpp"
#include "Wall.hpp"
#include "WayPoint.hpp"

#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>

#include "KalmanFilter.hpp"

namespace Model {
/**
 *
 */
Robot::Robot() : Robot("", wxDefaultPosition) {}
/**
 *
 */
Robot::Robot(const std::string &aName) : Robot(aName, wxDefaultPosition) {}
/**
 *
 */
Robot::Robot(const std::string &aName, const wxPoint &aPosition)
    : beliefPosition(wxPoint(0, 0)), beliefOrientation(90.0), name(aName),
      size(wxDefaultSize), position(aPosition), front(0, 0), speed(0.0),
      acting(false), driving(false), communicating(false),
      particleFilterEnabled(false), kalmanFilterEnabled(false),

      currentOdomMeasurement(10), currentCompassMeasurement(90) {
  std::shared_ptr<AbstractSensor> laserSensor =
      std::make_shared<LaserDistanceSensor>(*this);
  std::shared_ptr<AbstractSensor> lidarSensor =
      std::make_shared<LidarDistanceSensor>(*this);

  std::shared_ptr<AbstractSensor> compassSensor =
      std::make_shared<CompassSensor>(*this);
  std::shared_ptr<AbstractSensor> odometerSensor =
      std::make_shared<OdometerSensor>(*this);

  attachSensor(laserSensor);
  attachSensor(lidarSensor);
  attachSensor(compassSensor);
  attachSensor(odometerSensor);

  // We use the real position for starters, not an estimated position.
  startPosition = position;
}
/**
 *
 */
Robot::~Robot() {
  if (driving) {
    Robot::stopDriving();
  }
  if (acting) {
    Robot::stopActing();
  }
  if (communicating) {
    stopCommunicating();
  }
}
/**
 *
 */
void Robot::setName(const std::string &aName,
                    bool aNotifyObservers /*= true*/) {
  name = aName;
  if (aNotifyObservers == true) {
    notifyObservers();
  }
}
/**
 *
 */
wxSize Robot::getSize() const { return size; }
/**
 *
 */
void Robot::setSize(const wxSize &aSize, bool aNotifyObservers /*= true*/) {
  size = aSize;
  if (aNotifyObservers == true) {
    notifyObservers();
  }
}
/**
 *
 */
void Robot::setPosition(const wxPoint &aPosition,
                        bool aNotifyObservers /*= true*/) {
  position = aPosition;
  if (aNotifyObservers == true) {
    notifyObservers();
  }
}
/**
 *
 */
BoundedVector Robot::getFront() const { return front; }
/**
 *
 */
void Robot::setFront(const BoundedVector &aVector,
                     bool aNotifyObservers /*= true*/) {
  front = aVector;
  if (aNotifyObservers == true) {
    notifyObservers();
  }
}
/**
 *
 */
// cppcheck-suppress unusedFunction
float Robot::getSpeed() const { return speed; }
/**
 *
 */
void Robot::setSpeed(float aNewSpeed, bool aNotifyObservers /*= true*/) {
  speed = aNewSpeed;
  if (aNotifyObservers == true) {
    notifyObservers();
  }
}
/**
 *
 */
void Robot::startActing() {
  acting = true;
  std::thread newRobotThread([this] { startDriving(); });
  robotThread.swap(newRobotThread);
}
/**
 *
 */
void Robot::stopActing() {
  acting = false;
  driving = false;
  robotThread.join();
}
/**
 *
 */
void Robot::startDriving() {
  driving = true;

  goal = RobotWorld::getRobotWorld().getGoal("Goal");
  calculateRoute(goal);

  if (particleFilterEnabled) {
    driveWithParticlefilter();
  } else if (kalmanFilterEnabled) {
    // TODO add kalman filter
    driveWithKalmanfilter();
  } else {
    drive();
  }
}
/**
 *
 */
void Robot::stopDriving() { driving = false; }
/**
 *
 */
void Robot::startCommunicating() {
  if (!communicating) {
    communicating = true;

    std::string localPort = "12345";
    if (Application::MainApplication::isArgGiven("-local_port")) {
      localPort = Application::MainApplication::getArg("-local_port").value;
    }

    if (Messaging::CommunicationService::getCommunicationService()
            .isStopped()) {
      TRACE_DEVELOP("Restarting the Communication service");
      Messaging::CommunicationService::getCommunicationService().restart();
    }

    server = std::make_shared<Messaging::Server>(
        static_cast<unsigned short>(std::stoi(localPort)), toPtr<Robot>());
    Messaging::CommunicationService::getCommunicationService().registerServer(
        server);
  }
}
/**
 *
 */
void Robot::stopCommunicating() {
  if (communicating) {
    communicating = false;

    std::string localPort = "12345";
    if (Application::MainApplication::isArgGiven("-local_port")) {
      localPort = Application::MainApplication::getArg("-local_port").value;
    }

    Messaging::Client c1ient("localhost",
                             static_cast<unsigned short>(std::stoi(localPort)),
                             toPtr<Robot>());
    Messaging::Message message(Messaging::StopCommunicatingRequest, "stop");
    c1ient.dispatchMessage(message);
  }
}
/**
 *
 */
wxRegion Robot::getRegion() const {
  wxPoint translatedPoints[] = {getFrontRight(), getFrontLeft(), getBackLeft(),
                                getBackRight()};
  return wxRegion(4, translatedPoints); // @suppress("Avoid magic numbers")
}
/**
 *
 */
bool Robot::intersects(const wxRegion &aRegion) const {
  wxRegion region = getRegion();
  region.Intersect(aRegion);
  return !region.IsEmpty();
}
/**
 *
 */
wxPoint Robot::getFrontLeft() const {
  // x and y are pointing to top left now
  int x = position.x - (size.x / 2);
  int y = position.y - (size.y / 2);

  wxPoint originalFrontLeft(x, y);
  double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

  wxPoint frontLeft(
      static_cast<int>((originalFrontLeft.x - position.x) * std::cos(angle) -
                       (originalFrontLeft.y - position.y) * std::sin(angle) +
                       position.x),
      static_cast<int>((originalFrontLeft.y - position.y) * std::cos(angle) +
                       (originalFrontLeft.x - position.x) * std::sin(angle) +
                       position.y));

  return frontLeft;
}
/**
 *
 */
wxPoint Robot::getFrontRight() const {
  // x and y are pointing to top left now
  int x = position.x - (size.x / 2);
  int y = position.y - (size.y / 2);

  wxPoint originalFrontRight(x + size.x, y);
  double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

  wxPoint frontRight(
      static_cast<int>((originalFrontRight.x - position.x) * std::cos(angle) -
                       (originalFrontRight.y - position.y) * std::sin(angle) +
                       position.x),
      static_cast<int>((originalFrontRight.y - position.y) * std::cos(angle) +
                       (originalFrontRight.x - position.x) * std::sin(angle) +
                       position.y));

  return frontRight;
}
/**
 *
 */
wxPoint Robot::getBackLeft() const {
  // x and y are pointing to top left now
  int x = position.x - (size.x / 2);
  int y = position.y - (size.y / 2);

  wxPoint originalBackLeft(x, y + size.y);

  double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

  wxPoint backLeft(
      static_cast<int>((originalBackLeft.x - position.x) * std::cos(angle) -
                       (originalBackLeft.y - position.y) * std::sin(angle) +
                       position.x),
      static_cast<int>((originalBackLeft.y - position.y) * std::cos(angle) +
                       (originalBackLeft.x - position.x) * std::sin(angle) +
                       position.y));

  return backLeft;
}
/**
 *
 */
wxPoint Robot::getBackRight() const {
  // x and y are pointing to top left now
  int x = position.x - (size.x / 2);
  int y = position.y - (size.y / 2);

  wxPoint originalBackRight(x + size.x, y + size.y);

  double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

  wxPoint backRight(
      static_cast<int>((originalBackRight.x - position.x) * std::cos(angle) -
                       (originalBackRight.y - position.y) * std::sin(angle) +
                       position.x),
      static_cast<int>((originalBackRight.y - position.y) * std::cos(angle) +
                       (originalBackRight.x - position.x) * std::sin(angle) +
                       position.y));

  return backRight;
}
/**
 *
 */
void Robot::handleNotification() {
  //	std::unique_lock<std::recursive_mutex> lock(robotMutex);

  static int update = 0;
  if ((++update % 200) == 0) // @suppress("Avoid magic numbers")
  {
    notifyObservers();
  }
}
/**
 *
 */
void Robot::handleRequest(Messaging::Message &aMessage) {
  FUNCTRACE_TEXT_DEVELOP(aMessage.asString());

  switch (aMessage.getMessageType()) {
  case Messaging::StopCommunicatingRequest: {
    aMessage.setMessageType(Messaging::StopCommunicatingResponse);
    aMessage.setBody("StopCommunicatingResponse");
    // Handle the request. In the limited context of this works. I am not sure
    // whether this works OK in a real application because the handling is time
    // sensitive, i.e. 2 async timers are involved: see
    // CommunicationService::stopServer and Server::stopHandlingRequests
    Messaging::CommunicationService::getCommunicationService().stopServer(
        12345, true); // @suppress("Avoid magic numbers")

    break;
  }
  case Messaging::EchoRequest: {
    aMessage.setMessageType(Messaging::EchoResponse);
    aMessage.setBody("Messaging::EchoResponse: " + aMessage.asString());
    break;
  }
  default: {
    TRACE_DEVELOP(__PRETTY_FUNCTION__ +
                  std::string(": default not implemented"));
    break;
  }
  }
}
/**
 *
 */
void Robot::handleResponse(const Messaging::Message &aMessage) {
  FUNCTRACE_TEXT_DEVELOP(aMessage.asString());

  switch (aMessage.getMessageType()) {
  case Messaging::StopCommunicatingResponse: {
    // Messaging::CommunicationService::getCommunicationService().stop();
    break;
  }
  case Messaging::EchoResponse: {
    break;
  }
  default: {
    TRACE_DEVELOP(__PRETTY_FUNCTION__ +
                  std::string(": default not implemented, ") +
                  aMessage.asString());
    break;
  }
  }
}
/**
 *
 */
std::string Robot::asString() const {
  std::ostringstream os;

  os << "Robot " << name << " at (" << position.x << "," << position.y << ")";

  return os.str();
}
/**
 *
 */
std::string Robot::asDebugString() const {
  std::ostringstream os;

  os << "Robot:\n";
  os << AbstractAgent::asDebugString();
  os << "Robot " << name << " at (" << position.x << "," << position.y << ")\n";

  return os.str();
}
/**
 *
 */

void Robot::driveWithKalmanfilter() {

  try {
    for (std::shared_ptr<AbstractSensor> sensor : sensors) {
      sensor->setOn();
    }

    // Compare a float/double with another float/double: use epsilon...
    if (std::fabs(speed - 0.0) <= std::numeric_limits<float>::epsilon()) {
      setSpeed(10.0, false); // @suppress("Avoid magic numbers")
    }

    unsigned pathPoint = 0;
    KalmanFilter filter(this->position);

    previousPosition = position;
    while (position.x > 0 && position.x < 1024 && position.y > 0 &&
           position.y < 1024 &&
           pathPoint < path.size()) // @suppress("Avoid magic numbers")
    {
      // Do the update
      const PathAlgorithm::Vertex &vertex =
          path[pathPoint += static_cast<unsigned int>(speed)];
      front = BoundedVector(vertex.asPoint(), position);
      position.x = vertex.x;
      position.y = vertex.y;

      // Do the measurements / handle all percepts
      // TODO There are race conditions here:
      //			1. size() is not atomic
      //			2. any percepts added after leaving the while
      // will not be used during the belief update
      while (perceptQueue.size() > 0) {
        std::optional<std::shared_ptr<AbstractPercept>> percept =
            perceptQueue.dequeue();
        if (percept) {
          // We cannot dereference the percept in typeid() because clang-tidy
          // gives a warning: warning: expression with side effects will be
          // evaluated despite being used as an operand to 'typeid'
          const AbstractPercept &abstractPercept{*percept.value().get()};

          if (typeid(abstractPercept) ==
              typeid(
                  DistancePercept)) // single percept, this comes from the laser
          {
            DistancePercept *distancePercept =
                dynamic_cast<DistancePercept *>(percept.value().get());
            currentRadarPointCloud.push_back(*distancePercept);
          } else if (typeid(abstractPercept) == typeid(OdometerPercept)) {
            OdometerPercept *odomPercept =
                dynamic_cast<OdometerPercept *>(percept.value().get());
            this->currentOdomMeasurement = odomPercept->distanceDriven;

          } else if (typeid(abstractPercept) == typeid(CompassPercept)) {
            CompassPercept *odomPercept =
                dynamic_cast<CompassPercept *>(percept.value().get());
            this->currentCompassMeasurement = odomPercept->angle;
          }

        } else {
          Application::Logger::log("Huh??");
        }
      }

      if (currentCompassMeasurement != 90) {

        auto beliefAsPoint = filter.iterateFilter(
            this->position, this->previousPosition,
            this->currentCompassMeasurement, this->currentOdomMeasurement);

        Application::Logger::log(
            std::string("BELIEF: ") + std::to_string(beliefAsPoint.x) +
            std::string(", ") + std::to_string(beliefAsPoint.y));
        beliefRoute.push_back(beliefAsPoint);

      }

      // Stop on arrival or collision
      if (arrived(goal) || collision()) {
        Application::Logger::log(__PRETTY_FUNCTION__ +
                                 std::string(": arrived or collision"));
        driving = false;
      }

      notifyObservers();

      // If there is no sleep_for here the robot will immediately be on its
      // destination....
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100)); // @suppress("Avoid magic numbers")

      // this should be the last thing in the loop
      if (driving == false) {
        break;
      }
      previousPosition = position;
    } // while

    for (std::shared_ptr<AbstractSensor> sensor : sensors) {
      sensor->setOff();
    }
  } catch (std::exception &e) {
    Application::Logger::log(__PRETTY_FUNCTION__ + std::string(": ") +
                             e.what());
    std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
  } catch (...) {
    Application::Logger::log(__PRETTY_FUNCTION__ +
                             std::string(": unknown exception"));
    std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
  }
}

/**
 *
 */
void Robot::drive() {

  try {
    for (std::shared_ptr<AbstractSensor> sensor : sensors) {
      sensor->setOn();
    }

    // Compare a float/double with another float/double: use epsilon...
    if (std::fabs(speed - 0.0) <= std::numeric_limits<float>::epsilon()) {
      setSpeed(10.0, false); // @suppress("Avoid magic numbers")
    }

    // We use the real position for starters, not an estimated position.

    unsigned pathPoint = 0;
    while (position.x > 0 && position.x < 1024 && position.y > 0 &&
           position.y < 1024 &&
           pathPoint < path.size()) // @suppress("Avoid magic numbers")
    {
      // Do the update
      const PathAlgorithm::Vertex &vertex =
          path[pathPoint += static_cast<unsigned int>(speed)];
      front = BoundedVector(vertex.asPoint(), position);
      position.x = vertex.x;
      position.y = vertex.y;

      // Do the measurements / handle all percepts
      // TODO There are race conditions here:
      //			1. size() is not atomic
      //			2. any percepts added after leaving the while
      // will not be used during the belief update
      while (perceptQueue.size() > 0) {
        std::optional<std::shared_ptr<AbstractPercept>> percept =
            perceptQueue.dequeue();
        if (percept) {
          // We cannot dereference the percept in typeid() because clang-tidy
          // gives a warning: warning: expression with side effects will be
          // evaluated despite being used as an operand to 'typeid'
          const AbstractPercept &abstractPercept{*percept.value().get()};

          if (typeid(abstractPercept) ==
              typeid(
                  DistancePercept)) // single percept, this comes from the laser
          {
            DistancePercept *distancePercept =
                dynamic_cast<DistancePercept *>(percept.value().get());
            currentRadarPointCloud.push_back(*distancePercept);
          } else {
            Application::Logger::log(std::string("Unknown type of percept:") +
                                     typeid(abstractPercept).name());
          }
        } else {
          Application::Logger::log("Huh??");
        }
      }

      // Update the belief

      // Stop on arrival or collision
      if (arrived(goal) || collision()) {
        Application::Logger::log(__PRETTY_FUNCTION__ +
                                 std::string(": arrived or collision"));
        driving = false;
      }

      notifyObservers();

      // If there is no sleep_for here the robot will immediately be on its
      // destination....
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100)); // @suppress("Avoid magic numbers")

      // this should be the last thing in the loop
      if (driving == false) {
        break;
      }
    } // while

    for (std::shared_ptr<AbstractSensor> sensor : sensors) {
      sensor->setOff();
    }
  } catch (std::exception &e) {
    Application::Logger::log(__PRETTY_FUNCTION__ + std::string(": ") +
                             e.what());
    std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
  } catch (...) {
    Application::Logger::log(__PRETTY_FUNCTION__ +
                             std::string(": unknown exception"));
    std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
  }
}

void Robot::driveWithParticlefilter() {
  try {
    for (std::shared_ptr<AbstractSensor> sensor : sensors) {
      sensor->setOn();
    }

    // Compare a float/double with another float/double: use epsilon...
    if (std::fabs(speed - 0.0) <= std::numeric_limits<float>::epsilon()) {
      setSpeed(10.0, false); // @suppress("Avoid magic numbers")
    }

    // We use the real position for starters, not an estimated position.
    startPosition = position;
    beliefPosition = position;

    unsigned pathPoint = 0;

    LidarDistanceSensor *lidarDistanceSensor =
        dynamic_cast<LidarDistanceSensor *>(sensors[1].get());

    auto particleFilter = ParticleFilter(500, lidarDistanceSensor);
    auto pos = particleFilter.getParticlePositions();
    this->particlePositions = pos;

    while (position.x > 0 && position.x < 1024 && position.y > 0 &&
           position.y < 1024 &&
           pathPoint < path.size()) // @suppress("Avoid magic numbers")
    {
      // Do the update
      const PathAlgorithm::Vertex &vertex =
          path[pathPoint += static_cast<unsigned int>(speed)];
      front = BoundedVector(vertex.asPoint(), position);
      position.x = vertex.x;
      position.y = vertex.y;

      // Do the measurements / handle all percepts
      // TODO There are race conditions here:
      //			1. size() is not atomic
      //			2. any percepts added after leaving the while
      // will not be used during the belief update
      while (perceptQueue.size() > 0) {
        std::optional<std::shared_ptr<AbstractPercept>> percept =
            perceptQueue.dequeue();
        if (percept) {
          // We cannot dereference the percept in typeid() because clang-tidy
          // gives a warning: warning: expression with side effects will be
          // evaluated despite being used as an operand to 'typeid'
          const AbstractPercept &abstractPercept{*percept.value().get()};

          if (typeid(abstractPercept) ==
              typeid(
                  DistancePercept)) // single percept, this comes from the laser
          {
            DistancePercept *distancePercept =
                dynamic_cast<DistancePercept *>(percept.value().get());
            currentRadarPointCloud.push_back(*distancePercept);
          } else if (typeid(abstractPercept) == typeid(DistancePercepts)) {
            DistancePercepts *distancePercepts =
                dynamic_cast<DistancePercepts *>(percept.value().get());
            currentLidarPointCloud = distancePercepts->pointCloud;
          } else {
            Application::Logger::log(std::string("Unknown type of percept:") +
                                     typeid(abstractPercept).name());
          }
        } else {
          Application::Logger::log("Huh??");
        }
      }

      // Update the belief

      particleFilter.calculateWeight(currentLidarPointCloud, getPosition().x,
                                     getPosition().y, beliefPosition);

      int speed = this->getSpeed();
      double angle = Utils::Shape2DUtils::getAngle(getFront());

      int speedX = speed * cos(angle);
      int speedY = speed * sin(angle);

      particleFilter.getUpdatedParticles(speedX, speedY);
      this->particlePositions = particleFilter.getParticlePositions();
      beliefPosition = particleFilter.getBelievedPosition();

      beliefRoute.push_back(beliefPosition);
      beliefOrientation = Utils::Shape2DUtils::getAngle(
          BoundedVector(beliefPosition, beliefRoute[beliefRoute.size() - 2]));

      // Stop on arrival or collision
      if (arrived(goal) || collision()) {
        Application::Logger::log(__PRETTY_FUNCTION__ +
                                 std::string(": arrived or collision"));
        driving = false;
      }

      notifyObservers();

      // If there is no sleep_for here the robot will immediately be on its
      // destination....
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100)); // @suppress("Avoid magic numbers")

      // this should be the last thing in the loop
      if (driving == false) {
        break;
      }
    } // while

    for (std::shared_ptr<AbstractSensor> sensor : sensors) {
      sensor->setOff();
    }
  } catch (std::exception &e) {
    Application::Logger::log(__PRETTY_FUNCTION__ + std::string(": ") +
                             e.what());
    std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
  } catch (...) {
    Application::Logger::log(__PRETTY_FUNCTION__ +
                             std::string(": unknown exception"));
    std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
  }
}
/**
 *
 */
void Robot::calculateRoute(GoalPtr aGoal) {
  path.clear();
  if (aGoal) {
    // Turn off logging if not debugging AStar
    Application::Logger::setDisable();

    front = BoundedVector(aGoal->getPosition(), position);
    // handleNotificationsFor( astar);
    path = astar.search(position, aGoal->getPosition(), size);
    // stopHandlingNotificationsFor( astar);

    Application::Logger::setDisable(false);
  }
}
/**
 *
 */
bool Robot::arrived(GoalPtr aGoal) {
  if (aGoal && intersects(aGoal->getRegion())) {
    return true;
  }
  return false;
}
/**
 *
 */
bool Robot::collision() {
  wxPoint frontLeft = getFrontLeft();
  wxPoint frontRight = getFrontRight();
  wxPoint backLeft = getBackLeft();
  wxPoint backRight = getBackRight();

  const std::vector<WallPtr> &walls = RobotWorld::getRobotWorld().getWalls();
  for (WallPtr wall : walls) {
    if (Utils::Shape2DUtils::intersect(frontLeft, frontRight, wall->getPoint1(),
                                       wall->getPoint2()) ||
        Utils::Shape2DUtils::intersect(frontLeft, backLeft, wall->getPoint1(),
                                       wall->getPoint2()) ||
        Utils::Shape2DUtils::intersect(frontRight, backRight, wall->getPoint1(),
                                       wall->getPoint2()))
    // cppcheck-suppress useStlAlgorithm
    {
      return true;
    }
  }
  const std::vector<RobotPtr> &robots = RobotWorld::getRobotWorld().getRobots();
  for (RobotPtr robot : robots) {
    if (getObjectId() == robot->getObjectId()) {
      continue;
    }
    if (intersects(robot->getRegion())) {
      return true;
    }
  }
  return false;
}

bool Robot::isKalmanFilterActive() const { return kalmanFilterEnabled; }

void Robot::setKalmanFilterActive(bool kalmanFilterActive) {
  this->kalmanFilterEnabled = kalmanFilterActive;
}

void Robot::setParticleFilterActive(bool particleFilterActive) {
  this->particleFilterEnabled = particleFilterActive;
}

bool Robot::isParticleFilterActive() const { return particleFilterEnabled; }

wxPoint Robot::getBelievedPosition() const { return beliefPosition; }

std::vector<wxPoint> Robot::getBelievedRoute() const { return beliefRoute; }

wxPoint Robot::getPreviousPosition() { return previousPosition; }

} // namespace Model
