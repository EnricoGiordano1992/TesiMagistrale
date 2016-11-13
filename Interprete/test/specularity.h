#ifndef SPECULARITY_H
#define SPECULARITY_H

#include "robot_model.h"

typedef enum {S_INIT, S_WALK, S_STOP, S_RECONFIG, S_LEFT, S_RIGHT} STATES;
typedef enum {LYING, ERECT} ROBOT_CONFIG;


extern float horizontalPosition[numHorizontalJoint][numHorizontalJoint] ;
extern float verticalPosition[numVerticalJoint][numVerticalJoint] ;

extern std::vector<simInt> verticalJoints;
extern std::vector<simInt> horizontalJoints;

extern simxInt clientID;

extern simInt reference;
extern float ref_position[3];
extern float ref_orientation[3];

extern simxFloat vel[3];

extern simxUChar detectionState;
extern simxFloat detectedPoint[3];
extern simxInt detectedObjectHandle;
extern simxFloat detectedSurfaceNormalVector[3];
extern simInt sensor_up;
extern simInt sensor_down;
extern simInt sensor_left;
extern simInt sensor_right;
extern simInt sensor_finished_obstacle;
extern simxInt visionSensor;
extern simxInt resolution[2];
extern simxUChar* image;


#endif
