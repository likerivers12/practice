(require 'usocket)
(require 'bordeaux-threads)

(defpackage :comm-client-test
  (:use :cl :usocket :bordeaux-threads))

(in-package :comm-client-test)

(defparameter *remote-ip* "127.0.0.1")  ;;"192.168.0.95"
(defparameter *remote-port* 8006) 

(defun run-client (port)
  (let* ((con (usocket:socket-connect *remote-ip* port))
	 (th-read-sock (bordeaux-threads:make-thread
			#'(lambda ()
			    (thread-read-sock con 1)) :name "read"))
	 (th-read-local (bordeaux-threads:make-thread
			 #'(lambda ()
			     (thread-read-local con 2)) :name "write")))
    (unwind-protect
	 (progn
	   (format t "Thread Started~%")
	   (bordeaux-threads:join-thread th-read-local)
	   (bordeaux-threads:join-thread th-read-sock))
      (progn
	(usocket:socket-close con))
      )))


(defun thread-read-sock (con id)
  (with-open-stream (s (usocket:socket-stream con))
    (loop
       (let ((line (read-line s nil nil)))
	 (format t "RECV>>>~A~%" line)
	 (force-output t)
	 (if (eq line nil)
	     (return))
	 (force-output t)
	 ))
))

(defun thread-read-local (con id)
  (with-open-stream (s (usocket:socket-stream con))
    (loop
       (let ((line (read-line)))
	 (format s "~A~%" line)
	 (force-output s)
	 (if (eq line nil)
	     (return))))
))

(run-client *remote-port*)
