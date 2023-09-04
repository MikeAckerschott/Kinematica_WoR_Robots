

#include "Particle.hpp"

namespace Model {

Particle::Particle(const DistanceStimuli &lidarScan, unsigned long long weight, long unsigned x, long unsigned y): lidarScan(lidarScan), weight(weight), x(x), y(y) {

} // namespace Model

Particle::Particle(const Particle &other): lidarScan(other.lidarScan), weight(other.weight), x(other.x), y(other.y) {

}

Particle::~Particle() {

}

} // namespace Model
