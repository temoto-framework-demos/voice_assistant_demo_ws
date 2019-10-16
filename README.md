# roscon_2019_ws
Demo materials for the ROSCon 2019 presentation.

## Setup
The demo contains 3 major components:
* ***ROS catkin workspace*** that implements the robot control part
* ***Google Action*** and ***Alexa Skill*** front-end
* ***A server*** which captures *Google Action / Amazon Alexa* requests and allows to access the request JSON by UMRF parser TeMoto Actions ([Google Assistant](https://github.com/temoto-telerobotics-demos/roscon_2019_ws/tree/master/src/roscon_temoto_ws/actions/ta_google_assistant_parser); [Amazon Alexa](https://github.com/temoto-telerobotics-demos/roscon_2019_ws/tree/master/src/roscon_temoto_ws/actions/ta_amazon_alexa_parser))

### Setting up the catkin workspace

``` bash
git clone https://github.com/temoto-telerobotics-demos/roscon_2019_ws
cd roscon_2019_ws
git submodule update --init --recursive

sudo apt install libcurlpp-dev
```

If you want the latest versions of the submodules, then (after that the state of this workspace may not be the same as it was for the demo)
``` bash
git submodule foreach git pull origin master
```

Build the workspace
``` bash
catkin build
```

### Setting up the front-end system

* Follow [this guide](https://github.com/temoto-telerobotics-demos/roscon_2019_ws/tree/master/src/roscon_temoto_ws/actions/ta_google_assistant_parser/google_assistant_project) to set up the Google Assistant Action
* Follow [this guide](https://github.com/temoto-telerobotics-demos/roscon_2019_ws/tree/master/src/roscon_temoto_ws/actions/ta_amazon_alexa_parser/amazon_alexa_skill) to set up the Amazon Alexa Skill 

### Setting up the TeMoto Commander Proxy server

In case this server has been taken down or is inoperable, you can deploy the same server by clicking on the `Deploy to Heroku` button in https://github.com/RValner/temoto-commander-proxy

## Running the demo

Launch the robot simulator
``` bash
roslaunch turtlebot3_fake turtlebot3_fake.launch 
```

Launch TeMoto
``` bash
roslaunch roscon_temoto_ws temoto.launch temoto_namespace:=Johnny
```

The `temoto_namespace` argument is important for addressing the appropriate robot, e.g., if you say "Turn Michael clockwise" to Google Assistant
then *Michael* is parsed as a `temoto_namespace` by [GoogleActions-to-UMRF parser action](https://github.com/temoto-telerobotics-demos/roscon_2019_ws/tree/master/src/roscon_temoto_ws/actions/ta_google_assistant_parser)