

;;; SICP 1.1.7 p29
(defn improve
  [guess x]
  (average guess (/ x guess)))

(defn average
  [x y]
  (/ (+ x y) 2))

(defn square
  [x]
  (* x x))

(defn good-enough?
  [guess x]
  (< (Math/abs (- (square guess) x)) 0.0001))

(defn sqrt-iter
  [guess x]
  (if (good-enough? guess x)
    guess
    (sqrt-iter (improve guess x)
               x)))

(defn sqrt
  [x]
  (sqrt-iter 1.0 x))

(sqrt 2)



