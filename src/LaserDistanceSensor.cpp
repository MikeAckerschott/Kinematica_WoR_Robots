#include "LaserDistanceSensor.hpp"

#include "Logger.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"
#include "Widgets.hpp"

#include <random>

namespace Model
{
	/**
	 *
	 */
	/* static */ double LaserDistanceSensor::stddev = 10.0;
	/**
	 *
	 */
	LaserDistanceSensor::LaserDistanceSensor()
	{
	}
	/**
	 *
	 */
	LaserDistanceSensor::LaserDistanceSensor( Robot* aRobot) :
								AbstractSensor( aRobot)
	{
	}
	/**
	 *
	 */
	LaserDistanceSensor::~LaserDistanceSensor()
	{
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractStimulus > LaserDistanceSensor::getStimulus() const
	{
		Robot* robot = dynamic_cast<Robot*>(agent);
		if(robot)
		{
			std::random_device rd{};
			std::mt19937 gen{rd()};
		    std::normal_distribution<> noise{0,LaserDistanceSensor::stddev};

			double angle = Utils::Shape2DUtils::getAngle( robot->getFront());

			std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();
			for (std::shared_ptr< Wall > wall : walls)
			{
				Point wallPoint1 = wall->getPoint1();
				Point wallPoint2 = wall->getPoint2();
				Point robotLocation = robot->getPosition();
				Point laserEndpoint{static_cast<int>(robotLocation.x + std::cos( angle) * laserBeamLength + noise(gen)) ,
									static_cast<int>(robotLocation.y + std::sin( angle) * laserBeamLength + noise(gen))};

				Point interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, robotLocation, laserEndpoint);

				if(interSection != DefaultPosition)
				{
					double distance = Utils::Shape2DUtils::distance(robotLocation,interSection);
					return std::shared_ptr< AbstractStimulus >( new DistanceStimulus( angle,distance));
				}
			}
			std::shared_ptr< AbstractStimulus > distanceStimulus( new DistanceStimulus( noAngle,noDistance));
			return distanceStimulus;
		}
		std::shared_ptr< AbstractStimulus > distanceStimulus( new DistanceStimulus( noAngle,noDistance));
		return distanceStimulus;
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractPercept > LaserDistanceSensor::getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const
	{
		Robot* robot = dynamic_cast< Robot* >( agent);
		if (robot)
		{
			Point robotLocation = robot->getPosition();

			DistanceStimulus* distanceStimulus = dynamic_cast< DistanceStimulus* >( anAbstractStimulus.get());
			if(distanceStimulus)
			{
				if(distanceStimulus->distance == noDistance)
				{
					return std::shared_ptr< AbstractPercept > ( new DistancePercept( Point(noObject,noObject)));
				}
				Point endpoint{	static_cast< int >( robotLocation.x + std::cos( distanceStimulus->angle)*distanceStimulus->distance),
								static_cast< int >( robotLocation.y + std::sin( distanceStimulus->angle)*distanceStimulus->distance)};

				return std::shared_ptr< AbstractPercept >( new DistancePercept( endpoint));
			}
		}

		return std::shared_ptr< AbstractPercept > ( new DistancePercept( Point(invalidDistance,invalidDistance)));
	}
	/**
	 *
	 */
	std::string LaserDistanceSensor::asString() const
	{
		return "LaserDistanceSensor";
	}
	/**
	 *
	 */
	std::string LaserDistanceSensor::asDebugString() const
	{
		return asString();
	}
} // namespace Model
