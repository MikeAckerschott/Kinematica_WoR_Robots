#ifndef COMPASSPERCEPT_HPP_
#define COMPASSPERCEPT_HPP_

#include "Config.hpp"

#include "DistanceStimulus.hpp"
#include "Point.hpp"

#include <limits>

namespace Model
{
	/**
	 *
	 */
	class CompassPercept : public AbstractPercept
	{
		public:
			/**
			 *
			 */
			explicit CompassPercept( const double& angle) :
				angle(angle)
			{
			}
			double angle;
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const override
			{
				return "DistancePercept: " + std::to_string(angle);
			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override
			{
				return asString();
			}
	}; //	class CompassPercept
} // namespace Model

#endif /* COMPASSPERCEPT_HPP_ */
