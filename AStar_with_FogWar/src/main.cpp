#include <stdio.h>
#include <stdlib.h>
#include <iostream>       // std::cout
#include <stack>          // std::stack
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>

#define SHMSZ     27

using namespace std;

typedef enum {AHEAD=0, BACK, UP, DOWN, LEFT, RIGHT} Movement;

class Action{

public:

  Movement m;
  bool obstacle_up;
  bool obstacle_down;

  Action(Movement m, bool obstacle_up, bool obstacle_down){
    this->m = m;
    this->obstacle_up = obstacle_up;
    this->obstacle_down = obstacle_down;
  }
};

std::stack<Action*> actions;
bool trying_right = false;
bool trying_left = false;

void send_message(Movement m, char *shm_read, char *shm_write){

  char *s = shm_write;
  char *s_foo;

  *s++ = ((int) m) + 48;
  cout << "scritto " << ((int) m) + 48 << endl;
  /*
   * change the first character of the
   * segment to '*', indicating we have read
   * the segment.
   */
  *s = '*';
  *shm_read = '*';
  //sleep(1);

}

void wait(char *shm_read, char *shm_write, int time){

  int counter = 0;
  char *s = shm_write;

  while(time--){
    counter = 0;
    s = shm_write;

    while(*(shm_read+counter) != '*' || counter == 0){
      if(*(shm_read+counter) != '*'){
        counter++;
      }
    }
    *s++ = 0;
    *s = '*';
    *shm_read = '*';
  }
}

int main(void){

  int sensor_up = 0;
  int sensor_down = 0;
  int sensor_left = 0;
  int sensor_right = 0;
  int sensor_finished_obstacle = 0;

  int sensor_finish = 0;

  bool im_standing = true;

  bool tried_left = false;
  bool tried_right = false;

  bool under_the_obstacle = false;

  int count_far_from_obstacle = 0;

  string sensors_values = "";

  //server
  char c;
  int shmid_write, shmid_read;
  key_t key_write, key_read;
  char *shm_write, *s_write, *shm_read, *s_read;
  key_write = 5679;
  key_read = 5678;

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

  int counter = 0;

  do{

    counter = 0;
    s_read = shm_read;

    sensors_values = "";
    while(*(s_read+counter) != '*' || counter == 0){
      if(*(s_read+counter) != '*'){
        sensors_values += *(s_read+counter);
        counter++;
      }
    }

    cout << "letto: " << sensors_values << endl;

    stringstream stream(sensors_values);

    stream >> sensor_finish >> sensor_finished_obstacle >> sensor_up >> sensor_down >> sensor_left >> sensor_right;

    //if path not finished
    if(sensor_finish != 1){
      //if I can go ahead
      if(sensor_up != 1 && im_standing){
        actions.push(new Action(AHEAD, false, false));
        cout << "avanti" << endl;
        tried_left = false;
        tried_right = false;
        trying_right = true;
        trying_left = false;
        send_message(AHEAD, shm_read, shm_write);
      }
      //else if I can go ahead but I'm lying
      else if(sensor_up != 1 && !im_standing && sensor_finished_obstacle != 1 && under_the_obstacle == false){
        actions.push(new Action(AHEAD, true, false));
        cout << "avanti strisciando" << endl;
        tried_left = false;
        tried_right = false;
        trying_right = true;
        trying_left = false;
        send_message(AHEAD, shm_read, shm_write);
      }
      //else if I'm under the obstacle
      else if(sensor_up != 1 && !im_standing && sensor_finished_obstacle == 1){
          actions.push(new Action(AHEAD, true, false));
          under_the_obstacle = true;
          cout << "sono sotto l'ostacolo" << endl;
          send_message(AHEAD, shm_read, shm_write);
      }
      else if(sensor_up != 1 && !im_standing && sensor_finished_obstacle != 1 && under_the_obstacle == true && count_far_from_obstacle++ <= 10){
        actions.push(new Action(AHEAD, true, false));
        under_the_obstacle = true;
        cout << "passetti dopo l'ostacolo" << endl;
        send_message(AHEAD, shm_read, shm_write);
      }
      //else if I can grow up
      else if(sensor_up != 1 && !im_standing && sensor_finished_obstacle != 1 && under_the_obstacle == true && count_far_from_obstacle > 10){
        actions.push(new Action(UP, true, false));
        cout << "mi rialzo" << endl;
        tried_left = false;
        tried_right = false;
        trying_right = true;
        trying_left = false;
        im_standing = true;
        under_the_obstacle = false;
        count_far_from_obstacle = 0;
        send_message(UP, shm_read, shm_write);
        wait(shm_read, shm_write, 1);
      }
      //else if I can't go ahead standing
      else if(sensor_up == 1 && im_standing){
        actions.push(new Action(DOWN, true, false));
        cout << "c'è un ostacolo" << endl;
        im_standing = false;
        send_message(DOWN, shm_read, shm_write);
        wait(shm_read, shm_write, 1);
      }
      //else if I can't go ahead lying (try to go right)
      else if(sensor_up == 1 && !im_standing && sensor_right != 1 && !trying_left && !tried_right){
        actions.push(new Action(RIGHT, true, true));
        cout << "provo ad andare a dx" << endl;
        trying_right = true;
        trying_left = false;
        send_message(RIGHT, shm_read, shm_write);
        wait(shm_read, shm_write, 2);
      }
      //else if I can't go ahead lying (try to go left)
      else if(sensor_up == 1 && !im_standing && sensor_left != 1 && !trying_right && !tried_left){
        actions.push(new Action(LEFT, true, true));
        cout << "provo ad andare a sx" << endl;
        trying_right = false;
        trying_left = true;
        send_message(LEFT, shm_read, shm_write);
        wait(shm_read, shm_write, 2);
      }
      //else if I can't try another movement to right
      else if(sensor_up == 1 && sensor_right == 1 && trying_right){
          while(actions.top()->m == RIGHT){
            actions.pop();
            cout << "torno indietro a sx" << endl;
            send_message(LEFT, shm_read, shm_write);
            wait(shm_read, shm_write, 2);
          }
          tried_right = true;
          trying_left = false;
          trying_right = false;
      }
      //else if I can't try another movement to right
      else if(sensor_up == 1 && sensor_left == 1 && trying_left){
          while(actions.top()->m == LEFT){
            actions.pop();
            cout << "torno indietro a dx" << endl;
            send_message(RIGHT, shm_read, shm_write);
            wait(shm_read, shm_write, 2);
          }
          tried_left = true;
          trying_left = false;
          trying_right = false;
      }
      else{
        cout << "boh" << endl;
        return 0;
      }

    }

  }while(sensor_finish != 1);

  send_message(DOWN, shm_read, shm_write);

  cout << "finito!" <<endl;

  return 0;

}
