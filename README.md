# CarND-Path-Planning-Project
Self-Driving Car Engineer Nanodegree Program
### Reflections

The goal of the path planning project to find a way for the ego car to drive around the track (in the simulator). The
 path taken should be in a manner which is safe (no collisions etc. with other vehicles), comfortable (no jerks) and 
 efficient.
 The current information about other vehicles on the track is given by the sensor fusion data. Generally, the sensor 
 fusion data would be used to predict the trajectory of the vehicles using multiple model algorithm or 
 approaches like naive bayes classifier.
   
  Since, the simulator simulates highway driving here no extensive prediction techniques were used. Instead, the 
  sensor fusion data was directly used to extrapolate the position to a little further out to check for lane 
  change feasibility. 

#### Behavioral Planning

One of the initial steps was to plan the behavior of the ego vehicle i.e whether to change the lane when being blocked 
by a slower vehicle, when to speed up/slow down. This was accomplished by creating a cost/score function for each 
lane. 
##### Lane cost/score estimation

Multiple score functions were defined and then the score for all added up estimate the score for the lane.
The score functions were selected to score the lane on different factors e.g. safety, comfort, efficiency. The score 
functions selected were:
* _Lane keep score (Comfort)_ - To keep lane changing to a minimum and only when required for efficiency. This would 
help in maintaining a comfortable ride.
* _Closest vehicle ahead (Efficiency & safety)_ - speed and reference distance - The closest vehicle distance from the 
ego vehicle and the speed at which travelling. The farther away the vehicle ahead of us is and the faster it is travelling is better as 
it would allow the ego vehicle to travel faster.
* _Closest vehicle behind (Safety)_ - speed and reference distance - The closest vehicle behind's distance and velocity
 is also scored. The slower and farther away that vehicle is the better it is for safety in case we need to change 
 lanes.
* _Slower vehicles ahead (Efficiency)_ - The number of vehicles slower ahead of the ego vehicle. This would allow to 
penalize lanes with lot of slow vehicles ahead for efficiency.

The code for the calculating the score and different score functions are defined in the *Lane.h* header file and 
implemented in the *Lane.cpp* class.

##### Feasibility

The lanes were sorted according to their score and then iterated to get a feasible list of lanes where different 
driving actions can be undertaken like KL (keep lane), LC (lane change), Skip LC (double lane change). The basic 
feasibility check is 
defined in the *isChangeFeasible* in the *Lane.cpp* class.

##### Transition

Once feasible lanes were defined the transition function calculated further the state for the next action. The state 
would be the lane and the velocity to drive. The transition function is defined in the *Transition.h* header file and
 implemented in the *Transition.cpp* file in the transition function. These parameters are then used for the 
 trajectory generation.

#### Trajectory Generation

