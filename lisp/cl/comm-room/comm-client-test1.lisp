(require 'usocket)
(require 'bordeaux-threads)

(defpackage :comm-client-test
  (:use :cl :usocket :bordeaux-threads))

(in-package :comm-client-test)

;; ;; OK!
;; (defun run-client (port)
;;   (let* ((con (usocket:socket-connect "127.0.0.1" port))
;; 	 (th-read-sock (bordeaux-threads:make-thread
;; 		   #'(lambda ()
;; 		       (thread-read-sock con 1)) :name "read"))
;; 	 (th-read-local (bordeaux-threads:make-thread
;; 		    #'(lambda ()
;; 			(thread-read-local con 2)) :name "write")))
;;     (bordeaux-threads:join-thread th-read-local)
;;     (bordeaux-threads:join-thread th-read-sock)
;;     (usocket:socket-close con)))

;; OK!
(defun run-client (port)
  (let* ((con (usocket:socket-connect "127.0.0.1" port))
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
;  (let ((s (usocket:socket-stream con)))
  (with-open-stream (s (usocket:socket-stream con))
    (loop
       (let ((line (read-line s nil nil)))
;;	 (format t "ID:~A:RECV>>>~A~%" id line)
	 (format t "RECV>>>~A~%" line)
	 (force-output t)
	 (if (eq line nil)
	     (return))))
))
;    (usocket:socket-close con)))

(defun thread-read-local (con id)
;  (let ((s (usocket:socket-stream con)))
  (with-open-stream (s (usocket:socket-stream con))
    (loop
       (let ((line (read-line)))
;	 (format s "ID:~A:SEND<<<~A~%" id line)
	 (format s "~A~%" line)
	 (force-output s)
	 (if (eq line nil)
	     (return))))
))
;    (usocket:socket-close con)))

(run-client 8006)
