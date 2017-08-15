#include <fstream>
#include "Lane.h"

using namespace std;

double Lane::score() {
	closestCarAheadS = S_SAFE_DELTA;
	closestCarAheadSpeed = MAX_VEL;
	closestCarBehindS = S_SAFE_DELTA;
	closestCarBehindSpeed = MAX_VEL;

	for (int j = 0; j < vehicles.size(); j++) {
		Vehicle v = vehicles[j];
		std::__1::cout << "car " << v.id << " v.s " << v.s << " v.speed " << v.speed << std::__1::endl;
		std::__1::cout << "car_s " << car_s << std::__1::endl;

		if (v.s > car_s) { // car ahead
			if (v.s - car_s < closestCarAheadS) {
				closestCarAheadS = v.s - car_s;
				closestCarAheadSpeed = v.speed;
			}
			if (v.speed < car_speed) {
				numVehiclesSlowerAhead += 1.0;
			}
		} else if (car_s - v.s < closestCarBehindS) { // car behind
			closestCarBehindS = car_s - v.s;
			closestCarBehindSpeed = v.speed;
		}

		std::__1::cout << "closestCarAheadS " << closestCarAheadS << std::__1::endl;
		std::__1::cout << "closestCarAheadSpeed " << closestCarAheadSpeed << std::__1::endl;
		std::__1::cout << "closestCarBehindS " << closestCarBehindS << std::__1::endl;
		std::__1::cout << "closestCarBehindSpeed " << closestCarBehindSpeed << std::__1::endl;

	}

	score_val = laneScore() + frontVehicleDistanceScore() + frontVehicleSpeedScore()
							+ behindVehicleDistanceScore() + behindVehicleSpeedScore() + slowVechiclesScore();
	return score_val;
}

Lane::Lane(double car_speed, double car_s, double lane) {
	this->car_speed = car_speed;
	this->car_s = car_s;
	this->lane = lane;
}

double Lane::laneScore() {
	return (1.1 * (1.0 - ((abs(id - lane)) / 2.0)));
}

double Lane::frontVehicleDistanceScore() {
	return (1.0 * (1.0 - abs(S_SAFE_DELTA - closestCarAheadS) / S_SAFE_DELTA));
}

double Lane::frontVehicleSpeedScore() {
	return (1.0 * (1.0 - abs(MAX_VEL - closestCarAheadSpeed) / MAX_VEL));
}

double Lane::behindVehicleDistanceScore() {
	return (1.0 * (1.0 - abs(S_SAFE_DELTA - closestCarBehindS) / S_SAFE_DELTA));
}

double Lane::behindVehicleSpeedScore() {
	return (1.0 * (1.0 - abs(MAX_VEL - closestCarBehindSpeed) / MAX_VEL));
}

double Lane::slowVechiclesScore() {
	return (0.75 * (1.0 - numVehiclesSlowerAhead / vehicles.size()));
}

bool Lane::isChangeFeasible() {
	if (closestCarAheadSpeed <= car_speed) {
		if (closestCarAheadS < S_SAFE_DELTA) {
			return false;
		}
	} else if (closestCarAheadSpeed > car_speed) {
		if (closestCarAheadS < S_DELTA) {
			return false;
		}
	}

	if (closestCarBehindSpeed >= car_speed) {
		if (closestCarBehindS < S_SAFE_DELTA) {
			return false;
		}
	} else if (closestCarBehindSpeed < car_speed) {
		if (closestCarBehindS < S_DELTA) {
			return false;
		}
	}
	return true;
}