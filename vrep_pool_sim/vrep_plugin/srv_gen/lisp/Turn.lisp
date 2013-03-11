; Auto-generated. Do not edit!


(cl:in-package vrep_plugin-srv)


;//! \htmlinclude Turn-request.msg.html

(cl:defclass <Turn-request> (roslisp-msg-protocol:ros-message)
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

(cl:defclass Turn-request (<Turn-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Turn-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Turn-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name vrep_plugin-srv:<Turn-request> is deprecated: use vrep_plugin-srv:Turn-request instead.")))

(cl:ensure-generic-function 'angle-val :lambda-list '(m))
(cl:defmethod angle-val ((m <Turn-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-srv:angle-val is deprecated.  Use vrep_plugin-srv:angle instead.")
  (angle m))

(cl:ensure-generic-function 'power-val :lambda-list '(m))
(cl:defmethod power-val ((m <Turn-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-srv:power-val is deprecated.  Use vrep_plugin-srv:power instead.")
  (power m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Turn-request>) ostream)
  "Serializes a message object of type '<Turn-request>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Turn-request>) istream)
  "Deserializes a message object of type '<Turn-request>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Turn-request>)))
  "Returns string type for a service object of type '<Turn-request>"
  "vrep_plugin/TurnRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Turn-request)))
  "Returns string type for a service object of type 'Turn-request"
  "vrep_plugin/TurnRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Turn-request>)))
  "Returns md5sum for a message object of type '<Turn-request>"
  "77743d160fcb6c8057dc6d1329fd8fdb")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Turn-request)))
  "Returns md5sum for a message object of type 'Turn-request"
  "77743d160fcb6c8057dc6d1329fd8fdb")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Turn-request>)))
  "Returns full string definition for message of type '<Turn-request>"
  (cl:format cl:nil "float32 angle~%float32 power~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Turn-request)))
  "Returns full string definition for message of type 'Turn-request"
  (cl:format cl:nil "float32 angle~%float32 power~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Turn-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Turn-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Turn-request
    (cl:cons ':angle (angle msg))
    (cl:cons ':power (power msg))
))
;//! \htmlinclude Turn-response.msg.html

(cl:defclass <Turn-response> (roslisp-msg-protocol:ros-message)
  ((score
    :reader score
    :initarg :score
    :type cl:integer
    :initform 0))
)

(cl:defclass Turn-response (<Turn-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Turn-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Turn-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name vrep_plugin-srv:<Turn-response> is deprecated: use vrep_plugin-srv:Turn-response instead.")))

(cl:ensure-generic-function 'score-val :lambda-list '(m))
(cl:defmethod score-val ((m <Turn-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-srv:score-val is deprecated.  Use vrep_plugin-srv:score instead.")
  (score m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Turn-response>) ostream)
  "Serializes a message object of type '<Turn-response>"
  (cl:let* ((signed (cl:slot-value msg 'score)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Turn-response>) istream)
  "Deserializes a message object of type '<Turn-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'score) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Turn-response>)))
  "Returns string type for a service object of type '<Turn-response>"
  "vrep_plugin/TurnResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Turn-response)))
  "Returns string type for a service object of type 'Turn-response"
  "vrep_plugin/TurnResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Turn-response>)))
  "Returns md5sum for a message object of type '<Turn-response>"
  "77743d160fcb6c8057dc6d1329fd8fdb")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Turn-response)))
  "Returns md5sum for a message object of type 'Turn-response"
  "77743d160fcb6c8057dc6d1329fd8fdb")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Turn-response>)))
  "Returns full string definition for message of type '<Turn-response>"
  (cl:format cl:nil "int32 score~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Turn-response)))
  "Returns full string definition for message of type 'Turn-response"
  (cl:format cl:nil "int32 score~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Turn-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Turn-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Turn-response
    (cl:cons ':score (score msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Turn)))
  'Turn-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Turn)))
  'Turn-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Turn)))
  "Returns string type for a service object of type '<Turn>"
  "vrep_plugin/Turn")