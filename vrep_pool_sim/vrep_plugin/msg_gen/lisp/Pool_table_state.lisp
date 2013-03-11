; Auto-generated. Do not edit!


(cl:in-package vrep_plugin-msg)


;//! \htmlinclude Pool_table_state.msg.html

(cl:defclass <Pool_table_state> (roslisp-msg-protocol:ros-message)
  ((balls
    :reader balls
    :initarg :balls
    :type (cl:vector geometry_msgs-msg:Point32)
   :initform (cl:make-array 15 :element-type 'geometry_msgs-msg:Point32 :initial-element (cl:make-instance 'geometry_msgs-msg:Point32)))
   (white_ball
    :reader white_ball
    :initarg :white_ball
    :type geometry_msgs-msg:Point32
    :initform (cl:make-instance 'geometry_msgs-msg:Point32))
   (cue_position
    :reader cue_position
    :initarg :cue_position
    :type geometry_msgs-msg:Point32
    :initform (cl:make-instance 'geometry_msgs-msg:Point32))
   (cue_orientation
    :reader cue_orientation
    :initarg :cue_orientation
    :type (cl:vector cl:float)
   :initform (cl:make-array 3 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass Pool_table_state (<Pool_table_state>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Pool_table_state>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Pool_table_state)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name vrep_plugin-msg:<Pool_table_state> is deprecated: use vrep_plugin-msg:Pool_table_state instead.")))

(cl:ensure-generic-function 'balls-val :lambda-list '(m))
(cl:defmethod balls-val ((m <Pool_table_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-msg:balls-val is deprecated.  Use vrep_plugin-msg:balls instead.")
  (balls m))

(cl:ensure-generic-function 'white_ball-val :lambda-list '(m))
(cl:defmethod white_ball-val ((m <Pool_table_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-msg:white_ball-val is deprecated.  Use vrep_plugin-msg:white_ball instead.")
  (white_ball m))

(cl:ensure-generic-function 'cue_position-val :lambda-list '(m))
(cl:defmethod cue_position-val ((m <Pool_table_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-msg:cue_position-val is deprecated.  Use vrep_plugin-msg:cue_position instead.")
  (cue_position m))

(cl:ensure-generic-function 'cue_orientation-val :lambda-list '(m))
(cl:defmethod cue_orientation-val ((m <Pool_table_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-msg:cue_orientation-val is deprecated.  Use vrep_plugin-msg:cue_orientation instead.")
  (cue_orientation m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Pool_table_state>) ostream)
  "Serializes a message object of type '<Pool_table_state>"
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'balls))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'white_ball) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'cue_position) ostream)
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'cue_orientation))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Pool_table_state>) istream)
  "Deserializes a message object of type '<Pool_table_state>"
  (cl:setf (cl:slot-value msg 'balls) (cl:make-array 15))
  (cl:let ((vals (cl:slot-value msg 'balls)))
    (cl:dotimes (i 15)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point32))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream)))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'white_ball) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'cue_position) istream)
  (cl:setf (cl:slot-value msg 'cue_orientation) (cl:make-array 3))
  (cl:let ((vals (cl:slot-value msg 'cue_orientation)))
    (cl:dotimes (i 3)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Pool_table_state>)))
  "Returns string type for a message object of type '<Pool_table_state>"
  "vrep_plugin/Pool_table_state")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Pool_table_state)))
  "Returns string type for a message object of type 'Pool_table_state"
  "vrep_plugin/Pool_table_state")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Pool_table_state>)))
  "Returns md5sum for a message object of type '<Pool_table_state>"
  "d77375b50bdafc81f2a2287c1843bbde")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Pool_table_state)))
  "Returns md5sum for a message object of type 'Pool_table_state"
  "d77375b50bdafc81f2a2287c1843bbde")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Pool_table_state>)))
  "Returns full string definition for message of type '<Pool_table_state>"
  (cl:format cl:nil "geometry_msgs/Point32[15] balls~%geometry_msgs/Point32 white_ball~%geometry_msgs/Point32 cue_position~%float32[3] cue_orientation~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Pool_table_state)))
  "Returns full string definition for message of type 'Pool_table_state"
  (cl:format cl:nil "geometry_msgs/Point32[15] balls~%geometry_msgs/Point32 white_ball~%geometry_msgs/Point32 cue_position~%float32[3] cue_orientation~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Pool_table_state>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'balls) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'white_ball))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'cue_position))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'cue_orientation) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Pool_table_state>))
  "Converts a ROS message object to a list"
  (cl:list 'Pool_table_state
    (cl:cons ':balls (balls msg))
    (cl:cons ':white_ball (white_ball msg))
    (cl:cons ':cue_position (cue_position msg))
    (cl:cons ':cue_orientation (cue_orientation msg))
))
