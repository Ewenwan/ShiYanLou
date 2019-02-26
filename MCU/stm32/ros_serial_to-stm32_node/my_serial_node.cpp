#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <serial/serial.h>
#include <tf2_msgs/TFMessage.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Transform.h>
#include <trajectory_msgs/MultiDOFJointTrajectory.h>
#include <trajectory_msgs/MultiDOFJointTrajectoryPoint.h>

serial::Serial ser;

ros::Publisher Reference_pub;

#define sendBufsize 44
unsigned char send_bufferOdometry[sendBufsize];
unsigned char send_bufferReference[sendBufsize];

typedef union{
	unsigned char cv[4];
	float fv;
}float_union;


void callback(const nav_msgs::OdometryConstPtr &odom_msg){
	float_union position_x,position_y,position_z,velocity_x,
		velocity_y,velocity_z,Quaternion0,Quaternion1,Quaternion2,Quaternion3;

	Quaternion0.fv = odom_msg->pose.pose.orientation.w;
	Quaternion1.fv = odom_msg->pose.pose.orientation.x;
	Quaternion2.fv = odom_msg->pose.pose.orientation.y;
	Quaternion3.fv = odom_msg->pose.pose.orientation.z;

	position_x.fv = odom_msg->pose.pose.position.x;
	position_y.fv = odom_msg->pose.pose.position.y;
	position_z.fv = odom_msg->pose.pose.position.z;

	velocity_x.fv = odom_msg->twist.twist.linear.x;
	velocity_y.fv = odom_msg->twist.twist.linear.y;
	velocity_z.fv = odom_msg->twist.twist.linear.z;

	memset(send_bufferOdometry,0,sizeof(send_bufferOdometry));
	send_bufferOdometry[0] = 0x55;
	send_bufferOdometry[1] = 0xAA;
	send_bufferOdometry[2] = 0x10;

	send_bufferOdometry[3] = position_x.cv[0];
	send_bufferOdometry[4] = position_x.cv[1];
	send_bufferOdometry[5] = position_x.cv[2];
	send_bufferOdometry[6] = position_x.cv[3];

	send_bufferOdometry[7] = position_y.cv[0];
	send_bufferOdometry[8] = position_y.cv[1];
	send_bufferOdometry[9] = position_y.cv[2];
	send_bufferOdometry[10] = position_y.cv[3];

	send_bufferOdometry[11] = position_z.cv[0];
	send_bufferOdometry[12] = position_z.cv[1];
	send_bufferOdometry[13] = position_z.cv[2];
	send_bufferOdometry[14] = position_z.cv[3];

	send_bufferOdometry[15] = velocity_x.cv[0];
	send_bufferOdometry[16] = velocity_x.cv[1];
	send_bufferOdometry[17] = velocity_x.cv[2];
	send_bufferOdometry[18] = velocity_x.cv[3];

	send_bufferOdometry[19] = velocity_y.cv[0];
	send_bufferOdometry[20] = velocity_y.cv[1];
	send_bufferOdometry[21] = velocity_y.cv[2];
	send_bufferOdometry[22] = velocity_y.cv[3];

	send_bufferOdometry[23] = velocity_z.cv[0];
	send_bufferOdometry[24] = velocity_z.cv[1];
	send_bufferOdometry[25] = velocity_z.cv[2];
	send_bufferOdometry[26] = velocity_z.cv[3];

	send_bufferOdometry[27] = Quaternion0.cv[0];
	send_bufferOdometry[28] = Quaternion0.cv[1];
	send_bufferOdometry[29] = Quaternion0.cv[2];
	send_bufferOdometry[30] = Quaternion0.cv[3];

	send_bufferOdometry[31] = Quaternion1.cv[0];
	send_bufferOdometry[32] = Quaternion1.cv[1];
	send_bufferOdometry[33] = Quaternion1.cv[2];
	send_bufferOdometry[34] = Quaternion1.cv[3];

	send_bufferOdometry[35] = Quaternion2.cv[0];
	send_bufferOdometry[36] = Quaternion2.cv[1];
	send_bufferOdometry[37] = Quaternion2.cv[2];
	send_bufferOdometry[38] = Quaternion2.cv[3];

	send_bufferOdometry[39] = Quaternion3.cv[0];
	send_bufferOdometry[40] = Quaternion3.cv[1];
	send_bufferOdometry[41] = Quaternion3.cv[2];
	send_bufferOdometry[42] = Quaternion3.cv[3];

	send_bufferOdometry[43] = 0xAA;

	ser.write(send_bufferOdometry,sendBufsize);
}