The trajectory generation uses the [ttk:spline](https://github.com/ttk592/spline) library and is built using the idea
 demonstrated in the walk through video.
The steps to generate trajectory are the following:
* Create a spline with a couple of points from the present vehicle position and some anchor points further down and 
lateral position as center point of the lane selected from the surrounding waypoints. This would create a smooth 
curve on which to map the points to be used for the trajectory.
* Convert all to the local car reference co-ordinates.
* Create 50 points on the spline starting from the unused previous path points returned from the simulator.
* The number of remaining points to be created is decided in such a way that they are evenly spaced based on the 
reference velocity from the transition function above with distance upto 30 m further down so, that the acceleration 
does not go beyond the required value.  
* These 50 points which include the previous points and points 30 m further down the lane now provide the smooth
trajectory the vehicle will drive on. These points are converted back to the map co-ordinates before sending them to 
the simulator.

#### Result
The video of the output:
[![Project Video](https://img.youtube.com/vi/K-jz4CCnUkQ/0.jpg)](https://www.youtube.com/watch?v=K-jz4CCnUkQ)
#### Future Improvements

Future improvements that can be added:
* Behavior prediction for other vehicles on the track.
* Some further improvements to lane scoring by possibly looking not just at the closest cars but behind them to plan 
lane change as described in the lectures.

### Simulator
You can download the Term3 Simulator which contains the Path Planning Project from the [releases tab (https://github.com/udacity/self-driving-car-sim/releases).

### Goals
In this project your goal is to safely navigate around a virtual highway with other traffic that is driving +-10 MPH of the 50 MPH speed limit. You will be provided the car's localization and sensor fusion data, there is also a sparse map list of waypoints around the highway. The car should try to go as close as possible to the 50 MPH speed limit, which means passing slower traffic when possible, note that other cars will try to change lanes too. The car should avoid hitting other cars at all cost as well as driving inside of the marked road lanes at all times, unless going from one lane to another. The car should be able to make one complete loop around the 6946m highway. Since the car is trying to go 50 MPH, it should take a little over 5 minutes to complete 1 loop. Also the car should not experience total acceleration over 10 m/s^2 and jerk that is greater than 50 m/s^3.

#### The map of the highway is in data/highway_map.txt
Each waypoint in the list contains  [x,y,s,dx,dy] values. x and y are the waypoint's map coordinate position, the s value is the distance along the road to get to that waypoint in meters, the dx and dy values define the unit normal vector pointing outward of the highway loop.

The highway's waypoints loop around so the frenet s value, distance along the road, goes from 0 to 6945.554.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./path_planning`.

Here is the data provided from the Simulator to the C++ Program

#### Main car's localization Data (No Noise)

["x"] The car's x position in map coordinates

["y"] The car's y position in map coordinates

["s"] The car's s position in frenet coordinates

["d"] The car's d position in frenet coordinates

["yaw"] The car's yaw angle in the map

["speed"] The car's speed in MPH

#### Previous path data given to the Planner

//Note: Return the previous list but with processed points removed, can be a nice tool to show how far along
the path has processed since last time. 

["previous_path_x"] The previous list of x points previously given to the simulator

["previous_path_y"] The previous list of y points previously given to the simulator

#### Previous path's end s and d values 

["end_path_s"] The previous list's last point's frenet s value

["end_path_d"] The previous list's last point's frenet d value

#### Sensor Fusion Data, a list of all other car's attributes on the same side of the road. (No Noise)

["sensor_fusion"] A 2d vector of cars and then that car's [car's unique ID, car's x position in map coordinates, car's y position in map coordinates, car's x velocity in m/s, car's y velocity in m/s, car's s position in frenet coordinates, car's d position in frenet coordinates. 

## Details

1. The car uses a perfect controller and will visit every (x,y) point it recieves in the list every .02 seconds. The units for the (x,y) points are in meters and the spacing of the points determines the speed of the car. The vector going from a point to the next point in the list dictates the angle of the car. Acceleration both in the tangential and normal directions is measured along with the jerk, the rate of change of total Acceleration. The (x,y) point paths that the planner recieves should not have a total acceleration that goes over 10 m/s^2, also the jerk should not go over 50 m/s^3. (NOTE: As this is BETA, these requirements might change. Also currently jerk is over a .02 second interval, it would probably be better to average total acceleration over 1 second and measure jerk from that.

2. There will be some latency between the simulator running and the path planner returning a path, with optimized code usually its not very long maybe just 1-3 time steps. During this delay the simulator will continue using points that it was last given, because of this its a good idea to store the last points you have used so you can have a smooth transition. previous_path_x, and previous_path_y can be helpful for this transition since they show the last points given to the simulator controller with the processed points already removed. You would either return a path that extends this previous path or make sure to create a new path that has a smooth transition with this last path.

## Tips

A really helpful resource for doing this project and creating smooth trajectories was using http://kluge.in-chemnitz.de/opensource/spline/, the spline function is in a single hearder file is really easy to use.

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `install-mac.sh` or `install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!


## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./
