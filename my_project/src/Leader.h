#ifndef __Leader_AGENT__H
#define __Leader_AGENT__H

#include <string>
#include <math.h>
#include "enviro.h"

using namespace std;
namespace
{

    using namespace enviro;
    class TopRightCorner : public State, public AgentInterface
    {
    public:
        double x,y,X,Y;
        void entry(const Event &e) {
          x = 300;
          y = -150;
          cout << "Going to Top Right" << endl;
        }
        void during()
        {
            move_toward(x,y,30,15);
            cout << sensor_value(0) << ","<<sensor_value(1)<<","<<sensor_value(2) << endl;
            if ((x - position().x  < 0.1 && abs(y - position().y) < 0.1) && (sensor_value(0) > 100 && sensor_value(2) < 30)) {
              cout << "Arrived Top Right" << endl;
              emit(Event(tick_name));
            }
        }
        void exit(const Event &e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    class TopLeftCorner : public State, public AgentInterface
    {
    public:
        double x,y,X,Y;
        void entry(const Event &e) {
          x =  -300;
          y = -150;
          cout << "Going to Top Left" << endl;
        }
        void during()
        {
            move_toward(x,y,30,15);
            cout << sensor_value(0) << ","<<sensor_value(1)<<","<<sensor_value(2) << endl;
            if ((abs(x - position().x)  < 0.1 && abs(y - position().y) < 0.1) && (sensor_value(0) > 100 && sensor_value(1) > 150 && sensor_value(2) < 30)) {
              cout << "Arrived Top Left" << endl;
              emit(Event(tick_name));
            }
        }
        void exit(const Event &e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    class BotRightCorner : public State, public AgentInterface
    {
    public:
        double x,y,X,Y;
        void entry(const Event &e) {
          x = 300;
          y = 150;
          cout << "Going to Bot Right" << endl;
        }
        void during()
        {
            move_toward(x,y,30,15);
            cout << sensor_value(0) << ","<<sensor_value(1)<<","<<sensor_value(2) << endl;
            if ((abs(x - position().x)  < 0.1 && (y - position().y) < 0.1) && (sensor_value(0) > 100 && sensor_value(1) > 150 && sensor_value(2) < 30)) {
              cout << "Arrived Bot Right" << endl;
              emit(Event(tick_name));
            }
        }
        void exit(const Event &e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    class BotLeftCorner : public State, public AgentInterface
    {
    public:
        double x,y,X,Y;
        void entry(const Event &e) {
          x = -300;
          y = 150;
          cout << "Going to Bot Left" << endl;
        }
        void during()
        {
            move_toward(x,y,30,15);
            cout << sensor_value(0) << ","<<sensor_value(1)<<","<<sensor_value(2) << endl;
            if ((abs(x - position().x)  < 0.1 && (y - position().y) < 0.1) && (sensor_value(0) > 100 && sensor_value(1) > 150 && sensor_value(2) < 30)) {
              cout << "Arrived Bot Left" << endl;
              emit(Event(tick_name));
            }
        }
        void exit(const Event &e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };


    class LeaderController : public StateMachine, public AgentInterface
    {

    public:
        LeaderController() : StateMachine()
        {

            set_initial(top_right);
            tick_name = "tick_" + std::to_string(rand() % 1000); // use an agent specific generated
                                                                 // event name in case there are
            //add_transition(tick_name, top_right, top_left);
            //add_transition(tick_name, top_left,  bot_left);
            //add_transition(tick_name, bot_left,  bot_right);                                                      // multiple instances of this class
            //add_transition(tick_name, bot_right, top_right);

            add_transition(tick_name, top_right, bot_right);
            add_transition(tick_name, bot_right, bot_left);
            add_transition(tick_name, bot_left, top_left);                                                      // multiple instances of this class
            add_transition(tick_name, top_left, top_right);

            top_right.set_tick_name(tick_name);
            top_left.set_tick_name(tick_name);
            bot_left.set_tick_name(tick_name);
            bot_right.set_tick_name(tick_name);
        }


        TopRightCorner top_right;
        TopLeftCorner top_left;
        BotLeftCorner bot_left;
        BotRightCorner bot_right;
        std::string tick_name;
    };

    class Leader : public Agent
    {

    public:
        Leader(json spec, World &world) : Agent(spec, world)
        {
            add_process(wc);
        }

        LeaderController wc;
    };

    DECLARE_INTERFACE(Leader);

}

#endif
