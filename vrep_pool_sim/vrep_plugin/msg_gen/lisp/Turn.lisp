; Auto-generated. Do not edit!


(cl:in-package vrep_plugin-msg)


;//! \htmlinclude Turn.msg.html

(cl:defclass <Turn> (roslisp-msg-protocol:ros-message)
  ((angle
    :reader angle
    :initarg :angle
    :type cl:float
    :initform 0.0)
   (power
    :reader power
    :initarg :power
    :type cl:float
    :initform 0.0))
)

(cl:defclass Turn (<Turn>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Turn>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Turn)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name vrep_plugin-msg:<Turn> is deprecated: use vrep_plugin-msg:Turn instead.")))

(cl:ensure-generic-function 'angle-val :lambda-list '(m))
(cl:defmethod angle-val ((m <Turn>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-msg:angle-val is deprecated.  Use vrep_plugin-msg:angle instead.")
  (angle m))

(cl:ensure-generic-function 'power-val :lambda-list '(m))
(cl:defmethod power-val ((m <Turn>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-msg:power-val is deprecated.  Use vrep_plugin-msg:power instead.")
  (power m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Turn>) ostream)
  "Serializes a message object of type '<Turn>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'power))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Turn>) istream)
  "Deserializes a message object of type '<Turn>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'power) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Turn>)))
  "Returns string type for a message object of type '<Turn>"
  "vrep_plugin/Turn")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Turn)))
  "Returns string type for a message object of type 'Turn"
  "vrep_plugin/Turn")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Turn>)))
  "Returns md5sum for a message object of type '<Turn>"
  "ae900cd85006e289b45ba5c8c98cce4f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Turn)))
  "Returns md5sum for a message object of type 'Turn"
  "ae900cd85006e289b45ba5c8c98cce4f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Turn>)))
  "Returns full string definition for message of type '<Turn>"
  (cl:format cl:nil "float32 angle~%float32 power~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Turn)))
  "Returns full string definition for message of type 'Turn"
  (cl:format cl:nil "float32 angle~%float32 power~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Turn>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Turn>))
  "Converts a ROS message object to a list"
  (cl:list 'Turn
    (cl:cons ':angle (angle msg))
    (cl:cons ':power (power msg))
))
