
(require 'tetris)

(in-package :tetris)


;;;---------------
(defun make-canvas (width height)
  (make-instance 'canvas :width width :height height))

(defun show-canvas (canvas) (pack canvas))

(defun focus-on (canvas) (force-focus canvas))

(defun draw-polygon (canvas coords)
  (create-polygon canvas coords))

(defun set-color (canvas polygon color)
  (itemconfigure canvas polygon :fill color))

(defun bind-event (canvas event func)
  (bind canvas event func))

(defun animate-after (milisecs func)
  (after milisecs func))

(defmacro under-gui (&body body)
  `(with-ltk () .,body))

;;;---------------
;; 캔버스 보이기
(under-gui
	   (let ((canvas (make-canvas 200 200))
		 (show-canvas canvas))))

;;;---------------
;; 캔버스에 사각형 그리기
(under-gui
  (let* ((canvas (make-canvas 200 200))
	 (box (draw-polygon canvas '(0 0 100 0 100 100 0 100))))
    (show-canvas canvas)))

;;;---------------
;; 사각형 이동 (1번)
(under-gui
  (let* ((canvas (make-canvas 200 200))
	 (box (draw-polygon canvas '(0 0 100 0 100 100 0 100))))
    (animate-after 1000 
		   (lambda ()
		     (move-obj canvas box
			       '(100 0 200 0 200 100 100 100))))
    (show-canvas canvas)))

;;;---------------
;; 사각형 점차적 이동
(defun go-east (canvas obj coords)
  (animate-after 100 (lambda ()
                       (cond ((> (first coords) 200) 'done)
                             (t (move-obj canvas obj
                                          (new-coords-for-rectangle coords 10 0))
                                (go-east canvas obj (new-coords-for-rectangle coords 10 0)))))))

(defun new-coords-for-rectangle (coords dx dy)
  (destructuring-bind (x1 y1 x2 y2 x3 y3 x4 y4) coords
    (list (+ x1 dx) (+ y1 dy) (+ x2 dx) (+ y2 dy)
          (+ x3 dx) (+ y3 dy) (+ x4 dx) (+ y4 dy))))

(under-gui
  (let* ((canvas (make-canvas 500 200))
         (coords '(0 0 100 0 100 100 0 100))
         (box (draw-polygon canvas coords)))
    (go-east canvas box coords)
    (show-canvas canvas)))

;;;---------------
;; 키 입력으로 사각형 이동
(under-gui
  (let* ((canvas (make-canvas 400 200))
         (coords '(0 0 100 0 100 100 0 100))
         (box (draw-polygon canvas coords)))
    (focus-on canvas)
    (bind-event canvas "<Right>"
                (lambda (evt)
                  (declare (ignore evt))
                  (set-color canvas box "Red")
                  (go-east canvas box coords)))
    (show-canvas canvas)))

;;;--------------------------------------------------------
;; 방향키대로 이동  
;; my

(defparameter *width* 500)
(defparameter *height* 300)
(defparameter *coords* '(0 0 100 0 100 100 0 100))

(defun can-move? (width height cur-coords dx dy)
  (destructuring-bind (x1 y1 x2 y2 x3 y3 x4 y4) cur-coords
    (and (>= (+ x1 dx) 0)
	 (<= (+ x2 dx) width)
	 (<= (+ y3 dy) height)
	 (>= (+ y1 dy) 0))))

(defun move-once (canvas obj coords dx dy)
  (animate-after *time-ani*
		 (lambda ()
		   (destructuring-bind (x1 y1 x2 y2 x3 y3 x4 y4) coords
		     (if (can-move? *width* *height* coords dx dy)
			 (let ((new-coords (new-coords-for-rectangle coords dx dy)))
			   (move-obj canvas 
				     obj
				     new-coords)
			   (setf *coords* new-coords)))))))

(defparameter *coords* '(0 0 100 0 100 100 0 100))

(defparameter *time-ani* 10)

(under-gui
  (let* ((canvas (make-canvas *width* *height*))
         (box (draw-polygon canvas *coords*)))
    (focus-on canvas)
    (bind-event canvas "<Right>"
                (lambda (evt)
                  (declare (ignore evt))
                  (set-color canvas box "Red")
                  (move-once canvas box *coords* 10 0)))
    (bind-event canvas "<Left>"
                (lambda (evt)
                  (declare (ignore evt))
                  (set-color canvas box "Red")
                  (move-once canvas box *coords* -10 0)))
    (bind-event canvas "<Down>"
                (lambda (evt)
                  (declare (ignore evt))
                  (set-color canvas box "Red")
                  (move-once canvas box *coords* 0 10)))
    (bind-event canvas "<Up>"
                (lambda (evt)
                  (declare (ignore evt))
                  (set-color canvas box "Red")
                  (move-once canvas box *coords* 0 -10)))
    (show-canvas canvas)))


;;;--------------------------------------------------------

(defparameter *width* 500)
(defparameter *height* 300)
(defparameter *coords* '(0 0 100 0 100 100 0 100))

(defun can-move? (width height cur-coords dx dy)
  (destructuring-bind (x1 y1 x2 y2 x3 y3 x4 y4) cur-coords
    (and (>= (+ x1 dx) 0)
	 (<= (+ x2 dx) width)
	 (<= (+ y3 dy) height)
	 (>= (+ y1 dy) 0))))

(defun move-once (canvas obj coords dx dy)
  (animate-after *time-ani*
		 (lambda ()
		   (destructuring-bind (x1 y1 x2 y2 x3 y3 x4 y4) coords
		     (if (can-move? *width* *height* coords dx dy)
			 (let ((new-coords (new-coords-for-rectangle coords dx dy)))
			   (move-obj canvas 
				     obj
				     new-coords)
			   (setf *coords* new-coords)))))))

(defun bind-move-event (canvas box key-string ev-fn)
  (bind-event canvas key-string 
	      (lambda (evt)
		(declare (ignore evt))
		;(set-color canvas box "Red")
		(funcall ev-fn canvas box *coords*))))

(defun make-move-ev-fn (dx dy)
  (lambda (canvas box coords)
    (animate-after *time-ani*
		   (lambda ()
		     (destructuring-bind (x1 y1 x2 y2 x3 y3 x4 y4) coords
		       (if (can-move? *width* *height* coords dx dy)
			   (let ((new-coords (new-coords-for-rectangle coords dx dy)))
			     (move-obj canvas 
				       box
				       new-coords)
			     (setf *coords* new-coords))))))))


(defparameter *coords* '(0 0 100 0 100 100 0 100))

(defparameter *time-ani* 10)

(under-gui
  (let* ((canvas (make-canvas *width* *height*))
         (box (draw-polygon canvas *coords*)))
    (focus-on canvas)
    (bind-move-event canvas box "<Right>" 
		     (make-move-ev-fn 10 0))
    (bind-move-event canvas box "<Left>" 
		     (make-move-ev-fn -10 0))
    (bind-move-event canvas box "<Down>" 
		     (make-move-ev-fn 0 10))
    (bind-move-event canvas box "<Up>" 
		     (make-move-ev-fn 0 -10))
    (show-canvas canvas)))



;;;--------------------------------------------------------------------
;; 회전 추가

(defvar *size* 30)


;;; ============LTK===============
(defvar *canvas* nil)

(defvar *red* "Red")

(defvar *right-key* "<Right>")
(defvar *left-key* "<Left>") 
(defvar *down-key* "<Down>") 
(defvar *rotate-key* "<Up>") ;rotate


;;Draws a rectangle with (x1,y1) and (x2,y2)
(defun draw-rectangle (x1 y1 x2 y2)
  (create-polygon *canvas* (list x1 y1 x2 y1 x2 y2 x1 y2)))

(defun draw-square (x y)
  (draw-rectangle (* *size* x) (* *size* y)
                  (* *size* (1+ x)) (* *size* (1+ y))))

(defun relocate-square (square new-x new-y)
  (set-coords *canvas* square
              (list (* *size* new-x) (* *size* new-y)
                    (* *size* (1+ new-x)) (* *size* new-y)
                    (* *size* (1+ new-x)) (* *size* (1+ new-y))
                    (* *size* new-x) (* *size* (1+ new-y)))))

(defun set-color (polygon color)
  (itemconfigure *canvas* polygon :fill color))

;;func is a function that takes event as an argument,
;;that is, (lambda (evt) ...)
(defun bind-event (event func)
  (bind *canvas* event func))

;;func is a thunk
(defun animate-after (milisecs func)
  (after milisecs func))

(defmacro play-with-gui (&body body)
  `(with-ltk ()
     (setf *canvas*
           (make-instance 'canvas :width 480 :height 600))
     (pack *canvas*)
     (force-focus *canvas*)
     .,body))

;;; ============LTK===============

(defclass Square ()
  ((x :accessor x-val :initarg :x)
   (y :accessor y-val :initarg :y)
   (size :accessor size :initform *size*)
   (color :accessor color :initarg :color)
   (picture :accessor picture :initarg :picture)))

(defclass Piece ()
  ((squares :accessor squares :initarg :squares)
   (center :accessor center :initarg :center)
   ;;rotation-type can be -1, 0 or 1.
   ;;-1: rotate back and forth,
   ;;0: do not rotate,
   ;;1: spin around the center
   (rotation-type :accessor rotation-type
                  :initarg :rotation-type)
   ;;rotation-direction can be -1, 0, or 1.
   ;;-1: counterclockwise
   ;;0: do not rotate
   ;;1:clockwise
   (rotation-direction :accessor rotation-direction
                       :initform 1
                       :initarg :rotation-direction)))

(defclass Board ()
  ((width :accessor width :initarg :width)
   (height :accessor height :initarg :height)
   (picture :accessor picture :initarg :picture)
   ))

(defun make-square (x y color)
  (let ((pic (draw-rectangle (* *size* x) (* *size* y)
                             (* *size* (1+ x)) (* *size* (1+ y)))))
    (set-color pic *red*)
    (make-instance 'Square
                   :x x :y y :color color
                   :picture pic)))

(defun make-piece (center rotation-type &rest squares)
  (make-instance 'Piece :squares squares
                 :center center :rotation-type rotation-type))

(defun make-board (width height)
  (make-instance 'Board :width width :height height
                 :picture 
                 (draw-rectangle 0 0 (* *size* width) (* *size* height))))


(defmethod move2 ((s Square) (brd Board) simple-navigator)
  ;; simple-navigator:: (x y) -> (new-x new-y)
  (destructuring-bind (x y) (funcall simple-navigator (x-val s) (y-val s))
    (setf (x-val s) x
          (y-val s) y)
    (relocate-square (picture s) x y)))

(defmethod move2 ((p Piece) (brd Board) navigator)
  ;;navigator:: ((center-x center-y) dir) -> ((x y) -> (new-x new-y))
  (let ((simple-navigator (funcall navigator (center p) (rotation-direction p))))
    (when (iter (for s in (squares p))
                (always (in-boundary? (funcall simple-navigator (x-val s) (y-val s)) brd)))
      (destructuring-bind (cx cy) (center p)
        (setf (center p) (funcall simple-navigator cx cy))
        (setf (squares p)
              (mapcar (lambda (s)
                        (move2 s brd simple-navigator)
                        s)
                      (squares p)))))))

(defun in-boundary? (xy brd)
  (destructuring-bind (x y) xy
    (let ((w (width brd))
          (h (height brd)))
      (and (>= x 0) (< x w)
           (>= y 0) (< y h)))))

(play-with-gui
  (let* ((brd (make-board 10 20))
         (s1 (make-square 0 0 *red*))
         (s2 (make-square 0 1 *red*))
         (s3 (make-square 0 2 *red*))
         (p (make-piece '(0 0) 1 s1 s2 s3)))
    (bind-event *right-key*
                (lambda (evt)
                  (declare (ignore evt))
                  (move2 p brd (lambda (c d)
                                (declare (ignore c d))
                                (lambda (x y)
                                  (list (1+ x) y))))))
    (bind-event *left-key*
                (lambda (evt)
                  (declare (ignore evt))
                  (move2 p brd (lambda (c d)
                                (declare (ignore c d))
                                (lambda (x y)
                                  (list (1- x) y))))))
    (bind-event *down-key*
                (lambda (evt)
                  (declare (ignore evt))
                  (move2 p brd (lambda (c d)
                                (declare (ignore c d))
                                (lambda (x y)
                                  (list x (1+ y)))))))
    (bind-event *rotate-key*
                (lambda (evt)
                  (declare (ignore evt))
                  (move2 p brd (lambda (c d)
                                (destructuring-bind (cx cy) c
                                  (lambda (x y)
                                    (list (+ cx (- (* d cy)) (* d y))
                                          (+ cy (* d cx) (- (* d x))))))))))))


;;;---------------


;;;---------------


;;;---------------


;;;---------------


