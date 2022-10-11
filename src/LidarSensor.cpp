#include "LidarSensor.hpp"

#include "DistancePercepts.hpp"
#include "Logger.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"
#include "Widgets.hpp"



namespace Model
{
	/**
	 *
	 */
	LidarSensor::LidarSensor()
	{
	}
	/**
	 *
	 */
	LidarSensor::LidarSensor( Robot* aRobot) :
					AbstractSensor( aRobot)
	{
	}
	/**
	 *
	 */
	LidarSensor::~LidarSensor()
	{
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractStimulus > LidarSensor::getStimulus() const
	{
		Robot *robot = dynamic_cast< Robot* >( agent);
		if (robot)
		{
			std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();

			Stimuli stimuli;

			for( int i = 0; i < numberOfBeams ; ++i)
			{
				double angle = (static_cast<float>(i)/numberOfBeams) * (2*Utils::PI);
				Point robotLocation = robot->getPosition();

				Point intersection{-1,-1};
				for (std::shared_ptr< Wall > wall : walls)
				{
					Point wallPoint1 = wall->getPoint1();
					Point wallPoint2 = wall->getPoint2();

					Point laserEndpoint{static_cast< int >( robotLocation.x + std::cos( angle - 0.5 * Utils::PI) * lidarBeamLength),
										static_cast< int >( robotLocation.y + std::sin( angle - 0.5 * Utils::PI) * lidarBeamLength) };

					Point currentIntersection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, robotLocation, laserEndpoint);
					if (currentIntersection != DefaultPosition)
					{
						if(intersection == DefaultPosition)
						{
							intersection = currentIntersection;
						}else if(Utils::Shape2DUtils::distance(robotLocation,currentIntersection) < Utils::Shape2DUtils::distance(robotLocation,intersection))
						{
							intersection = currentIntersection;
						}
					}
				}

				if (intersection != DefaultPosition)
				{
					double distance = Utils::Shape2DUtils::distance( robotLocation, intersection);
					stimuli.push_back(DistanceStimulus( angle, distance));
				} else
				{
					stimuli.push_back(DistanceStimulus( angle, noDistance));
				}
			}
			// Return  the vector with the stimuli
			return std::shared_ptr< AbstractStimulus >( new DistanceStimuli(stimuli));;
		}

		// Return an empty vector
		Application::Logger::log("Huh? no stimuli?");
		return std::shared_ptr< AbstractStimulus >( new DistanceStimuli);
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractPercept > LidarSensor::getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const
	{
		Robot* robot = dynamic_cast< Robot* >( agent);
		if (robot)
		{
			Point robotLocation = robot->getPosition();

			DistanceStimuli* distanceStimuli = dynamic_cast< DistanceStimuli* >( anAbstractStimulus.get());
			if (distanceStimuli)
			{
				PointCloud pointCloud;
				for(const DistanceStimulus& d : distanceStimuli->stimuli)
				{
					Point endPoint;
					if(d.distance == noDistance)
					{
						endPoint.x = noObject;
						endPoint.y = noObject;
					}else
					{
						endPoint.x = static_cast< int >( robotLocation.x + std::cos( d.angle - 0.5 * Utils::PI) * d.distance);
						endPoint.y = static_cast< int >( robotLocation.y + std::sin( d.angle - 0.5 * Utils::PI) * d.distance);
					}
					pointCloud.push_back(DistancePercept(endPoint));
				}
				return std::shared_ptr< AbstractPercept >( new DistancePercepts( pointCloud));
			}
		}
		return std::shared_ptr< AbstractPercept >( new DistancePercepts);
	}
	/**
	 *
	 */
	std::string LidarSensor::asString() const
	{
		return "LidarSensor";
	}
	/**
	 *
	 */
	std::string LidarSensor::asDebugString() const
	{
		return asString();
	}
} // namespace Model
