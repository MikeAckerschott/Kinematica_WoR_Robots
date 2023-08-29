#ifndef ODOMETERSTIMULUS_HPP_
#define ODOMETERSTIMULUS_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"

#include <limits>

namespace Model {
/**
 *
 */
class OdometerStimulus : public AbstractStimulus {
public:
  /**
   *
   */
  OdometerStimulus(double aDistance) : distance(aDistance) {}
  double distance;
  /**
   * @name Debug functions
   */
  //@{
  /**
   * Returns a 1-line description of the object
   */
  virtual std::string asString() const override {
    return "OdometerStimulus: " + std::to_string(distance);
  }
  /**
   * Returns a description of the object with all data of the object usable for
   * debugging
   */
  virtual std::string asDebugString() const override { return asString(); }
}; // class DistanceStimulus
} // namespace Model

#endif /* DISTANCESTIMULUS_HPP_ */
