; Auto-generated. Do not edit!


(cl:in-package vrep_plugin-srv)


;//! \htmlinclude Turn_score-request.msg.html

(cl:defclass <Turn_score-request> (roslisp-msg-protocol:ros-message)
  ((turn_data
    :reader turn_data
    :initarg :turn_data
    :type vrep_plugin-msg:Turn
    :initform (cl:make-instance 'vrep_plugin-msg:Turn)))
)

(cl:defclass Turn_score-request (<Turn_score-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Turn_score-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Turn_score-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name vrep_plugin-srv:<Turn_score-request> is deprecated: use vrep_plugin-srv:Turn_score-request instead.")))

(cl:ensure-generic-function 'turn_data-val :lambda-list '(m))
(cl:defmethod turn_data-val ((m <Turn_score-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-srv:turn_data-val is deprecated.  Use vrep_plugin-srv:turn_data instead.")
  (turn_data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Turn_score-request>) ostream)
  "Serializes a message object of type '<Turn_score-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'turn_data) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Turn_score-request>) istream)
  "Deserializes a message object of type '<Turn_score-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'turn_data) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Turn_score-request>)))
  "Returns string type for a service object of type '<Turn_score-request>"
  "vrep_plugin/Turn_scoreRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Turn_score-request)))
  "Returns string type for a service object of type 'Turn_score-request"
  "vrep_plugin/Turn_scoreRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Turn_score-request>)))
  "Returns md5sum for a message object of type '<Turn_score-request>"
  "cf114221970573a59c393bab275b9be4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Turn_score-request)))
  "Returns md5sum for a message object of type 'Turn_score-request"
  "cf114221970573a59c393bab275b9be4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Turn_score-request>)))
  "Returns full string definition for message of type '<Turn_score-request>"
  (cl:format cl:nil "vrep_plugin/Turn turn_data~%~%================================================================================~%MSG: vrep_plugin/Turn~%float32 angle~%float32 power~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Turn_score-request)))
  "Returns full string definition for message of type 'Turn_score-request"
  (cl:format cl:nil "vrep_plugin/Turn turn_data~%~%================================================================================~%MSG: vrep_plugin/Turn~%float32 angle~%float32 power~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Turn_score-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'turn_data))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Turn_score-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Turn_score-request
    (cl:cons ':turn_data (turn_data msg))
))
;//! \htmlinclude Turn_score-response.msg.html

(cl:defclass <Turn_score-response> (roslisp-msg-protocol:ros-message)
  ((score
    :reader score
    :initarg :score
    :type cl:integer
    :initform 0))
)

(cl:defclass Turn_score-response (<Turn_score-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Turn_score-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Turn_score-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name vrep_plugin-srv:<Turn_score-response> is deprecated: use vrep_plugin-srv:Turn_score-response instead.")))

(cl:ensure-generic-function 'score-val :lambda-list '(m))
(cl:defmethod score-val ((m <Turn_score-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader vrep_plugin-srv:score-val is deprecated.  Use vrep_plugin-srv:score instead.")
  (score m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Turn_score-response>) ostream)
  "Serializes a message object of type '<Turn_score-response>"
  (cl:let* ((signed (cl:slot-value msg 'score)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Turn_score-response>) istream)
  "Deserializes a message object of type '<Turn_score-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'score) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Turn_score-response>)))
  "Returns string type for a service object of type '<Turn_score-response>"
  "vrep_plugin/Turn_scoreResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Turn_score-response)))
  "Returns string type for a service object of type 'Turn_score-response"
  "vrep_plugin/Turn_scoreResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Turn_score-response>)))
  "Returns md5sum for a message object of type '<Turn_score-response>"
  "cf114221970573a59c393bab275b9be4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Turn_score-response)))
  "Returns md5sum for a message object of type 'Turn_score-response"
  "cf114221970573a59c393bab275b9be4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Turn_score-response>)))
  "Returns full string definition for message of type '<Turn_score-response>"
  (cl:format cl:nil "int32 score~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Turn_score-response)))
  "Returns full string definition for message of type 'Turn_score-response"
  (cl:format cl:nil "int32 score~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Turn_score-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Turn_score-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Turn_score-response
    (cl:cons ':score (score msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Turn_score)))
  'Turn_score-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Turn_score)))
  'Turn_score-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Turn_score)))
  "Returns string type for a service object of type '<Turn_score>"
  "vrep_plugin/Turn_score")