/****************************************************************************

驱动ID

PX4使用驱动ID将独立传感器贯穿于整个系统。这些ID存储于配置参数中，用于匹配传感器校正值，以及决定哪些传感器被记录到log中。

传感器的顺序（例如一个是/dev/mag0，另一个是/dev/mag1）于优先级是不挂钩的，优先级实际是在发布uORB topic时确定的。

举个例子

有关系统上三个磁力计的示例，使用飞行日志（.px4log）转存变量。三个参数(devtype、adress、bus)对传感器ID进行编码，

MAG_PRIME识别哪个磁力计被选为主传感器。每一个MAGx_ID是一个24bit数值，左面手工填零补充。

CAL_MAG0_ID = 73225.0
CAL_MAG1_ID = 66826.0
CAL_MAG2_ID = 263178.0
CAL_MAG_PRIME = 73225.0


大家先不妨对下面的24位ID进行解码。

通过I2C连接的外部HMC5883，总线1，地址0x1E：在log中以IMU.MagX显示。

# device ID 73225 in 24-bit binary:
00000001  00011110  00001 001

# decodes to:
HMC5883   0x1E    bus 1 I2C


通过SPI连接的内部HMC5883，总线1，选择slot5。在log中以IMU1.MagX显示。

# device ID 66826 in 24-bit binary:
00000001  00000101  00001 010

# decodes to:
HMC5883   dev 5   bus 1 SPI

以及通过SPI总线连接的内部MPU9250磁力计，总线1，从设备选择slot4。在log中以IMU2.MagX显示。

# device ID 263178 in 24-bit binary:
00000100  00000100  00001 010

#decodes to:
MPU9250   dev 4   bus 1 SPI

设备ID编码

根据此格式，设备ID是一个24位的数字。注意，第一字段是上述解码示例中的最低有效位。

struct DeviceStructure {
  enum DeviceBusType bus_type : 3; //低3字节
  uint8_t bus: 5;    // which instance of the bus type  //5 字节
  uint8_t address;   // address on the bus (eg. I2C address) //8字节
  uint8_t devtype;   // device class specific device type    //8字节
};
这里bus_type 按以下方式解码：

enum DeviceBusType {
  DeviceBusType_UNKNOWN = 0,
  DeviceBusType_I2C     = 1,
  DeviceBusType_SPI     = 2,
  DeviceBusType_UAVCAN  = 3,
};


devtype 按以下方式解码：

Firmware/src/drivers/drv_sensor.h
#define DRV_MAG_DEVTYPE_HMC5883  0x01
#define DRV_MAG_DEVTYPE_LSM303D  0x02
#define DRV_MAG_DEVTYPE_ACCELSIM 0x03
#define DRV_MAG_DEVTYPE_MPU9250  0x04
#define DRV_ACC_DEVTYPE_LSM303D  0x11
#define DRV_ACC_DEVTYPE_BMA180   0x12
#define DRV_ACC_DEVTYPE_MPU6000  0x13
#define DRV_ACC_DEVTYPE_ACCELSIM 0x14
#define DRV_ACC_DEVTYPE_GYROSIM  0x15
#define DRV_ACC_DEVTYPE_MPU9250  0x16
#define DRV_GYR_DEVTYPE_MPU6000  0x21
#define DRV_GYR_DEVTYPE_L3GD20   0x22
#define DRV_GYR_DEVTYPE_GYROSIM  0x23
#define DRV_GYR_DEVTYPE_MPU9250  0x24
#define DRV_RNG_DEVTYPE_MB12XX   0x31
#define DRV_RNG_DEVTYPE_LL40LS   0x32


设备ID解码示例


 *
 ****************************************************************************/

/**
 * @file drv_sensor.h
 *
 * Common sensor API and ioctl definitions.
 */

#ifndef _DRV_SENSOR_H
#define _DRV_SENSOR_H

#include <px4_defines.h>
#include <stdint.h>
#include <sys/ioctl.h>

#include "drv_device.h"

/**
 * Sensor type definitions.
 *
 * Used to create a unique device id for redundant and combo sensors
 */

