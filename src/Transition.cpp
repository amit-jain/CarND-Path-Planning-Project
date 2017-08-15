#include <fstream>
#include "Transition.h"

using namespace std;

Transition::Transition(int curr_lane, double ref_vel) {
	this->hot = false;
	this->curr_lane = curr_lane;
	this->keep_lane = false;
	this->skip_lane = false;
	this->ref_vel = ref_vel;
	this->finalVelocity = ref_vel;
}

void Transition::setFinalVelocity(double vel) {
	this->finalVelocity = vel;
}

void Transition::push(int in_lane, bool in_hot) {
	if (curr_lane == in_lane) {
		keep_lane = true;
	} else {
		if (abs(curr_lane - in_lane) > 1) {
			skip_lane = true;
		}
		lane_change.push_back(in_lane);
	}
	this->hot = in_hot;
}

void Transition::transition() {
	if (skip_lane && lane_change.size() == 1) {
		KL();
	} else if (skip_lane) {
		skip_LC();
	} else if (lane_change.size() == 1) {
		LC();
	} else {
		KL();
	}
}

void Transition::KL() {
	if (hot) {
		cout << "Decreasing velocity from " << finalVelocity << endl;
		setFinalVelocity(finalVelocity = finalVelocity - VEL_DELTA);
	} else if (finalVelocity < MAX_VEL) {
		cout << "Increasing velocity from " << finalVelocity << endl;
		setFinalVelocity(finalVelocity = finalVelocity + VEL_DELTA);
	}
	lane = curr_lane;
}

void Transition::LC() {
	if (finalVelocity < MAX_VEL) {
		cout << "Increasing velocity from " << finalVelocity << endl;
		setFinalVelocity(finalVelocity = finalVelocity + VEL_DELTA);
	}
	this->lane = lane_change[0];
}

void Transition::skip_LC() {
	if (finalVelocity > 40.0) {
    setFinalVelocity(finalVelocity = finalVelocity - VEL_SKIP_DELTA);
		cout << "Lowering speed for double lane change " << finalVelocity << endl;
	} else {
		lane = lane_change[0];
		cout << "Executing double lane change " << lane << endl;
	}
}