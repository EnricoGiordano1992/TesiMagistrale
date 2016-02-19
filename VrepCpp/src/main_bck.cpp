#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cassert>


extern "C" {
    #include "extApi.h"
    #include <v_repLib.h>
/*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
}



int main(int argc,char* argv[])
{
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
	
	int numWheel = 4;


 	float position[numWheel][3] ;
		 

   	std::vector<simInt> wheelJoints{-1, -1, -1, -1};
   // ittengo l'handle delle ruote --> devo dare il nome dell'oggetto che sta all'interno della scena, c'è il disegnino del giunto
   	simxGetObjectHandle(clientID, "rollingJoint_fl", &wheelJoints[0], simx_opmode_oneshot_wait);
    simxGetObjectHandle(clientID, "rollingJoint_rl", &wheelJoints[1], simx_opmode_oneshot_wait);
 	simxGetObjectHandle(clientID, "rollingJoint_rr", &wheelJoints[2], simx_opmode_oneshot_wait);
 	simxGetObjectHandle(clientID, "rollingJoint_fr", &wheelJoints[3], simx_opmode_oneshot_wait);

  // ottengo la posizione dei joint 
 // simxGetObjectPosition(simxInt clientID,simxInt objectHandle,simxInt relativeToObjectHandle,simxFloat* position,simxInt operationMode)

 	for (int i = 0; i < numWheel; ++i)
 	{
	 	 simxGetObjectPosition(clientID,  wheelJoints.at(i),-1, position[i], simx_opmode_oneshot_wait);
	 	 fprintf(stderr, "WHEEL(%d) =  %d\n",i,wheelJoints.at(i) );
	 	 fprintf(stderr, "POSITION =  x %f ; y %f ; z %f  \n",position[i][0],position[i][1],position[i][2] );
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



   simxFloat vel[3] = {1,0,0};


   while(simxGetConnectionId(clientID) != -1)
   {
	   assert(simxPauseCommunication(clientID, 1) == simx_return_ok); //necessario quando si da un TARGET VELOCITY al joint

		for (int i = 0; i < numWheel; ++i)
	 	{
	   		simxSetJointTargetVelocity(clientID, wheelJoints.at(i) , *vel, simx_opmode_oneshot );
		}

	   assert(simxPauseCommunication(clientID, 0) == simx_return_ok);

	   simxSynchronousTrigger(clientID);  //  sincronizzazione tra simulazione e remoteApi

	}


	simxFinish(clientID);

	return 0;
}
