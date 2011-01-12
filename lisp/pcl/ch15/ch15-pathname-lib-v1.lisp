
;;;(defpackage :com.gigamonkeys.pathnames
(defpackage :my.pcl.pathnames
  (:use :common-lisp)
  (:export
   :list-directory
   :file-exists-p
   :directory-pathname-p
   :file-pathname-p
   :pathname-as-directory
   :pathname-as-file
   :walk-directory
   :directory-p
   :file-p))

;;;(in-package :cl-user)
(in-package :my.pcl.pathnames)

(defun component-present-p (value)
  (and value (not (eql value :unspecific))))

(defun directory-pathname-p (p)
  (and
   (not (component-present-p (pathname-name p)))
   (not (component-present-p (pathname-type p)))
   p))

;CL-USER> (pathname-type "/a/b/c.txt")
;"txt"
;CL-USER> (pathname-name "/a/b/c.txt")
;"c"
;CL-USER> (pathname-directory "/a/b/c.txt")
;(:ABSOLUTE "a" "b")
;CL-USER> (pathname-directory "../a/b/c.txt")
;(:RELATIVE :UP "a" "b")
;CL-USER> 

(defun pathname-as-directory (name)
  (let ((pathname (pathname name)))
    (when (wild-pathname-p pathname)
      (error "Can't reliably convert wild pathnames."))
    (if (not (directory-pathname-p name))
	(make-pathname
	 :directory (append (or (pathname-directory pathname) (list :relative))
			    (list (file-namestring pathname)))
	 :name nil
	 :type nil
	 :defaults pathname)
	pathname)))
;; (pathname-as-directory ".")
;; #P".\\"

(defun directory-wildcard (dirname)
  (make-pathname
   :name :wild
   :type #-clisp :wild #+clisp nil
   :defaults (pathname-as-directory dirname)))
;; (directory-wildcard ".")
;; #P".\\*"
;; (directory-wildcard (pathname-as-directory "d:/data/pr/lisp"))
;; #P"D:\\data\\pr\\lisp\\*"

(defun list-directory (dirname)
  (when (wild-pathname-p dirname)
    (error "Can only list concrete directory names."))
  (let ((wildcard (directory-wildcard dirname)))
    
    #+(or sbcl cmu lispworks)
    (directory wildcard)
    
    #+openmcl
    (directory wildcard :directories t)
    
    #+allegro
    (directory wildcard :directories-are-files nil)
    
    #+clisp
    (nconc
     (directory wildcard)
     (directory (clisp-subdirectories-wildcard wildcard)))
    
    #-(or sbcl cmu lispworks openmcl allegro clisp)
    (error "list-directory not implemented")))
;; (list-directory (pathname-as-directory "d:/data/pr/lisp"))

;; (directory (directory-wildcard (pathname-as-directory "d:/data/pr/lisp")))
;; (#P"D:\\data\\pr\\lisp\\zpng-test.lisp"
;;  #P"D:\\data\\pr\\lisp\\word_count.lisp~"
;;  #P"D:\\data\\pr\\lisp\\word_count.lisp"
;;  #P"D:\\data\\pr\\lisp\\ways-to-pay.lisp~"
;;  #P"D:\\data\\pr\\lisp\\ways-to-pay.lisp" #P"D:\\data\\pr\\lisp\\test.lisp~"
;;  #P"D:\\data\\pr\\lisp\\test.lisp" #P"D:\\data\\pr\\lisp\\test.lib"
;;  #P"D:\\data\\pr\\lisp\\test.fas" #P"D:\\data\\pr\\lisp\\test-clppcre.lisp"
;;  #P"D:\\data\\pr\\lisp\\test-cd-db.txt" #P"D:\\data\\pr\\lisp\\PA_HW1.lisp~"
;;  #P"D:\\data\\pr\\lisp\\PA_HW1.lisp" #P"D:\\data\\pr\\lisp\\opencv-test.lisp"
;;  #P"D:\\data\\pr\\lisp\\macro_ntimes.lisp~"
;;  #P"D:\\data\\pr\\lisp\\macro_ntimes.lisp" #P"D:\\data\\pr\\lisp\\iter.lisp~"
;;  #P"D:\\data\\pr\\lisp\\iter.lisp" #P"D:\\data\\pr\\lisp\\hello.lisp~"
;;  #P"D:\\data\\pr\\lisp\\hello.lisp" #P"D:\\data\\pr\\lisp\\clppcre-test.lisp"
;;  #P"D:\\data\\pr\\lisp\\cffi-manual-exercise.lisp"
;;  #P"D:\\data\\pr\\lisp\\bbc_wc.csv"
;;  #P"D:\\data\\pr\\lisp\\asdf-install-test.lisp~"
;;  #P"D:\\data\\pr\\lisp\\asdf-install-test.lisp.bak"
;;  #P"D:\\data\\pr\\lisp\\asdf-install-test.lisp"
;;  #P"D:\\data\\pr\\lisp\\asdf-install-test-cygwin-home.lisp~"
;;  #P"D:\\data\\pr\\lisp\\asdf-install-test-cygwin-home.lisp.bak"
;;  #P"D:\\data\\pr\\lisp\\asdf-install-test-cygwin-home.lisp"
;;  #P"D:\\data\\pr\\lisp\\ant_series.lisp")

#+clisp
(defun clisp-subdirectories-wildcard (wildcard)
  (make-pathname
   :directory (append (pathname-directory wildcard) (list :wild))
   :name nil
   :type nil
   :defaults wildcard))
;;(clisp-subdirectories-wildcard (directory-wildcard (pathname-as-directory "d:/data/pr/lisp")))
;;#P"D:\\data\\pr\\lisp\\*\\"
;;(pathname-directory (directory-wildcard (pathname-as-directory "d:/data/pr/lisp")))
;;(:ABSOLUTE "data" "pr" "lisp")

(defun file-exists-p (pathname)
  #+(or sbcl lispworks openmcl)
  (prob-file pathname)
  
  #+(or allegro cmu)
  (or (probe-file (pathname-as-directory pathname))
      (prob-file pathname))

  #+clisp
  (or (ignore-errors
	(probe-file (pathname-as-file pathname)))
      (ignore-errors
	(let ((directory-form (pathname-as-directory pathname)))
	  (when (ext:probe-directory directory-form)
	    directory-form)))))
;; (file-exists-p ".")
;; #P"D:\\cygwin\\home\\suil\\."

(defun pathname-as-file (name)
  (let ((pathname (pathname name)))
    (when (wild-pathname-p pathname)
      (error "Can't reliably convert wild pathnames."))
    (if (directory-pathname-p name)
	(let* ((directory (pathname-directory pathname))
	       (name-and-type (pathname (first (last directory)))))
	  (make-pathname
	   :directory (butlast directory)
	   :name (pathname-name name-and-type)
	   :type (pathname-type name-and-type)
	   :defaults pathname))
	pathname)))

(defun walk-directory (dirname fn &key directories (test (constantly t)))
  (labels
      ((walk (name)
	 (cond
	   ((directory-pathname-p name)
	    (when (and directories (funcall test name))
	      (funcall fn name))
	    (dolist (x (list-directory name)) (walk x)))
	   ((funcall test name) (funcall fn name)))))
    (walk (pathname-as-directory dirname))))
