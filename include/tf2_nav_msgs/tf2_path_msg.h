// This file could replace the one in pips_trajectory_testing, but I don't think that the tf dependencies should be required just to use the messages

#ifndef TF2_NAV_MSGS_TF2_PATH_MSG_H
#define TF2_NAV_MSGS_TF2_PATH_MSG_H

#include <nav_msgs/Path.h>
#include <geometry_msgs/TransformStamped.h>
//#include <Eigen/Eigen>
#include <tf2/convert.h>  //needed to give access to templates
//#include <tf2_eigen/tf2_eigen.h>  //needed to transform to eigen
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

namespace tf2
{


inline bool isEmpty(const std_msgs::Header& h)
{
  return (h.frame_id == "" || h.stamp == ros::Time ( 0 ) );
}

inline const std_msgs::Header& getHeader(const nav_msgs::Path& t)
{
  if(!isEmpty(t.header))
  {
    return t.header;
  }
  else
  {
    //ROS_ASSERT_MSG(t.poses.size()>0, "Invalid header for path, and no poses in list!");
    return t.poses[0].header;
  }
}
  
// method to extract timestamp from object
template <>
inline
  const ros::Time& getTimestamp(const nav_msgs::Path& t) {return getHeader(t).stamp;}

// method to extract frame id from object
template <>
inline
  const std::string& getFrameId(const nav_msgs::Path& t) {return getHeader(t).frame_id;}

// this method needs to be implemented by client library developers
template <>
inline
  void doTransform(const nav_msgs::Path& t_in, nav_msgs::Path& t_out, const geometry_msgs::TransformStamped& transform_stamped)
  {
    //NOTE: It might be more efficient to perform the conversion only once, but not going to worry about it at this point
    //tf2::Transform t;
    //fromMsg(transform_stamped.transform, t);
    
    for(size_t i = 0; i < t_in.poses.size(); i++)
    {
      geometry_msgs::PoseStamped pose;
      doTransform((geometry_msgs::PoseStamped)t_in.poses[i], pose, transform_stamped);
      t_out.poses.push_back(pose);
    }
    
    
    t_out.header.stamp = transform_stamped.header.stamp; 
    t_out.header.frame_id = transform_stamped.header.frame_id;
  }


} //end namespace tf2

#endif /* TF2_NAV_MSGS_TF2_PATH_MSG_H */
