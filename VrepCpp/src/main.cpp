#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <time.h>
#include <unistd.h>
#include <iostream>


extern "C" {
    #include "extApi.h"
    #include <v_repLib.h>
/*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
}


int main(int argc,char* argv[])
{
   sleep(2);

    simxInt clientID;

    simxFinish(-1);
    clientID = simxStart((simxChar *) "127.0.0.1", 19997, 1, 1, 200000, 1);
    if (clientID == -1) {
       fprintf(stderr, "Non sono connesso\n");
    }

    simxStartSimulation(clientID, simx_opmode_oneshot);
    fprintf(stderr, "Connesso a vrep!\n");
    

   simxSynchronous(clientID, 1);


    // 
	
  int numHorizontalJoint = 4;
  int numVerticalJoint = 4;  

 	float horizontalPosition[numHorizontalJoint][3] ;
  float verticalPosition[numVerticalJoint][3] ;
		 
  std::vector<simInt> verticalJoints{-1, -1, -1, -1};
  std::vector<simInt> horizontalJoints{-1, -1, -1, -1};


  //ottengo l'handle dei giunti --> devo dare il nome dell'oggetto che sta all'interno della scena, c'è il disegnino del giunto
  simxGetObjectHandle(clientID, "snake_joint_v1", &verticalJoints[0], simx_opmode_oneshot_wait);
  simxGetObjectHandle(clientID, "snake_joint_v2", &verticalJoints[1], simx_opmode_oneshot_wait);
 	simxGetObjectHandle(clientID, "snake_joint_v3", &verticalJoints[2], simx_opmode_oneshot_wait);
 	simxGetObjectHandle(clientID, "snake_joint_v3", &verticalJoints[3], simx_opmode_oneshot_wait);

  simxGetObjectHandle(clientID, "snake_joint_h1", &horizontalJoints[0], simx_opmode_oneshot_wait);
  simxGetObjectHandle(clientID, "snake_joint_h2", &horizontalJoints[1], simx_opmode_oneshot_wait);
  simxGetObjectHandle(clientID, "snake_joint_h3", &horizontalJoints[2], simx_opmode_oneshot_wait);
  simxGetObjectHandle(clientID, "snake_joint_h3", &horizontalJoints[3], simx_opmode_oneshot_wait);

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


   // ottengo l'handle chiamato "ref" , HO FATTO COSI PER COMODITA' solo per
   // muovere le ruote tutte insieme con la stessa velocità, e avere una
   // referenza della posizione e orientamento del robot 
   // il -1 mi indica come referenza il WORLD.
   simInt reference;
   float ref_position[3];
   float ref_orientation[3];

   simxGetObjectHandle(clientID, "ref", &reference, simx_opmode_oneshot_wait); 
   fprintf(stderr, "reference = %d\n", reference );
   
   simxGetObjectPosition(clientID, reference, -1, ref_position, simx_opmode_oneshot_wait);
   fprintf(stderr, "ref_position =  x %f ; y %f ; z %f  \n",ref_position[0],ref_position[1],ref_position[2] );

   simxGetObjectOrientation(clientID,  reference ,-1, ref_orientation,simx_opmode_oneshot_wait);



   simxFloat vel[3] = {1,1,1};


   while(simxGetConnectionId(clientID) != -1)
   {
	   assert(simxPauseCommunication(clientID, 1) == simx_return_ok); //necessario quando si da un TARGET VELOCITY al joint

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
        simxSetJointTargetPosition(clientID, horizontalJoints.at(i), 2, simx_opmode_oneshot);
    }
    for (int i = 0; i < numVerticalJoint; ++i)
    {
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 2, simx_opmode_oneshot);
    }

    for (int i = 0; i < numHorizontalJoint; ++i)
    {
        simxSetJointTargetPosition(clientID, horizontalJoints.at(i), -2, simx_opmode_oneshot);
    }
    for (int i = 0; i < numVerticalJoint; ++i)
    {
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), -2, simx_opmode_oneshot);
    }

	   assert(simxPauseCommunication(clientID, 0) == simx_return_ok);

	  simxSynchronousTrigger(clientID);  //  sincronizzazione tra simulazione e remoteApi

	}


	simxFinish(clientID);

	return 0;
}
