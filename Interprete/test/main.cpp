#include <systemc.h>
#include "Top.h"
#include "robot_model.h"



int main(int argc, char* argv[])
{

  float horizontalPosition[numHorizontalJoint][numHorizontalJoint] ;
float verticalPosition[numVerticalJoint][numVerticalJoint] ;

std::vector<simInt> verticalJoints{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
std::vector<simInt> horizontalJoints{-1, -1, -1, -1, -1, -1, -1, -1};
simxInt clientID;
simInt reference;
float ref_position[3];
float ref_orientation[3];
simxFloat vel[3] = {0.001,0.001,0.001};
simxUChar detectionState;
simxFloat detectedPoint[3];
simxInt detectedObjectHandle;
simxFloat detectedSurfaceNormalVector[3];
simInt sensor_up;
simInt sensor_down;
simInt sensor_left;
simInt sensor_right;
simInt sensor_finished_obstacle;
simxInt visionSensor;
simxInt resolution[2];
simxUChar* image;


  Top* top = new Top("top");

  
simxFinish(-1);
clientID = simxStart((simxChar *) "127.0.0.1", 19997, 1, 1, 200000, 1);
if (clientID == -1) {
  fprintf(stderr, "Non sono connesso\n");
}

simxStartSimulation(clientID, simx_opmode_oneshot);
fprintf(stderr, "Connesso a vrep!\n");

simxSynchronous(clientID, 1);

//ottengo l'handle dei giunti --> devo dare il nome dell'oggetto che sta all'interno della scena, c'e' il disegnino del giunto
simxGetObjectHandle(clientID, "left_bot_v1", &verticalJoints[L_B_V1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_bot_v2", &verticalJoints[L_B_V2], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_bot_v3", &verticalJoints[L_B_V3], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_bot_v1", &verticalJoints[R_B_V1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_bot_v2", &verticalJoints[R_B_V2], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_bot_v3", &verticalJoints[R_B_V3], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_top_v1", &verticalJoints[L_T_V1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_top_v2", &verticalJoints[L_T_V2], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_top_v3", &verticalJoints[L_T_V3], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_top_v1", &verticalJoints[R_T_V1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_top_v2", &verticalJoints[R_T_V2], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_top_v3", &verticalJoints[R_T_V3], simx_opmode_oneshot_wait);

simxGetObjectHandle(clientID, "left_bot_h1", &horizontalJoints[L_B_H1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_bot_h2", &horizontalJoints[L_B_H2], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_bot_h1", &horizontalJoints[R_B_H1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_bot_h2", &horizontalJoints[R_B_H2], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_top_h1", &horizontalJoints[L_T_H1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "left_top_h2", &horizontalJoints[L_T_H2], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_top_h1", &horizontalJoints[R_T_H1], simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "right_top_h2", &horizontalJoints[R_T_H2], simx_opmode_oneshot_wait);

// ottengo la posizione dei joint
// simxGetObjectPosition(simxInt clientID,simxInt objectHandle,simxInt relativeToObjectHandle,simxFloat* position,simxInt operationMode)

for (int i = 0; i < numHorizontalJoint; ++i)
{
  simxGetObjectPosition(clientID,  horizontalJoints.at(i),-1, horizontalPosition[i], simx_opmode_oneshot_wait);
  fprintf(stderr, "HJ(%d) =  %d\n",i,horizontalJoints.at(i) );
  fprintf(stderr, "POSITION =  x %f ; y %f ; z %f  \n",horizontalPosition[i][0],horizontalPosition[i][1],horizontalPosition[i][2] );
}
for (int i = 0; i < numVerticalJoint; ++i)
{
  simxGetObjectPosition(clientID,  verticalJoints.at(i),-1, verticalPosition[i], simx_opmode_oneshot_wait);
  fprintf(stderr, "VJ(%d) =  %d\n",i,verticalJoints.at(i) );
  fprintf(stderr, "POSITION =  x %f ; y %f ; z %f  \n",verticalPosition[i][0],verticalPosition[i][1],verticalPosition[i][2] );
}


simxGetObjectHandle(clientID, "ref", &reference, simx_opmode_oneshot_wait);
fprintf(stderr, "reference = %d\n", reference );

simxGetObjectPosition(clientID, reference, -1, ref_position, simx_opmode_oneshot_wait);
fprintf(stderr, "ref_position =  x %f ; y %f ; z %f  \n",ref_position[0],ref_position[1],ref_position[2] );

simxGetObjectOrientation(clientID,  reference ,-1, ref_orientation,simx_opmode_oneshot_wait);

for (int i = 0; i < numVerticalJoint; ++i)
{
   simxSetJointTargetVelocity(clientID, verticalJoints.at(i) , *vel, simx_opmode_oneshot );
}
for (int i = 0; i < numHorizontalJoint; ++i)
{
   simxSetJointTargetVelocity(clientID, horizontalJoints.at(i) , *vel, simx_opmode_oneshot );
}

for (int i = 0; i < numHorizontalJoint; ++i)
{
   simxSetJointTargetPosition(clientID, horizontalJoints.at(i), 1, simx_opmode_oneshot);
}

simxGetObjectHandle(clientID, "Proximity_sensor_up", &sensor_up, simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "Proximity_sensor_down", &sensor_down, simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "Proximity_sensor_left", &sensor_left, simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "Proximity_sensor_right", &sensor_right, simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "Proximity_sensor_finished_obstacle", &sensor_finished_obstacle, simx_opmode_oneshot_wait);
simxGetObjectHandle(clientID, "Vision_sensor", &visionSensor, simx_opmode_oneshot_wait);

simxGetVisionSensorImage(clientID,visionSensor,resolution,&image,1,simx_opmode_streaming);


  sc_start();
  delete top;
  return 0;

}
