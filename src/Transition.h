#ifndef PATH_PLANNING_TRANSITION_H
#define PATH_PLANNING_TRANSITION_H

using namespace std;

class Transition {
    /* The max reference velocity permitted */
    static constexpr double MAX_VEL = 49.5;

    /* The normal velocity delta */
    static constexpr double VEL_DELTA = 0.224;

    /* The velocity delta in case of skip lane change */
    static constexpr double VEL_SKIP_DELTA = 0.336;

    private:
        /* Internal variables */

        vector<int> lane_change;
        bool hot;
        int curr_lane;
        int ref_vel;
        bool keep_lane;
        bool skip_lane;

    public:
        /* The final velocity of the state */
        double finalVelocity;
        /* The lane to transition to */
        int lane;

/**
* Default constructor
*/
Transition(): hot(false), keep_lane(false), skip_lane(false), curr_lane(0), ref_vel(0.0), finalVelocity(0.0) {}

/**
* Default destructor
*/
~Transition(){}

/**
* Constructor which takes as input the current lane and the current reference velocity.
* @param curr_lane
* @param ref_vel
*/
Transition(int curr_lane, double ref_vel);

/**
* Setter for final velocity
* @param vel
*/
void setFinalVelocity(double vel);

/**
* Push the feasible lane. Also, keep track of whether the vehicle ahead is close.
* @param in_lane
* @param in_hot
*/
void push(int lane, bool hot);

/**
* Transition to get the state
*/
void transition();

/**
* Execute a Keel lane state and decrease velocity to maintain safe distance from vehicle ahead.
*/
void KL();

/**
* Execute a lane change and also increasing velocity if less than the maximum.
*/
void LC();

/**
* Execute a double lane change.
* If speed greater than 40 KL and lower the speed to minimize jerk.
*/
void skip_LC();
};


#endif //PATH_PLANNING_TRANSITION_H
