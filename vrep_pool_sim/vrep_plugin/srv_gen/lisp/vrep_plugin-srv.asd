
(cl:in-package :asdf)

(defsystem "vrep_plugin-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :vrep_plugin-msg
)
  :components ((:file "_package")
    (:file "Turn_score" :depends-on ("_package_Turn_score"))
    (:file "_package_Turn_score" :depends-on ("_package"))
  ))