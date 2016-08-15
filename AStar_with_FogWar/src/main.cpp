#include <stdio.h>
#include <stdlib.h>
#include <iostream>       // std::cout
#include <stack>          // std::stack

using namespace std;

typedef enum {AHEAD, BACK, UP, DOWN, LEFT, RIGHT} Movement;

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

int main(void){

  int sensor_up;
  int sensor_down;
  int sensor_left;
  int sensor_right;

  int sensor_finish;

  bool im_standing = true;
  bool trying_right = false;
  bool trying_left = false;

  bool tried_left = false;
  bool tried_right = false;

  do{
    scanf("%d %d %d %d %d", &sensor_finish, &sensor_up, &sensor_down, &sensor_left, &sensor_right);

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
      }
      //else if I can go ahead but I'm lying
      else if(sensor_up != 1 && !im_standing){
        actions.push(new Action(AHEAD, true, false));
        cout << "avanti strisciando" << endl;
        tried_left = false;
        tried_right = false;
        trying_right = true;
        trying_left = false;
      }
      //else if I can't go ahead standing
      else if(sensor_up == 1 && im_standing){
        actions.push(new Action(DOWN, true, false));
        cout << "c'è un ostacolo" << endl;
        im_standing = false;
      }
      //else if I can't go ahead lying (try to go right)
      else if(sensor_up == 1 && !im_standing && sensor_right != 1 && !trying_left && !tried_right){
        actions.push(new Action(RIGHT, true, true));
        cout << "provo ad andare a dx" << endl;
        trying_right = true;
        trying_left = false;
      }
      //else if I can't go ahead lying (try to go left)
      else if(sensor_up == 1 && !im_standing && sensor_left != 1 && !trying_right && !tried_left){
        actions.push(new Action(LEFT, true, true));
        cout << "provo ad andare a sx" << endl;
        trying_right = false;
        trying_left = true;
      }
      //else if I can't try another movement to right
      else if(sensor_up == 1 && sensor_right == 1 && trying_right){
          while(actions.top()->m == RIGHT){
            actions.pop();
            cout << "torno indietro a sx" << endl;
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
          }
          tried_left = true;
          trying_left = false;
          trying_right = false;
      }

    }

  }while(sensor_finish != 1);

  cout << "finito!" <<endl;

  return 0;

}
