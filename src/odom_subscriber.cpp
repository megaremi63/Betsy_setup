#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>

#include <iostream>
  

  float x=0;
  float y=0;
  float th=0;

//fonction ajouter
void poseCallback(const geometry_msgs::Pose2DConstPtr& pose)
{
	
	x = pose->x;
	y = pose->y;
	th = pose->theta;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "odom_tf_pub");

  ros::NodeHandle n;
  tf::TransformBroadcaster odom_broadcaster;

//sub ajouter
  ros::Subscriber pose_sub = n.subscribe("pose", 10, poseCallback);

//ros time
  ros::Time current_time;
  current_time = ros::Time::now();

//frequence
  ros::Rate r(10);
  while(n.ok()){

    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();


    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    
    
    r.sleep();
  }
}
