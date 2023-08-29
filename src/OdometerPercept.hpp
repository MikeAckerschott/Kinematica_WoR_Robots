#ifndef ODOMETERPERCEPT_HPP_
#define ODOMETERPERCEPT_HPP_

#include "Config.hpp"

#include "DistanceStimulus.hpp"
#include "Point.hpp"

#include <limits>

namespace Model
{
	/**
	 *
	 */
	class OdometerPercept : public AbstractPercept
	{
		public:
			/**
			 *
			 */
			explicit OdometerPercept( const double& distanceDriven) :
				distanceDriven(distanceDriven)
			{
			}
			double distanceDriven;
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const override
			{
				return "DistancePercept: " + std::to_string(distanceDriven);
			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override
			{
				return asString();
			}
	}; //	class OdometerPercept
} // namespace Model

#endif /* ODOMETERPERCEPT_HPP_ */
