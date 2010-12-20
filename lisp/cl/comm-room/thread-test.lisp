(require 'bordeaux-threads)

(defpackage :thread-test
    (:use :cl :bordeaux-threads))

(in-package :thread-test)

(defparameter *count* 0)

(defun test (t-name)
    (dotimes (x 10)
      (sleep (+ (random 2) 1))
      (incf *count*)
      (format t "~A:~d~%" t-name *count*)))

;(bordeaux-threads:make-thread #'test :name "count 1" )
(bordeaux-threads:make-thread #'(lambda () (test "count1")) :name "count 1")

(defun test2 ()
    (dotimes (x 10)
      (sleep (+ (random 2) 1))
      (incf *count*)
      (format t "~d~%" *count*)))

(bordeaux-threads:make-thread #'test2 :name "count 2")
(bordeaux-threads:current-thread)
(bordeaux-threads:all-threads)

(bordeaux-threads:thread-name "count 1")

(bordeaux-threads:current-thread)
(bordeaux-threads:all-threads)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(thread-name (current-thread))

(let ((lock (make-lock "lock1")))
  (acquire-lock lock)
  (incf *count*)
  (format t "count:~d~%" *count*)
  (release-lock lock))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defparameter *lock* (make-lock "lock1"))

(progn
  (acquire-lock *lock*)
  (incf *count*)
  (format t "count:~d~%" *count*)
  (release-lock *lock*))

(acquire-lock *lock*)
(release-lock *lock*)
 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defparameter *lock2* (make-lock "lock2"))

(acquire-lock *lock2*)

*count*

(make-thread #'(lambda ()
		 (progn
		   (acquire-lock *lock2*)
		   (incf *count*)
		   (format t "count:~d~%" *count*)
		   (release-lock *lock2*))) :name "lock2")

*count*

(release-lock *lock2*)

*count*
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
(thread-join ())
(destroy-thread ())


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; TEST
(defun th-read ()
  (loop
    (let ((*line* (read-line)))
      (incf *count*)
      (format t "read:~d~%" *count*))))

(defun th-write ()
  (loop
       (let ((prev *count*))
	 (sleep 1)
	 (let ((cur *count*))
	   (if (not (= prev cur))
	       (format t "write:~d:~A~%" *count* *line*))))))

(defun th-test ()
  (let* ((thr (make-thread #'(lambda () (th-read)) :name "read t"))
	 (thw (make-thread #'(lambda () (th-write)) :name "write t")))
    (format t "Thread Start!~%")
    (join-thread thr)
    (join-thread thw)
    (format t "Thread End!~%")))

;;(th-test)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
