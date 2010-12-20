(in-package :cl-web-exercise)
;(in-package :my-login-db)

(defvar *db* nil)

(defun make-comparison-expr (field value)
  `(equal (getf user ,field) ,value))

(defun make-comparison-list (fields)
  (loop while fields
       collecting (make-comparison-expr (pop fields) (pop fields))))

(defmacro where (&rest clauses)
  `#'(lambda (user) (and ,@(make-comparison-list clauses))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun make-user (id passwd vote-flag)
  (list :id id :passwd passwd :vote-flag vote-flag))

(defun add-record (user) 
  (let* ((in-id (getf user :id))
	 (db-id (getf (car (select (where :id in-id))) :id)))
    (if (eq db-id nil)
	(progn
	  (push user *db*)
	  t)
	nil)))
	
(defun add-user ()
  (loop (add-record (prompt-for-user))
     (if (not (y-or-n-p "Another? [y/n]: "))
	 (return))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun save-db (filename)
  (with-open-file (out filename 
		       :direction :output
		       :if-exists :supersede)
    (with-standard-io-syntax
      (print *db* out))))

(defun load-db (filename)
  (with-open-file (in filename)
    (with-standard-io-syntax
      (setf *db* (read in)))))

(defun dump-db ()
  (dolist (user *db*)
    (format t "~{~a:~10t~a~%~}~%" user)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun prompt-read (prompt)
  (format *query-io* "~a: " prompt)
  (force-output *query-io*)
  (read-line *query-io*))

(defun prompt-for-user ()
  (make-user
   (prompt-read "ID")
   (prompt-read "PASSWD")))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun select (selector-fn)
  (remove-if-not selector-fn *db*))

(defun update (selector-fn &key passwd vote-flag)
  (setf *db*
 	(mapcar
 	 #'(lambda (row)
 	     (when (funcall selector-fn row)
 	       (if passwd (setf (getf row :passwd) passwd))
 	       (if vote-flag (setf (getf row :vote-flag) vote-flag)))
 	     row) *db*)))

(defun delete-rows (selector-fn)
  (setf *db* (remove-if selector-fn *db*)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;
;; (progn
;;   (setf *db* nil)
;;   (add-record (make-user "AAA" "aaaa"))
;;   (add-record (make-user "BBB" "bbbb"))

;; ;  (update (where :artist "Dixie Chicks") :rating 11)
;;   (select (where :id "AAA"))
;; ;  (delete-rows (where :artist "Dixie Chicks" :title "Fly"))

;;   (dump-db)
;;   (save-db "d:/data/pr/lisp/klisper-seminar/login-db.txt"))
