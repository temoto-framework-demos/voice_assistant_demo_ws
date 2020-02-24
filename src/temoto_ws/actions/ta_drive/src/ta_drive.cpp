
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *  The basis of this file has been automatically generated
 *  by the TeMoto action package generator. Modify this file
 *  as you wish but please note:
 *
 *    WE HIGHLIY RECOMMEND TO REFER TO THE TeMoto ACTION
 *    IMPLEMENTATION TUTORIAL IF YOU ARE UNFAMILIAR WITH
 *    THE PROCESS OF CREATING CUSTOM TeMoto ACTION PACKAGES
 *    
 *  because there are plenty of components that should not be
 *  modified or which do not make sence at the first glance.
 *
 *  See TeMoto documentation & tutorials at: 
 *    https://temoto-telerobotics.github.io
 *
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/* REQUIRED BY TEMOTO */
#include <class_loader/class_loader.h>
#include "ta_drive/temoto_action.h"
#include "ta_drive/macros.h"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

/* 
 * ACTION IMPLEMENTATION of TaDrive 
 */
class TaDrive : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaDrive()
{
  // ---> YOUR CONSTRUCTION ROUTINES HERE <--- //
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  verb = GET_PARAMETER("verb", std::string);
  direction = GET_PARAMETER("direction", std::string);
  parseVerbalCommands();
  
  // Create a node handle and cmd vel publisher
  ros::NodeHandle nh;
  ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",10);
  float b = 0.9; // Complementary filter gain

  // Initialize the cmd_vel message
  geometry_msgs::Twist cmd_vel_msg;
  cmd_vel_msg.linear.x = 0;
  cmd_vel_msg.linear.y = 0;
  cmd_vel_msg.linear.z = 0;
  cmd_vel_msg.angular.x = 0;
  cmd_vel_msg.angular.y = 0;
  cmd_vel_msg.angular.z = 0;

  // Start the main loop
  while(actionOk())
  {
    cmd_vel_msg.linear.x =  b*cmd_vel_msg.linear.x + (1-b)*gain_linear*vel_lin_x;
    cmd_vel_msg.angular.z = b*cmd_vel_msg.angular.z + (1-b)*gain_angular*vel_ang_z;
    cmd_vel_pub.publish(cmd_vel_msg);
    ros::Duration(0.1).sleep();
  }
}

/**
 * @brief Process the verbal commands and set cmd_vel based on the command
 * 
 */
void parseVerbalCommands()
{
  TEMOTO_INFO_STREAM("Got a verbal command: verb = " << verb << "; direction = " << direction);
  if(verb == "stop")
  {
    vel_lin_x = 0;
    vel_ang_z = 0;
  }
  else if ((verb == "go")||(verb == "move")||(verb == "drive"))
  {
    /*
     * Handle translation commands
     */
    if ((direction == "forward"||(direction == "forwards"))||(direction == "ahead")||(direction == "forth")||(direction == "ahead")||(direction == "fore"))
    {
      // if direction = positive
      vel_lin_x = 1;
      vel_ang_z = 0;
    } 
    else if ((direction == "backward") ||
             (direction == "rearward") ||
             (direction == "rearwards") ||
             (direction == "rear") ||
             (direction == "back") ||
             (direction == "backwards") ||
             (direction == "aft")) 
    {
      // if direction = negative
      vel_lin_x = -1;
      vel_ang_z = 0;
    }
    // otherwise
    else 
    {
      //direction error
      TEMOTO_ERROR("Can't move in that direction");
      vel_lin_x = 0;
      vel_ang_z = 0;
    }
  } 
  else if ((verb == "rotate")||(verb=="turn"))
  {     
    /*
     * Handle rotation commands
     */
    if ((direction == "left")||(direction == "counterclockwise")||(direction == "anti clockwise"))
    {
      // if direction = positive (left, counterclockwise, anti
      vel_lin_x = 0;
      vel_ang_z = 1;
    }
    else if ((direction == "right")||(direction == "clockwise"))
    {
      // else if direction = negative (right, clockwise)
      vel_lin_x = 0;
      vel_ang_z = -1;
    }
    else
    { 
      //direction error
      TEMOTO_ERROR("Can't turn in that direction");
      vel_lin_x = 0;
      vel_ang_z = 0;   
    }
  }
  else 
  {
    // verb error
    TEMOTO_ERROR("Can't execute that verb");
    vel_lin_x = 0;
    vel_ang_z = 0;
  }
}

/**
 * @brief Invoked when parameters are updated
 * 
 */
void onParameterUpdate()
{
  TEMOTO_INFO_STREAM("Updating parameters ...");

  verb = GET_PARAMETER("verb", std::string);
  direction = GET_PARAMETER("direction", std::string);
  parseVerbalCommands();

  TEMOTO_INFO_STREAM("Parameters updated");
}

// Destructor
~TaDrive()
{
  // ---> YOUR CONSTRUCTION ROUTINES HERE <--- //
  TEMOTO_PRINT_OF("Destructor", getUmrfPtr()->getName());
}

std::string verb;
std::string direction;
float vel_lin_x = 0;
float vel_ang_z = 0;
float gain_linear = 0.15;
float gain_angular = 0.4;

}; // TaDrive class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaDrive, ActionBase);
