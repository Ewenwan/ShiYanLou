/****************************************************************************
 *
 基于 卡尔曼滤波 的地形估计    使用 机身姿态  普通传感器数据  以及距离传感器数据 
 预测过程 + 驱动过程  + 校正过程  预测  飞机离地面高度   z轴速度     z轴加速度偏差(不变) 
 姿态数据 补偿  距离数据       GPS数据 校正Z轴速度 
\Firmware\src\lib\terrain_estimation
 ****************************************************************************/

/**
 * @file terrain_estimator.cpp
 * A terrain estimation kalman filter.
 */

#include "terrain_estimator.h"
#include <geo/geo.h>

#define DISTANCE_TIMEOUT 100000		// 微妙 time in usec after which laser is considered dead

TerrainEstimator::TerrainEstimator() :
	_distance_last(0.0f),
	_terrain_valid(false),
	_time_last_distance(0),
	_time_last_gps(0)
{
	/*
	// "terrain_estimator.h" 中的变量声明定义 
	 // 卡尔曼滤波器 相关变量 
	matrix::Vector<float, n_x> _x;  // 预测状态矩阵  1*3  地面高度  速度 z轴 加速度偏差 state: ground distance, velocity, accel bias in z direction
	float  _u_z;		        	// acceleration in earth z direction
	matrix::Matrix<float, 3, 3> _P;	   // 用于估计卡尔曼增益 等 的 描述 预测的协方差矩阵 covariance matrix  3*3 一般为 变量个数的 方阵 
	*/
	
	memset(&_x._data[0], 0, sizeof(_x._data));//初始化变量为0 初始状态 为0  
	_u_z = 0.0f;                              //Z轴 加速度 
	_P.setIdentity();                         // 协方差矩阵初始化为 单位阵 
	//[1 0 0]
	//[0 1 0]
	//[0 0 1]
}

// 判断距离测量结果 是否有效 
bool TerrainEstimator::is_distance_valid(float distance)
{
	if (distance > 40.0f || distance < 0.00001f) {
		return false;

	} else {
		return true;
	}
}

// 卡尔曼滤波 预测部分          时间       姿态   普通传感器数据   距离传感器数据(雷达 超声 宫外)
void TerrainEstimator::predict(float dt, const struct vehicle_attitude_s *attitude,
			       const struct sensor_combined_s *sensor,
			       const struct distance_sensor_s *distance)
{
	//获取当前姿态旋转矩阵   四元素-> 旋转矩阵 
	matrix::Dcmf R_att = matrix::Quatf(attitude->q);
	matrix::Vector<float, 3> a(&sensor->accelerometer_m_s2[0]);//加速度计读数(机身坐标系) 
	matrix::Vector<float, 3> u;                                //加速度计读数(导航坐标系)
	
	u = R_att * a;//将加速度计读数投影到导航坐标系；
	//得到Z轴的 加速度计
	_u_z = u(2) + CONSTANTS_ONE_G;//补偿重力加速度 9.80665f  m/s^2 compensate for gravity

	// dynamics matrix   预测转移矩阵 
	matrix::Matrix<float, n_x, n_x> A;
	A.setZero();//[0 1 0]
	A(0, 1) = 1;//[0 0 1]
	A(1, 2) = 1;//[0 0 0]

	// input matrix     驱动向量 
	matrix::Matrix<float, n_x, 1>  B;
	B.setZero(); //[0]
	B(1, 0) = 1; //[1]  
				 //[0]
				 
	// input noise variance  驱动过程 噪声 
	 // 博主认为此处 R 为加速度计参数
	float R = 0.135f;  

	// process noise convariance  预测转移过程 高斯噪声 
	//这里的Q为零阵，好像没有用，作用被上方的R代替；
	matrix::Matrix<float, n_x, n_x>  Q;
	Q(0, 0) = 0; //[0 0 0] 
	Q(1, 1) = 0; //[0 0 0]
				 //[0 0 0]
				 
	// do prediction 预测过程   转移过程 + 驱动过程 
	matrix::Vector<float, n_x>  dx = (A * _x) * dt;//状态变化 步长   Q为0 就没有加入预测过程噪声 
	dx(1) += B(1, 0) * _u_z * dt;// z轴 垂直速度  的加速度 补偿   驱动过程 
    // dx = [速度*时间 即 v*dt]          = 距离变化量 
	//		[(z轴加速度偏差+加速度)*dt]  = 速度变化量 
	//		[0]                          = z轴加速度偏差变化量 
	// propagate state and covariance matrix
	_x += dx;//状态累计  预测的 状态 
	_P += (A * _P + _P * A.transpose() + //预测的协方差啊    转移过程  + 驱动过程 
	       B * R * B.transpose() + Q) * dt;
}

