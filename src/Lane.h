#ifndef PATH_PLANNING_LANE_H
#define PATH_PLANNING_LANE_H

using namespace std;

/**
 * Class to keep track of vehicle info for a lane.
 */
class Vehicle {
    public:
        double id;
        double s;
        double d;
        double speed;
};

/**
 * Class Lane keeps track of the vechicles in the lane and constructs
 * interesting characteristics of the vehicle speed and distance to
 * score the lane. The lane with the best score will be transitioned to if
 * feasible.
 */
class Lane {
    /* The max reference velocity permitted */
    static constexpr double MAX_VEL = 49.5;
    /* The safe distance to keep when the reference vehicle travelling at greater speed */
    static constexpr double S_SAFE_DELTA = 60.0;
    /* The safe distance to keep when the rererence vehicle travelling at slower speed */
    static constexpr double S_DELTA = 30.0;

private:
    double car_speed;
    double car_s;
    double lane;

public:
    /* The lane number */
    int id;

    /* Interesting vehicle characteristics to help score lane on various parameters */
    double closestCarAheadS;
    double closestCarAheadSpeed;
    double closestCarBehindS;
    double closestCarBehindSpeed;
    double numVehiclesSlowerAhead;

    /* The current lane score */
    double score_val;
    vector<Vehicle> vehicles;

    /**
     * Empty constructor
     */
    Lane() {};
    /**
     * Destructor
     */
    ~Lane() {};
    /**
     * Constructor takes as input the current vehicle speed, vehicle s and the current lane.
     * @param car_speed
     * @param car_s
     * @param lane
     */
    Lane(double car_speed, double car_s, int lane);

//		const bool operator< (const Lane &other) {
//			return (score_val > other.score_val);
//		}

    /**
     * Calculate the total score
     * @return score
     */
    double score();
    /**
     * Score for the lane where current lane gets maximum weightage for comfort.
     * @return
     */
    double laneScore();
    /**
     * Score based on how close the closest vehicle is ahead in the lane for safety.
     * @return
     */
    double frontVehicleDistanceScore();
    /**
     * Score based on the velocity of the closest vehicle ahead in the lane for safety.
     * @return
     */
    double frontVehicleSpeedScore();
    /**
     * Score based on how close the closest vehicle is behind in the lane for safety.
     * @return
     */
    double behindVehicleDistanceScore();
    /**
     * Score based on the velocity of the closest vehicle behind in the lane for safety.
     * @return
     */
    double behindVehicleSpeedScore();
    /**
     * Score based on the number of slower vehicles ahead relative to the ego vehicle in the lane.
     * @return
     */
    double slowVechiclesScore();
    /**
     * Calculates if the change to this lane is feasible for safety.
     * @return
     */
    bool isChangeFeasible();
};

bool laneCompare (const Lane i, const Lane j) {
    return (i.score_val > j.score_val);
}

#endif //PATH_PLANNING_LANE_H
