;;;; executing list-unit test

;;; 1. Load "lisp-unit.lisp"
(load "d:/lang/LISP/lisp-unit/lisp-unit.lisp")

;;; 2. evaluate below
(use-package :lisp-unit)

;;; 3. Load source file to test
(load "d:/data/pr/lisp/pcl/ch3/ch3-db.lisp")

;;; 4. Load unit test file
(load "d:/data/pr/lisp/pcl/ch3/ch3-db-unit-test.lisp")

;;;
(run-tests)
;(run-tests add1)
;;;
;(run-tests testname1 testname2)