#define DRV_MAG_DEVTYPE_HMC5883  0x01
#define DRV_MAG_DEVTYPE_LSM303D  0x02
#define DRV_MAG_DEVTYPE_ACCELSIM 0x03
#define DRV_MAG_DEVTYPE_MPU9250  0x04
#define DRV_MAG_DEVTYPE_LIS3MDL  0x05
#define DRV_MAG_DEVTYPE_IST8310  0x06
#define DRV_ACC_DEVTYPE_LSM303D  0x11
#define DRV_ACC_DEVTYPE_BMA180   0x12
#define DRV_ACC_DEVTYPE_MPU6000  0x13
#define DRV_ACC_DEVTYPE_ACCELSIM 0x14
#define DRV_ACC_DEVTYPE_GYROSIM  0x15
#define DRV_ACC_DEVTYPE_MPU9250  0x16
#define DRV_ACC_DEVTYPE_BMI160   0x17
#define DRV_GYR_DEVTYPE_MPU6000  0x21
#define DRV_GYR_DEVTYPE_L3GD20   0x22
#define DRV_GYR_DEVTYPE_GYROSIM  0x23
#define DRV_GYR_DEVTYPE_MPU9250  0x24
#define DRV_GYR_DEVTYPE_BMI160   0x25
#define DRV_RNG_DEVTYPE_MB12XX   0x31
#define DRV_RNG_DEVTYPE_LL40LS   0x32
#define DRV_ACC_DEVTYPE_MPU6050  0x33
#define DRV_ACC_DEVTYPE_MPU6500  0x34
#define DRV_GYR_DEVTYPE_MPU6050  0x35
#define DRV_GYR_DEVTYPE_MPU6500  0x36
#define DRV_ACC_DEVTYPE_ICM20602	0x37
#define DRV_GYR_DEVTYPE_ICM20602	0x38
#define DRV_ACC_DEVTYPE_ICM20608	0x39
#define DRV_GYR_DEVTYPE_ICM20608	0x3A
#define DRV_ACC_DEVTYPE_ICM20689	0x3B
#define DRV_GYR_DEVTYPE_ICM20689	0x3C
#define DRV_BARO_DEVTYPE_MS5611		0x3D
#define DRV_BARO_DEVTYPE_MS5607		0x3E
#define DRV_BARO_DEVTYPE_BMP280		0x3F
#define DRV_BARO_DEVTYPE_LPS25H		0x40

/*
 * ioctl() definitions
 *
 * Note that a driver may not implement all of these operations, but
 * if the operation is implemented it should conform to this API.
 */

#define _SENSORIOCBASE		(0x2000)
#define _SENSORIOC(_n)		(_PX4_IOC(_SENSORIOCBASE, _n))

/**
 * Set the driver polling rate to (arg) Hz, or one of the SENSOR_POLLRATE
 * constants
 */
#define SENSORIOCSPOLLRATE	_SENSORIOC(0)

/**
 * Return the driver's approximate polling rate in Hz, or one of the
 * SENSOR_POLLRATE values.
 */
#define SENSORIOCGPOLLRATE	_SENSORIOC(1)

#define SENSOR_POLLRATE_MANUAL		1000000	/**< poll when read */
#define SENSOR_POLLRATE_EXTERNAL	1000001	/**< poll when device signals ready */
#define SENSOR_POLLRATE_MAX		1000002	/**< poll at device maximum rate */
#define SENSOR_POLLRATE_DEFAULT		1000003	/**< poll at driver normal rate */

/**
 * Set the internal queue depth to (arg) entries, must be at least 1
 *
 * This sets the upper bound on the number of readings that can be
 * read from the driver.
 */
#define SENSORIOCSQUEUEDEPTH	_SENSORIOC(2)

/** return the internal queue depth */
#define SENSORIOCGQUEUEDEPTH	_SENSORIOC(3)

/**
 * Reset the sensor to its default configuration
 */
#define SENSORIOCRESET		_SENSORIOC(4)

/**
 * Set the sensor orientation
 */
#define SENSORIOCSROTATION	_SENSORIOC(5)

/**
 * Get the sensor orientation
 */
#define SENSORIOCGROTATION	_SENSORIOC(6)

/**
 * Test the sensor calibration
 */
#define SENSORIOCCALTEST	_SENSORIOC(7)

#endif /* _DRV_SENSOR_H */