void callback1(const trajectory_msgs::MultiDOFJointTrajectoryConstPtr &trajectory_msg){
	float_union reference_posx,reference_posy,reference_posz,reference_velx,
				reference_vely,reference_velz,reference_accx,reference_accy,reference_accz;
  geometry_msgs::PointStamped point_stamped;

	reference_posx.fv = trajectory_msg->points[0].transforms[0].translation.x;
	reference_posy.fv = trajectory_msg->points[0].transforms[0].translation.y;
	reference_posz.fv = trajectory_msg->points[0].transforms[0].translation.z;

	reference_velx.fv = trajectory_msg->points[0].velocities[0].linear.x;
	reference_vely.fv = trajectory_msg->points[0].velocities[0].linear.y;
	reference_velz.fv = trajectory_msg->points[0].velocities[0].linear.z;

	reference_accx.fv = trajectory_msg->points[0].accelerations[0].linear.x;
	reference_accy.fv = trajectory_msg->points[0].accelerations[0].linear.y;
	reference_accz.fv = trajectory_msg->points[0].accelerations[0].linear.z;

  point_stamped.header.frame_id = "world";
  point_stamped.header.seq = trajectory_msg->header.seq;
  point_stamped.header.stamp = trajectory_msg->header.stamp;

  point_stamped.point.x = reference_posx.fv;
  point_stamped.point.y = reference_posy.fv;
  point_stamped.point.z = reference_posz.fv;

  Reference_pub.publish(point_stamped);

	memset(send_bufferReference,0,sizeof(send_bufferReference));
	send_bufferReference[0] = 0x55;
	send_bufferReference[1] = 0xAA;
	send_bufferReference[2] = 0x20;

	send_bufferReference[3] = reference_posx.cv[0];
	send_bufferReference[4] = reference_posx.cv[1];
	send_bufferReference[5] = reference_posx.cv[2];
	send_bufferReference[6] = reference_posx.cv[3];

	send_bufferReference[7] = reference_posy.cv[0];
	send_bufferReference[8] = reference_posy.cv[1];
	send_bufferReference[9] = reference_posy.cv[2];
	send_bufferReference[10] = reference_posy.cv[3];

	send_bufferReference[11] = reference_posz.cv[0];
	send_bufferReference[12] = reference_posz.cv[1];
	send_bufferReference[13] = reference_posz.cv[2];
	send_bufferReference[14] = reference_posz.cv[3];

	send_bufferReference[15] = reference_velx.cv[0];
	send_bufferReference[16] = reference_velx.cv[1];
	send_bufferReference[17] = reference_velx.cv[2];
	send_bufferReference[18] = reference_velx.cv[3];

	send_bufferReference[19] = reference_vely.cv[0];
	send_bufferReference[20] = reference_vely.cv[1];
	send_bufferReference[21] = reference_vely.cv[2];
	send_bufferReference[22] = reference_vely.cv[3];

	send_bufferReference[23] = reference_velz.cv[0];
	send_bufferReference[24] = reference_velz.cv[1];
	send_bufferReference[25] = reference_velz.cv[2];
	send_bufferReference[26] = reference_velz.cv[3];

	send_bufferReference[27] = reference_accx.cv[0];
	send_bufferReference[28] = reference_accx.cv[1];
	send_bufferReference[29] = reference_accx.cv[2];
	send_bufferReference[30] = reference_accx.cv[3];

	send_bufferReference[31] = reference_accy.cv[0];
	send_bufferReference[32] = reference_accy.cv[1];
	send_bufferReference[33] = reference_accy.cv[2];
	send_bufferReference[34] = reference_accy.cv[3];

	send_bufferReference[35] = reference_accz.cv[0];
	send_bufferReference[36] = reference_accz.cv[1];
	send_bufferReference[37] = reference_accz.cv[2];
	send_bufferReference[38] = reference_accz.cv[3];

	send_bufferReference[39] = 0x00;
	send_bufferReference[40] = 0x00;
	send_bufferReference[41] = 0x00;
	send_bufferReference[42] = 0x00;

	send_bufferReference[43] = 0xAA;

	ser.write(send_bufferReference,sendBufsize);


//	ROS_INFO(" reference_posx.fv: %f", reference_posx.fv);
//	ROS_INFO(" reference_posy.fv: %f", reference_posy.fv);
//	ROS_INFO(" reference_posz.fv: %f \r\n", reference_posz.fv);
}

int main (int argc, char** argv){
     ros::init(argc, argv, "my_serial_node");
     ros::NodeHandle n;
     //订阅主题command
     ros::Subscriber Odometry_sub = n.subscribe("/vins_estimator/odometry", 1000, callback);
     ros::Subscriber Navigator_sub = n.subscribe("/command/trajectory", 1000,callback1);

     Reference_pub = n.advertise<geometry_msgs::PointStamped>("/reference/trajectory",1000);
     try
     {
        ser.setPort("/dev/ttyUSB0");
        ser.setBaudrate(230400);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
     }
     catch (serial::IOException& e)
     {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
     }

     if(ser.isOpen()){
         ROS_INFO_STREAM("Serial Port initialized");
     }else{
         return -1;
     }

     ros::Rate loop_rate(500);
     while(ros::ok()){
        ros::spinOnce();
				loop_rate.sleep();
     }
     return 0;
}
