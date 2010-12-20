(defpackage :sbcl-socket-test
  (:use :cl :sb-bsd-sockets))

(in-package :sbcl-socket-test)

;(defparameter *server-socket* (make-instance 'inet-socket
;					     :type :stream
;					     :protocol :tcp))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
(defun resolve-hostname (name)
  (cond
    ((eql name :any)  #(0 0 0 0))
    ((typep name '(vector * 4)) name)
    (t (car (sb-bsd-sockets:host-ent-addresses
         (sb-bsd-sockets:get-host-by-name name))))))
 
(defun open-server (&key (host :any) (port 0)
		    (reuse-address t)
		    (backlog 1)
		    (protocol :tcp))
  "Returns a server socket"
  (let ((sock (make-instance 'sb-bsd-sockets:inet-socket
			     :type :stream
			     :protocol protocol)))
    (when reuse-address
      (setf (sb-bsd-sockets:sockopt-reuse-address sock) t))
    (sb-bsd-sockets:socket-bind  sock #(127 0 0 1) port)
;    (sb-bsd-sockets:socket-bind  sock (resolve-hostname host) port)
    (sb-bsd-sockets:socket-listen sock backlog)
    sock))

;(open-server :port 8001 :reuse-address t :backlog 1 :protocol :tcp)

;; OK!
(defun run-server (port)
  (let* ((sock (open-server :port port :reuse-address t :backlog 1 :protocol :tcp))
	 (con (sb-bsd-sockets:socket-accept sock))
	 (fd (sb-bsd-sockets:socket-file-descriptor con))
	 (s (sb-bsd-sockets:socket-make-stream con 
					       :input t :output t
					       :element-type 'character
					       :buffering :none)))
    (let ((line (read-line s)))
      (format t "Line from client: ~A~%" line)
      (format s "response from server~%"))
    (close s)
    (sb-bsd-sockets:socket-close con)
    (sb-bsd-sockets:socket-close sock)))

;(run-server 8002)
