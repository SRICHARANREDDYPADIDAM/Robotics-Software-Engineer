#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "ball_chaser/DriveToTarget.h"

/* Define a global client that can request services */
ros::ServiceClient srvClient;

/* This function calls the command_robot service to drive the robot in the specified direction */
void drive_robot(float lin_x, float ang_z)
{
  // Request a service and pass the velocities to it to drive the robot
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;
  
  // Call the drive_bot service and pass the requested velocities
  if (!srvClient.call(srv)) {
    ROS_ERROR("Failed to call service drive_bot!");
  }
  
}


/* This callback function continuously executes and reads the image data */
void process_image_callback(const sensor_msgs::Image img)
{
  int white_pixel = 255;

  /* ***
    This function works in those steps:
    Step I: Loop through each pixel in the image and check if there's a bright white pixel;
    Step II: If yes, identify whether this pixel falls in the left or right side of the image;
    Step III: Depending on the white ball position, call the drive_bot function and pass velocities to it.
              (Note -- it will request a stop when there's no white ball seen by the camera.)
  * ***/
  bool is_ball_found = false;
  float lin_x = 0;
  float ang_z = 0;

  for (int i = 0; i < img.height * img.step; i += 3) {
    if (img.data[i] == 255 && img.data[i+1] == 255 && img.data[i+2] == 255) {
      is_ball_found = true;
      int row_found = i % img.step;
      if(row_found < img.step /3){
      lin_x = 0.0;
      ang_z = 0.2;
      }
      else if (row_found > img.step * 2/3) {
      lin_x =0.0;
      ang_z = -0.2;
      }
      else{
      lin_x = 0.5;
      ang_z = 0.0;
      }
	break;
      }
    }
    drive_robot(lin_x, ang_z);
}


int main(int argc, char** argv)
{
  // Initialize the process_image node and create a NodeHandle to it
  ros::init(argc, argv, "process_image");
  ros::NodeHandle nh;

  // Define a service client capabale of requesting services from "/ball_chaser/command_robot"
  srvClient = nh.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

  // Subscribe to /camera/rgb/image_raw topic to read the image data inside
  ros::Subscriber sub = nh.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

  // Handle ROS communication events
  ros::spin();

  return 0;

}
