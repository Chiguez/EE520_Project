#ifndef __FOLLOWER_AGENT__H
#define __FOLLOWER_AGENT__H

#include "enviro.h"
#include <math.h>
#include <chrono>
#include <thread>

using namespace enviro;
using namespace std;

class followerController : public Process, public AgentInterface {

    public:
    double x,y,v,r_target,r_loc;
    double d_error;
    int robot_id;
    string event_name;
    followerController() : Process(), AgentInterface() {}

    void init() {
      robot_id = id();
      event_name = "F_posn" + to_string(robot_id);
      if (robot_id < 2) {
        watch("Leader_Posn", [this](Event e) {
          x = e.value()["x"];
          y = e.value()["y"];
          v = e.value()["v"];
        });
        watch("screen_click", [this](Event e) {
             Agent& v = add_agent("follower", 0, 0, 0, {{"fill": "bllue"},{"stroke": "black"}});
         });
     } else {
       watch("F_Posn" + std::to_string(robot_id-1), [this](Event e) {
         x = e.value()["x"];
         y = e.value()["y"];
         v = e.value()["v"];
       });
       watch("screen_click", [this](Event e) {
            Agent& v = add_agent("follower", 0, 0, 0, {{"fill": "bllue"},{"stroke": "black"}});
        });
     }
    }

    void start() {}
    void update() {

      emit(Event("F_Posn"+ std::to_string(robot_id), {{"x",position().x},{"y",position().y},{"v",v}}));
      /*
      if (robot_id < 2) {
        watch("Leader_Posn", [this](Event e) {
          x = e.value()["x"];
          y = e.value()["y"];
          v = e.value()["v"];
        });
     } else {
       watch("F_Posn" + std::to_string(robot_id-1), [this](Event e) {
         x = e.value()["x"];
         y = e.value()["y"];
         v = e.value()["v"];
       });
     }
     */
      r_target = pow(x*x+y*y,0.5);
      r_loc =  pow(position().x*position().x+position().y*position().y,0.5);
      d_error=pow ((x-position().x)*(x-position().x)+(y-position().y)*(y-position().y),.5);
     //cout << "moving to: " << target_x <<  "," << target_y << endl;
     if (d_error > 100) {
        //cout << "fast" << endl;
        move_toward(x,y,1.2*v,100);
     } else if (40 < d_error && d_error < 100) {
        //cout << "slow" << endl;
        move_toward(x,y,0.5*v,100);
     } else {
        //cout << "too close!" << endl;
        track_velocity(0,0);
     }

/*
     d_error=pow ((x-position().x)*(x-position().x)+(y-position().y)*(y-position().y),.5);

        if(d_error>=100){
            //move_toward(goal_x,goal_y,100,500);
            move_toward(x,y,(v)+50,500);
        }
        if(d_error<100){
            //move_toward(goal_x,goal_y,25,500);
            move_toward(x,y,(v)-25,500);
        } */


     //other_robot_id = 0;
     //Agent& other_robot = find_agent(other_robot_id);
     //attach_to(other_robot);
   }
    void stop() {}

};

class follower : public Agent {
    public:
    follower(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    followerController c;
};

DECLARE_INTERFACE(follower)

#endif
