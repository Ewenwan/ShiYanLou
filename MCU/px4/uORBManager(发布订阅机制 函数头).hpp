/****************************************************************************

 uORB(Micro Object Request Broker,微对象请求代理器)  话题消息发布 订阅 机制
  ?uORB(Micro Object Request Broker,微对象请求代理器)是PX4/Pixhawk系统中非常重要且关键的一个模块，
  它肩负了整个系统的数据传输任务，所有的传感器数据、GPS、PPM信号等都要从芯片获取后通过uORB进行传输到
  各个模块进行计算处理。实际上uORB是一套跨「进程」 的IPC通讯模块。在Pixhawk中， 
  所有的功能被独立以进程模块为单位进行实现并工作。而进程间的数据交互就由为重要，
  必须要能够符合实时、有序的特点。 
??Pixhawk使用的是NuttX实时ARM系统，uORB实际上是多个进程打开同一个设备文件，
进程间通过此文件节点进行数据交互和共享。进程通过命名的「总线」交换的消息称之为「主题」(topic)，
在Pixhawk 中，一个主题仅包含一种消息类型，通俗点就是数据类型。每个进程可以「订阅」或者「发布」主题，
可以存在多个发布者，或者一个进程可以订阅多个主题，但是一条总线上始终只有一条消息。

 ****************************************************************************/

#ifndef _uORBManager_hpp_
#define _uORBManager_hpp_

#include "uORBCommon.hpp"
#include "uORBDevices.hpp"
#include <stdint.h>
#ifdef __PX4_NUTTX
#include "ORBSet.hpp"
#else
#include <string>
#include <set>
#define ORBSet std::set<std::string>
#endif

#include "uORBCommunicator.hpp"

namespace uORB
{
class Manager;//命名空间下的对象类 
}

/**
 * This is implemented as a singleton.  This class manages creating the
 * uORB nodes for each uORB topics and also implements the behavor of the
 * uORB Api's.
  创建节点 
 */
class uORB::Manager  : public uORBCommunicator::IChannelRxHandler
{
public:
	// public interfaces for this class.

	/**
	 * Initialize the singleton. Call this before everything else.
	 * @return true on success
	 */
	static bool initialize();

	/**
	 * Method to get the singleton instance for the uORB::Manager.
	 * Make sure initialize() is called first.
	 * @return uORB::Manager*
	 */
	static uORB::Manager *get_instance()
	{
		return _Instance;
	}

	/**
	 * Get the DeviceMaster for a given Flavor. If it does not exist,
	 * it will be created and initialized.
	 * Note: the first call to this is not thread-safe.
	 * @return nullptr if initialization failed (and errno will be set)
	 */
	uORB::DeviceMaster *get_device_master(Flavor flavor);

	// ==== uORB interface methods ====
	/**
	 * Advertise as the publisher of a topic.
	 
	 *
	 * This performs the initial advertisement of a topic; it creates the topic
	 * node in /obj if required and publishes the initial data.
	 *
	 * Any number of advertisers may publish to a topic; publications are atomic
	 * but co-ordination between publishers is not provided by the ORB.
	 *
	 * Internally this will call orb_advertise_multi with an instance of 0 and
	 * default priority.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @param data    A pointer to the initial data to be published.
	 *      For topics updated by interrupt handlers, the advertisement
	 *      must be performed from non-interrupt context.
	 * @param queue_size  Maximum number of buffered elements. If this is 1, no queuing is
	 *      used.
	 * @return    nullptr on error, otherwise returns an object pointer
	 *      that can be used to publish to the topic.
	 *      If the topic in question is not known (due to an
	 *      ORB_DEFINE with no corresponding ORB_DECLARE)
	 *      this function will return nullptr and set errno to ENOENT.
	 */
	/*
功能：公告发布者的主题；
说明：在发布主题之前是必须的；否则订阅者虽然能订阅，但是得不到数据；
参数：
    meta:uORB元对象，可以认为是主题id，一般是通过ORB_ID(主题名)来赋值;
    data:指向一个已被初始化，发布者要发布的数据存储变量的指针；
返回值：错误则返回ERROR;
        成功则返回一个可以发布主题的句柄；  
		         可用  orb_publish(const struct orb_metadata *meta, orb_advert_t handle, const void *data) ; 进行消息发布 
		如果待发布的主题没有定义或声明则会返回-1，
		然后会将errno赋值为ENOENT;
eg:
    struct vehicle_attitude_s att; //定义一个 与话题 相符合的 数据类型变量 
    memset(&att, 0, sizeof(att));  // 初始化为 0  
    int att_pub_fd = orb_advertise(ORB_ID(vehicle_attitude), &att);
	
	*/ 
	 //	 广播发布 话题消息  单个消息  
	orb_advert_t orb_advertise(const struct orb_metadata *meta, const void *data, unsigned int queue_size = 1)
	{
		return orb_advertise_multi(meta, data, nullptr, ORB_PRIO_DEFAULT, queue_size);
	}

