#ifndef LIDARDISTANCESENSOR_HPP_
#define LIDARDISTANCESENSOR_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"
#include "DistancePercept.hpp"
#include "DistanceStimuli.hpp"
#include "DistanceStimulus.hpp"

namespace Model
{
	/**
	 * Compile time configurable length of the Lidar beam
	 */
	const short int lidarBeamLength = 2024;

	class Robot;
	typedef std::shared_ptr<Robot> RobotPtr;

	/**
	 *
	 */
	class LidarDistanceSensor : public AbstractSensor
	{
		public:
			/**
			 *
			 */
			LidarDistanceSensor( Robot& aRobot);

			LidarDistanceSensor(wxPoint aPosition);
			/**
			 *
			 */
			virtual std::shared_ptr< AbstractStimulus > getStimulus() const override;

			virtual std::shared_ptr< AbstractStimulus > getStimulus(wxPoint position) const;
			/**
			 *
			 */
			virtual std::shared_ptr< AbstractPercept > getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const override;
			/**
			 *
			 */
			static void setStdDev(double aStdDev) {LidarDistanceSensor::stddev = aStdDev;}
			/**
			 *
			 */
			static double getStdDev(){ return stddev;}
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
			/**
			 * Standard deviation of the odometer per 10 pixels
			 */
			static double stddev;

			wxPoint position;
	};
} // namespace Model
#endif /* LIDARDISTANCESENSOR_HPP_ */
