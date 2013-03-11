
(cl:in-package :asdf)

(defsystem "vrep_plugin-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "Turn" :depends-on ("_package_Turn"))
    (:file "_package_Turn" :depends-on ("_package"))
    (:file "Pool_table_state" :depends-on ("_package_Pool_table_state"))
    (:file "_package_Pool_table_state" :depends-on ("_package"))
  ))