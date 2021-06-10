## EE520_Project The Leader and Follower

This repository contains the source code for the leader and follower

## Overview

The main purpose of this project was to incorporate what we have learned about
C++, Elma and Enviro to make a robot that moves around the arena in a long looping path and
another robot that would follow the leader. The main requirements are:
1. Create a `Follower` robot that uses a range finder to stay a respectful distance behind the leader.
2. Next, add more `Follower` robots to create a train of robots.
3. Extend or improve the example in which a robot moves around the arena with robots following it. Create a longer train of robots. Create flocking behaviors, and so on.

## Implementation

As a starting base we took 2 different approaches. For the leader we based the class off of the Wanderer class. Using a finite state machine we would coordinate 4 locations (each being a different state to move to) to form our loop. The leader would then emit it's position and velocity for the follower to receive. Since the follower is to just follow we had it watch the emitted location of the Leader and would move to it's location. Through trial and error we would tune the follower to change speed based on its distance from the Leader.

## Challenges

There were 2 main challenges that came up during the development of this project.


- The first was tuning each robot to move smoothly. Because of the physics implemented in the code (friction and such) we had to mindful of all values selected in terms of position, velocity, and etc. For example, setting a rotational speed could cause the robot to overshoot and overturn thus it would take a few moment until it would correct itself and move straight. The followers required a lot of tuning in order for it maintain a good speed and distance behind the leader without crashing. To address this, it was a lot of trial and error of methods and values until we reached something consistent.

 - The second was trying to cleverly create a single follower class, that would know whether to follow the leader or follow the follower. To address this we took advantage of the built-in id() function. We then concatenate the id with our emit so that if there are multiple follower, each will emit a unique process.
```bash
emit(Event("F_Posn"+ std::to_string(robot_id), {{"x",position().x},{"y",position().y},{"v",v}}));
```

 To sort this we use an if statement to decide if the follower should be watching the leader or follower. See section below:

 ```bash
 void init() {
   robot_id = id();
   event_name = "F_posn" + to_string(robot_id);
   if (robot_id < 2) {
     watch("Leader_Posn", [this](Event e) {
       x = e.value()["x"];
       y = e.value()["y"];
       v = e.value()["v"];
     });
     /*
     watch("screen_click", [this](Event e) {
          Agent& v = add_agent("follower", 0, 0, 0, {{"fill": "blue"},{"stroke": "black"}});
      });*/
  } else {

    //watches the previous robot
    watch("F_Posn" + std::to_string(robot_id-1), [this](Event e) {
      x = e.value()["x"];
      y = e.value()["y"];
      v = e.value()["v"];
    });
    /*watch("screen_click", [this](Event e) {
         Agent& v = add_agent("follower", 0, 0, 0, {{"fill": "blue"},{"stroke": "black"}});
     });*/
  }
 }

## Set Up Instructions

Please start your docker container with something like

```bash
docker pull klavins/elma:latest
```
To start a Docker container with ENVIRO pre-loaded into it, do:

```bash
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:alpha bash
esm start
```

The above commands do the following:

- The `-p80:80 option maps _port_ 80 from the Docker container to ports on your host computer. This should allow you to go to

  > http://localhost
  > with your web browser and see the ENVIRO client. It should say "Error: Failed to fetch. Is the server running?" at this point.

- The `-p8765:8765` option allows the ENVIRO client to communicate with the `enviro` server (not yet running) via a _WebSocket_.

- The command `esm start` uses the <u>e</u>nviro <u>s</u>etup <u>m</u>anager to start a web server from which your browser can access the ENVRIO client.

- Clone the following: git clone https://github.com/Chiguez/EE520_Project

-Make the project and start the enviro as follows:
```bash
make
enviro
```
From here you should be able to view the robot and followers from http://localhost

## Acknowledgements
- Lectures from https://github.com/tbonaciUW/EEP_520_Spring2021
- Info on ENVIRO: https://github.com/klavinslab/enviro
- Info on Elma: http://klavinslab.org/elma/index.html
