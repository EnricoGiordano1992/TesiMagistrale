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

typedef enum {S0, S1, S2, S3, S4, S5, S6, S7, S8, S9} STATES;

#define numHorizontalJoint 8
#define numVerticalJoint 12

#define L_B_V1 0
#define L_B_V2 L_B_V1+1
#define L_B_V3 L_B_V2+1
#define R_B_V1 L_B_V3+1
#define R_B_V2 R_B_V1+1
#define R_B_V3 R_B_V2+1
#define L_T_V1 R_B_V3+1
#define L_T_V2 L_T_V1+1
#define L_T_V3 L_T_V2+1
#define R_T_V1 L_T_V3+1
#define R_T_V2 R_T_V1+1
#define R_T_V3 R_T_V2+1

#define L_B_H1 0
#define L_B_H2 L_B_H1+1
#define R_B_H1 L_B_H2+1
#define R_B_H2 R_B_H1+1
#define L_T_H1 R_B_H2+1
#define L_T_H2 L_T_H1+1
#define R_T_H1 L_T_H2+1
#define R_T_H2 R_T_H1+1


float horizontalPosition[numHorizontalJoint][numHorizontalJoint] ;
float verticalPosition[numVerticalJoint][numVerticalJoint] ;

std::vector<simInt> verticalJoints{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
std::vector<simInt> horizontalJoints{-1, -1, -1, -1, -1, -1, -1, -1};

simInt sensor = -1;

simxInt clientID;

simInt reference;
float ref_position[3];
float ref_orientation[3];

simxFloat vel[3] = {0.001,0.001,0.001};



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

  //poi lo raddrizzi
  //TODO
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


/*********************************
*
*       MAIN
*
***********************************/

int main(int argc,char* argv[])
{
    //sleep(1);

    simxFinish(-1);
    clientID = simxStart((simxChar *) "127.0.0.1", 19997, 1, 1, 200000, 1);
    if (clientID == -1) {
       fprintf(stderr, "Non sono connesso\n");
    }

    simxStartSimulation(clientID, simx_opmode_oneshot);
    fprintf(stderr, "Connesso a vrep!\n");


    simxSynchronous(clientID, 1);


  //ottengo l'handle dei giunti --> devo dare il nome dell'oggetto che sta all'interno della scena, c'è il disegnino del giunto
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

   simxUChar detectionState;
   simxFloat detectedPoint[3];
   simxInt detectedObjectHandle;
   simxFloat detectedSurfaceNormalVector[3];
   simxGetObjectHandle(clientID, "Proximity_sensor", &sensor, simx_opmode_oneshot_wait);

   simxInt errorCode;

   abbassati();
   wait_move(10);

   while(simxGetConnectionId(clientID) != -1)
   {
/*
     printf("alzati\n");
     alzati();

     errorCode=simxReadProximitySensor(clientID,sensor, &detectionState, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_streaming);
     printf("%d %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);

      printf("wait\n");
      wait_move(20);

      errorCode=simxReadProximitySensor(clientID,sensor, &detectionState, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_streaming);
      printf("%d %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);

      printf("cammina\n");
      cammina(2);

      errorCode=simxReadProximitySensor(clientID,sensor, &detectionState, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_streaming);
      printf("%d %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);

      printf("abbassati\n");
      abbassati();

      errorCode=simxReadProximitySensor(clientID,sensor, &detectionState, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_streaming);
      printf("%d %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);

      printf("wait\n");
      wait_move(20);

      errorCode=simxReadProximitySensor(clientID,sensor, &detectionState, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_streaming);
      printf("%d %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);

      printf("striscia\n");
      striscia(2);

      errorCode=simxReadProximitySensor(clientID,sensor, &detectionState, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_streaming);
      printf("%d %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);

      printf("wait\n");
      wait_move(20);

      errorCode=simxReadProximitySensor(clientID,sensor, &detectionState, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_streaming);
      printf("%d %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);
*/
      rotea_sx();
      wait_move(20);
	}


	simxFinish(clientID);

	return 0;
}
