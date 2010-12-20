(require 'usocket)
(require 'bordeaux-threads)

(defpackage :comm-server
  (:use :cl :usocket :bordeaux-threads))

(in-package :comm-server)

(defparameter *server-ip* '#(127 0 0 1))
;;(defparameter *server-ip* "127 0 0 1")    ;; X
(defparameter *server-port* 8006)

(defparameter *count* 0)
(defparameter *connections* nil)

(defun run-server (ip port)
  (let* ((sock (usocket:socket-listen ip port 
			     :reuse-address t 
			     :backlog 1
			     :element-type 'character))
	 (th-accept (bordeaux-threads:make-thread 
		     #'(lambda ()
			 (thread-accept sock)) :name "accept")))
    (unwind-protect
	 (progn
	   (format t "Thread Started~%")
	   (bordeaux-threads:join-thread th-accept))
      (progn
	(usocket:socket-close sock)))
    ))

(defun thread-accept (sock)
    (loop
       (let ((con (usocket:socket-accept sock)))
	 (format t "Accept : ~d~%" (incf *count*))
	 (let ((th-read-sock (bordeaux-threads:make-thread 
			      #'(lambda () 
				  (thread-read-sock con *count*)) 
			      :name *count*)))
	   (pushnew (cons *count* con) *connections*)))))

(defun thread-read-sock (con id)
  (let ((s (usocket:socket-stream con)))
    (unwind-protect 
;    (handler-case
	(loop
	   (let ((line (read-line s nil nil)))
	     (format t ">>>RECV FROM:ID=[~A],MSG=[~A]~%" id line)
	     (force-output t)
	     (if (eq line nil)
		 (return))
	     (mapcar #'(lambda (x) (format t "~A~%" x)) *connections*)
	     (broadcast-msg id line)))
;      (error ()
	(progn
	  (format t "Error! - in thread-read-sock:ID=[~A]~%" id)
	  (format t "closing a connection:ID=[~A]~%" id)
	  (usocket:socket-close con)
	  (format t "closed a connection:ID=[~A]~%" id)

	  (format t "deleting a connection:ID=[~A]~%" id)
	  (setf *connections* (delete-connection id))
	  (format t "deleted a connection:ID=[~A]~%" id)

	  (mapcar #'(lambda (x) (format t "~A~%" x)) *connections*)
	  )
;	)
	)
    ))

(defun broadcast-msg (id line)
  (flet ((send-msg (pair)
	   (let ((con-id (car pair))
		 (con (cdr pair)))
	     (if (not (= id con-id))
		 (handler-case 
		     (let ((s (usocket:socket-stream con)))
		       (format t "<<<SEND TO:ID=[~A],MSG=[~A]~%" con-id line)
		       (format s "ID=[~A]:~A~%" id line)
		       (force-output s))
		   (error () 
		     (progn 
		       (format t "Error! in broadcast-msg:CON-ID=[~A]~%" con-id)
		       (close s)
		       (format t "deleting a connection:CON-ID=[~A]~%" con-id)
		       (setf *connections* (delete-connection con-id))
		       (format t "deleted a connection:CON-ID=[~A]~%" con-id)
		       
		       (usocket:socket-close con)
		       )))))))
    (mapcar #'send-msg *connections*)))

(defun delete-connection (id)
  "delete id-connection from *connections*"
  (delete-if #'(lambda (x) 
		 (if (eq (car x) id)
		     t))
	     *connections*))

(run-server *server-ip* *server-port*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
