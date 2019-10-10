# roscon_2019_ws
Demo materials for the ROSCon 2019 presentation.

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
