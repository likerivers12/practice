;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(in-package :cl-web-exercise)

(defclass survey-item ()
  ((name  :reader   name 
	  :initarg  :name)
   (votes :accessor votes 
	  :initform 0)))

(defmethod vote-for (user-selected-item)
  (incf (votes user-selected-item))
  (incf *total-votes*))

(defvar *total-votes* 0)
(defvar *items* '())
(defparameter *stat-range* 20)

(defun reset-votes ()
  (setf *total-votes* 0)
  (setf *items* '()))

(defun item-from-name (item-name)
  (find item-name *items* :test #'string-equal :key  #'name))

(defun item-stored? (item-name)
  (item-from-name item-name))

(defun items ()
  (sort (copy-list *items*) #'> :key #'votes))

(defun add-item (name)
  (unless (item-stored? name)
    (push (make-instance 'survey-item :name name) *items*)))

;;exe
;(add-item "Lisp")
;(item-from-name "Lisp")
;(mapcar #'name (items))

