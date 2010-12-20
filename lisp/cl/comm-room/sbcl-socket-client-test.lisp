(defpackage :sbcl-socket-test
  (:use :cl :sb-bsd-sockets))

(in-package :sbcl-socket-test)

;(defparameter *client-socket* (make-instance 'sb-bsd-sockets:inet-socket
;					     :type :stream
;					     :protocol :tcp))

;; OK!
(defun run-client (port)
  (let* ((sock (make-instance 'sb-bsd-sockets:inet-socket :type :stream :protocol :tcp))
	 (con (sb-bsd-sockets:socket-connect sock #(127 0 0 1) port))
;	 (fd (sb-bsd-sockets:socket-file-descriptor con))
	 (s (sb-bsd-sockets:socket-make-stream sock 
					       :input t :output t
					       :element-type 'character
					       :buffering :none)))
    (format t "~A~%~A~%" sock con)
    (format s "a message from a client~%")
    (force-output s)
    (sleep 1)
    (let ((line (read-line s)))
      (format t "Line from server: ~A~%" line)
;      (format s "response from server~%")
      )
;    (sb-bsd-sockets:socket-close con)
    (close s)
    (sb-bsd-sockets:socket-close sock)))
  
;(run-client 8002)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