	/**
	 * Advertise as the publisher of a topic.
	 *
	 * This performs the initial advertisement of a topic; it creates the topic
	 * node in /obj if required and publishes the initial data.
	 *
	 * Any number of advertisers may publish to a topic; publications are atomic
	 * but co-ordination between publishers is not provided by the ORB.
	 *
	 * The multi can be used to create multiple independent instances of the same topic
	 * (each instance has its own buffer).
	 * This is useful for multiple publishers who publish the same topic. The subscriber
	 * then subscribes to all instances and chooses which source he wants to use.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @param data    A pointer to the initial data to be published.
	 *      For topics updated by interrupt handlers, the advertisement
	 *      must be performed from non-interrupt context.
	 * @param instance  Pointer to an integer which will yield the instance ID (0-based)
	 *      of the publication. This is an output parameter and will be set to the newly
	 *      created instance, ie. 0 for the first advertiser, 1 for the next and so on.
	 * @param priority  The priority of the instance. If a subscriber subscribes multiple
	 *      instances, the priority allows the subscriber to prioritize the best
	 *      data source as long as its available. The subscriber is responsible to check
	 *      and handle different priorities (@see orb_priority()).
	 * @param queue_size  Maximum number of buffered elements. If this is 1, no queuing is
	 *      used.
	 * @return    ERROR on error, otherwise returns a handle
	 *      that can be used to publish to the topic.
	 *      If the topic in question is not known (due to an
	 *      ORB_DEFINE with no corresponding ORB_DECLARE)
	 *      this function will return -1 and set errno to ENOENT.
	 */
	 
	/*
功能：设备/驱动器的多个实例实现公告，利用此函数可以注册多个类似的驱动程序；
说明：例如在飞行器中有多个相同的传感器，那他们的数据类型则类似，不必要注册几个不同的话题；
参数：
    meta:uORB元对象，可以认为是主题id，一般是通过ORB_ID(主题名)来赋值;
    data:指向一个已被初始化，发布者要发布的数据存储变量的指针；
    instance:整型指针，指向实例的ID（从0开始）；
    priority:实例的优先级。如果用户订阅多个实例，优先级的设定可以使用户使用优先级高的最优数据源；
返回值：
    错误则返回ERROR;成功则返回一个可以发布主题的句柄；
	如果待发布的主题没有定义或声明则会返回-1，然后会将errno赋值为ENOENT;
eg:
    struct orb_test t;
    t.val = 0;
    int instance0;
    orb_advert_t pfd0 = orb_advertise_multi(ORB_ID(orb_multitest), &t, &instance0, ORB_PRIO_MAX);
    
	*/ 
    //广播发布 话题消息  多个消息 （对于 不同的发布节点 发布相同话题式  对于不同的 缓冲区，供给订阅者 有选择性的订阅） 
	orb_advert_t orb_advertise_multi(const struct orb_metadata *meta, const void *data, int *instance,
					 int priority, unsigned int queue_size = 1) ;


	/**
	 * Unadvertise a topic.
	 
	 * 
	 
	 * @param handle  handle returned by orb_advertise or orb_advertise_multi.
	 * @return 0 on success
	 */

    
	 // 取消发布话题  
	int orb_unadvertise(orb_advert_t handle);

