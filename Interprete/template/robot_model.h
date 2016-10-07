#ifndef ROBOT_MODEL_H
#define ROBOT_MODEL_H

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

#endif
