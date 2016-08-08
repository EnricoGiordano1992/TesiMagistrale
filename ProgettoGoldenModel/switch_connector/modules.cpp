#include "modules.h"

/***********************************
*
* MOVEMENT FUNCTIONS
*
***************************************/

void wait_move(int n){
  int i = n;
  while(i--){
    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);

    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);

    simxSynchronousTrigger(clientID);
  }
}


void alzati(){
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);

  //alzo di 45 gradi i v3
  for (int i = 0; i < numVerticalJoint; ++i)
  {
      if(i == L_B_V3 || i == L_T_V3 || i == R_B_V3 || i == R_T_V3)
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0.78, simx_opmode_oneshot);
      else
      simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0, simx_opmode_oneshot);
  }
  for (int i = 0; i < numHorizontalJoint; ++i)
  {
      simxSetJointTargetPosition(clientID, horizontalJoints.at(i), 0, simx_opmode_oneshot);
  }

  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);
  wait_move(2);
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);

  //abbasso di 90 gradi i v2 e -45 i v1
  for (int i = 0; i < numVerticalJoint; ++i)
  {
      if(i != L_B_V3 && i != L_T_V3 && i != R_B_V3 && i != R_T_V3){
        if(i == L_B_V2 || i == L_T_V2 || i == R_B_V2 || i == R_T_V2)
          simxSetJointTargetPosition(clientID, verticalJoints.at(i), 1.57, simx_opmode_oneshot);
        else
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), -0.78, simx_opmode_oneshot);
      }
  }

  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);
  wait_move(2);
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);

  //alzo di -45 gradi i v3, alzo a 45 i v2, resetto i v1
  for (int i = 0; i < numVerticalJoint; ++i)
  {
      if(i == L_B_V3 || i == L_T_V3 || i == R_B_V3 || i == R_T_V3)
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), -0.78, simx_opmode_oneshot);
      else if(i == L_B_V2 || i == L_T_V2 || i == R_B_V2 || i == R_T_V2)
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0.78, simx_opmode_oneshot);
      else
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0, simx_opmode_oneshot);
  }
  for (int i = 0; i < numHorizontalJoint; ++i)
  {
      simxSetJointTargetPosition(clientID, horizontalJoints.at(i), 0, simx_opmode_oneshot);
  }

  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

}

void abbassati(){
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);

  //alzo di -45 gradi i v3, alzo a 45 i v2, resetto i v1
  for (int i = 0; i < numVerticalJoint; ++i)
  {
      if(i == L_B_V3 || i == L_T_V3 || i == R_B_V3 || i == R_T_V3)
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), -0.78, simx_opmode_oneshot);
      else if(i == L_B_V2 || i == L_T_V2 || i == R_B_V2 || i == R_T_V2)
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0.78, simx_opmode_oneshot);
      else
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0, simx_opmode_oneshot);
  }
  for (int i = 0; i < numHorizontalJoint; ++i)
  {
      simxSetJointTargetPosition(clientID, horizontalJoints.at(i), 0, simx_opmode_oneshot);
  }

  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);
  wait_move(10);
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);

  //abbasso di 90 gradi i v2 e -45 i v1
  for (int i = 0; i < numVerticalJoint; ++i)
  {
      if(i == L_B_V3 || i == L_T_V3 || i == R_B_V3 || i == R_T_V3)
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0.78, simx_opmode_oneshot);
      else if(i == L_B_V2 || i == L_T_V2 || i == R_B_V2 || i == R_T_V2)
        simxSetJointTargetPosition(clientID, verticalJoints.at(i), 1.57, simx_opmode_oneshot);
      else
      simxSetJointTargetPosition(clientID, verticalJoints.at(i), -0.78, simx_opmode_oneshot);
  }

  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);
  wait_move(10);
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);

  //reset
  for (int i = 0; i < numVerticalJoint; ++i)
  {
      simxSetJointTargetPosition(clientID, verticalJoints.at(i), 0, simx_opmode_oneshot);
  }
  for (int i = 0; i < numHorizontalJoint; ++i)
  {
      simxSetJointTargetPosition(clientID, horizontalJoints.at(i), 0, simx_opmode_oneshot);
  }

  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);
}


void cammina(int passi){
  int n = passi;
  while(n--){

    //SINISTRA
    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.26, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 1.57, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), -1.30, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 1.04, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);


    //DESTRA
    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.26, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 1.57, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), -1.30, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 1.04, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    wait_move(2);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);

  }
}



void striscia(int passi){
  int n = passi;
  while(n--){

    //parte bot
    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    //inizia parte top
    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.52, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 1.04, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.52, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

  }
}


void striscia_al_contrario(int passi){
  int n = passi;

  //prima lo rovesci
  //TODO
  while(n--){

    //parte bot
    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    //inizia parte top
    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.52, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 1.04, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.78, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -1.04, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.52, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.78, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

    assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), -0.52, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
    simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
    assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
    simxSynchronousTrigger(clientID);
    //wait_move(1);

  }
}


void rotea_sx(){
  //step1
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(2);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(6);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(8);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), -1.57, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(2);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 1.57, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(6);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(8);

}


void rotea_dx(){
  //step1
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), -1.57, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(2);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 1.57, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(6);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(8);

  //step2
  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(2);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), -1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 1.57, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(6);

  assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_B_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V1), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V2), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(L_T_V3), 0.0, simx_opmode_oneshot);
  simxSetJointTargetPosition(clientID, verticalJoints.at(R_T_V3), 0.0, simx_opmode_oneshot);
  assert(simxPauseCommunication(clientID, 0) == simx_return_ok);
  simxSynchronousTrigger(clientID);

  wait_move(8);

}


/********************************************/





void Lying_configuration::proc()
{
    STATUS = S_INIT;

    while(true) {
        //receive FSM status from bus
        NEXT_STATUS =  static_cast<STATES>(in1.read());
        // start transaction after first data has been read

        RC_TRANSACTION {

          STATUS = NEXT_STATUS;

          switch(STATUS){

            case S_INIT:
              abbassati();
              break;

            case S_WALK:
              striscia(1);
              break;

            case S_STOP:
              wait_move(20);
              break;

            case S_RECONFIG:
              wait_move(20);
              break;

          }
          //wait(1000, SC_MS);

          RC_COUTL("Lying_configuration: executed " << STATUS
              << " (t=" << sc_time_stamp() << ")");

          out1.write(static_cast<int>(STATUS));
        } // end transaction
        // continue while loop
    }
}


void Erect_configuration::proc()
{
    STATUS = S_INIT;
    while(true) {
        //receive FSM status from bus
        NEXT_STATUS =  static_cast<STATES>(in1.read());
        // start transaction after first data has been read
        RC_COUTL("Erect_configuration: " << STATUS
            << " (t=" << sc_time_stamp() << ")");

        RC_TRANSACTION {

          STATUS = NEXT_STATUS;

          switch(STATUS){

            case S_INIT:
              alzati();
              break;

            case S_WALK:
              cammina(1);
              break;

            case S_STOP:
              wait_move(10);
              break;

            case S_RECONFIG:
              wait_move(10);
              break;

          }

          RC_COUTL("Erect_configuration: executed " << STATUS
              << " (t=" << sc_time_stamp() << ")");

          out1.write(static_cast<int>(STATUS));
        } // end transaction
        // continue while loop
    }
}