	/**
	 * Publish new data to a topic.
	 
 
	 
	 *
	 * The data is atomically published to the topic and any waiting subscribers
	 * will be notified.  Subscribers that are not waiting can check the topic
	 * for updates using orb_check and/or orb_stat.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @handle    The handle returned from orb_advertise.
	 * @param data    A pointer to the data to be published.
	 * @return    OK on success, ERROR otherwise with errno set accordingly.
	 */
	 
	 
	/*
功能：发布新数据到主题；
参数：
    meta:uORB元对象，可以认为是主题id，一般是通过ORB_ID(主题名)来赋值;
    handle:orb_advertise函数返回的句柄；如   int att_pub_fd = orb_advertise(ORB_ID(vehicle_attitude), &att);
    data:指向待发布数据的指针；
返回值：OK表示成功；错误返回ERROR；否则则有根据的去设置errno;
eg: 
              发布前需要 传入消息内容 
				att.q[0] = raw.accelerometer_m_s2[0];
				att.q[1] = raw.accelerometer_m_s2[1];
				att.q[2] = raw.accelerometer_m_s2[2];
    orb_publish(ORB_ID(vehicle_attitude), att_pub_fd, &att);
	*/ 
   // 向话题上发布新的消息 
	int  orb_publish(const struct orb_metadata *meta, orb_advert_t handle, const void *data) ;

	/**
	 * Subscribe to a topic.
	 *
	 * The returned value is a file descriptor that can be passed to poll()
	 * in order to wait for updates to a topic, as well as topic_read,
	 * orb_check and orb_stat.
	 *
	 * Subscription will succeed even if the topic has not been advertised;
	 * in this case the topic will have a timestamp of zero, it will never
	 * signal a poll() event, checking will always return false and it cannot
	 * be copied. When the topic is subsequently advertised, poll, check,
	 * stat and copy calls will react to the initial publication that is
	 * performed as part of the advertisement.
	 *
	 * Subscription will fail if the topic is not known to the system, i.e.
	 * there is nothing in the system that has declared the topic and thus it
	 * can never be published.
	 *
	 * Internally this will call orb_subscribe_multi with instance 0.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @return    ERROR on error, otherwise returns a handle
	 *      that can be used to read and update the topic.
	 */
	 /*
功能：订阅主题（topic）;
说明：即使订阅的主题没有被公告，但是也能订阅成功；但是在这种情况下，却得不到数据，直到主题被公告；
参数：
        meta:uORB元对象，可以认为是主题id，一般是通过ORB_ID(主题名)来赋值；
返回值：
      错误则返回ERROR;成功则返回一个可以读取数据、更新话题的句柄；
	  如果待订阅的主题没有定义或声明则会返回-1，然后会将errno赋值为ENOENT;
eg:
    int fd = orb_subscribe(ORB_ID(topicName));
	 */
	 // 订阅话题  上的单个消息类型 
	int  orb_subscribe(const struct orb_metadata *meta) ;
     

	/**
	 * Subscribe to a multi-instance of a topic.
	 *
	 * The returned value is a file descriptor that can be passed to poll()
	 * in order to wait for updates to a topic, as well as topic_read,
	 * orb_check and orb_stat.
	 *
	 * Subscription will succeed even if the topic has not been advertised;
	 * in this case the topic will have a timestamp of zero, it will never
	 * signal a poll() event, checking will always return false and it cannot
	 * be copied. When the topic is subsequently advertised, poll, check,
	 * stat and copy calls will react to the initial publication that is
	 * performed as part of the advertisement.
	 *
	 * Subscription will fail if the topic is not known to the system, i.e.
	 * there is nothing in the system that has declared the topic and thus it
	 * can never be published.
	 *
	 * If a publisher publishes multiple instances the subscriber should
	 * subscribe to each instance with orb_subscribe_multi
	 * (@see orb_advertise_multi()).
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @param instance  The instance of the topic. Instance 0 matches the
	 *      topic of the orb_subscribe() call, higher indices
	 *      are for topics created with orb_advertise_multi().
	 * @return    ERROR on error, otherwise returns a handle
	 *      that can be used to read and update the topic.
	 *      If the topic in question is not known (due to an
	 *      ORB_DEFINE_OPTIONAL with no corresponding ORB_DECLARE)
	 *      this function will return -1 and set errno to ENOENT.
	 */
	
 /*
 功能：订阅主题（topic）;
说明：通过实例的ID索引来确定是主题的哪个实例；
参数：
    meta:uORB元对象，可以认为是主题id，一般是通过ORB_ID(主题名)来赋值;
    instance:主题实例ID;实例ID=0与orb_subscribe()实现相同；
返回值：
    错误则返回ERROR;成功则返回一个可以读取数据、更新话题的句柄；如果待订阅的主题没有定义或声明则会返回-1，然后会将errno赋值为ENOENT;
eg:
    int sfd1 = orb_subscribe_multi(ORB_ID(orb_multitest), 1);
 */ 
 // 订阅话题  上的多个消息类型 
	int  orb_subscribe_multi(const struct orb_metadata *meta, unsigned instance) ;

