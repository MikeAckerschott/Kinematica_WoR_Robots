#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include "Config.hpp"
#include "DistancePercept.hpp"
#include "DistancePercepts.hpp"
#include "DistanceStimuli.hpp"
#include "LidarDistanceSensor.hpp"

#include <cstdlib>
#include <iostream>

namespace Model {

    class Particle{


    public:
        // Particle();
        Particle(DistanceStimuli lidarScan, double weight, int x, int y);
        virtual ~Particle();

        Particle(const Particle &other);

        double weight;
        DistanceStimuli lidarScan;
        int x;
        int y;

    };

} // namespace Model
#endif /* PARTICLE_HPP_ */
