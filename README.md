# roscon_2019_ws
Demo materials for the ROSCon 2019 presentation.

## Setup
The demo contains 3 major components:
* ROS catkin workspace that implements the robot control part
* Google Action and Alexa Skill front-end
* A server which captures Google Action/Amazon Alexa requests and allows to access the request JSON by UMRF parser TeMoto Actions

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

* Follow this guide to set up the *Google Assistant Action*
* Follow this guide to set up the *Amazon Alexa Skill*

### Setting up the TeMoto Commander Proxy server

In case this server has been taken down or is inoperable, you can deploy the same server by clicking on the *Deploy to Heroku* button in https://github.com/RValner/temoto-commander-proxy