	/**
	 * Unsubscribe from a topic.
	 *
	 * @param handle  A handle returned from orb_subscribe.
	 * @return    OK on success, ERROR otherwise with errno set accordingly.
	 */
	 	 /*
功能：取消订阅主题；
参数：
    handle:主题句柄；
返回值：
    OK表示成功；错误返回ERROR;否则则有根据的去设置errno;
eg:
	 int fd = orb_subscribe(ORB_ID(topicName));
	 
    ret = orb_unsubscribe(fd);
	*/
 // 取消订阅话题 
	int  orb_unsubscribe(int handle) ;

	/**
	 * Fetch data from a topic.
	 *
	 * This is the only operation that will reset the internal marker that
	 * indicates that a topic has been updated for a subscriber. Once poll
	 * or check return indicating that an updaet is available, this call
	 * must be used to update the subscription.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @param handle  A handle returned from orb_subscribe.
	 * @param buffer  Pointer to the buffer receiving the data, or NULL
	 *      if the caller wants to clear the updated flag without
	 *      using the data.
	 * @return    OK on success, ERROR otherwise with errno set accordingly.
	 */
	 /*
	 功能：从订阅的主题中获取数据并将数据保存到buffer中；
参数：
    meta:uORB元对象，可以认为是主题id，一般是通过ORB_ID(主题名)来赋值;
    handle:订阅主题返回的句柄；  如   int fd = orb_subscribe(ORB_ID(topicName));  topicName可以为 sensor_combined
    buffer:从主题中获取的数据；
返回值：
    返回OK表示获取数据成功，错误返回ERROR;否则则有根据的去设置errno;
eg:
    struct sensor_combined_s raw;
    orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &raw);
	 */
// 从话题上得到消息  复制到本地  的 内存变量里 
	int  orb_copy(const struct orb_metadata *meta, int handle, void *buffer) ;

	/**
	 * Check whether a topic has been published to since the last orb_copy.
	 *
	 * This check can be used to determine whether to copy the topic when
	 * not using poll(), or to avoid the overhead of calling poll() when the
	 * topic is likely to have updated.
	 *
	 * Updates are tracked on a per-handle basis; this call will continue to
	 * return true until orb_copy is called using the same handle. This interface
	 * should be preferred over calling orb_stat due to the race window between
	 * stat and copy that can lead to missed updates.
	 *
	 * @param handle  A handle returned from orb_subscribe.
	 * @param updated Set to true if the topic has been updated since the
	 *      last time it was copied using this handle.
	 * @return    OK if the check was successful, ERROR otherwise with
	 *      errno set accordingly.
	 */
/*
	功能：订阅者可以用来检查一个主题在发布者上一次更新数据后，有没有订阅者调用过ob_copy来接收、处理过；
说明：如果主题在在被公告前就有人订阅，那么这个API将返回“not-updated”直到主题被公告。可以不用poll，只用这个函数实现数据的获取。
参数：
    handle:主题句柄；
    updated:如果当最后一次更新的数据被获取了，检测到并设置updated为ture;
返回值：
    OK表示检测成功；错误返回ERROR;否则则有根据的去设置errno;
eg:
    if (PX4_OK != orb_check(sfd, &updated)) {
        return printf("check(1) failed");
    }
    if (updated) {
        return printf("spurious updated flag");
    }
 //or

    bool updated;
    struct random_integer_data rd;

    //check to see whether the topic has updated since the last time we read it /
    orb_check(topic_handle, &updated);

    if (updated) {
       //make a local copy of the updated data structure/
        orb_copy(ORB_ID(random_integer), topic_handle, &rd);
        printf("Random integer is now %d\n", rd.r);
    } 
*/

// 检查从上次取消息后  
	int  orb_check(int handle, bool *updated) ;

