#ifndef COMPASSSTIMULUS_HPP_
#define COMPASSSTIMULUS_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"

#include <limits>

namespace Model {
/**
 *
 */
class CompassStimulus : public AbstractStimulus {
public:
  /**
   *
   */
  explicit CompassStimulus(double angle) : angle(angle) {}
  double angle;
  /**
   * @name Debug functions
   */
  //@{
  /**
   * Returns a 1-line description of the object
   */
  virtual std::string asString() const override {
    return "CompassStimulus: " + std::to_string(angle);
  }
  /**
   * Returns a description of the object with all data of the object usable for
   * debugging
   */
  virtual std::string asDebugString() const override { return asString(); }
}; // class DistanceStimulus
} // namespace Model

#endif /* COMPASSSTIMULUS_HPP_ */
