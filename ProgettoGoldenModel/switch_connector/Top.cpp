#include "Top.h"
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SHMSZ     27

typedef enum {AHEAD=0, BACK, UP, DOWN, LEFT, RIGHT} Movement;


void Top::activate(rc_reconfigurable& module)
{
    if (active_module != &module) {
        if (active_module != NULL) {
            RC_COUTL("Top: unloading module"
                " '" << active_module->rc_get_name() << "'..."
                " (t=" << sc_time_stamp() << ")");

            ctrl.unload(*active_module);

            RC_COUTL("Top: module '"
                << active_module->rc_get_name() << "' unloaded."
                " (t=" << sc_time_stamp() << ")\n");
        }

        RC_COUTL("Top: activating module"
            " '" << module.rc_get_name() << "'..."
            " (t=" << sc_time_stamp() << ")");

        ctrl.activate(module);
        active_module = &module;

        RC_COUTL("Top: module '"
            << module.rc_get_name() << "' activated."
            " (t=" << sc_time_stamp() << ")");
    }
}

void Top::producer_proc()
{

    simxUChar detectionStateUp;
    simxUChar detectionStateDown;
    simxUChar detectionStateLeft;
    simxUChar detectionStateRight;
    simxUChar detectionStateFinishedObstacle;

    //client
    char c;
    int shmid_write, shmid_read;
    key_t key_write, key_read;
    char *shm_write, *s_write, *shm_read, *s_read;
    key_write = 5678;
    key_read = 5679;

    if ((shmid_write = shmget(key_write, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm_write = shmat(shmid_write, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    if ((shmid_read = shmget(key_read, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm_read = shmat(shmid_read, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

//////////////////////////////////////////////

      STATUS = S_INIT;
      in1_fifo.write(STATUS);
      command_executed = false;
      RC_COUTL("Top: input written (" << STATUS << ")"
          " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
      wait(1000, SC_MS);

      while(!command_executed);
      STATUS = S_STOP;
      in1_fifo.write(STATUS);
      command_executed = false;
      RC_COUTL("Top: input written (" << STATUS << ")"
      " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
      wait(1000, SC_MS);

      int counter = 0;

      while(1){

        c = 99;
        counter = 0;

        s_write = shm_write;

        simxReadProximitySensor(clientID,sensor_up, &detectionStateUp, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_oneshot_wait);
        simxReadProximitySensor(clientID,sensor_down, &detectionStateDown, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_oneshot_wait);
        simxReadProximitySensor(clientID,sensor_left, &detectionStateLeft, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_oneshot_wait);
        simxReadProximitySensor(clientID,sensor_right, &detectionStateRight, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_oneshot_wait);
        simxReadProximitySensor(clientID,sensor_finished_obstacle, &detectionStateFinishedObstacle, detectedPoint, &detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_oneshot_wait);

        RC_COUTL("Top: " << (int)detectionStateUp << (int)detectionStateLeft << (int)detectionStateDown << (int)detectionStateRight << (int)detectionStateFinishedObstacle);

        *s_write++ = '0';
        *s_write++ = ' ';
        *s_write++ = detectionStateFinishedObstacle + 48;
        *s_write++ = ' ';
        *s_write++ = detectionStateUp + 48;
        *s_write++ = ' ';
        *s_write++ = detectionStateDown + 48;
        *s_write++ = ' ';
        *s_write++ = detectionStateLeft + 48;
        *s_write++ = ' ';
        *s_write++ = detectionStateRight + 48;
        *s_write++ = '*';
        //*s = NULL;

        /*
         * wait until the other process
         * changes the first character of our memory
         * to '*', indicating that it has read what
         * we put there.
         */
        while (*shm_write != '*')
            wait(100, SC_MS);

        s_read = shm_read;

        while(*(s_read+counter) != '*' || counter == 0){
          if(*(s_read+counter) != '*'){
            c = *(s_read+counter) - 48;
            counter++;
          }
        }

        //AHEAD, BACK, UP, DOWN, LEFT, RIGHT
        switch((Movement) c){
          case AHEAD:
            STATUS = S_WALK;
            RC_COUTL("Top: WALK");
            break;
          case BACK:
            //TODO
            STATUS = STATUS;
            RC_COUTL("Top: BACK");
            break;
          case UP:
            STATUS = S_RECONFIG;
            RC_COUTL("Top: UP");
            break;
          case DOWN:
            STATUS = S_RECONFIG;
            RC_COUTL("Top: DOWN");
            break;
          case LEFT:
            STATUS = S_LEFT;
            RC_COUTL("Top: LEFT");
            break;
          case RIGHT:
            STATUS = S_RIGHT;
            RC_COUTL("Top: RIGHT");
            break;
          default:
            RC_COUTL("Top: unknown " << (int) c );
            STATUS = S_STOP;
        }

        while(!command_executed);
        in1_fifo.write(STATUS);
        command_executed = false;
        RC_COUTL("Top: input written (" << STATUS << ")"
        " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
        *shm_read = '*';
        wait(500, SC_MS);

        if(STATUS == S_RECONFIG){
          STATUS = S_INIT;
          while(!command_executed);
          in1_fifo.write(STATUS);
          command_executed = false;
          RC_COUTL("Top: input written (" << STATUS << ")"
          " (t=" << sc_time_stamp() << ", config="<< robot_config << ")");
          *shm_read = '*';
          wait(500, SC_MS);
        }
    }
}

void Top::control_proc()
{
    while(1){
      activate(m2);
      robot_config = ERECT;
      while(STATUS == S_RECONFIG)
        wait(500, SC_MS);
      while(STATUS != S_RECONFIG)
        wait(500, SC_MS);
      activate(m1);
      robot_config = LYING;
      while(STATUS == S_RECONFIG)
        wait(500, SC_MS);
      while(STATUS != S_RECONFIG)
        wait(500, SC_MS);
    }
}

void Top::monitor_proc()
{
    int i=0;
    while(true) {
        ++i;
        int x = out1_fifo.read();
        RC_COUTL("Top: " << i << ". output read (" << x << ")"
            " (t=" << sc_time_stamp() << ")");
        if(x == static_cast<int>(STATUS))
          command_executed = true;
    }
}