	/**
	 * Return the last time that the topic was updated. If a queue is used, it returns
	 * the timestamp of the latest element in the queue.
	 *
	 * @param handle  A handle returned from orb_subscribe.
	 * @param time    Returns the absolute time that the topic was updated, or zero if it has
	 *      never been updated. Time is measured in microseconds.
	 * @return    OK on success, ERROR otherwise with errno set accordingly.
	 */

/*
功能：订阅者可以用来检查一个主题最后的发布时间；
参数：
    handle:主题句柄；
    time:存放主题最后发布的时间；0表示该主题没有发布或公告；
返回值：
    OK表示检测成功；错误返回ERROR;否则则有根据的去设置errno;
eg:
    ret = orb_stat(handle,time);
*/
 // 返回 订阅话题 上次更新发布消息 的时间 
	int  orb_stat(int handle, uint64_t *time) ;

	/**
	 * Check if a topic has already been created (a publisher or a subscriber exists with
	 * the given instance).
	 *
	 * @param meta    ORB topic metadata.
	 * @param instance  ORB instance
	 * @return    OK if the topic exists, ERROR otherwise with errno set accordingly.
	 */
	 
	 
	 /*
	功能：检测一个主题是否存在；
参数：
    meta:uORB元对象，可以认为是主题id，一般是通过ORB_ID(主题名)来赋值;
    instance:ORB 实例ID;
返回值：
    OK表示检测成功；错误返回ERROR;否则则有根据的去设置errno;
eg:
    ret = orb_exists(ORB_ID(vehicle_attitude),0); 
	 */
// 检查 话题是否存在 是否已经被发布 
	int  orb_exists(const struct orb_metadata *meta, int instance) ;

	/**
	 * Return the priority of the topic
	 *
	 * @param handle  A handle returned from orb_subscribe.
	 * @param priority  Returns the priority of this topic. This is only relevant for
	 *      topics which are published by multiple publishers (e.g. mag0, mag1, etc.)
	 *      and allows a subscriber to pick the topic with the highest priority,
	 *      independent of the startup order of the associated publishers.
	 * @return    OK on success, ERROR otherwise with errno set accordingly.
	 */
	
	/*
	功能：获取主题优先级别；
参数：
    handle:主题句柄；
    priority:存放获取的优先级别；
返回值：
    OK表示检测成功；错误返回ERROR;否则则有根据的去设置errno;
eg:
    ret = orb_priority(handle,&priority);
    
	*/ 
//  一个话题  所有发布节点（者） 的优先次序（对于话题订阅者） 
	int  orb_priority(int handle, int32_t *priority) ;

	/**
	 * Set the minimum interval between which updates are seen for a subscription.
	 *
	 * If this interval is set, the subscriber will not see more than one update
	 * within the period.
	 *
	 * Specifically, the first time an update is reported to the subscriber a timer
	 * is started. The update will continue to be reported via poll and orb_check, but
	 * once fetched via orb_copy another update will not be reported until the timer
	 * expires.
	 *
	 * This feature can be used to pace a subscriber that is watching a topic that
	 * would otherwise update too quickly.
	 *
	 * @param handle  A handle returned from orb_subscribe.
	 * @param interval  An interval period in milliseconds.
	 * @return    OK on success, ERROR otherwise with ERRNO set accordingly.
	 */
	/*
	
功能：设置订阅的最小时间间隔；
说明：如果设置了，则在这间隔内发布的数据将订阅不到；需要注意的是，设置后，第一次的数据订阅还是由起初设置的频率来获取，
参数：
    handle:orb_subscribe函数返回的句柄；
    interval:间隔时间，单位ms;
返回值：OK表示成功；错误返回ERROR；否则则有根据的去设置errno;
eg:
	int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
    orb_set_interval(sensor_sub_fd, 1000);
    
	*/ 
// 设置 订阅者   orb_copy 取话题上消息的  最小时间间隔  
	int  orb_set_interval(int handle, unsigned interval) ;


	/**
	 * Get the minimum interval between which updates are seen for a subscription.
	 *
	 * @see orb_set_interval()
	 *
	 * @param handle  A handle returned from orb_subscribe.
	 * @param interval  The returned interval period in milliseconds.
	 * @return    OK on success, ERROR otherwise with ERRNO set accordingly.
	 */
// 得到 订阅者   orb_copy 取话题上消息的  时间间隔  
	int	orb_get_interval(int handle, unsigned *interval);

	/**
	 * Method to set the uORBCommunicator::IChannel instance.
	 * @param comm_channel
	 *  The IChannel instance to talk to remote proxies.
	 * @note:
	 *  Currently this call only supports the use of one IChannel
	 *  Future extensions may include more than one IChannel's.
	 */
	void set_uorb_communicator(uORBCommunicator::IChannel *comm_channel);