// 校正 
void TerrainEstimator::measurement_update(uint64_t time_ref, const struct vehicle_gps_position_s *gps,
		const struct distance_sensor_s *distance,
		const struct vehicle_attitude_s *attitude)
{
	// terrain estimate is invalid if we have range sensor timeout
	// 超时后  地形预测不准确 
	if (time_ref - distance->timestamp > DISTANCE_TIMEOUT) {
		_terrain_valid = false;
	}

/***********飞机里地面距离  的补偿校正(用姿态速度补偿)**********/
    // 距离数据有更新 
	if (distance->timestamp > _time_last_distance) {
		matrix::Quatf q(attitude->q);//姿态四元素 
		matrix::Eulerf euler(q);     //得到欧拉角 
		float d = distance->current_distance;//当前距离 

		matrix::Matrix<float, 1, n_x> C;     //测量过程向量
		C(0, 0) = -1; // measured altitude,

		float R = 0.009f;//测量数据方差 
		
		// 实际测量值 
		//校正 roll  pitch 引起的高度变化
		matrix::Vector<float, 1> y;
		y(0) = d * cosf(euler.phi()) * cosf(euler.theta());//实际测量计算的到的结果 

		// residual  残差  
		//计算卡尔曼增益   K  =   P' * 测量过程向量C.转置 *（测量过程向量C * P' * 测量过程向量C.转置 + 测量过程高斯噪声方差R).inv
		matrix::Matrix<float, 1, 1> S_I = (C * _P * C.transpose());
		S_I(0, 0) += R;
		S_I = matrix::inv<float, 1> (S_I);//求逆 
		matrix::Matrix<float, n_x, 1> K = _P * C.transpose() * S_I;//卡尔曼增益 
		
		//误差 = 实际测量值 y -  测量过程向量C * 预测量 _x 
		matrix::Vector<float, 1> r = y - C * _x;
		// some sort of outlayer rejection 野值处理
		if (fabsf(distance->current_distance - _distance_last) < 1.0f) {//测量距离在正常范围内 
			_x += K * r; //校正最优预测值   本次最优估计值 X = X’ + K * 误差
			_P -= K * C * _P;//最优预测的最小均方误差矩阵 P = ( 1 - K * C) * P' = P' - K * C * P'
		}

		// if the current and the last range measurement are bad then we consider the terrain
		// estimate to be invalid
		if (!is_distance_valid(distance->current_distance) && !is_distance_valid(_distance_last)) {
			_terrain_valid = false;

		} else {
			_terrain_valid = true;//当前距离或者上次具有有一个符合范围，地形估计数据就有效 
		}

		_time_last_distance = distance->timestamp;   //更新时间 记录 
		_distance_last = distance->current_distance; //更新距离 记录 
	}
	
	
/***************Z轴速度 的校正补偿，使用GPS数据************************************/
    // GPS数据可用 
	if (gps->timestamp > _time_last_gps && gps->fix_type >= 3) {
		matrix::Matrix<float, 1, n_x> C;//测量过程向量 
		C(0, 1) = 1;

		float R = 0.056f;//GPS测量噪声 
     
	   // 实际测量值 
		matrix::Vector<float, 1> y;
		y(0) = gps->vel_d_m_s;

		// residual  计算卡尔曼增益  K  =   P' * 测量过程向量C.转置 *（测量过程向量C * P' * 测量过程向量C.转置 + 测量过程高斯噪声方差R).inv
		matrix::Matrix<float, 1, 1> S_I = (C * _P * C.transpose());
		S_I(0, 0) += R;
		S_I = matrix::inv<float, 1>(S_I);//求逆 
		matrix::Matrix<float, n_x, 1> K = _P * C.transpose() * S_I;//卡尔曼增益 
		
		//误差 = 实际测量值 y -  测量过程向量C * 预测量 _x 
		matrix::Vector<float, 1> r = y - C * _x;
		_x += K * r;       //校正最优预测值   本次最优估计值 X = X’ + K * 误差
		_P -= K * C * _P;  //最优预测的最小均方误差矩阵 P = ( 1 - K * C) * P' = P' - K * C * P'

		_time_last_gps = gps->timestamp;//时间更新 
	}

	// reinitialise filter if we find bad data
	// 如果数据 有错误 重新初始化 卡尔曼滤波器 参数  
	bool reinit = false;

	for (int i = 0; i < n_x; i++) {
		if (!PX4_ISFINITE(_x(i))) {//最优预测 值的检测 
			reinit = true;
		}
	}

	for (int i = 0; i < n_x; i++) {
		for (int j = 0; j < n_x; j++) {
			if (!PX4_ISFINITE(_P(i, j))) {// 预测过程协方差的检测 
				reinit = true;
			}
		}
	}

	if (reinit) {
		memset(&_x._data[0], 0, sizeof(_x._data));//预测状态初始化为0 
		_P.setZero();                             //协方差初始化为0 
		_P(0, 0) = _P(1, 1) = _P(2, 2) = 0.1f;
	}
	//[0.1 0 0]  ？？？ 
	//[0 0.1 0]
	//[0 0 0.1]

}
