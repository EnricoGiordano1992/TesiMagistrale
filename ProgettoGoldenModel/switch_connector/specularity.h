#ifndef SPECULARITY_H
#define SPECULARITY_H

#include "robot_model.h"

typedef enum {S_INIT, S_WALK, S_STOP, S_RECONFIG} STATES;
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

#endif