	/**
	 * Gets the uORB Communicator instance.
	 */
	uORBCommunicator::IChannel *get_uorb_communicator(void);

	/**
	 * Utility method to check if there is a remote subscriber present
	 * for a given topic
	 */
	bool is_remote_subscriber_present(const char *messageName);

private: // class methods
	/**
	 * Advertise a node; don't consider it an error if the node has
	 * already been advertised.
	 *
	 * @todo verify that the existing node is the same as the one
	 *       we tried to advertise.
	 */
	int
	node_advertise
	(
		const struct orb_metadata *meta,
		int *instance = nullptr,
		int priority = ORB_PRIO_DEFAULT
	);

	/**
	 * Common implementation for orb_advertise and orb_subscribe.
	 *
	 * Handles creation of the object and the initial publication for
	 * advertisers.
	 */
	int
	node_open
	(
		Flavor f,
		const struct orb_metadata *meta,
		const void *data,
		bool advertiser,
		int *instance = nullptr,
		int priority = ORB_PRIO_DEFAULT
	);

private: // data members
	static Manager *_Instance;
	// the communicator channel instance.
	uORBCommunicator::IChannel *_comm_channel;
	ORBSet _remote_subscriber_topics;
	ORBSet _remote_topics;

	DeviceMaster *_device_masters[Flavor_count]; ///< Allow at most one DeviceMaster per Flavor

private: //class methods
	Manager();
	~Manager();

	/**
	 * Interface to process a received topic from remote.
	 * @param topic_name
	 * 	This represents the uORB message Name (topic); This message Name should be
	 * 	globally unique.
	 * @param isAdvertisement
	 * 	Represents if the topic has been advertised or is no longer avialable.
	 * @return
	 *  0 = success; This means the messages is successfully handled in the
	 *  	handler.
	 *  otherwise = failure.
	 */
	virtual int16_t process_remote_topic(const char *topic_name, bool isAdvertisement);

	/**
	   * Interface to process a received AddSubscription from remote.
	   * @param messageName
	   *  This represents the uORB message Name; This message Name should be
	   *  globally unique.
	   * @param msgRate
	   *  The max rate at which the subscriber can accept the messages.
	   * @return
	   *  0 = success; This means the messages is successfully handled in the
	   *    handler.
	   *  otherwise = failure.
	   */
	virtual int16_t process_add_subscription(const char *messageName,
			int32_t msgRateInHz);

	/**
	 * Interface to process a received control msg to remove subscription
	 * @param messageName
	 *  This represents the uORB message Name; This message Name should be
	 *  globally unique.
	 * @return
	 *  0 = success; This means the messages is successfully handled in the
	 *    handler.
	 *  otherwise = failure.
	 */
	virtual int16_t process_remove_subscription(const char *messageName);

	/**
	 * Interface to process the received data message.
	 * @param messageName
	 *  This represents the uORB message Name; This message Name should be
	 *  globally unique.
	 * @param length
	 *  The length of the data buffer to be sent.
	 * @param data
	 *  The actual data to be sent.
	 * @return
	 *  0 = success; This means the messages is successfully handled in the
	 *    handler.
	 *  otherwise = failure.
	 */
	virtual int16_t process_received_message(const char *messageName,
			int32_t length, uint8_t *data);


#ifdef ORB_USE_PUBLISHER_RULES

	struct PublisherRule {
		const char **topics; //null-terminated list of topic names
		const char *module_name; //only this module is allowed to publish one of the topics
		bool ignore_other_topics;
	};

	/**
	 * test if str starts with pre
	 */
	bool startsWith(const char *pre, const char *str);

	/**
	 * find a topic in a rule
	 */
	bool findTopic(const PublisherRule &rule, const char *topic_name);

	/**
	 * trim whitespace from the beginning of a string
	 */
	void strTrim(const char **str);

	/**
	 * Read publisher rules from a file. It has the format:
	 *
	 * restrict_topics: <topic1>, <topic2>, <topic3>
	 * module: <module_name>
	 * [ignore_others:true]
	 *
	 * @return 0 on success, <0 otherwise
	 */
	int readPublisherRulesFromFile(const char *file_name, PublisherRule &rule);

	PublisherRule _publisher_rule;
	bool _has_publisher_rules = false;

#endif /* ORB_USE_PUBLISHER_RULES */

};

#endif /* _uORBManager_hpp_ */
