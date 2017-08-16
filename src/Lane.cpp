#include <fstream>
#include "Lane.h"

using namespace std;

double Lane::score() {
	closestCarAheadS = numeric_limits<double>::max();
	closestCarAheadSpeed = MAX_VEL;
	closestCarBehindS = numeric_limits<double>::max();
	closestCarBehindSpeed = MAX_VEL;

  double closest_s = numeric_limits<double>::max();
	for (int j = 0; j < vehicles.size(); j++) {
		Vehicle v = vehicles[j];
		cout << "   car " << v.id << " v.s " << v.s << " v.speed " << v.speed << endl;
		cout << "   car_s " << car_s << endl;

    // Adjust for circuit finishing and vehicle's s position setting cycling back to 0
    bool carActuallyAhead = (car_s - v.s > 1000);
    bool carAhead = (v.s >= car_s) && (v.s - car_s <= 1000);
    double deltaFront = 0.0;
    if (carActuallyAhead) {
      deltaFront = 7000 - car_s + v.s;
    } else if (carAhead){
      deltaFront = v.s - car_s;
    }

    bool carActuallyBehind = (v.s - car_s > 1000);
    bool carBehind = (car_s >= v.s) && (car_s - v.s <= 1000);
    double deltaBehind = 0.0;
    if (carActuallyBehind) {
      deltaBehind = 7000 - v.s + car_s;
    } else if (carBehind) {
      deltaBehind = car_s - v.s;
    }

    if (carAhead || carActuallyAhead) {
      if (deltaFront < closestCarAheadS) {
        closestCarAheadS = deltaFront;
        closestCarAheadSpeed = v.speed;
        if (v.speed < car_speed) {
          numVehiclesSlowerAhead += 1.0;
        }
      }
    } else {
      if (deltaBehind < closestCarBehindS) {
        closestCarBehindS = deltaBehind;
        closestCarBehindSpeed = v.speed;
      }
    }
	}
	cout << "  closestCarAheadSpeed " << closestCarAheadSpeed << " car_speed " << car_speed << endl;
	cout << "  closestCarAheadS " << closestCarAheadS << " car_s " << car_s << endl;
	cout << "  closestCarBehindSpeed " << closestCarBehindSpeed << " car_speed " << car_speed << endl;
	cout << "  closestCarBehindS " << closestCarBehindS << " car_s " << car_s << endl;

	score_val = laneScore() + frontVehicleDistanceScore() + frontVehicleSpeedScore()
							+ behindVehicleDistanceScore() + behindVehicleSpeedScore() + slowVechiclesScore();
	return score_val;
}

Lane::Lane(double car_speed, double car_s, int id) {
	this->car_speed = car_speed;
	this->car_s = car_s;
	this->id = id;
}

double Lane::laneScore() {
	return (1.1 * (1.0 - ((abs(id - lane)) / 2.0)));
}

double Lane::frontVehicleDistanceScore() {
  const double refDist = S_SAFE_DELTA;
	return (1.0 * (1.0 - abs(S_SAFE_DELTA - min(closestCarAheadS, refDist)) / S_SAFE_DELTA));
}

double Lane::frontVehicleSpeedScore() {
	return (1.0 * (1.0 - abs(MAX_VEL - closestCarAheadSpeed) / MAX_VEL));
}

double Lane::behindVehicleDistanceScore() {
  const double refDist = S_SAFE_DELTA;
  return (1.0 * (1.0 - abs(S_SAFE_DELTA - min(closestCarBehindS, refDist)) / S_SAFE_DELTA));
}

double Lane::behindVehicleSpeedScore() {
	return (1.0 * (1.0 - abs(MAX_VEL - closestCarBehindSpeed) / MAX_VEL));
}

double Lane::slowVechiclesScore() {
  int num_v = vehicles.size();
  if (num_v > 0) {
    return (0.75 * (1.0 - numVehiclesSlowerAhead / vehicles.size()));
  } else {
    return 0.75;
  }
}

bool Lane::isChangeFeasible() {
  const double refVelocity = MAX_VEL;
  double safeDistTillCarCoversDelta = (closestCarAheadS / min(car_speed, refVelocity)) * closestCarAheadSpeed;
  if (safeDistTillCarCoversDelta < S_DELTA) {
    return false;
  }

  double safeDistTillBehindCoversDelta = (closestCarBehindS / closestCarBehindSpeed) * car_speed;
  if (safeDistTillBehindCoversDelta < S_DELTA) {
    return false;
  }

	return true;
}