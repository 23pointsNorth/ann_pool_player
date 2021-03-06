/* Auto-generated by genmsg_cpp for file /home/daniel/fuerte_workspace/sandbox/vrep_pool_sim/vrep_plugin/msg/Pool_table_state.msg */
#ifndef VREP_PLUGIN_MESSAGE_POOL_TABLE_STATE_H
#define VREP_PLUGIN_MESSAGE_POOL_TABLE_STATE_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "geometry_msgs/Point32.h"
#include "geometry_msgs/Point32.h"
#include "geometry_msgs/Point32.h"

namespace vrep_plugin
{
template <class ContainerAllocator>
struct Pool_table_state_ {
  typedef Pool_table_state_<ContainerAllocator> Type;

  Pool_table_state_()
  : balls()
  , white_ball()
  , cue_position()
  , cue_orientation()
  {
    cue_orientation.assign(0.0);
  }

  Pool_table_state_(const ContainerAllocator& _alloc)
  : balls()
  , white_ball(_alloc)
  , cue_position(_alloc)
  , cue_orientation()
  {
    balls.assign( ::geometry_msgs::Point32_<ContainerAllocator> (_alloc));
    cue_orientation.assign(0.0);
  }

  typedef boost::array< ::geometry_msgs::Point32_<ContainerAllocator> , 15>  _balls_type;
  boost::array< ::geometry_msgs::Point32_<ContainerAllocator> , 15>  balls;

  typedef  ::geometry_msgs::Point32_<ContainerAllocator>  _white_ball_type;
   ::geometry_msgs::Point32_<ContainerAllocator>  white_ball;

  typedef  ::geometry_msgs::Point32_<ContainerAllocator>  _cue_position_type;
   ::geometry_msgs::Point32_<ContainerAllocator>  cue_position;

  typedef boost::array<float, 3>  _cue_orientation_type;
  boost::array<float, 3>  cue_orientation;


  typedef boost::shared_ptr< ::vrep_plugin::Pool_table_state_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::vrep_plugin::Pool_table_state_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct Pool_table_state
typedef  ::vrep_plugin::Pool_table_state_<std::allocator<void> > Pool_table_state;

typedef boost::shared_ptr< ::vrep_plugin::Pool_table_state> Pool_table_statePtr;
typedef boost::shared_ptr< ::vrep_plugin::Pool_table_state const> Pool_table_stateConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::vrep_plugin::Pool_table_state_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::vrep_plugin::Pool_table_state_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace vrep_plugin

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::vrep_plugin::Pool_table_state_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::vrep_plugin::Pool_table_state_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::vrep_plugin::Pool_table_state_<ContainerAllocator> > {
  static const char* value() 
  {
    return "d77375b50bdafc81f2a2287c1843bbde";
  }

  static const char* value(const  ::vrep_plugin::Pool_table_state_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xd77375b50bdafc81ULL;
  static const uint64_t static_value2 = 0xf2a2287c1843bbdeULL;
};

template<class ContainerAllocator>
struct DataType< ::vrep_plugin::Pool_table_state_<ContainerAllocator> > {
  static const char* value() 
  {
    return "vrep_plugin/Pool_table_state";
  }

  static const char* value(const  ::vrep_plugin::Pool_table_state_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::vrep_plugin::Pool_table_state_<ContainerAllocator> > {
  static const char* value() 
  {
    return "geometry_msgs/Point32[15] balls\n\
geometry_msgs/Point32 white_ball\n\
geometry_msgs/Point32 cue_position\n\
float32[3] cue_orientation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point32\n\
# This contains the position of a point in free space(with 32 bits of precision).\n\
# It is recommeded to use Point wherever possible instead of Point32.  \n\
# \n\
# This recommendation is to promote interoperability.  \n\
#\n\
# This message is designed to take up less space when sending\n\
# lots of points at once, as in the case of a PointCloud.  \n\
\n\
float32 x\n\
float32 y\n\
float32 z\n\
";
  }

  static const char* value(const  ::vrep_plugin::Pool_table_state_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::vrep_plugin::Pool_table_state_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::vrep_plugin::Pool_table_state_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.balls);
    stream.next(m.white_ball);
    stream.next(m.cue_position);
    stream.next(m.cue_orientation);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct Pool_table_state_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::vrep_plugin::Pool_table_state_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::vrep_plugin::Pool_table_state_<ContainerAllocator> & v) 
  {
    s << indent << "balls[]" << std::endl;
    for (size_t i = 0; i < v.balls.size(); ++i)
    {
      s << indent << "  balls[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Point32_<ContainerAllocator> >::stream(s, indent + "    ", v.balls[i]);
    }
    s << indent << "white_ball: ";
s << std::endl;
    Printer< ::geometry_msgs::Point32_<ContainerAllocator> >::stream(s, indent + "  ", v.white_ball);
    s << indent << "cue_position: ";
s << std::endl;
    Printer< ::geometry_msgs::Point32_<ContainerAllocator> >::stream(s, indent + "  ", v.cue_position);
    s << indent << "cue_orientation[]" << std::endl;
    for (size_t i = 0; i < v.cue_orientation.size(); ++i)
    {
      s << indent << "  cue_orientation[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.cue_orientation[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // VREP_PLUGIN_MESSAGE_POOL_TABLE_STATE_H

