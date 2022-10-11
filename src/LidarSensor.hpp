#ifndef LIDARSENSOR_HPP_
#define LIDARSENSOR_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"
#include "DistancePercept.hpp"

namespace Model
{
	/**
	 * Compile time configurable length of the laser beam
	 */
	const short int lidarBeamLength = 1024;

	/**
	 * Compile time configurable number of beams in a circle
	 */
	const short int numberOfBeams = 180;


	class Robot;
	typedef std::shared_ptr<Robot> RobotPtr;

	/**
	 *
	 */
	class LidarSensor : public AbstractSensor
	{
		public:
			/**
			 *
			 */
			LidarSensor();
			/**
			 *
			 */
			explicit LidarSensor( Robot* aRobot);
			/**
			 *
			 */
			virtual ~LidarSensor();
			/**
			 * @see AbstractSensor::getStimulus
			 */
			virtual std::shared_ptr< AbstractStimulus > getStimulus() const override;
			/**
			 * @see AbstractSensor::getPerceptFor
			 */
			virtual std::shared_ptr< AbstractPercept > getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const override;
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const override;
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override;
			//@}
		protected:
		private:

	};
} // namespace Model
#endif /* LIDARSENSOR_HPP__HPP_ */
