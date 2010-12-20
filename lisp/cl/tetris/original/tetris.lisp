;;;; Simple tetris using LTk.

;;;; Future work.
;;;; 1. No "play-again?" option when the game is over.
;;;; 2. No score board,
;;;; 3. No next shape preveiw.
;;;; 4. No level system.
;;;; 5. No pause.
;;;; Every one of them is crucial for Tetris.
;;;; Nevertheless, they don't interest me at the moment.

;;;; LTk and iterate packages are used.
;;;; They can be easily installed by using asdf or clbuild.

(require 'ltk)
(require 'iterate)

(defpackage tetris
  (:use :cl :ltk :iterate))

(in-package :tetris)

;;; One square size in pixel.
(defvar *size* 30)

(defvar *board-width* 10)
(defvar *board-height* 20)

;;; Canvas size
(defvar *width* (+ 150 (* *size* *board-width*)))
(defvar *height* (* *size* *board-height*))

(defvar *shape-starts-at* (list (floor *board-width* 2) 1))
(defvar *canvas* nil)

;;; I, S, Z shapes roatate back and forth.
;;; O shape (that looks like a square) doesn't rotate at all
;;; And the rest spin around the center.
(defvar *shapes* '(I-shape J-shape L-shape O-shape S-shape T-shape Z-shape))

;;; All we need from LTk
;;;=============================================

(defvar *base-speed* 1000) ;moves one square down per 1000 miliseconds.

(defvar *I-color* "Blue")
(defvar *J-color* "Yellow")
(defvar *L-color* "Purple")
(defvar *O-color* "Green")
(defvar *S-color* "Red")
(defvar *T-color* "Cyan")
(defvar *Z-color* "Orange")
(defvar *Background-color* "Black")

(defvar *right-key* "<Right>") ;move-right
(defvar *left-key* "<Left>") ;move-left
(defvar *rotate-key* "<Up>") ;roate
(defvar *down-key* "<Down>") ;move-down
(defvar *drop-key* "<space>") ;drop

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

;;;=============================================

;;; Small square that constitutes each shape.
(defclass Square ()
  ((x :accessor square-x
      :initform 0
      :initarg :x)
   (y :accessor square-y
      :initform 0
      :initarg :y)
   (size :accessor square-size
	 :initform *size*
	 :initarg :size)
   (color :accessor square-color
	  :initform *Background-color*
	  :initarg :color)
   (picture :accessor square-picture)))

;;; Shape class.
(defclass Shape ()
  ((squares :accessor shape-squares
	    :initarg :squares)
   (color :accessor shape-color
	  :initarg :color)
   (dir :accessor shape-dir
	:initform 1)
   (dir-index :accessor shape-dir-index ;rotation direction index :0 -1 1
	      :initarg :dir-index
	      :initform nil)))

(defclass Board ()
  ((width :accessor board-width
	  :initarg :width
	  :initform *board-width*)
   (height :accessor board-height
	   :initarg :height
	   :initform *board-height*)
   (grid :accessor board-grid
	 :initform (make-hash-table :test #'equal))))

;;; Game controller
(defclass Controller ()
  ((current-shape :accessor controller-current-shape)
   (action-table :accessor controller-action-table
		 :initform (make-hash-table :test #'equal))
   (key-pressed :accessor controller-key-pressed
		:initform 'ready)
   (speed :accessor controller-speed
	  :initform *base-speed*)))


;;; When a shape touches the bottom of the board.
;;; the shape is added to the board.
(defmethod add-shape ((con Controller) (brd Board))
  (let ((sh (controller-current-shape con)))
    (iter (for s in (shape-squares sh))
	  (setf (gethash (list (square-x s) (square-y s)) (board-grid brd)) s))))

;;; When a shape is added to the board.
;;; Checks if there are any rows complete and remove them.
(defmethod add-shape :after ((con Controller) (brd Board))
  (let ((count 0))
    (iter (for i from 0 to (- (board-height brd) 1))
	  (when (row-complete? brd i)
	    (incf count)
	    (delete-row brd i)
	    (drop-down con brd i)))
    (when (> count 0)
      (re-render brd)))) ; They should not be necessay, but Ltk doesn't work perfectly so...

(defun re-render (brd)  
  (let ((grid (board-grid brd)))
    (iter (for line from 0 to (- (board-height brd) 1))
	  (iter (for col from 0 to (- (board-width brd) 1))
		(let ((s (gethash (list col line) grid)))
		  (if s
		      (let ((x (draw-polygon *canvas* (new-coords col line *size*))))
			(set-color *canvas* x (square-color s)))
		      (draw-polygon *canvas* (new-coords col line *size*))))))))


(defun row-complete? (brd i)
  (let ((grid (board-grid brd)))
    (iter (for k from 0 to (1- (board-width brd)))
	  (always (gethash (list k i) grid)))))

(defun delete-row (brd i)
  (let ((grid (board-grid brd)))
    (iter (for k from 0 to (1- (board-width brd)))
	  (set-color *canvas* (square-picture (gethash (list k i) grid)) *Background-color*)
	  (remhash (list k i) grid))))


;;; Drop one-row down for all the squares that are above the complete row.
(defun drop-down (con brd i)
  (declare (ignore con))
  (labels ((drop1 (brd n)
	     (iter (for col from 0 to (1- (board-width brd)))
		   (let ((s (gethash (list col n) (board-grid brd))))
		     (when s
		       (move s brd (lambda (x y) (list x (+ y 1)))))))))
    (iter (for line from (- i 1) downto 0)
	  (drop1 brd line))))


;;; Draws a square on a canvas.
(defmethod draw ((s square))
  (let ((size (square-size s))
	(x (square-x s))
	(y (square-y s)))
    (setf (square-picture s)
	  (draw-polygon *canvas* (new-coords x y size)))
    (set-color *canvas* (square-picture s) (square-color s))))


;;; Simple helper function to draw a rectangle easier.
(defun new-coords (x y size)
  (let ((sx (* size x))
	(sy (* size y)))
    (list sx sy (+ sx size) sy (+ sx size) (+ sy size) sx (+ sy size))))


;;; move method works for square, shape, and controller.
;;; I haven't given a concern for "(defgeneric ...)".

(defmethod move ((s square) (brd Board) simplified-navigator)
  ;; simplified-navigator returns a new location the object will move to next.
  (destructuring-bind (new-x new-y) (funcall simplified-navigator (square-x s) (square-y s))
    ;; Grid should be modified accordingly.
    (let ((grid (board-grid brd)))
      (when (gethash (list (square-x s) (square-y s)) grid)
	(remhash (list (square-x s) (square-y s)) grid)
	(setf (gethash (list new-x new-y) grid) s)))
    (setf (square-x s) new-x
	  (square-y s) new-y)
    (set-coords *canvas*
		(square-picture s) (new-coords new-x new-y *size*))))

(defmethod draw ((sh shape))
  (iter (for s in (shape-squares sh))
	(draw s)))

;;; Tests if the current shape can move
(defun can-move? (con brd navigator)
  (let ((sh (controller-current-shape con)))
    (iter (for s in (shape-squares sh))
	  (never
	   (destructuring-bind (new-x new-y) (funcall navigator con brd
						      (square-x s) (square-y s))
	     (or (out-bound? brd new-x new-y)
		 (gethash (list new-x new-y) (board-grid brd))))))))



(defmacro defshape (name positions color dir) ;dir-index 
  `(progn
     (defclass ,name (Shape)
       ((center :accessor shape-center
		:initarg :center)))
     ;; Variable capture wouldn't be much of a concern here.
     (defmethod draw :before ((s ,name))
       (destructuring-bind (x y) (shape-center s)
	 (setf (shape-dir-index s) ,dir)
	 (setf (shape-dir s) (* ,dir (shape-dir-index s)))
	 (setf (shape-color s) ,color)
	 (setf (shape-squares s)
	       ,(cons 'list (mapcar #'(lambda (p)
					`(make-instance 'Square
							:x (+ x ,(first p))
							:y (+ y ,(second p))
							:color ,color))
				    positions)))))))

(defshape I-shape ((-2 0) (-1 0) (0 0) (1 0)) *I-color* -1)
(defshape J-shape ((-1 0) (0 0) (1 0) (1 1)) *J-color* 1)
(defshape L-shape ((-1 1) (-1 0) (0 0) (1 0)) *L-color* 1)
(defshape O-shape ((-1 0) (0 0) (0 1) (-1 1)) *O-color* 0)
(defshape S-shape ((-1 1) (0 1) (0 0) (1 0)) *S-color* -1)
(defshape T-shape ((-1 0) (0 0) (0 1) (1 0)) *T-color* 1)
(defshape Z-shape ((-1 0) (0 0) (0 1) (1 1)) *Z-color* -1)


(defmethod move ((con Controller) (brd board) navigator)
  (let ((sh (controller-current-shape con)))
    (iter (for s in (shape-squares sh))
	  (move s brd (lambda (x y) (funcall navigator con brd x y))))
    (setf (shape-center sh)
	  (apply navigator con brd (shape-center sh)))
    (when (equal (controller-key-pressed con) *rotate-key*)
      (setf (shape-dir sh)
	    (* (shape-dir sh) (shape-dir-index sh))))))


(defun out-bound? (b x y)
  (or (< x 0) (>= x (board-width b))
      (< y 0) (>= y (board-height b))))


(defmethod draw ((b Board))
  (draw-polygon *canvas*
		(list 0 0 (* *size* (board-width b)) 0
		      (* *size* (board-width b)) (* *size* (board-height b))
		      0 (* *size* (board-height b)))))


;;; Each shape should move automatically without keyboard events.
(defmethod auto-down ((con Controller) (brd Board))
  (animate-after (controller-speed con) (lambda ()
					  (move-down con brd)
					  (if (board-full? con brd)
					      (print "Game Over")
					      (auto-down con brd)))))


(defmethod create-new-shape ((con Controller))
  (let ((new-shape (make-instance (random-choice *shapes*) :center *shape-starts-at*)))
    (setf (controller-current-shape con) new-shape)
    (draw new-shape)))

(defun make-bind (action con brd)
  (bind-event *canvas* action
	      (lambda (evt)
		(declare (ignore evt))
		(setf (controller-key-pressed con) action)
		(perform-action action con brd))))

(defun bind-action (con brd)
  (make-bind *right-key* con brd)
  (make-bind *left-key* con brd)
  (make-bind *down-key*  con brd)
  (make-bind *rotate-key* con brd) ;rotate
  (make-bind *drop-key* con brd))

(defun add-action (controller)
  (let ((table (controller-action-table controller)))
    (setf (gethash *right-key* table) #'move-right)
    (setf (gethash *left-key* table) #'move-left)
    (setf (gethash *down-key* table) #'move-down)
    (setf (gethash *rotate-key* table) #'move-rotate)
    (setf (gethash *drop-key* table) #'move-drop)))


(defun perform-action (action controller board)
  (funcall (gethash action (controller-action-table controller))
	   controller board))


;;; The following seeks the next location for each of the action.
;;; ==================================================
(defun navi-right (con brd x y)
  (declare (ignore con brd))
  (list (+ x 1) y))

(defun navi-left (con brd x y)
  (declare (ignore con brd))
  (list (- x 1) y))

(defun navi-down (con brd x y)
  (declare (ignore con brd))
  (list x (+ y 1)))

(defun navi-rotate (con brd x y)
  (declare (ignore brd))
  (let* ((sh (controller-current-shape con))
	 (dir (shape-dir sh)))
    (destructuring-bind (cx cy) (shape-center sh)
      (if (zerop dir)
	  (list x y)
	  (list (+ cx (- (* dir cy)) (* dir y))
		(+ cy (* dir cx) (- (* dir x))))))))
;;; ==================================================


;;; Proper actions for keyboard events
;;; ==================================================
(defun move-right (con brd)
  (when (can-move? con brd #'navi-right)
    (move con brd #'navi-right)))

(defun move-left (con brd)
  (when (can-move? con brd #'navi-left)
    (move con brd #'navi-left)))

(defun move-down (con brd)
  (cond ((can-move? con brd #'navi-down)
	 (move con brd #'navi-down))
	((touch-bottom? con brd #'navi-down)
	 (add-shape con brd)
	 (create-new-shape con)
	 (bind-action con brd))))

(defun move-drop (con brd)
  (iter (while (can-move? con brd #'navi-down))
	(move-down con brd)))

(defun move-rotate (con brd)
  (cond ((can-move? con brd #'navi-rotate)
	 (move con brd #'navi-rotate))
	((touch-bottom? con brd #'navi-rotate)
	 (add-shape brd con)
	 (create-new-shape con)
	 (bind-action con brd))))

;;;- 2010/11/20 add codes from cafe
(defun move-obj (canvas obj new-coords)
  (set-coords canvas obj new-coords))

;;; ==================================================

(defun board-full? (con brd)
  (let ((sh (controller-current-shape con))
	(grid (board-grid brd)))
    (iter (for s in (shape-squares sh))
	  (thereis
	   (gethash (list (square-x s) (square-y s)) grid)))))

(defun touch-bottom? (con brd navigator)
  (let ((sh (controller-current-shape con))
	(grid (board-grid brd)))
    (iter (for s in (shape-squares sh))
	  (thereis
	   (destructuring-bind (new-x new-y) (funcall navigator con brd (square-x s) (square-y s))
	     (or (>= new-y (board-height brd))
		 (gethash (list new-x new-y) grid)))))))


(defun random-choice (items)
  (nth (random (length items)) items))

(defun start-tetris ()
  (under-gui
   (setf *canvas* (make-canvas *width* *height*))
   (show-canvas *canvas*)
   (focus-on *canvas*)
   (let ((brd (make-instance 'Board))
	 (con (make-instance 'Controller)))
     (draw brd)
     (add-action con)
     (create-new-shape con)
     (bind-action con brd)
     (auto-down con brd))))

(start-tetris)