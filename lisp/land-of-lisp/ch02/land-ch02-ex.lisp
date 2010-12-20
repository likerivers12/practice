
(defparameter *small* 1)

(defparameter *big* 100)

;; p25

(defun guess-my-number ()
  (ash (+ *small* *big*) -1))

(defun smaller ()
  (setf *big* (1- (guess-my-number)))
  (guess-my-number))


(defun bigger ()
  (setf *small* (1+ (guess-my-number)))
  (guess-my-number))

(defun start-over ()
  (defparameter *small* 1)
  (defparameter *big* 100)
  (guess-my-number))


;; 컴퓨터가 숫자를 맞추는 게임
;; 1. 숫자를 결정하고(마음 속)
;; 2. (start-over) 실행
;;   -> 컴퓨터가 답을 제시
;; 3. 마음 속의 수가 그 수보다 크면  (bigger)
;;                         작으면 (smaller)
;;

